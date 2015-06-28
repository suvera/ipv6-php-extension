#define IPV6_ADDRESS_CLASS_NAME "IPv6Address"
zend_class_entry *ipv6_address_ce;


typedef struct _ipv6_address_object {
    zend_object std;
    ipv6_address addr;
} ipv6_address_object;


PHP_METHOD(IPv6Address, __construct);
PHP_METHOD(IPv6Address, __toString);
PHP_METHOD(IPv6Address, getFullAddress);
PHP_METHOD(IPv6Address, nextAddress);
PHP_METHOD(IPv6Address, prevAddress);
PHP_METHOD(IPv6Address, compare);
PHP_METHOD(IPv6Address, commonBits);
PHP_METHOD(IPv6Address, add);
PHP_METHOD(IPv6Address, subtract);

PHP_METHOD(IPv6Address, maskWithNetworkMask);

PHP_METHOD(IPv6Address, fromString);



zend_object_value ipv6_address_create_object_handler(zend_class_entry *class_type TSRMLS_DC);
static zend_object_handlers ipv6_address_object_handlers;


zend_function_entry ipv6_address_functions[] = {
    // PHP Class Functions
    PHP_ME(IPv6Address, __construct, NULL, ZEND_ACC_PROTECTED|ZEND_ACC_CTOR)
    PHP_ME(IPv6Address, __toString, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Address, getFullAddress, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, nextAddress, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, prevAddress, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, compare, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, commonBits, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, add, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Address, subtract, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Address, maskWithNetworkMask, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Address, fromString, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

    {NULL, NULL, NULL}    /* Must be the last line in ipv6 functions[] */
};
