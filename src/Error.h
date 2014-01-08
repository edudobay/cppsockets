#ifndef __ERROR_H
#define __ERROR_H

#include <exception>

using std::exception;

#define ERRNO_STRLEN 512

class UnixError : public exception {
protected:
   char buf[ERRNO_STRLEN];
   int num;

public:
   UnixError(int errnum);
   const char *what();
};

class NameResolutionError : public exception {
protected:
   char *description;

public:
   NameResolutionError(const char *);
   const char *what();
};

#endif /* __ERROR_H */
