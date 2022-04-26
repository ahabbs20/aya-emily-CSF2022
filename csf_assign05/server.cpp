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

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

  void *worker(void *arg) {
    pthread_detach(pthread_self());

    // TODO: use a static cast to convert arg from a void* to
    //       whatever pointer type describes the object(s) needed
    //       to communicate with a client (sender or receiver)
    Server::Conn_Info * conn = (Server::Conn_Info *)arg;

    // TODO: read login message (should be tagged either with
    //       TAG_SLOGIN or TAG_RLOGIN), send response
    Message msg;
    (*conn->connection).receive(msg);

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
    
    

    // TODO: depending on whether the client logged in as a sender or
    //       receiver, communicate with the client (implementing
    //       separate helper functions for each of these possibilities
    //       is a good idea)

    return nullptr;
  }

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  server_fd = open_listenfd(std::to_string(m_port).c_str());
  if (server_fd < 0) {
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (1) {
    int client_fd = Accept(server_fd, NULL, NULL);
    Connection c = Connection(client_fd);
    pthread_t * target;
    Conn_Info *conn = new Conn_Info();
    conn->connection = &c; 
    conn->server = this;
    pthread_create(target, NULL, worker, conn);
  }

}

Room* Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Room * found = m_rooms.at(room_name);

  if (found != NULL) {
    return found;
  } else {
    Room * temp = new Room(room_name);
    m_rooms.emplace(room_name, temp);
    return temp;
  }

}

void Server::chat_with_receiver(User * user, Conn_Info * conn) {
  // join 
  Message input;
  conn->connection->receive(input);

  if (input.tag.compare(TAG_JOIN) != 0) {
    conn->connection->send(Message(TAG_ERR, "Inorder to receive message, must be a member of a room"));
  } else if (input.data.empty() == 0) {
    conn->connection->send(Message(TAG_ERR, "Join message must specify room"));
  } else {
    Room * result = conn->server->find_or_create_room(input.data);
    result->add_member(user);
  }

  // user is now in room
  while (1) {
    Message * msg = user->mqueue->dequeue();
    if (msg != nullptr) {
      conn->connection->send(*msg);
      delete(msg);
    }
  }  
}

void Server::chat_with_sender(User * user, Conn_Info * conn) {
  Message input;
  conn->connection->receive(input);
  // room pointer

  if (input.tag.compare(TAG_JOIN) != 0) {
    conn->connection->send(Message(TAG_ERR, "Inorder to send messages, must be a member of a room"));
  } else if (input.data.empty() == 0) {
    conn->connection->send(Message(TAG_ERR, "Join message must specify room"));
  } else {
    Room * result = conn->server->find_or_create_room(input.data);
  }
  
  // join
  // leave
  // quit
  // sendall
  
}


