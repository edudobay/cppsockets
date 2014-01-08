#ifndef __TCPSOCKET_H
#define __TCPSOCKET_H

#include "Socket.h"

enum AddressFamily {
   IPV4 = AF_INET,
   IPV6 = AF_INET6,
};

class TCPSocket : public Socket {
protected:
   TCPSocket(socket_t s);
   TCPSocket(int family, int socktype, int protocol);

public:
   TCPSocket(addrinfo *addr);
   static TCPSocket forFamily(int family);

   // TODO: we have here a dummy argument to make this constructor
   // distinguishable from the socket_t constructor
   TCPSocket(AddressFamily, int);

   // void connect (const InetEndPoint& endpoint);
   void connect (const char *host, unsigned short port);
   void connect (const char *host, const char *service);

   void bind (const InetEndPoint& endpoint);
   void bind (unsigned short port);
   void bind (const char *service);

   shared_ptr<TCPSocket> accept(shared_ptr<InetEndPoint> &endpoint);

};

#endif /* __TCPSOCKET_H */
