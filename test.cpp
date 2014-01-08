#include <iostream>
#include "TCPSocket.h"
#include "Error.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
   if (argc < 3)
      return 1;

   try {
      TCPSocket sock = TCPSocket::forFamily(IPV4);

      cout << "Connecting to " << argv[1] << " on port " << argv[2] << endl;
      sock.connect(argv[1], argv[2]);

      cout << "Sending some data." << endl;
      sock.send("Hello world!\n");

      cout << "Receiving some data." << endl;
      string data = sock.recv(50);
      cout << "\033[37;40m" << data << "\033[0m" << endl;

      cout << "Disconnecting." << endl;
      sock.shutdown(Write);

      cout << "Closing." << endl;
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

