#include <poll.h>
#include <vector>

int poll (std::vector<pollfd> fds, int timeout_ms)
{
   return ::poll(fds.data(), fds.size(), timeout_ms);
}

