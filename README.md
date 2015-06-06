# PHP IPv6 extension
C extension for PHP - IPv6 Library

## Prerequisites
  1. <sub>PHP 5.2 or higher</sub>
  2. <sub>Make sure you've *phpize* and *php-config* installed.</sub>

## Installation
  1. <sub>cd ipv6-php-extension</sub>
  2. <sub>/path/to/phpize</sub>
  3. <sub>./configure --with-php-config=/path/to/php-config --enable-libipv6</sub>
  4. <sub>make</sub>
  5. <sub>make test # optional</sub>
  6. <sub>sudo make install</sub>

> Add *extension=libipv6.so* to your php.ini

> Test the code by running  **/path/to/php -d"extension=libipv6.so" tests/test.php**


# Functions
--------------

This extension will provide following functions.

## is_valid_ipv6

```php

$ip = 'fe80::226:2dff:fefa:dcba';

if (is_valid_ipv6($ip)) {
    echo "This is valid IPv6 Address\n";
}

```

## get_full_ipv6

Full expanded IPv6 address

```php

$ip = 'fe80::226:2dff:fefa:dcba';

echo get_full_ipv6($ip);

// prints: fe80:0000:0000:0000::0226:2dff:fefa:dcba

```

## get_short_ipv6

Full expanded IPv6 address

```php

$ip = 'fe80:0000:0000:0000::0226:2dff:fefa:dcba';

echo get_short_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcba

```


## get_next_ipv6

```php

$ip = 'fe80::226:2dff:fefa:dcba';


echo get_next_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcbb

```

## get_prev_ipv6


```php

$ip = 'fe80::226:2dff:fefa:dcbb';


echo get_prev_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcba

```

## compare_ipv6

Return any one of following things.

-1: First is less than second
 0: both are equal
 1: First is greater than second

 
```php

$first = 'fe80::226:2dff:fefa:dcba';
$second = 'fe80::226:2dff:fefa:dcbb';

echo compare_ipv6($first, $second);

// prints: -1


$first = 'fe80::226:2dff:fefa:dcbf';
$second = 'fe80::226:2dff:fefa:dcba';

echo compare_ipv6($first, $second);

// prints: 1


$first = 'fe80::226:2dff:fefa:dcba';
$second = 'fe80::226:2dff:fefa:dcba';

echo compare_ipv6($first, $second);

// prints: 0


```

## get_common_bits

get first N common bits (it should be in the range 0 to 128 (both inclusive))


```php

$first = 'fe80::226:2dff:fefa:dcb1';
$second = 'fe80::226:2dff:fefa:dcb2';

// they have first 126 bits in common

echo compare_ipv6($first, $second);

// prints: 126

```


## is_ipv4_mapped_ipv6

is IPv4-Compatible and IPv4-Mapped Embedded IPv6 Addresses.

it would be represented as 0:0:0:0:0:FFFF:222.1.41.90, or ::FFFF:222.1.41.90.


```php

$ip = '::FFFF:222.1.41.90';

if (is_ipv4_mapped_ipv6($ip)) {
    echo "This is valid IPv4-Mapped Embedded IPv6 Address\n";
}

```



# Classes 
------------

This extension will provide following classes.

## IPv6Range

## IPv6Network

## IPv6NetworkMask

## IPv6AddressList

## IPv6RangeList
