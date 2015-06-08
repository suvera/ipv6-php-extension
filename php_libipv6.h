
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

#include "stdlib.h"
#include "inttypes.h"


#define IPV6_TOTAL_BITS 128
#define IPV6_HEX_CHAR_LEN 32
#define IPV6_MAX_CHAR_LEN 39
#define IPV6_NUM_ZONES 8
#define IPV6_NUM_COLONS 7
#define IPV6_ZONE_INT 2
#define IPV6_HEX_ZONE_LEN 16
#define IPV6_ZONE_HEX_FORMAT "%016lx"
#define IPV6_ZONE_BIT_CHECKER 1UL
#define IPV6_INT_MAX UINT64_MAX
#define IPV6_INT uint64_t

#define STR_TO_INT(start, end, radix) \
    strtoull(start, end, radix)

typedef struct _ipv6_address {
    IPV6_INT zone[IPV6_ZONE_INT];
    uint8_t isIPv4;
} ipv6_address;

typedef struct _ipv6_range {
    ipv6_address start;
    ipv6_address end;
    uint8_t isNetwork;
    uint8_t networkBits;
} ipv6_range;


IPV6_INT applyBitMask(int n, IPV6_INT a);

IPV6_INT undoBitMask(int n, IPV6_INT a);

int testAnyBitSet(int n, IPV6_INT a);

int copyIPv6Zone(ipv6_address *from, ipv6_address *to TSRMLS_DC);

PHPAPI int ipv6StringToStruct(char* ip, ipv6_address* addr TSRMLS_DC);

PHPAPI int ipv6StuctToString(ipv6_address* addr, char* ip TSRMLS_DC);

PHPAPI int ipv6StuctToStringFull(ipv6_address* addr, char* ip TSRMLS_DC);

PHPAPI int getNextIPv6Struct(ipv6_address* addr, ipv6_address* next TSRMLS_DC);

PHPAPI int getPrevIPv6Struct(ipv6_address* addr, ipv6_address* prev TSRMLS_DC);

PHPAPI int compareIPv6Structs(ipv6_address* x, ipv6_address* y TSRMLS_DC);

PHPAPI int getCommonBitsIPv6Structs(ipv6_address* x, ipv6_address* y, int* bits TSRMLS_DC);

PHPAPI int ipv6NetworkToStruct(char* ip, ipv6_range* range, int strict TSRMLS_DC);

PHPAPI int ipv6RangeToStruct(char* ip1, char* ip2, ipv6_range* range TSRMLS_DC);

PHPAPI int inIPv6Range(ipv6_address* ipv6, ipv6_range* range TSRMLS_DC);

PHPAPI int isIPv6SubRangeOf(ipv6_range* subRange, ipv6_range* range TSRMLS_DC);

PHPAPI int getIPv6RangeIntersect(ipv6_range* range1, ipv6_range* range2, ipv6_range* result TSRMLS_DC);

PHPAPI int ipv6RangeMerge(ipv6_range* range1, ipv6_range* range2, ipv6_range* result TSRMLS_DC);

PHPAPI int IncrIPv6Struct(ipv6_address* addr, ipv6_address* result, long incr TSRMLS_DC);

PHPAPI int decrIPv6Struct(ipv6_address* addr, ipv6_address* result, long number TSRMLS_DC);



PHP_MINIT_FUNCTION(libipv6);
PHP_MSHUTDOWN_FUNCTION(libipv6);
PHP_RINIT_FUNCTION(libipv6);
PHP_RSHUTDOWN_FUNCTION(libipv6);
PHP_MINFO_FUNCTION(libipv6);



PHP_FUNCTION(is_valid_ipv6);
PHP_FUNCTION(get_full_ipv6);
PHP_FUNCTION(get_short_ipv6);
PHP_FUNCTION(get_next_ipv6);
PHP_FUNCTION(get_prev_ipv6);
PHP_FUNCTION(compare_ipv6);
PHP_FUNCTION(get_common_bits);
PHP_FUNCTION(is_ipv4_mapped_ipv6);

PHP_FUNCTION(is_valid_ipv6_network);



#include "IPv6Address.h"

#include "IPv6Range.h"

#ifdef ZTS
#define LIBIPV6_G(v) TSRMG(libipv6_globals_id, zend_libipv6_globals *, v)
#else
#define LIBIPV6_G(v) (libipv6_globals.v)
#endif

#endif

