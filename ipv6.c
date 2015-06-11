// Utility functions
// --------------------------------------
IPV6_INT applyBitMask(int n, IPV6_INT out) {
    int i = 0;
    
    for (; i < n; i++) {
        out &= ~(IPV6_ZONE_BIT_CHECKER << i);
    }
    
    return out;
}

IPV6_INT undoBitMask(int n, IPV6_INT out) {
    int i = 0;
    
    for (; i < n; i++) {
        out |= (IPV6_ZONE_BIT_CHECKER << i);
    }
    
    return out;
}

int testAnyBitSet(int n, IPV6_INT out) {
    int i = 0;
    
    for (; i < n; i++) {
        if (out & (IPV6_ZONE_BIT_CHECKER << i)) {
            return 1;
        }
    }
    
    return 0;
}

int copyIPv6Zone(ipv6_address *from, ipv6_address *to TSRMLS_CC) {
    int i = 0;
    
    for (; i < IPV6_ZONE_INT; i++) {
        to->zone[i] = from->zone[i];
    }
    
    return 1;
}

// ---------------------------------------




// API functions
// --------------------------------------
PHPAPI int ipv6StringToStruct(char* ip, ipv6_address* addr TSRMLS_CC) {
    int len = strlen(ip);
    int i = 0, chZones = 0, dividers = 0, twoColon = -1, j = len-1, pos = 0;
    char ch, final[IPV6_HEX_CHAR_LEN+1], num[IPV6_HEX_ZONE_LEN+1];;

    if (len < 2 ) {
        return 2;
    } else if (len > IPV6_MAX_CHAR_LEN) {
        return 3;
    }

    for (i = 0; i < len; i++) {
        ch = *(ip + i);

        if (ch == ':') {
            dividers++;

            if (i < j && *(ip + i + 1) == ':') {
                if (twoColon == -1) {
                    twoColon = i;
                } else {
                    // previously we met two colons, so this IP is invalid
                    return 3;
                }
            } else if (i == j && chZones > 0) {
                // format of "ffff::aaaaa:"  is not allowed
                return 4;
            }

            // zone changed
            chZones = 0;

        } else if ((ch > 47 && ch < 58) || (ch > 86 && ch < 103) || (ch > 64 && ch < 71)) {
            // eligible characters

            if (i == 1 && dividers == 1) {
                // format of ":ffff::aaaaa"  is not allowed
                return 5;
            }

            if (++chZones > 4) {
                return 6;
            }
            
        } else if (ch == '.') {
            if (!checkIPv4Mapped(ip, addr, i-chZones, len TSRMLS_CC)) {
                return 9;
            }
            
            return 0;

        } else if (i < len) {
           return 7;
        }
    }

    if (dividers < 2 || dividers > IPV6_NUM_COLONS || (twoColon == -1 && dividers != IPV6_NUM_COLONS)) {
        return 8;
    }

    memset(final, '0', IPV6_HEX_CHAR_LEN);

    if (len > 2) {
        i = 0;
        while (i < len) {
            if (twoColon == i) {
                i += 2;

                pos += (IPV6_NUM_ZONES - dividers) * 4;

                if (twoColon == 0 || twoColon == len-2) {
                    pos += 4;
                }

                continue;

            } else if (*(ip + i) == ':') {
                i++;
            }

            chZones = 0;
            // find number of characters in a zone
            for (j = i; j < len; j++) {
                if (*(ip + j) == ':')
                    break;
                chZones++;
            }

            pos += (4 - chZones);
            for (j = 0; j < chZones; j++) {
                final[pos++] = *(ip + i);
                i++;
            }
        }
    }

    for (i = 0; i < IPV6_ZONE_INT; i++) {
        memcpy(num, &final[i*IPV6_HEX_ZONE_LEN], IPV6_HEX_ZONE_LEN);
        
        num[IPV6_HEX_ZONE_LEN] = 0;
        
        addr->zone[i] = STR_TO_INT(num, (char **)NULL, 16);
    }
    
    addr->isIPv4 = 0;

    return 0;
}



