
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


// functions Arguments
ZEND_BEGIN_ARG_INFO(arginfo_ipv6_address, 0)
	ZEND_ARG_INFO(0, address)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arginfo_ipv6_addresses2, 0)
	ZEND_ARG_INFO(0, address1)
    ZEND_ARG_INFO(0, address2)
ZEND_END_ARG_INFO()

//---------------------------------------------------

static int le_libipv6;


const zend_function_entry libipv6_functions[] = {

    // IPv6 functions
    PHP_FE(is_valid_ipv6, arginfo_ipv6_address)
    PHP_FE(get_full_ipv6, arginfo_ipv6_address)
    PHP_FE(get_short_ipv6, arginfo_ipv6_address)
    PHP_FE(get_next_ipv6, arginfo_ipv6_address)
    PHP_FE(get_prev_ipv6, arginfo_ipv6_address)
    PHP_FE(compare_ipv6, arginfo_ipv6_addresses2)
    PHP_FE(get_common_bits, arginfo_ipv6_address)
    PHP_FE(is_ipv4_mapped_ipv6, arginfo_ipv6_address)
    
    PHP_FE(is_valid_ipv6_network, arginfo_ipv6_address)
    
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
    zend_class_entry tmp_ce;
    
    memcpy(&ipv6_address_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    ipv6_address_object_handlers.clone_obj = NULL;
    INIT_CLASS_ENTRY(tmp_ce, IPV6_ADDRESS_CLASS_NAME, ipv6_address_functions);
    ipv6_address_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    ipv6_address_ce->create_object = ipv6_address_create_object_handler;
    
    
    memcpy(&ipv6_range_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    ipv6_range_object_handlers.clone_obj = NULL;
    INIT_CLASS_ENTRY(tmp_ce, IPV6_RANGE_CLASS_NAME, ipv6_range_functions);
    ipv6_range_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    ipv6_range_ce->create_object = ipv6_range_create_object_handler;
    
    
    memcpy(&ipv6_network_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    ipv6_network_object_handlers.clone_obj = NULL;
    INIT_CLASS_ENTRY(tmp_ce, IPV6_NETWORK_CLASS_NAME, ipv6_network_functions);
    tmp_ce.create_object = ipv6_range_create_object_handler;
    ipv6_network_ce = zend_register_internal_class_ex(&tmp_ce, ipv6_range_ce, NULL TSRMLS_CC);
    
    
    memcpy(&ipv6_networkmask_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    ipv6_networkmask_object_handlers.clone_obj = NULL;
    INIT_CLASS_ENTRY(tmp_ce, IPV6_NETWORKMASK_CLASS_NAME, ipv6_networkmask_functions);
    tmp_ce.create_object = ipv6_range_create_object_handler;
    ipv6_networkmask_ce = zend_register_internal_class_ex(&tmp_ce, ipv6_network_ce, NULL TSRMLS_CC);
    
    
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




