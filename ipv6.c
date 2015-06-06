PHPAPI int ipv6StringToStruct(char* ip, ipv6_address* addr TSRMLS_CC) {
    int len = strlen(ip);
    int i = 0, chZones = 0, dividers = 0, twoColon = -1, j = len-1, pos = 0;
    char ch, final[IPV6_HEX_CHAR_LEN+1];

    if (len < 2 ) {
        return 2;
    } else if (len > 39) {
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
        char num[IPV6_HEX_ZONE_LEN+1];
        
        memcpy(num, &final[i*IPV6_HEX_ZONE_LEN], IPV6_HEX_ZONE_LEN);
        
        num[IPV6_HEX_ZONE_LEN] = 0;
        
        addr->zone[i] = STR_TO_INT(num, (char **)NULL, 16);
    }

    return 1;
}



PHPAPI int ipv6StuctToString(ipv6_address* addr, char* text TSRMLS_CC) {
    int k, z = 0, n = 0, cnt = 0, zeroCounter = 0, curMaxPos = -1, maxPos = -1, maxZeros = 0;
    char zones[IPV6_NUM_ZONES][5], ip[IPV6_MAX_CHAR_LEN + 1];
    
    for (k = 0; k < IPV6_ZONE_INT; k++) {
        char buffer[IPV6_HEX_ZONE_LEN + 1];
        
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
    
    for (i = 0; i < IPV6_ZONE_INT; i++) {
        char buffer[IPV6_HEX_ZONE_LEN+1];
        
        sprintf(buffer, IPV6_ZONE_HEX_FORMAT, addr->zone[i]);
    
        for (z = 0; z < IPV6_HEX_ZONE_LEN/4; z++) {
            memcpy(text + t, buffer + (z * 4), 4);
            t += 4;
            
            text[++t] = ':';
        }
    }
    
    text[IPV6_MAX_CHAR_LEN] = 0;
    
    return 1;
}

