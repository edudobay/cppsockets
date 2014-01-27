#include "Poll.h"
#include "Error.h"

namespace cppsockets {

int poll (std::vector<pollfd>& fds, int timeout_ms)
{
   int result = ::poll(fds.data(), fds.size(), timeout_ms);
   // pass-through signal interrupt 'error'
   if (result == -1 && errno != EINTR) {
      SOCKET_ERROR(errno, "poll");
   }
   return result;
}

} // namespace cppsockets
