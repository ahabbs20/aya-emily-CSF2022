Sample README.txt

Eventually your report about how you implemented thread synchronization
in the server should go here

Since synchronization is an important part of this assignment, we'd 
like you to support a report on your synchronization in your README.txt. 
Please include:
    - where your critical sections are, how you determined them, 
    - why you chose the synchronization primitives for each sections. 
    - You should also explain how your critical sections ensure that the synchronization requires 
    are met without introducing synchronization hazards (e.g. race conditions and deadlocks).

Locations + synchronization primitives:
    server.cpp: mutexes
    find_or_create_room

    room.cpp: mutexes
    add
    remove
    broadcast_message

    message_queue: semaphores + mutexes
    enqueue
    dequeue 

Explanation:
    For server.cpp and room.cpp, we used mutexes for find_or_create_room, add, remove, and broadcast_message. We wanted these to be critical regions
    since we wanted to guarantee atomicity for a given thread. For example, if a thread gained access to add user as another thread was removing a user, 
    the correct behaviour is not guaranteed. Thus, regions involving room and users, we generally tried to maintain critical regions around. 
    The reason we used mutexes for these functions is because we had unique resources that cannot be executed by
    multiple threads at the same time. For instead, in room.cpp:add and remove, if multiple threads were able to modify
    user lists, we would run into issues. This idea holds for all other functions which use mutexes. 

    For message_queue.cpp we used mutexes and semaphores for both enqueue and dequeue. First, one main distinction is that there is a shared resource,
    which implies we should use semaphores. In deque, we use semaphores in order to make dequeue wait if there is nothing in the queue (wait until there is something to dequeue). 
    In enqueue we utilize semaphores in order to indicate that an item is available. We still want to use mutexes for message_queue.cpp becasue the correct behavior isn't guaranteed if 
    one thread enqueues and another dequeues at the same time. 

    To avoid deadlocks, we primarily used Guard objects. This why, we did not need to worry about locking and unlocking, so we avoided self-deadlocks. 
    Additionally, we made sure to maintain order of access. Meaning, we never accesses message_queue and then room or room and then message_queue on two threads respectively.
    This prevented deadlocks as well. 