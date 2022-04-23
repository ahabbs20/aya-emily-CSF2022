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

Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  //get port
  std::string p = std::to_string(port);

  int fd = open_clientfd(hostname.c_str(), p.c_str());
  if (fd < 0) {
    //if error code returned print error to cerr
    std::cerr << "ERROR: Unable to connect to server";
  } 

  m_fd = fd;
  rio_readinitb(&m_fdbuf, m_fd);
}


Connection::~Connection() {
  close();
}


bool Connection::is_open() const {
  return m_fd >= 0;
}

void Connection::close() {
  if (is_open()) {
    Close(m_fd); 
  }
}

bool Connection::send(const Message &msg) {
  std::string payload = msg.to_string(); 
  ssize_t result = rio_writen(m_fd, payload.c_str(), payload.length()); 

  if (result != (signed int) payload.length()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
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
    //if invalid message, set tag to err and data to the string below
    //return false
    msg.tag = TAG_ERR;
    msg.data = "ERROR: Invalid message";
    return false;
  } else {
    //if valid message, set tag to the substring up to first :
    //set data to the rest of the string (up until null terminator) and return false
    m_last_result = SUCCESS;
    msg.tag = message.substr(0, loc_of_tag);
    msg.data = message.substr(loc_of_tag + 1);
    return true;
  }
}

//print error to cerror and return the given return value
int Connection::error_and_return(std::string to_print, int return_val) {
  std::cerr << to_print << std::endl;
  return return_val;
}