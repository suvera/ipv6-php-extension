/*
* free the object
*/
static void ipv6_range_free_object_storage_handler(ipv6_range_object *intern TSRMLS_DC)
{
    zend_object_std_dtor(&intern->std TSRMLS_CC);
    efree(intern);
}

/*
* Create object
*/
zend_object_value ipv6_range_create_object_handler(zend_class_entry *class_type TSRMLS_DC)
{
    zend_object_value retval;

    ipv6_range_object *intern = (ipv6_range_object*) emalloc(sizeof(ipv6_range_object));
    memset(intern, 0, sizeof(ipv6_range_object));

    zend_object_std_init(&intern->std, class_type TSRMLS_CC);

    object_properties_init(&intern->std, class_type);

    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t) zend_objects_destroy_object,
        (zend_objects_free_object_storage_t) ipv6_range_free_object_storage_handler,
        NULL
        TSRMLS_CC
    );

    retval.handlers = (zend_object_handlers *) &ipv6_range_object_handlers;

    return retval;
}


PHP_METHOD(IPv6Range, __construct) {
    // nothing to do here since this is a protected method
}

PHP_METHOD(IPv6Range, __toString) {
    zval *object = getThis();
    ipv6_range_object *thisObj = (ipv6_range_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[80], ip[40];
    
    
    ipv6StuctToString(&thisObj->range.start, ip TSRMLS_CC);
    strcpy(out, ip);
    strcat(out, "-");
    
    ipv6StuctToString(&thisObj->range.end, ip TSRMLS_CC);
    strcpy(out, ip);
    
    RETURN_STRING(out, 1);
}



PHP_METHOD(IPv6Range, containsAddress) {
    zval *object = getThis();
    ipv6_range_object *thisObj = (ipv6_range_object *) zend_object_store_get_object(object TSRMLS_CC);
    zval *val;
    ipv6_address_object *ipv6Obj;
    ipv6_address tmp;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &val) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (Z_TYPE_P(val) == IS_STRING) {
    
        if (ipv6StringToStruct(Z_STRVAL_P(val), &tmp TSRMLS_CC) == 0) {
            if (inIPv6Range(&tmp, &thisObj->range TSRMLS_CC)) {
                RETURN_TRUE;
            } else {
                RETURN_FALSE;
            }
        } else {
            RETURN_FALSE;
        }
    
    } else if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), ipv6_address_ce TSRMLS_CC)) {
        ipv6Obj = (ipv6_address_object *) zend_object_store_get_object(val TSRMLS_CC);
        
        if (inIPv6Range(&ipv6Obj->addr, &thisObj->range TSRMLS_CC)) {
            RETURN_TRUE;
        } else {
            RETURN_FALSE;
        }
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(IPv6Range, containsRange) {
    zval *object = getThis();
    ipv6_range_object *thisObj = (ipv6_range_object *) zend_object_store_get_object(object TSRMLS_CC);
    zval *val;
    ipv6_range_object *range;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &val, ipv6_range_ce) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    range = (ipv6_range_object *) zend_object_store_get_object(val TSRMLS_CC);
        
    if (isIPv6SubRangeOf(&range->range, &thisObj->range TSRMLS_CC)) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}




PHP_METHOD(IPv6Range, fromString) {
    int address_len, address_len2;
    char *address, *address2;
    ipv6_range range;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &address, &address_len, &address2, &address_len2) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    
    if (ipv6RangeToStruct(address, address2, &range TSRMLS_CC)) {
        object_init_ex(return_value, ipv6_range_ce);
        ipv6_range_object* resultObject = (ipv6_range_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->range = range;
    } else {
        RETURN_NULL();
    }
}


PHP_METHOD(IPv6Range, merge) {
    zval *val, *val2;
    ipv6_range_object *range, *range2, *resultObject;
    ipv6_range result;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO", &val, ipv6_range_ce, &val2, ipv6_range_ce) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    range = (ipv6_range_object *) zend_object_store_get_object(val TSRMLS_CC);
    range2 = (ipv6_range_object *) zend_object_store_get_object(val2 TSRMLS_CC);
        
    if (ipv6RangeMerge(&range->range, &range2->range, &result TSRMLS_CC)) {
        object_init_ex(return_value, ipv6_range_ce);
        resultObject = (ipv6_range_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->range = result;
    } else {
        RETURN_NULL();
    }
}


PHP_METHOD(IPv6Range, intersect) {
    zval *val, *val2;
    ipv6_range_object *range, *range2, *resultObject;
    ipv6_range result;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO", &val, ipv6_range_ce, &val2, ipv6_range_ce) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    range = (ipv6_range_object *) zend_object_store_get_object(val TSRMLS_CC);
    range2 = (ipv6_range_object *) zend_object_store_get_object(val2 TSRMLS_CC);
        
    if (getIPv6RangeIntersect(&range->range, &range2->range, &result TSRMLS_CC)) {
        object_init_ex(return_value, ipv6_range_ce);
        resultObject = (ipv6_range_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->range = result;
    } else {
        RETURN_NULL();
    }
}



PHP_METHOD(IPv6Network, fromString) {
    int address_len;
    char *address;
    ipv6_range range;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    
    if (ipv6NetworkToStruct(address, &range, 0 TSRMLS_CC)) {
        object_init_ex(return_value, ipv6_network_ce);
        ipv6_range_object* resultObject = (ipv6_range_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->range = range;
    } else {
        RETURN_NULL();
    }
}


PHP_METHOD(IPv6Network, __toString) {
    zval *object = getThis();
    ipv6_range_object *thisObj = (ipv6_range_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[43], ip[40], bits[4];
    
    ipv6StuctToString(&thisObj->range.start, ip TSRMLS_CC);
    strcpy(out, ip);
    strcat(out, "/");
    
    sprintf(bits, "%d", (int) thisObj->range.networkBits);
    
    strcpy(out, bits);
    
    RETURN_STRING(out, 1);
}



PHP_METHOD(IPv6NetworkMask, fromPrefixLength) {
    long prefix = 0;
    ipv6_range range;
    char address[60];

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &prefix) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    sprintf(address, "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/%ld", prefix);
    
    if (ipv6NetworkToStruct(address, &range, 0 TSRMLS_CC)) {
        object_init_ex(return_value, ipv6_networkmask_ce);
        ipv6_range_object* resultObject = (ipv6_range_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->range = range;
    } else {
        RETURN_NULL();
    }
}


