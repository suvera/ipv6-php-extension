
#ifndef PHP_LIBIPV6_H
#define PHP_LIBIPV6_H

extern zend_module_entry libipv6_module_entry;
#define phpext_libipv6_ptr &libipv6_module_entry

#define PHP_LIBIPV6_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_LIBIPV6_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_LIBIPV6_API __attribute__ ((visibility("default")))
#else
#	define PHP_LIBIPV6_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(libipv6);
PHP_MSHUTDOWN_FUNCTION(libipv6);
PHP_RINIT_FUNCTION(libipv6);
PHP_RSHUTDOWN_FUNCTION(libipv6);
PHP_MINFO_FUNCTION(libipv6);




#ifdef ZTS
#define LIBIPV6_G(v) TSRMG(libipv6_globals_id, zend_libipv6_globals *, v)
#else
#define LIBIPV6_G(v) (libipv6_globals.v)
#endif

#endif

