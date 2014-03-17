#include "Error.h"
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>

namespace cppsockets {

using std::stringstream;

ITracksSource::ITracksSource (const char *func, const char *filename, int line)
   : func(func), filename(filename), line(line)
{
}

UnixError::UnixError(int errnum, const char *failed_func, const char *source_func, const char *filename, int line)
   : ITracksSource(source_func, filename, line),
     num(errnum), failed_func(failed_func)
{
   // add the error description
   const char *msg = strerror_r(errnum, error_str, ERRNO_STRLEN);
   if (msg != error_str) {
      strncpy(error_str, msg, ERRNO_STRLEN);
   }
}

const char *UnixError::what() const noexcept
{
   stringstream ss;
   ss << "[Error " << num << "] " << error_str;
   ss << "; " << source();
   return ss.str().c_str();
}

const char *UnixError::source() const noexcept
{
   stringstream ss;
   ss << failed_func << " failed at " << func << "(" << filename << ":" << line << ")";
   return ss.str().c_str();
}

int UnixError::number() const noexcept {
   return num;
}

// ---------------------------------------------------------------------------

NameResolutionError::NameResolutionError(const char *message)
{
   int len = strlen(message);
   description = new char[len + 1];
   strcpy(description, message);
   description[len] = 0;
}

const char *NameResolutionError::what()
{
   return description;
}

} // namespace cppsockets
