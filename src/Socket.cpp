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
      throw UnixError(errno);
   }
}

Socket::Socket(EndPoint& endpoint, int socktype, int protocol)
{
   descriptor = socket(endpoint.getAddressFamily(),
         socktype, protocol);

   if (descriptor == -1) {
      throw UnixError(errno);
   }

   connect(endpoint);
}

void Socket::connect (const EndPoint &endpoint)
{
   if (::connect(descriptor, endpoint.getSockAddr(), endpoint.getSockAddrLen()) == -1) {
      throw UnixError(errno);
   }
}

void Socket::bind (const EndPoint& endpoint)
{
   if (::bind(descriptor, endpoint.getSockAddr(), endpoint.getSockAddrLen()) == -1) {
      throw UnixError(errno);
   }
}

void Socket::listen (int num)
{
   if (::listen(descriptor, num) == -1) {
      throw UnixError(errno);
   }
}

shared_ptr<Socket> Socket::accept (shared_ptr<EndPoint> &endpoint)
{
   sockaddr_storage sa;
   socklen_t sa_size = sizeof sa;

   socket_t client = ::accept(descriptor, reinterpret_cast<sockaddr*> (&sa), &sa_size);
   if (client == -1) {
      throw UnixError(errno);
   }

   endpoint.reset(new EndPoint(reinterpret_cast<sockaddr*> (&sa), sa_size));
   return shared_ptr<Socket>(new Socket(client));
}

void Socket::shutdown (ShutdownMode mode)
{
   if (::shutdown(descriptor, static_cast<int>(mode)) == -1) {
      throw UnixError(errno);
   }
}

void Socket::close ()
{
   if (::close(descriptor) == -1) {
      throw UnixError(errno);
   }
}

int Socket::send (const char *data, size_t length)
{
   int nbytes = ::send(descriptor, data, length, 0);
   if (nbytes == -1) {
      throw UnixError(errno);
   }
   return nbytes;
}

int Socket::send (string str)
{
   return send(str.c_str(), str.length());
}

int Socket::recv (char *data, size_t length)
{
   int nbytes = ::recv(descriptor, data, length, 0);
   if (nbytes == -1) {
      throw UnixError(errno);
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

