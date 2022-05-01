#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <sstream>

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  pthread_mutex_init(&lock, NULL); //initialize mutex
}

Room::~Room() {
  pthread_mutex_destroy(&lock); //destroy mutex
}

void Room::add_member(User *user) {
  Guard g(lock); //lock mutex
  members.insert(user); //add user to user set
}

void Room::remove_member(User *user) {
  Guard g(lock); //lock mutex
  members.erase(user); //remove user from user set
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  Guard g(lock); //lock mutex
  for (User * member : members) { //iterate through all users
    if (sender_username.compare(member->username) != 0) { //if sender isn't the current member, send the message
      std::stringstream ss;
      ss << room_name << ':' << sender_username << ':' << message_text; //get message text into ss
      Message * toSend = new Message(TAG_DELIVERY, ss.str()); //set message to stringstream
      member->mqueue->enqueue(toSend); //enqueue this message 
    }
  }

}