static int checkIPv4Mapped(char* ip, ipv6_address* addr, int start, int len TSRMLS_CC) {
    char ipv6Part[32], part[4], ch, ipv4Hex[9];
    int i = 0, k = 0, dots = 0, ipv4Len = 0;
    long tmp = 0;
    
    if (start < 7) {
        // length is less than 7
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, length is less than 7");
        return 0;
    }
    
    memset(ipv6Part, 0, sizeof(ipv6Part));
    strncpy(ipv6Part, ip, start);
    
    //printf("IPv6 part length is %d, IPv6: %s, IPv4: %s, ( %d )\n", start, ipv6Part, ipv4Part, len-start);
    
    if (strcasecmp(ipv6Part, "::ffff:") != 0 && strcasecmp(ipv6Part, "0:0:0:0:0:ffff:") != 0) {
        // not a valid IPv6 part
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, not a valid IPv6 part %s ", ipv6Part);
        return 0;
    }
    
    ipv4Len = len-start;
    
    memset(part, 0, sizeof(part));
    memset(ipv4Hex, 0, sizeof(ipv4Hex));
    
    for (i = 0; i < ipv4Len; i++) {
        ch = *(ip + start + i);
        
        if (ch <= '9' && ch >= '0') {
            if (k > 2) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, not a valid IPv4 number %s ", part);
                return 0;
            }
            
            part[k++] = ch;
        } else if (ch == '.') {
            if (dots > 2 || k == 0) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, total dots: %d,  IPv6 number length: %d ", dots, k);
                return 0;
            }
            
            k = 0;
            
            tmp = strtol(part, (char **)NULL, 10);
            if (tmp > 255) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, IPv4 number is greater than 255: %s > %ld ", part, tmp);
                return 0;
            }
            
            sprintf((ipv4Hex + (dots * 2)), "%02lx", tmp);
            
            memset(part, 0, sizeof(part));
            dots++;
        } else {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, special character found: %c ", ch);
            return 0;
        }
    }
    
    if (dots != 3 || k == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, Final total dots: %d, IPv6 number length: %d ", dots, k);
        return 0;
    }
    
    tmp = strtol(part, (char **)NULL, 10);
    if (tmp > 255) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a valid IPv4 mapped IPv6 address, IPv4 number is greater than 255: %s > %ld ", part, tmp);
        return 0;
    }
    
    sprintf((ipv4Hex + (dots * 2)), "%02lx", tmp);
    
    addr->isIPv4 = 1;
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        addr->zone[i] = 0;
    }
    
    addr->zone[IPV6_ZONE_INT-1] = STR_TO_INT(ipv4Hex, (char **)NULL, 16);
    
    return 1;
}




