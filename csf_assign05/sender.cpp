#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

using std::cerr;
using std::string;
using std::endl;
using std::cin;

int main(int argc, char **argv) {
  if (argc != 4) {
    cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  string server_hostname;
  int server_port;
  string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // Connection to port56
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    cerr << "Unable to connect with: " << server_hostname << " and " << server_port << endl;
    return -1;
  }

  Message return_msg;
  bool result_of_last_connection;
  
  // Send slogin message
  result_of_last_connection = conn.send(Message(TAG_SLOGIN, username));
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

  string input;
  while (1) {
      cin >> input;
    if (input[0] != '/') { // check if it is not a command
      //rest of message
      string rest;
      std::getline(cin, rest);
      input += rest;
      result_of_last_connection = conn.send(Message(TAG_SENDALL, input));

    } else {
      if (input.compare("/join") == 0) { // join request
        string room;
        cin >> room; 
        if (room.compare("") == 0) {
          cerr << "ERROR: Join request must be of format \"/join [room]\"\n";
        }
        result_of_last_connection = conn.send(Message(TAG_JOIN, room));

      } else if (input.compare("/leave") == 0) {  // leave request
        result_of_last_connection = conn.send(Message(TAG_LEAVE, ""));

      } else if (input.compare("/quit") == 0) { // quit request
        result_of_last_connection = conn.send(Message(TAG_QUIT, ""));
        result_of_last_connection = conn.receive(return_msg);
        if (return_msg.tag.compare(TAG_OK) == 0) { // if can leave, just leave
          return 0;
        } else if (return_msg.tag.compare(TAG_ERR) == 0) { // else pass error
          cerr << "ERROR: Unable to quit, killing program\n";
          return -1;
        }

      } else { // cannot parse unknown command
        std::cerr << "ERROR: Unknown command, please use on of the following: /join, /leave, /quit\n";

      }
    }
 
    if (conn.get_last_result() != conn.SUCCESS) { // check to see if error arose from writing to server
      cerr << "ERROR: Unable to send message\n";
      return -1; // connection on 
    }
    
    result_of_last_connection = conn.receive(return_msg);
    if (return_msg.tag.compare(TAG_ERR) == 0) { // check if server sent any error
      cerr << return_msg.data << endl;
    }

    // empty buffer
    std::cout.flush();
  }

  conn.close();
  
  return 0;
}
