#include "Error.h"
#include <cstring>
#include <cstdio>

UnixError::UnixError(int errnum)
   : num(errnum)
{
   // prepend the "[Error %d] " string, store a pointer to its end
   snprintf(buf, ERRNO_STRLEN, "[Error %d] ", errnum);
   int size = strlen(buf);
   char *dst = buf + size;

   // add the error description
   const char *msg = strerror_r(errnum, dst, ERRNO_STRLEN - size);
   if (msg != dst) {
      strncpy(dst, msg, ERRNO_STRLEN - size);
      buf[ERRNO_STRLEN-1] = 0;
   }
}

const char *UnixError::what()
{
   return buf;
}

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

