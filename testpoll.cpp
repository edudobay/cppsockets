#include <iostream>
#include "TCPSocket.h"
#include "Error.h"
#include "Poll.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
   if (argc < 2)
      return 1;

   try {
      TCPSocket sock = TCPSocket::forFamily(IPV4);
      InetEndPoint local = InetEndPoint::queryOne(NULL, argv[1]);
      sock.bind(local);
      sock.listen(5);
      cout << "Listening on TCP port " << argv[1] << "..." << endl;

      {
         vector<shared_ptr<TCPSocket> > clients;
         vector<pollfd> fds;

         fds.push_back({.fd = sock, .events = POLLIN, .revents = 0});

         bool shutdown = false;
         while (!shutdown) {
            int nevents = poll(fds, 1000);
            if (nevents <= 0)
               continue;

            // new connection for the server to accept
            if (fds[0].revents & POLLIN) {
               shared_ptr<InetEndPoint> remote;
               shared_ptr<TCPSocket> p_client = sock.accept(remote);
               TCPSocket& client = *p_client;
               cout << "Remote connection from " << remote->toString() << endl;

               clients.push_back(p_client);
               fds.push_back({.fd = client, .events = POLLIN | POLLOUT, .revents = 0});
            }

            for (auto it = fds.begin() + 1; it != fds.end(); ++it) {
               pollfd& fd = *it;
               auto client_it = (it - fds.begin() - 1 + clients.begin());
               shared_ptr<TCPSocket> client = *client_it;

               if (fd.revents & POLLIN) {
                  string data = client->recv(50);
                  cout << "Received data\n\033[37;40m" << data << "\033[0m" << endl;

                  if (data == ".\n") {
                     cout << "Closing this client connection" << endl;
                     client->shutdown(ReadWrite);
                     client->close();

                     // preserve old iterator to continue the loop
                     auto old_it = it - 1;
                     fds.erase(it);
                     clients.erase(client_it);
                     it = old_it;
                  }
                  else if (data == "!\n") {
                     shutdown = true;
                  }
               }
            }

            if (shutdown) {
               for (auto it = clients.begin(); it != clients.end(); ++it) {
                  shared_ptr<TCPSocket> client = *it;
                  client->shutdown(ReadWrite);
                  client->close();
               }
               clients.clear();
            }
         }

      }

      cout << "Shutting down listener." << endl;
      sock.shutdown(ReadWrite);

      sock.close();
   }
   catch (UnixError& ex) {
      cout << "UnixError: " << ex.what() << endl;
   }
   catch (NameResolutionError& ex) {
      cout << "NameResolutionError: " << ex.what() << endl;
   }

   return 0;
}

