#ifndef __CPPSOCKETS__POLL_H
#define __CPPSOCKETS__POLL_H

#include <poll.h>
#include <vector>

namespace cppsockets {

int poll (std::vector<pollfd>& fds, int timeout_ms);

} // namespace cppsockets

#endif /* __CPPSOCKETS__POLL_H */
