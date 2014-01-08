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

class InetEndPoint : public EndPoint {
public:
   InetEndPoint(addrinfo *ai);
   InetEndPoint(sockaddr *address, socklen_t address_len);

   static addrinfo *queryAddrInfo (const char *hostname, const char *service, int family);
   static vector<InetEndPoint> query (const char *hostname, const char *service, int family = AF_UNSPEC);
   static InetEndPoint queryOne (const char *hostname, const char *service, int family = AF_UNSPEC);

   void getHostnameAndPort (string&, unsigned short&) const;
   string toString () const;


};

string getAddressAndPort (int family, const sockaddr *sa, unsigned short& port);
string sockaddrParse (const sockaddr *sa, unsigned short& port);
string addrinfoParse (const addrinfo *addr, unsigned short& port);

// ---------------------------------------------------------------------------

class UnixEndPoint : public EndPoint {
   UnixEndPoint(const char *path);
   UnixEndPoint(const char *path, int path_len);
   UnixEndPoint(sockaddr *address, socklen_t address_len);
};

#endif /* __ENDPOINT_H */
