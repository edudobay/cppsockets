#include "TCPSocket.h"
#include "Error.h"

#include <errno.h>

TCPSocket::TCPSocket(socket_t s)
   : Socket(s)
{}

TCPSocket::TCPSocket(int family, int socktype, int protocol)
   : Socket(family, socktype, protocol)
{}

TCPSocket TCPSocket::forFamily(int family)
{
   return TCPSocket(family, SOCK_STREAM, 0);
}

TCPSocket::TCPSocket(addrinfo *addr)
{
   addrinfo *p;

   for (p = addr; p; p = p->ai_next) {
      descriptor = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
      if (descriptor != -1) {
         return;
      }
   }

   if (descriptor == -1) {
      SOCKET_ERROR(errno, "socket");
   }
}

void TCPSocket::connect (const char *hostname, const char *service)
{
   InetEndPoint ep = InetEndPoint::queryOne (hostname, service, protocol_family);

   if (::connect(descriptor, ep.getSockAddr(), ep.getSockAddrLen()) == -1) {
      SOCKET_ERROR(errno, "connect");
   }
}

void TCPSocket::connect (const char *hostname, unsigned short port)
{
   // convert the port to a string
   char port_s[7];
   snprintf(port_s, 7, "%hu", port);

   connect(hostname, port_s);
}

void TCPSocket::bind (const InetEndPoint &ep)
{
   Socket::bind (ep);
}

shared_ptr<TCPSocket> TCPSocket::accept (shared_ptr<InetEndPoint> &endpoint)
{
   sockaddr_storage sa;
   socklen_t sa_size = sizeof sa;

   socket_t client = ::accept(descriptor, reinterpret_cast<sockaddr*> (&sa), &sa_size);
   if (client == -1) {
      SOCKET_ERROR(errno, "accept");
   }

   endpoint.reset(new InetEndPoint(reinterpret_cast<sockaddr*> (&sa), sa_size));
   return shared_ptr<TCPSocket>(new TCPSocket(client));
}

