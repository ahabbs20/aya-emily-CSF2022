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

//main function controls for while loop in main
//returns 1 if all goes well, -1 for any fatal errors
int while_loop(bool result_of_last_connection, Connection &conn, Message &return_msg) {
  result_of_last_connection = conn.receive(return_msg);
  if (!result_of_last_connection) {
    return conn.error_and_return("Unable to perform receive message", -1);  // fatal error
  } else if (return_msg.tag.compare(TAG_ERR) == 0) {
    cerr << return_msg.data << endl;
  } else if (return_msg.tag.compare(TAG_DELIVERY) == 0) {
    std::vector<string> payload = return_msg.split_payload();
    cout << payload.at(1) << ": " << payload.at(2) << endl;
  }
  return 1;
}

int main(int argc, char **argv) {
  Connection conn;

  if (argc != 5) {
    return conn.error_and_return("Usage: ./receiver [server_address] [port] [username] [room]\n", 1);
  }

  string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  string username = argv[3];
  string room_name = argv[4];

  
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    return conn.error_and_return("Unable to connect with: " + server_hostname + " and " + argv[2], -1);
  }

  Message return_msg;
  bool result_of_last_connection;
  
  // Send slogin message
  result_of_last_connection = conn.send(Message(TAG_RLOGIN, username));
  if (!result_of_last_connection) {  return conn.error_and_return("ERROR: unable to login with username " + username, -1); }
  
  // read response: If not ok:
  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) { return conn.error_and_return(return_msg.data, -1); }

  result_of_last_connection = conn.send(Message(TAG_JOIN, room_name));
  if (!result_of_last_connection) { return conn.error_and_return("ERROR: Unable to join due to EOF or other Error", -1); }

  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) {
    return conn.error_and_return(return_msg.data, -1);
  } else if (!result_of_last_connection) {
    return conn.error_and_return("Unable to perform action: Join.", -1);
  }

  while (1) {
    int result = while_loop(result_of_last_connection, conn, return_msg);
    if (result != 1) {
      return result;
    }
  }
  
  conn.close();

  return 0;
}