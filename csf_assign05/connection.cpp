#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <string>

Connection::Connection(): m_fd(-1), m_last_result(SUCCESS) {
}

/*Done*/
Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
   // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

/*done?*/
void Connection::connect(const std::string &hostname, int port) {
  std::string p = std::to_string(port);

  int fd = open_clientfd(hostname.c_str(), p.c_str());
  if (fd < 0) {
    std::cerr << "ERROR: Unable to connect to server";
  } 

  m_fd = fd;
  rio_readinitb(&m_fdbuf, m_fd);
}

/*Done*/
Connection::~Connection() {
  close();
}

/*Done?*/
bool Connection::is_open() const {
  return m_fd >= 0;
}

/*Done*/
void Connection::close() {
  if (is_open()) {
    Close(m_fd); 
  }
}


/*Done*/
bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  std::string payload = msg.to_string(); 
  ssize_t result = rio_writen(m_fd, payload.c_str(), payload.length()); 

  if (result != (signed int) payload.length()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}

/*Done*/
bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and  in msg
  // return true if successful, false if notdata
  // make sure that m_last_result is set appropriately

  // block on tcp socket until can read message
  // read message into message type


  char buffer[255];
  ssize_t i = rio_readlineb(&m_fdbuf, buffer, 255);
  if (i < 1) {
    msg.tag = EOF_OR_ERROR;
    return false;
  }
  
  std::stringstream ss;
  ss << buffer;
  std::string message = trim(ss.str()); // figure out why it's not letting me do this.

  // take input, try to convert to message.
  unsigned int loc_of_tag = message.find_first_of(":");
  if (loc_of_tag == std::string::npos) {
    msg.tag = TAG_ERR;
    msg.data = "ERROR: Invalid message";
    return false;
  } else {
    msg.tag = message.substr(0, loc_of_tag);
    msg.data = message.substr(loc_of_tag + 1);
  }

  m_last_result = SUCCESS;
  return true;
}



