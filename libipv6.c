
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_exceptions.h"
#include "arpa/inet.h"
#include "php_libipv6.h"



static int le_libipv6;

const zend_function_entry libipv6_functions[] = {
	PHP_FE_END
};


zend_module_entry libipv6_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"libipv6",
	libipv6_functions,
	PHP_MINIT(libipv6),
	PHP_MSHUTDOWN(libipv6),
	PHP_RINIT(libipv6),	
	PHP_RSHUTDOWN(libipv6),
	PHP_MINFO(libipv6),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_LIBIPV6_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_LIBIPV6
ZEND_GET_MODULE(libipv6)
#endif





PHP_MINIT_FUNCTION(libipv6)
{
	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(libipv6)
{
	return SUCCESS;
}



PHP_RINIT_FUNCTION(libipv6)
{
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(libipv6)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(libipv6)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "libipv6 support", "enabled");
	php_info_print_table_end();

}

#include "ipv6.c"