PHPAPI int ipv6StuctToString(ipv6_address* addr, char* text TSRMLS_CC) {
    int k, z = 0, n = 0, cnt = 0, zeroCounter = 0, curMaxPos = -1, maxPos = -1, maxZeros = 0;
    char zones[IPV6_NUM_ZONES][5], ip[IPV6_MAX_CHAR_LEN + 1], buffer[IPV6_HEX_ZONE_LEN + 1];
    
    if (addr->isIPv4 == 1) {
        strcpy(text, "::ffff:");
        z = 7;
        
        sprintf(buffer, "%08lx", addr->zone[IPV6_ZONE_INT-1]);
        
        ip[2] = 0;
        for (k = 0; k < 8; k+=2) {
            ip[0] = buffer[k];
            ip[1] = buffer[k+1];
            
            n = strtol(ip, (char **)NULL, 16);
            
            sprintf(&text[z], "%d", n);
            z += (n > 99) ? 3 : ((n > 9) ? 2 : 1);
            
            if (k < 6) text[z++] = '.';
        }
        
        return 1;
    }
    
    
    for (k = 0; k < IPV6_ZONE_INT; k++) {
        sprintf(buffer, IPV6_ZONE_HEX_FORMAT, addr->zone[k]);
        
        memcpy(ip + ( k * IPV6_HEX_ZONE_LEN ), buffer, IPV6_HEX_ZONE_LEN);
    }

    for (k = 0; k < IPV6_HEX_CHAR_LEN; k += 4) {
        cnt = 0;

        if (ip[k] == '0') {

            if (ip[k+1] == '0') {

                if (ip[k+2] == '0') {

                    zones[z][cnt++] = ip[k+3];

                    if (ip[k+3] == '0') {
                        if (zeroCounter == 0) {
                            curMaxPos = z;
                        }

                        zeroCounter++;

                        if (zeroCounter > maxZeros) {
                            maxZeros = zeroCounter;
                            maxPos = curMaxPos;
                        }
                    }
                } else {
                    zeroCounter = 0;
                    zones[z][cnt++] = ip[k+2];
                    zones[z][cnt++] = ip[k+3];
                }
            } else {
                zeroCounter = 0;
                zones[z][cnt++] = ip[k+1];
                zones[z][cnt++] = ip[k+2];
                zones[z][cnt++] = ip[k+3];
            }
        } else {
            zeroCounter = 0;
            zones[z][cnt++] = ip[k];
            zones[z][cnt++] = ip[k+1];
            zones[z][cnt++] = ip[k+2];
            zones[z][cnt++] = ip[k+3];
        }

        zones[z++][cnt] = '\0';
    }

    z = 0;
    for (k = 0; k < IPV6_NUM_ZONES; k++) {
        if (maxPos == k && maxZeros > 1) {
            k += maxZeros-1;

            text[z++] = ':';

            if (maxPos == 0) {
                text[z++] = ':';
            }
            continue;
        } else {
            cnt = strlen(zones[k]);
            for (n = 0; n < cnt; n++) {
                text[z++] = zones[k][n];
            }

            if (k < 7) {
                text[z++] = ':';
            }
        }
    }

    text[z] = '\0';

    return 1;
}




PHPAPI int ipv6StuctToStringFull(ipv6_address* addr, char* text TSRMLS_CC) {
    int i = 0, z = 0, t = 0;
    char ip[IPV6_MAX_CHAR_LEN + 1];
    
    if (addr->isIPv4 == 1) {
        return ipv6StuctToStringFull(addr, text);
    }
    
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        char buffer[IPV6_HEX_ZONE_LEN+1];
        
        sprintf(buffer, IPV6_ZONE_HEX_FORMAT, addr->zone[i]);
    
        for (z = 0; z < IPV6_HEX_ZONE_LEN/4; z++) {
            memcpy(&text[t], &buffer[z * 4], 4);
            t += 4;
            
            if (t < IPV6_MAX_CHAR_LEN) {
                text[t] = ':';
            }
            
            ++t;
        }
    }
    
    text[IPV6_MAX_CHAR_LEN] = 0;
    
    return 1;
}




PHPAPI int getNextIPv6Struct(ipv6_address* addr, ipv6_address* next TSRMLS_CC) {
    int i = 0, ret = 0;
    ipv6_address tmp;
    
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        tmp.zone[i] = addr->zone[i];
    }
    
    for (i = IPV6_ZONE_INT - 1; i >= 0; i--) {
        if (tmp.zone[i] < IPV6_INT_MAX) {
            tmp.zone[i]++;
            ret = 1;
            break;
        } else if (tmp.zone[i] == IPV6_INT_MAX) {
            tmp.zone[i] = 0;
        }
    }
    
    if (ret) {
        for (i = 0; i < IPV6_ZONE_INT; i++) {
            next->zone[i] = tmp.zone[i];
        }
    }
    
    return ret;
}





PHPAPI int getPrevIPv6Struct(ipv6_address* addr, ipv6_address* prev TSRMLS_CC) {
    int i = 0, ret = 0;
    ipv6_address tmp;
    
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        tmp.zone[i] = addr->zone[i];
    }
    
    for (i = IPV6_ZONE_INT - 1; i >= 0; i--) {
        if (tmp.zone[i] > 0) {
            tmp.zone[i]--;
            ret = 1;
            break;
        } else if (tmp.zone[i] == 0) {
            tmp.zone[i] = IPV6_INT_MAX;
        }
    }
    
    if (ret) {
        for (i = 0; i < IPV6_ZONE_INT; i++) {
            prev->zone[i] = tmp.zone[i];
        }
    }
    
    return ret;
}

