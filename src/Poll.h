#ifndef __POLL_H
#define __POLL_H

#include <poll.h>
#include <vector>

int poll (std::vector<pollfd>& fds, int timeout_ms);

#endif /* __POLL_H */
