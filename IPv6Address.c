/*
* free the object
*/
static void ipv6_address_free_object_storage_handler(ipv6_address_object *intern TSRMLS_DC)
{
    zend_object_std_dtor(&intern->std TSRMLS_CC);
    efree(intern);
}

/*
* Create object
*/
zend_object_value ipv6_address_create_object_handler(zend_class_entry *class_type TSRMLS_DC)
{
    zend_object_value retval;

    ipv6_address_object *intern = (ipv6_address_object*) emalloc(sizeof(ipv6_address_object));
    memset(intern, 0, sizeof(ipv6_address_object));

    zend_object_std_init(&intern->std, class_type TSRMLS_CC);

    object_properties_init(&intern->std, class_type);

    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t) zend_objects_destroy_object,
        (zend_objects_free_object_storage_t) ipv6_address_free_object_storage_handler,
        NULL
        TSRMLS_CC
    );

    retval.handlers = (zend_object_handlers *) &ipv6_address_object_handlers;

    return retval;
}



PHP_METHOD(IPv6Address, __construct) {}

PHP_METHOD(IPv6Address, __toString) {}

PHP_METHOD(IPv6Address, getFullAddress) {}

PHP_METHOD(IPv6Address, nextAddress) {}

PHP_METHOD(IPv6Address, prevAddress) {}

PHP_METHOD(IPv6Address, compare) {}

PHP_METHOD(IPv6Address, commonBits) {}

PHP_METHOD(IPv6Address, fromString) {}