PHPAPI int compareIPv6Structs(ipv6_address* x, ipv6_address* y TSRMLS_CC) {
    int i = 0, ret = 0;

    for (i = 0; i < IPV6_ZONE_INT; i++) {
        if (x->zone[i] < y->zone[i]) {
            ret = -1;
            break;
        } else if (x->zone[i] > y->zone[i]) {
            ret = 1;
            break;
        }
    }
    
    return ret;
}



PHPAPI int getCommonBitsIPv6Structs(ipv6_address* x, ipv6_address* y, int* bits TSRMLS_CC) {
    int i = 0, j = 0;
    IPV6_INT b;
    
    *bits = 0;
    
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        b = x->zone[i] ^ y->zone[i];
        
        if (b == 0) {
        
            *bits += ( IPV6_HEX_ZONE_LEN * 4 );
            
        } else {
            for (j = (IPV6_HEX_ZONE_LEN * 4) - 1; j >= 0; j--) {
                if (b & (IPV6_ZONE_BIT_CHECKER << j)) {
                    break;
                } else {
                    *bits += 1;
                }
            }
            break;
        }
    }
    
    return 1;
}


PHPAPI int ipv6NetworkToStruct(char* ip, ipv6_range* range, int strict TSRMLS_CC) {
    int len = strlen(ip), ret = 0, a = 0, b = 0, i = 0;
    char ch, address[IPV6_MAX_CHAR_LEN+1], bits[3];
    long int maskBits = 0;
    
    if (len > 43) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "IPv6 Network length is too long %d ", len);
        return ret;
    }

    for (i = 0; i < len; i++) {
        ch = *(ip + i);
        
        if (b > 2 || a > IPV6_MAX_CHAR_LEN) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Network, Address Length: %d, MaskBit Length: %d", a, b);
            return 0;
        }

        if (ret) {
            bits[b++] = ch;
        } else {
            if (ch == '/') {
                ret = 1;
            } else {
                address[a++] = ch;
            }
        }
    }
    
    address[a] = 0;
    bits[b] = 0;
    
    maskBits = strtol(bits, (char **)NULL, 10);
    
    if (maskBits < 1 || maskBits > IPV6_TOTAL_BITS) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Network, MaskBits: %ld", maskBits);
        return 0;
    }
    
    ret = ipv6StringToStruct(address, &range->start TSRMLS_CC);
    
    if (ret != 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Network, Address: %s", address);
        return 0;
    }
    
    range->isNetwork = 1;
    range->networkBits = (uint8_t) maskBits;
    
    // split bits by zone and mask IP address
    b = IPV6_HEX_ZONE_LEN * 4;
    for (i = IPV6_ZONE_INT-1; i >= 0; i--) {
    
        if (maskBits < 1 ) {
            range->end.zone[i] = range->start.zone[i];
            continue;
        }
        
        a = (int) ((maskBits > b ) ? maskBits - b : maskBits);
        
        if (strict && testAnyBitSet(a, range->start.zone[i])) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Network, Address has '%s' more length than provided Network mask '%s' ", address, bits);
            // IPv6 Trap
            return 0;
        }
        
        range->start.zone[i] = applyBitMask(a, range->start.zone[i]);
        
        range->end.zone[i] = undoBitMask(a, range->start.zone[i]);
        
        maskBits -= b;
    }
    
    return 1;
}



PHPAPI int ipv6RangeToStruct(char* ip1, char* ip2, ipv6_range* range TSRMLS_CC) {
    int ret;
    ipv6_address tmp;
    
    ret = ipv6StringToStruct(ip1, &range->start TSRMLS_CC);
    
    if (ret != 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Address: %s", ip1);
        return 0;
    }
    
    ret = ipv6StringToStruct(ip2, &range->end TSRMLS_CC);
    
    if (ret != 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid IPv6 Address: %s", ip2);
        return 0;
    }
    
    range->isNetwork = 0;
    range->networkBits = 0;
    
    if (compareIPv6Structs(&range->start, &range->end TSRMLS_CC) > 0) {
        tmp = range->start;
        range->start = range->end;
        range->end = tmp;
    }
    
    return 1;
}


