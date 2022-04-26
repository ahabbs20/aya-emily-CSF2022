#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
class Room;

class Server {
public:
  typedef struct {
    Connection * connection;
    Server * server;
  } Conn_Info;

  Server(int port);
  ~Server();

  bool listen();

  void handle_client_requests();

  Room *find_or_create_room(const std::string &room_name);
  void chat_with_sender(User * user, Conn_Info * conn);
  void chat_with_receiver(User * user, Conn_Info * conn);

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
};

#endif // SERVER_H
