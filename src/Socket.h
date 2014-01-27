#ifndef __CPPSOCKETS__SOCKET_H
#define __CPPSOCKETS__SOCKET_H

#include "EndPoint.h"

#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <string>

namespace cppsockets {

typedef int socket_t;

using std::shared_ptr;
using std::string;

enum class ShutdownMode {
   Read = SHUT_RD,
   Write = SHUT_WR,
   ReadWrite = SHUT_RDWR
};

enum class BlockingMode {
   Blocking,
   NonBlocking
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
   ~Socket();

   void connect (const EndPoint& endpoint);
   void bind (const EndPoint& endpoint);
   void listen (int num);
   virtual shared_ptr<Socket> accept(shared_ptr<EndPoint> &endpoint);
   void shutdown (ShutdownMode mode);
   void close ();

   void setBlockMode (BlockingMode mode);

   int send (const char *data, size_t length);
   int send (string str);
   int send_all (const char *data, size_t length);
   int send_all (string str);

   int recv (char *data, size_t length);
   string recv (size_t length);

   operator socket_t () const;
};

} // namespace cppsockets

#endif /* __CPPSOCKETS__SOCKET_H */
