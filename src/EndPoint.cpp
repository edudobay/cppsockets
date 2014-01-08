#include "EndPoint.h"
#include "Error.h"
#include <sys/un.h>
#include <cstring>
#include <errno.h>
#include <sstream>

using std::stringstream;

EndPoint::EndPoint()
{}

EndPoint::EndPoint(sockaddr *address, socklen_t address_len)
{
   memcpy(&sa, address, address_len);
   sa_len = address_len;
}

int EndPoint::getAddressFamily() const
{
   return sa.ss_family;
}

const sockaddr *EndPoint::getSockAddr() const
{
   return reinterpret_cast<const sockaddr*>(&sa);
}

socklen_t EndPoint::getSockAddrLen() const
{
   return sa_len;
}

// ---------------------------------------------------------------------------

InetEndPoint::InetEndPoint (sockaddr *address, socklen_t address_len)
   : EndPoint(address, address_len)
{}

InetEndPoint::InetEndPoint (addrinfo *ai)
   : EndPoint(ai->ai_addr, ai->ai_addrlen)
{}

addrinfo *InetEndPoint::queryAddrInfo (const char *hostname, const char *service, int family)
{
   addrinfo hints;
   addrinfo *address;

   // inform which type of socket we want
   memset(&hints, 0, sizeof hints);
   hints.ai_family = family;
   if (!hostname) // get local 
      hints.ai_flags |= AI_PASSIVE;

   int status = getaddrinfo(hostname, service, &hints, &address);
   if (status != 0) {
      if (status != EAI_SYSTEM) {
         throw NameResolutionError(gai_strerror(status));
      }
      else {
         throw UnixError(errno);
      }
   }

   return address;
}

vector<InetEndPoint> InetEndPoint::query (const char *hostname, const char *service, int family /* = AF_UNSPEC */)
{
   addrinfo *address = queryAddrInfo(hostname, service, family);

   vector<InetEndPoint> results;
   for (addrinfo *p = address; p; p = p->ai_next) {
      results.push_back(InetEndPoint(p));
   }
   freeaddrinfo(address);

   return results;
}

InetEndPoint InetEndPoint::queryOne (const char *hostname, const char *service, int family /* = AF_UNSPEC */)
{
   addrinfo *address = queryAddrInfo (hostname, service, family);
   InetEndPoint endpoint(address);
   freeaddrinfo(address);
   return endpoint;
}

void InetEndPoint::getHostnameAndPort (string& hostname, unsigned short& port) const
{
   hostname.assign (sockaddrParse(reinterpret_cast<const sockaddr*>(&sa), port));
}

string InetEndPoint::toString () const
{
   string hostname;
   unsigned short port;
   getHostnameAndPort (hostname, port);

   stringstream ss;
   ss << hostname << ":" << port;
   return ss.str();
}

string getAddressAndPort (int family, const sockaddr *sa, unsigned short& port)
{
   char address_buf[INET6_ADDRSTRLEN];

   const void *in_addr;
   if (family == AF_INET) {
      const sockaddr_in *ipv4 = reinterpret_cast<const sockaddr_in*> (sa);
      in_addr = &(ipv4->sin_addr);
      port = ipv4->sin_port;
   }
   else { // AF_INET6
      const sockaddr_in6 *ipv6 = reinterpret_cast<const sockaddr_in6*> (sa);
      in_addr = &(ipv6->sin6_addr);
      port = ipv6->sin6_port;
   }

   const char *result = inet_ntop(family, in_addr, address_buf, sizeof address_buf);
   if (!result)
      throw UnixError(errno);

   return string(address_buf);
}

string sockaddrParse (const sockaddr *sa, unsigned short& port)
{
   return getAddressAndPort (sa->sa_family, sa, port);
}

string addrinfoParse (const addrinfo *addr, unsigned short& port)
{
   return getAddressAndPort (addr->ai_family, addr->ai_addr, port);
}

// ---------------------------------------------------------------------------

UnixEndPoint::UnixEndPoint(const char *path)
   : UnixEndPoint(path, strlen(path))
{}

UnixEndPoint::UnixEndPoint(const char *path, int path_len)
{
   sockaddr_un *addr = reinterpret_cast<sockaddr_un*> (&sa);

   addr->sun_family = AF_UNIX;

   int maxSize = sizeof addr->sun_path;
   int len = (path_len > maxSize) ? maxSize : path_len;
   memcpy(addr->sun_path, path, len);

   sa_len = len + reinterpret_cast<char*> (&(addr->sun_path)) - 
      reinterpret_cast<char*> (addr);
}

UnixEndPoint::UnixEndPoint(sockaddr *address, socklen_t address_len)
   : EndPoint(address, address_len)
{}

