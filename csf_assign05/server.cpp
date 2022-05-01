#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"
////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

  void *worker(void *arg) {
    pthread_detach(pthread_self());

    Server::Conn_Info * conn = static_cast<Server::Conn_Info *>(arg);

    Message msg;
    if (!(*conn->connection).receive(msg)) {
      if (conn->connection->get_last_result() == Connection::INVALID_MSG) { // unable to process last message
        conn->connection->send(Message(TAG_ERR, "invalid message"));
      }
      return nullptr;
    }

    if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) { // must login in order to have access to server
      conn->connection->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
      return nullptr;
    }

    std::string resp = "welcome " + msg.data;
    if (!conn->connection->send(Message(TAG_OK, resp))) { // send OK message
      return nullptr;
    }

    User * user = new User(msg.data);
    user->mqueue = new MessageQueue();

    //rlogin = receiver, slogin = sender
    if (msg.tag.compare(TAG_SLOGIN) == 0) {
      //sender
      (*conn->server).chat_with_sender(user, conn);
    } else {
      //receiver
      (*conn->server).chat_with_receiver(user, conn);
    }

    delete(conn);
    return nullptr;
  }

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);

}

bool Server::listen() {
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
  while (1) {
    int client_fd = Accept(m_ssock, nullptr, nullptr); // accept client connection

    if (client_fd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }

    // create client thread
    Connection * c = new Connection(client_fd); // conn info contains server and connection information
    pthread_t target;
    Server::Conn_Info *conn = new Conn_Info();
    conn->connection = c; 
    conn->server = this;
    if (pthread_create(&target, NULL, worker, static_cast<void *>(conn)) != 0) { // create thread 
      std::cerr << "Could not create thread\n"; // error encountered creating thread must be thrown to cerr
      return;
    }
  } 
}

Room* Server::find_or_create_room(const std::string &room_name) {
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second; // collect room
  }

  return room;
}

/*
  Communicate with 
*/
void Server::chat_with_receiver(User * user, Conn_Info * conn) {
  // join 
  Message input;
  conn->connection->receive(input);

  if (input.tag.compare(TAG_JOIN) != 0) { // first message from receiver must be join
    conn->connection->send(Message(TAG_ERR, "Inorder to receive messages, must be a member of a room"));
  } else if (server_join(user, conn, input.data)) { // join a room if a room is specified
    conn->connection->send(Message(TAG_OK, "Able to join room")); // join a room was successful
  }

  // user is now in room
  while (1) {
    Message * msg = user->mqueue->dequeue(); // dequeue messages if available
    if (msg != nullptr) { // if there is no issue with the message (ie message is not properly deallocated)
      conn->connection->send(*msg);
      delete(msg);
    }
  }  
}

/*
  Process sender messages
*/
void Server::chat_with_sender(User * user, Conn_Info * conn) {
  Message input;
  std::string room_name = "";
  bool result; 

  while (1) {
    conn->connection->receive(input);
    if (input.tag.compare(TAG_JOIN) == 0) {
      if (room_name.compare("") != 0) { // if in a room, leave
        server_leave(user, conn, room_name);
      }
      room_name = input.data;
      result = server_join(user, conn, room_name); 
      if (result) { // no error was sent, thus send ok message instead. 
        conn->connection->send(Message(TAG_OK, "Able to join room"));
      }
    } else if (input.tag.compare(TAG_LEAVE) == 0) { 
      if (room_name.compare("") != 0) { // if in a room, leave, else do nothing
        server_leave(user, conn, room_name);
        conn->connection->send(Message(TAG_OK, "Able to leave room"));
        room_name = ""; // mark as not in room
      }
    } else if (input.tag.compare(TAG_QUIT) == 0) {
      if (room_name.compare("") != 0) { // if in a room, leave
        server_leave(user, conn, room_name);
      }
      conn->connection->send(Message(TAG_OK, "Able to quit room"));
      break;  
    } else if (input.tag.compare(TAG_SENDALL) == 0) {
      server_sendall(user, conn, input, room_name); // send message to everyone
    } else {
      conn->connection->send(Message(TAG_ERR, "Unable to parse tag."));
    }
  }
  
}

/*
  Process join request
*/
bool Server::server_join(User * user, Conn_Info * conn, std::string& room_name) {
  if (room_name.empty()) { // join must specify user. 
    conn->connection->send(Message(TAG_ERR, "Join message must specify room"));
    return false; // unsuccessful join
  } else { 
    Room * result = conn->server->find_or_create_room(room_name);
    result->add_member(user);
    return true; // successful join
  }
}

/*
  Process leave request
*/
void Server::server_leave(User * user, Conn_Info * conn, std::string &room_name) {
  Room * result = conn->server->find_or_create_room(room_name);
  result->remove_member(user); // remove member
}

/*
  Process send all request
*/
void Server::server_sendall(User * user, Conn_Info * conn, Message &input, std::string &room_name) {
  Room * result = conn->server->find_or_create_room(room_name); // find room
  result->broadcast_message(user->username, input.data);  // broadcast
  conn->connection->send(Message(TAG_OK, "Able to delivery message")); // send ok message
}

