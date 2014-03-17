#ifndef __CPPSOCKETS__ERROR_H
#define __CPPSOCKETS__ERROR_H

#include <exception>
#include <string>

namespace cppsockets {

using std::exception;
using std::string;

#define ERRNO_STRLEN 512

// ---------------------------------------------------------------------------

class ITracksSource {
protected:
   string func, filename;
   int line;

public:
   ITracksSource(const char *func, const char *filename, int line);
   string getSourceFunction() const;
   string getSourceFilename() const;
   int getSourceLine();
};

// ---------------------------------------------------------------------------

class UnixError : public exception, public ITracksSource {
protected:
   int num;
   string failed_func;
   char error_str[ERRNO_STRLEN];
   string description;

public:
   UnixError(int errnum, const char *failed_func,
         const char *source_func, const char *filename, int line);
   const char *what() const noexcept;
   const char *source() const noexcept;
   int number() const noexcept;
};

// ---------------------------------------------------------------------------

class NameResolutionError : public exception {
protected:
   char *description;

public:
   NameResolutionError(const char *);
   const char *what();
};

// ---------------------------------------------------------------------------

#define SOCKET_ERROR(num, func) throw UnixError(num, func, __PRETTY_FUNCTION__, __FILE__, __LINE__)

} // namespace cppsockets

#endif /* __CPPSOCKETS__ERROR_H */
