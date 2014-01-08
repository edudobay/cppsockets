#include "Poll.h"

int poll (std::vector<pollfd>& fds, int timeout_ms)
{
   return ::poll(fds.data(), fds.size(), timeout_ms);
}

