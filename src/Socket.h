#ifndef __SOCKET_H
#define __SOCKET_H

#include "EndPoint.h"

#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <string>

typedef int socket_t;

using std::shared_ptr;
using std::string;

enum class ShutdownMode {
   Read = SHUT_RD,
   Write = SHUT_WR,
   ReadWrite = SHUT_RDWR
};

class Socket {
protected:
   socket_t descriptor = -1;

   Socket();
   Socket(socket_t s);

   int protocol_family;
   int type;
   int protocol;

public:
   Socket(int family, int socktype, int protocol);
   Socket(EndPoint& ep, int socktype, int protocol);
   //Socket(addrinfo *addr);

   void connect (const EndPoint& endpoint);
   void bind (const EndPoint& endpoint);
   void listen (int num);
   virtual shared_ptr<Socket> accept(shared_ptr<EndPoint> &endpoint);
   void shutdown (ShutdownMode mode);
   void close ();

   int send (const char *data, size_t length);
   int send (string str);
   int send_all (const char *data, size_t length);
   int send_all (string str);

   int recv (char *data, size_t length);
   string recv (size_t length);

   operator socket_t () const;
};

#endif /* __SOCKET_H */
