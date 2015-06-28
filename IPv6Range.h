#define IPV6_RANGE_CLASS_NAME "IPv6Range"
#define IPV6_NETWORK_CLASS_NAME "IPv6Network"
#define IPV6_NETWORKMASK_CLASS_NAME "IPv6NetworkMask"
zend_class_entry *ipv6_range_ce;
zend_class_entry *ipv6_network_ce;
zend_class_entry *ipv6_networkmask_ce;


typedef struct _ipv6_range_object {
    zend_object std;
    ipv6_range range;
} ipv6_range_object;


PHP_METHOD(IPv6Range, __construct);
PHP_METHOD(IPv6Range, __toString);
PHP_METHOD(IPv6Range, containsAddress);
PHP_METHOD(IPv6Range, containsRange);

PHP_METHOD(IPv6Range, fromString);
PHP_METHOD(IPv6Range, merge);
PHP_METHOD(IPv6Range, intersect);

PHP_METHOD(IPv6Range, getMinAddress);
PHP_METHOD(IPv6Range, getMaxAddress);

zend_object_value ipv6_range_create_object_handler(zend_class_entry *class_type TSRMLS_DC);
static zend_object_handlers ipv6_range_object_handlers;

zend_function_entry ipv6_range_functions[] = {
    // PHP Class Functions
    PHP_ME(IPv6Range, __construct, NULL, ZEND_ACC_PROTECTED|ZEND_ACC_CTOR)
    PHP_ME(IPv6Range, __toString, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Range, containsAddress, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Range, containsRange, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Range, getMinAddress, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(IPv6Range, getMaxAddress, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(IPv6Range, fromString, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(IPv6Range, merge, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(IPv6Range, intersect, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

    {NULL, NULL, NULL}    /* Must be the last line in ipv6 functions[] */
};



// ------------ IPv6Network ---------------------------------
PHP_METHOD(IPv6Network, fromString);
PHP_METHOD(IPv6Network, __toString);

zend_object_value ipv6_network_create_object_handler(zend_class_entry *class_type TSRMLS_DC);
static zend_object_handlers ipv6_network_object_handlers;

zend_function_entry ipv6_network_functions[] = {
    // PHP Class Functions
    PHP_ME(IPv6Network, fromString, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(IPv6Network, __toString, NULL, ZEND_ACC_PUBLIC)

    {NULL, NULL, NULL}    /* Must be the last line in ipv6 functions[] */
};



// ------------ IPv6NetworkMask ---------------------------------

PHP_METHOD(IPv6NetworkMask, fromPrefixLength);

zend_object_value ipv6_networkmask_create_object_handler(zend_class_entry *class_type TSRMLS_DC);
static zend_object_handlers ipv6_networkmask_object_handlers;

zend_function_entry ipv6_networkmask_functions[] = {
    // PHP Class Functions
    PHP_ME(IPv6NetworkMask, fromPrefixLength, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

    {NULL, NULL, NULL}    /* Must be the last line in ipv6 functions[] */
};
