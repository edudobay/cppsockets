#ifndef __ENDPOINT_H
#define __ENDPOINT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::shared_ptr;
using std::vector;

class EndPoint {
protected:
   sockaddr_storage sa;
   socklen_t sa_len = 0;

   EndPoint();

public:
   EndPoint(sockaddr *address, socklen_t address_len);
   virtual int getAddressFamily() const;
   virtual const sockaddr *getSockAddr() const;
   virtual socklen_t getSockAddrLen() const;
};

// ---------------------------------------------------------------------------

typedef unsigned short ip_port_t;

class InetEndPoint : public EndPoint {
public:
   InetEndPoint(addrinfo *ai);
   InetEndPoint(sockaddr *address, socklen_t address_len);

   static addrinfo *queryAddrInfo (const char *hostname, const char *service, int family);
   static vector<InetEndPoint> query (const char *hostname, const char *service, int family = AF_UNSPEC);
   static InetEndPoint queryOne (const char *hostname, const char *service, int family = AF_UNSPEC);
   static InetEndPoint queryOne (const char *hostname, ip_port_t port, int family = AF_UNSPEC);

   void getAddressAndPort (string&, ip_port_t&) const;
   string getAddressAndPort (ip_port_t& port) const;
   string getAddress () const;
   string toString () const;


};

string getAddressAndPort (int family, const sockaddr *sa, ip_port_t& port);
string sockaddrParse (const sockaddr *sa, ip_port_t& port);
string addrinfoParse (const addrinfo *addr, ip_port_t& port);

// ---------------------------------------------------------------------------

class UnixEndPoint : public EndPoint {
   UnixEndPoint(const char *path);
   UnixEndPoint(const char *path, int path_len);
   UnixEndPoint(sockaddr *address, socklen_t address_len);
};

#endif /* __ENDPOINT_H */
