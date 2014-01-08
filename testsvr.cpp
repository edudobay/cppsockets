#include <iostream>
#include "TCPSocket.h"
#include "Error.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
   if (argc < 2)
      return 1;

   try {
      TCPSocket sock = TCPSocket::forFamily(IPV4);

      cout << "Will listen on TCP port " << argv[1] << endl;
      InetEndPoint local = InetEndPoint::queryOne(NULL, argv[1]);
      cout << "Created local endpoint" << endl;
      sock.bind(local);
      cout << "Listener socket bound" << endl;

      sock.listen(1);
      cout << "Listener socket set to listen" << endl;

      {
         shared_ptr<InetEndPoint> remote;
         shared_ptr<TCPSocket> p_client = sock.accept(remote);
         TCPSocket& client = *p_client;
         cout << "Remote connection from " << remote->toString() << endl;

         client.send("Hello, dear customer!\n");
         while (true) {
            string data = client.recv(50);
            client.send("Well done!\n");
            cout << "Received data\n\033[37;40m" << data << "\033[0m" << endl;

            if (data == ".\n")
               break;
         }


         cout << "Disconnecting." << endl;
         client.shutdown(Write);

         cout << "Closing." << endl;
         client.close();
      }

      cout << "Shutting down listener." << endl;
      sock.shutdown(ReadWrite);

      cout << "Closing listener." << endl;
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

