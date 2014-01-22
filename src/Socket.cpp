#include "Socket.h"
#include "Error.h"

#include <errno.h>

Socket::Socket()
   : descriptor(-1)
{}

Socket::Socket(socket_t s)
   : descriptor(s)
{}

Socket::Socket(int family, int socktype, int protocol)
   : protocol_family(family), type(socktype), protocol(protocol)
{
   descriptor = socket(family, socktype, protocol);

   if (descriptor == -1) {
      SOCKET_ERROR(errno, "socket");
   }
}

Socket::Socket(EndPoint& endpoint, int socktype, int protocol)
{
   descriptor = socket(endpoint.getAddressFamily(),
         socktype, protocol);

   if (descriptor == -1) {
      SOCKET_ERROR(errno, "socket");
   }

   connect(endpoint);
}

void Socket::connect (const EndPoint &endpoint)
{
   if (::connect(descriptor, endpoint.getSockAddr(), endpoint.getSockAddrLen()) == -1) {
      SOCKET_ERROR(errno, "connect");
   }
}

void Socket::bind (const EndPoint& endpoint)
{
   if (::bind(descriptor, endpoint.getSockAddr(), endpoint.getSockAddrLen()) == -1) {
      SOCKET_ERROR(errno, "bind");
   }
}

void Socket::listen (int num)
{
   if (::listen(descriptor, num) == -1) {
      SOCKET_ERROR(errno, "listen");
   }
}

shared_ptr<Socket> Socket::accept (shared_ptr<EndPoint> &endpoint)
{
   sockaddr_storage sa;
   socklen_t sa_size = sizeof sa;

   socket_t client = ::accept(descriptor, reinterpret_cast<sockaddr*> (&sa), &sa_size);
   if (client == -1) {
      SOCKET_ERROR(errno, "accept");
   }

   endpoint.reset(new EndPoint(reinterpret_cast<sockaddr*> (&sa), sa_size));
   return shared_ptr<Socket>(new Socket(client));
}

void Socket::shutdown (ShutdownMode mode)
{
   if (::shutdown(descriptor, static_cast<int>(mode)) == -1) {
      SOCKET_ERROR(errno, "shutdown");
   }
}

void Socket::close ()
{
   if (::close(descriptor) == -1) {
      SOCKET_ERROR(errno, "close");
   }
}

int Socket::send (const char *data, size_t length)
{
   int nbytes = ::send(descriptor, data, length, 0);
   if (nbytes == -1) {
      SOCKET_ERROR(errno, "send");
   }
   return nbytes;
}

int Socket::send (string str)
{
   return send(str.c_str(), str.length());
}

int Socket::send_all (const char *data, size_t length)
{
   size_t sent = 0;
   const char *p = data;
   while (sent < length) {
      sent += send(p, length - sent);
      p = data + sent;
   }
   return sent;
}

int Socket::send_all (string str)
{
   return send_all(str.c_str(), str.length());
}

int Socket::recv (char *data, size_t length)
{
   int nbytes = ::recv(descriptor, data, length, 0);
   if (nbytes == -1) {
      SOCKET_ERROR(errno, "recv");
   }
   return nbytes;
}

string Socket::recv (size_t length)
{
   char buf[length];
   int size = recv (buf, length);
   return string(buf, size);
}

Socket::operator socket_t () const
{
   return descriptor;
}

