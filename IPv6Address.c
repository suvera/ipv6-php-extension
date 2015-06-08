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



PHP_METHOD(IPv6Address, __construct) {
    // nothing to do here... this is protected method
}



PHP_METHOD(IPv6Address, __toString) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[40];
    
    ipv6StuctToString(&thisObj->addr, out TSRMLS_CC);
    
    RETURN_STRING(out, 1);
}



PHP_METHOD(IPv6Address, getFullAddress) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[40];
    
    ipv6StuctToStringFull(&thisObj->addr, out TSRMLS_CC);
    
    RETURN_STRING(out, 1);
}



PHP_METHOD(IPv6Address, nextAddress) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[40];
    zend_bool asString = 0;
    ipv6_address result;
    ipv6_address_object *resultObject;
    
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &asString) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (!getNextIPv6Struct(&thisObj->addr, &result TSRMLS_CC)) {
        RETURN_NULL();
    }
    
    if(asString) {
        ipv6StuctToString(&result, out TSRMLS_CC);
        RETURN_STRING(out, 1);
    }
    
    
    object_init_ex(return_value, ipv6_address_ce);
    resultObject = (ipv6_address_object *) zend_object_store_get_object(return_value TSRMLS_CC);
    resultObject->addr = result;
}




PHP_METHOD(IPv6Address, prevAddress) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[40];
    zend_bool asString = 0;
    ipv6_address result;
    ipv6_address_object *resultObject;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &asString) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (!getPrevIPv6Struct(&thisObj->addr, &result TSRMLS_CC)) {
        RETURN_NULL();
    }
    
    if(asString) {
        ipv6StuctToString(&result, out TSRMLS_CC);
        RETURN_STRING(out, 1);
    }
    
    
    object_init_ex(return_value, ipv6_address_ce);
    resultObject = (ipv6_address_object *) zend_object_store_get_object(return_value TSRMLS_CC);
    resultObject->addr = result;
}




PHP_METHOD(IPv6Address, compare) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    zval *val;
    ipv6_address_object *ipv6Obj;
    ipv6_address tmp;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &val) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (Z_TYPE_P(val) == IS_STRING) {
    
        if (ipv6StringToStruct(Z_STRVAL_P(val), &tmp TSRMLS_CC) == 0) {
            RETURN_LONG(compareIPv6Structs(&thisObj->addr, &tmp TSRMLS_CC));
        } else {
            RETURN_FALSE;
        }
    
    } else if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), ipv6_address_ce TSRMLS_CC)) {
        ipv6Obj = (ipv6_address_object *) zend_object_store_get_object(val TSRMLS_CC);
        
        RETURN_LONG(compareIPv6Structs(&thisObj->addr, &ipv6Obj->addr TSRMLS_CC));
    } else {
        RETURN_FALSE;
    }
}




PHP_METHOD(IPv6Address, commonBits) {
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    zval *val;
    ipv6_address_object *ipv6Obj;
    ipv6_address tmp;
    int bits = 0;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &val) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (Z_TYPE_P(val) == IS_STRING) {
    
        if (ipv6StringToStruct(Z_STRVAL_P(val), &tmp TSRMLS_CC) == 0) {
            getCommonBitsIPv6Structs(&thisObj->addr, &tmp, &bits TSRMLS_CC);
            
            RETURN_LONG(bits);
        } else {
            RETURN_FALSE;
        }
    
    } else if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), ipv6_address_ce TSRMLS_CC)) {
        ipv6Obj = (ipv6_address_object *) zend_object_store_get_object(val TSRMLS_CC);
        
        getCommonBitsIPv6Structs(&thisObj->addr, &ipv6Obj->addr, &bits TSRMLS_CC);
        
        RETURN_LONG(bits);
    } else {
        RETURN_FALSE;
    }
}



static void addSubstractIPv6Address(INTERNAL_FUNCTION_PARAMETERS, int opt)
{
    zval *object = getThis();
    ipv6_address_object *thisObj = (ipv6_address_object *) zend_object_store_get_object(object TSRMLS_CC);
    char out[40];
    zend_bool asString = 0;
    ipv6_address result;
    ipv6_address_object *resultObject;
    long number = 0;
    
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|b", &number, &asString) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (number < 0) {
        number = -number;
        
        if (opt == 0) {
            if (!decrIPv6Struct(&thisObj->addr, &result, number TSRMLS_CC)) {
                RETURN_NULL();
            }
        } else {
            if (!IncrIPv6Struct(&thisObj->addr, &result, number TSRMLS_CC)) {
                RETURN_NULL();
            }
        }
    } else {
    
        if (opt == 0) {
            if (!IncrIPv6Struct(&thisObj->addr, &result, number TSRMLS_CC)) {
                RETURN_NULL();
            }
        } else {
            if (!decrIPv6Struct(&thisObj->addr, &result, number TSRMLS_CC)) {
                RETURN_NULL();
            }
        }
    }
    
    
    if(asString) {
        ipv6StuctToString(&result, out TSRMLS_CC);
        RETURN_STRING(out, 1);
    }
    
    object_init_ex(return_value, ipv6_address_ce);
    resultObject = (ipv6_address_object *) zend_object_store_get_object(return_value TSRMLS_CC);
    resultObject->addr = result;
}



PHP_METHOD(IPv6Address, add) {
    addSubstractIPv6Address(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}




PHP_METHOD(IPv6Address, subtract) {
    addSubstractIPv6Address(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}



PHP_METHOD(IPv6Address, fromString) {
    int ip_len;
    char *ip;
    ipv6_address_object *resultObject;
    ipv6_address addr;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip, &ip_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    
    if (ipv6StringToStruct(ip, &addr TSRMLS_CC) == 0) {
        object_init_ex(return_value, ipv6_address_ce);
        resultObject = (ipv6_address_object *) zend_object_store_get_object(return_value TSRMLS_CC);
        resultObject->addr = addr;
    } else {
        RETURN_NULL();
    }
}



