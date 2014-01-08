#include <poll.h>
#include <vector>
#include "TCPSocket.h"

typedef short int poll_events_t;

// XXX: Workaround for the fact that we implemented TCP sockets directly
// without a parent Socket class
typedef TCPSocket Socket;

class SocketSet : protected std::vector<pollfd> {
public:
   SocketSet();
   // SocketSet(int size); // initial size

   void add(const Socket& socket, poll_events_t events);
   void replace(const Socket& socket, poll_events_t events);
   void remove(const Socket& socket);
   void removeAll();

   poll_events_t check(const Socket& socket);

   int poll(int timeout_ms);

};

SocketSet::SocketSet()
{}

void SocketSet::add (const Socket& socket, poll_events_t events)
{
   pollfd elem = {.fd = socket, .events = events, .revents = 0};
   push_back(elem);
}

// replace the events selector for a given socket
// XXX: if it is not found in this set, a new entry is added
void SocketSet::replace (const Socket& socket, poll_events_t events)
{
   for (pollfd& elem : *this) {
      if (elem.fd == socket) {
         elem.events = events;
         return;
      }
   }

   // add the element if it was not found
   add (socket, events);
}

void SocketSet::remove (const Socket& socket)
{
   for (auto it = begin(); it != end(); ++it) {
      pollfd& elem = *it;
      if (elem.fd == socket) {
         erase(it);
         return;
      }
   }

   // not found
   // what TODO? nothing?
}

void SocketSet::removeAll ()
{
   clear();
}

poll_events_t SocketSet::check (const Socket& socket)
{
   for (pollfd& elem : *this) {
      if (elem.fd == socket) {
         return elem.revents;
      }
   }

   // not found
   // what TODO?
   return 0;
}

int SocketSet::poll (int timeout_ms)
{
   return ::poll(this->data(), this->size(), timeout_ms);
}