PHPAPI int inIPv6Range(ipv6_address* ipv6, ipv6_range* range TSRMLS_CC) {
    if (compareIPv6Structs(ipv6, &range->start TSRMLS_CC) >= 0 && compareIPv6Structs(ipv6, &range->end TSRMLS_CC) <= 0) {
        return 1;
    }
    
    return 0;
}

PHPAPI int isIPv6SubRangeOf(ipv6_range* subRange, ipv6_range* range TSRMLS_CC) {

    if (compareIPv6Structs(&subRange->start, &range->start TSRMLS_CC) >= 0 && compareIPv6Structs(&subRange->end, &range->end TSRMLS_CC) <= 0) {
        return 1;
    }
    
    return 0;
}

PHPAPI int getIPv6RangeIntersect(ipv6_range* range1, ipv6_range* range2, ipv6_range* result TSRMLS_CC) {
    int a, b, c, d;
    
    a = compareIPv6Structs(&range1->start, &range2->start TSRMLS_CC);
    b = compareIPv6Structs(&range1->end, &range2->end TSRMLS_CC);
    
    if (a <= 0 && b >= 0) {
        copyIPv6Zone(&result->start, &range1->start);
        copyIPv6Zone(&result->end, &range1->end);
        return 1;
    } else if (a >= 0 && b <= 0) {
        copyIPv6Zone(&result->start, &range2->start);
        copyIPv6Zone(&result->end, &range2->end);
        return 1;
    }
    
    c = compareIPv6Structs(&range1->start, &range2->end TSRMLS_CC);
    d = compareIPv6Structs(&range1->end, &range2->start TSRMLS_CC);
    
    
    if (a <= 0 && d >=0 && b < 0) {
        copyIPv6Zone(&result->start, &range2->start);
        copyIPv6Zone(&result->end, &range1->end);
        return 1;
    } else if (a > 0 && c <= 0 && b >= 0) {
        copyIPv6Zone(&result->start, &range1->start);
        copyIPv6Zone(&result->end, &range2->end);
        return 1;
    }
    
    return 0;
}

PHPAPI int ipv6RangeMerge(ipv6_range* range1, ipv6_range* range2, ipv6_range* result TSRMLS_CC) {
    ipv6_address a, b;
    int ret = 0;
    
    
    if (getNextIPv6Struct(&range1->end, &a TSRMLS_CC)) {
        if (compareIPv6Structs(&range2->start, &a TSRMLS_CC) == 0) {
            copyIPv6Zone(&result->start, &range1->start);
            copyIPv6Zone(&result->end, &range2->end);
            ret = 1;
        }
    }
    
    if (!ret && getNextIPv6Struct(&range2->end, &b TSRMLS_CC)) {
        if (compareIPv6Structs(&range1->start, &a TSRMLS_CC) == 0) {
            copyIPv6Zone(&result->start, &range2->start);
            copyIPv6Zone(&result->end, &range1->end);
            ret = 1;
        }
    }
    
    
    return ret;
}


PHPAPI int IncrIPv6Struct(ipv6_address* addr, ipv6_address* result, long incr TSRMLS_CC) {
    int i = 0;
    IPV6_INT diff = 0, carry = incr;
    ipv6_address tmp;
    
    copyIPv6Zone(addr, &tmp TSRMLS_CC);
    
    for (i = IPV6_ZONE_INT - 1; i >= 0; i--) {
        diff = IPV6_INT_MAX - tmp.zone[i];
        
        if (diff >= carry) {
            tmp.zone[i] = carry + tmp.zone[i];
            
            carry = 0;
            break;
        } else {
            tmp.zone[i] = carry - diff - 1;
            carry = 1;
        }
    }
    
    if (!carry) {
        copyIPv6Zone(&tmp, result TSRMLS_CC);
    }
    
    return carry ? 0 : 1;
}


