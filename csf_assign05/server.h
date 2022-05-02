#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
#include "connection.h"
class User;
class Room;
class Message;
class Server {

public:
  Server(int port);
  ~Server();

  struct Conn_Info {
    Connection * connection;
    Server * server;

    Conn_Info() {
      
    }

    ~Conn_Info() {
      delete connection;
    }
  };
  
  /*
  Information and methods shared by both chat_with_receiver 
  and chat_with_sender.
  */
  class Chat {
    public: 
      Chat(Conn_Info * con, User * us) {
        conn = con;
        user = us;
      }

      Chat() {
        
      }

    bool server_join(std::string& room_name);

    protected:
      Conn_Info * conn; 
      User * user;
  };

  //formerly chat_with_receiver interaction stuff
  class Chat_Receiver: public Chat{
    public:
      Chat_Receiver(Conn_Info * con, User * us, Message * in) {
        conn = con;
        user = us;
        input = in;
      }

      //send any response messages necessary before looped processing
      void send_response_messages();

      //looped processing of incoming messages
      void loop();
    private:
      Message * input;
  };

  //formerly chat_with_sender interaction stuff
  class Chat_Sender: public Chat{
    public:
      Chat_Sender(Conn_Info * con, User * us) {
        conn = con; 
        user = us; 
      }

      //main sender loop
      void loop();
      
      //server interaction functions not inherited from chat class
      void server_leave(std::string& room_name);
      void server_sendall(Message &input, std::string &room_name);
    private:
      Conn_Info * conn; 
      User * user; 
  };

  bool listen();

  void handle_client_requests();

  Room *find_or_create_room(const std::string &room_name);
  void chat_with_sender(User * user, Conn_Info * conn);
  void chat_with_receiver(User * user, Conn_Info * conn);

  void receiver_send_response_messages(User * user, Conn_Info * conn, Message * input);
  void receiver_loop(User * user, Conn_Info * conn);

private:
  // prohibit value semantics
  Server(const Server &);
  Server &operator=(const Server &);

  typedef std::map<std::string, Room *> RoomMap;

  // These member variables are sufficient for implementing
  // the server operations
  int m_port;
  int m_ssock;
  int server_fd;
  RoomMap m_rooms;
  pthread_mutex_t m_lock;

  // do we need a function that matches user to room? instead of room directly

  // functions to aid with sending
  bool server_join(User * user, Conn_Info * conn, std::string &room_name);
  void server_leave(User * user, Conn_Info * conn, std::string &room_name);
  void server_sendall(User * user, Conn_Info * conn, Message &input, std::string &room_name);

};

#endif // SERVER_H
