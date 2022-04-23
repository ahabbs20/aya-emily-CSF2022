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

//On command join enter this function
bool join(Connection &conn) {
  string room;
  cin >> room; 
  if (room.compare("") == 0) {
    cerr << "ERROR: Join request must be of format \"/join [room]\"\n";
  }
  return conn.send(Message(TAG_JOIN, room));
}

//returns 0 if exit main
//returns -1 for error
//both of those need to be returned to main
//returns 1 otherwise if not important
int command(Connection &conn, bool result_of_last_connection, string input, Message &return_msg) {
  if (input.compare("/join") == 0) { // join request
    result_of_last_connection = join(conn);
  } else if (input.compare("/leave") == 0) {  // leave request
    result_of_last_connection = conn.send(Message(TAG_LEAVE, ""));
  } else if (input.compare("/quit") == 0) { // quit request
    result_of_last_connection = conn.send(Message(TAG_QUIT, ""));
    result_of_last_connection = conn.receive(return_msg);
    if (return_msg.tag.compare(TAG_OK) == 0) { // if can leave, just leave
      return 0;
    } else if (return_msg.tag.compare(TAG_ERR) == 0) { // else pass error
      return conn.error_and_return("ERROR: Unable to quit, killing program\n", -1);
    }
  } else { // cannot parse unknown command
    std::cerr << "ERROR: Unknown command, please use on of the following: /join, /leave, /quit\n";
  }
  return 1;
}

//End cleanup for main
int cleanup(Connection &conn) {
  conn.close();
  return 0;
}

//Main loop done here- looped handling of input
//returns 1 if all is well, 0 if exit main, and -1 if exit due to error
int while_loop(string &input, bool result_of_last_connection, Connection &conn, Message &return_msg){
  cin >> input;
  if (input[0] != '/') { // check if it is not a command
    //rest of message
    string rest;
    std::getline(cin, rest);
    input += rest;
    result_of_last_connection = conn.send(Message(TAG_SENDALL, input));
  } else { //it is a command
    int temp_return = command(conn, result_of_last_connection, input, return_msg);
    if (temp_return != 1) {
      return temp_return;
    }
  }
  if (conn.get_last_result() != conn.SUCCESS) { // check to see if error arose from writing to server
    return conn.error_and_return("ERROR: Unable to send message\n", -1); // connection on 
  }
  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) { // check if server sent any error
    cerr << return_msg.data << endl;
  }

  // empty buffer
  std::cout.flush();
  return 1;
}

int main(int argc, char **argv) {
  Connection conn;
  if (argc != 4) {
    return conn.error_and_return("Usage: ./sender [server_address] [port] [username]\n", 1);
  }

  //initialize important info
  string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  string username = argv[3];

  // Connection to port56
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    return conn.error_and_return("Unable to connect with: " + server_hostname + " and " + argv[2], -1);
  }

  Message return_msg;
  
  // Send slogin message
  bool result_of_last_connection = conn.send(Message(TAG_SLOGIN, username));
  if (!result_of_last_connection) {
    return conn.error_and_return("ERROR: unable to login with username " + username, -1);
  }
  
  // read response: If not ok:
  result_of_last_connection = conn.receive(return_msg);
  if (return_msg.tag.compare(TAG_ERR) == 0) {
    return conn.error_and_return(return_msg.data, -1);
  }

  string input;
  while (1) {
    int result = while_loop(input, result_of_last_connection, conn, return_msg);
    if (result != 1) {
      return result;
    }
  }

  return cleanup(conn);
}