PHPAPI int decrIPv6Struct(ipv6_address* addr, ipv6_address* result, long number TSRMLS_CC) {
    int i = 0;
    IPV6_INT carry = number;
    ipv6_address tmp;
	
	if (number < 0) {
		carry = number = -number;
	}
    
    copyIPv6Zone(addr, &tmp TSRMLS_CC);
    
    for (i = IPV6_ZONE_INT - 1; i >= 0; i--) {
        if (tmp.zone[i] >= carry) {
            tmp.zone[i] = tmp.zone[i] - carry;
            
            carry = 0;
            break;
        } else {
            carry = 1;
			tmp.zone[i] = IPV6_INT_MAX - (carry - tmp.zone[i]) + 1;
        }
    }
    
    if (!carry) {
        copyIPv6Zone(&tmp, result TSRMLS_CC);
    }
    
    return carry ? 0 : 1;
}

// END -------------------------------------------------------------




// PHP Interface Functions
// ---------------------------------------------------------
PHP_FUNCTION(is_valid_ipv6) {
    int address_len;
    char *address;
    ipv6_address ipv6;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}




PHP_FUNCTION(get_full_ipv6) {
    int address_len;
    char *address, out[40];
    ipv6_address ipv6;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
        ipv6StuctToStringFull(&ipv6, out TSRMLS_CC);
        
        RETURN_STRING(out, 1);
    } else {
        RETURN_FALSE;
    }
}




PHP_FUNCTION(get_short_ipv6) {
    int address_len;
    char *address, out[40];
    ipv6_address ipv6;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
        ipv6StuctToString(&ipv6, out TSRMLS_CC);
        
        RETURN_STRING(out, 1);
    } else {
        RETURN_FALSE;
    }
}





PHP_FUNCTION(get_next_ipv6) {
    int address_len;
    char *address, out[40];
    ipv6_address ipv6, next;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
    
        if (getNextIPv6Struct(&ipv6, &next TSRMLS_CC)) {
            ipv6StuctToString(&next, out TSRMLS_CC);
            
            RETURN_STRING(out, 1);
        } else {
            RETURN_FALSE;
        }
    } else {
        RETURN_FALSE;
    }
}




PHP_FUNCTION(get_prev_ipv6) {
    int address_len;
    char *address, out[40];
    ipv6_address ipv6, prev;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
    
        if (getPrevIPv6Struct(&ipv6, &prev TSRMLS_CC)) {
            ipv6StuctToString(&prev, out TSRMLS_CC);
            
            RETURN_STRING(out, 1);
        } else {
            RETURN_FALSE;
        }
    } else {
        RETURN_FALSE;
    }
}




PHP_FUNCTION(compare_ipv6) {
    int address_len, address_len2;
    char *address, *address2;
    ipv6_address ipv6, y;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &address, &address_len, &address2, &address_len2) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0 && ipv6StringToStruct(address2, &y TSRMLS_CC) == 0) {
    
        RETURN_LONG(compareIPv6Structs(&ipv6, &y TSRMLS_CC));
        
    } else {
        RETURN_FALSE;
    }
}




PHP_FUNCTION(get_common_bits) {
    int address_len, address_len2, out = 0;
    char *address, *address2;
    ipv6_address ipv6, y;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &address, &address_len, &address2, &address_len2) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0 && ipv6StringToStruct(address2, &y TSRMLS_CC) == 0) {
    
        getCommonBitsIPv6Structs(&ipv6, &y, &out TSRMLS_CC);
        
        RETURN_LONG(out);
        
    } else {
        RETURN_FALSE;
    }
}



PHP_FUNCTION(is_ipv4_mapped_ipv6) {
    int address_len;
    char *address;
    ipv6_address ipv6;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6StringToStruct(address, &ipv6 TSRMLS_CC) == 0) {
        if (ipv6.isIPv4)
            RETURN_TRUE;
    }
    
    RETURN_FALSE;
}



PHP_FUNCTION(is_valid_ipv6_network) {
    int address_len;
    char *address;
    ipv6_range range;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to read input address");
        RETURN_FALSE;
    }
    
    if (ipv6NetworkToStruct(address, &range, 1 TSRMLS_CC)) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}


#include "IPv6Address.c"

#include "IPv6Range.c"