#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"

MessageQueue::MessageQueue() {
  //intiialize mutex and semaphore
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, 0);
}

MessageQueue::~MessageQueue() {
  //destroy mutex and semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  Guard g(m_lock); //lock mutex using guard
  m_messages.push_back(msg); //add message to queue
  sem_post(&m_avail); //post semaphore (add resource)
}

Message *MessageQueue::dequeue() {
  sem_wait(&m_avail); //semaphore will wait until something is in the queue

  Guard g(m_lock); //lock mutex using guard
  if (!m_messages.empty()) { //if m_messages is not empty
    Message *msg = m_messages.front(); //get front message
    m_messages.pop_front(); //remove front
    return msg;
  }

  return nullptr; //return nullptr if nothing to return
}
