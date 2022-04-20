#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

using std::cerr;
using std::string;
using std::endl;
using std::cin;
using std::cout;

int main(int argc, char **argv) {
  if (argc != 5) {
    cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  string username = argv[3];
  string room_name = argv[4];

  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    cerr << "Unable to connect with: " << server_hostname << " and " << server_port << endl;
    return -1;
  }

  Message return_msg;
  bool result_of_last_connection;
  
  // Send slogin message
  result_of_last_connection = conn.send(Message(TAG_RLOGIN, username));
  if (!result_of_last_connection) {
    std::cerr << "ERROR: unable to login with username " << username << endl;  
    return -1;
  }
  
  // read response: If not ok:
  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) {
    cerr << return_msg.data << endl;
    return -1;
  }

  result_of_last_connection = conn.send(Message(TAG_JOIN, room_name));
  if (!result_of_last_connection) {
    std::cerr << "ERROR: Unable to join due to EOF or other Error" << endl;  
    return -1;
  }

  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) {
    cerr << return_msg.data << endl;
    return -1;
  } else if (!result_of_last_connection) {
    cerr << "Unable to perform action: Join." << endl;
    return -1;
  }

  while (1) {
    result_of_last_connection = conn.receive(return_msg);
    if (!result_of_last_connection) {
      cerr << "Unable to perform receive message" << endl; // fatal error
      return -1;
    } else if (return_msg.tag.compare(TAG_ERR) == 0) {
      cerr << return_msg.data << endl;
    } else if (return_msg.tag.compare(TAG_DELIVERY) == 0) {
      std::vector<string> payload = return_msg.split_payload();
      cout << payload.at(1) << ": " << payload.at(2) << endl;
    }
  }
  
  conn.close();

  return 0;
}

