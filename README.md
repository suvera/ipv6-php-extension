# PHP IPv6 extension
C extension for PHP - IPv6 Library

## Prerequisites
  1. PHP 5.2 or higher
  2. Make sure you've *phpize* and *php-config* installed.

## Installation
  1. cd ipv6-php-extension
  2. /path/to/phpize
  3. ./configure --with-php-config=/path/to/php-config --enable-libipv6
  4. make
  5. make test # optional
  6. sudo make install

> Add *extension=libipv6.so* to your php.ini

> Test the code by running  **/path/to/php -d"extension=libipv6.so" tests/test.php**


# Functions
--------------

This extension will provide following functions.

## is_valid_ipv6

```php
<?php

$ip = 'fe80::226:2dff:fefa:dcba';

if (is_valid_ipv6($ip)) {
    echo "This is valid IPv6 Address\n";
}

```

## get_full_ipv6

Full expanded IPv6 address

```php
<?php

$ip = 'fe80::226:2dff:fefa:dcba';

echo get_full_ipv6($ip);

// prints: fe80:0000:0000:0000::0226:2dff:fefa:dcba

```

## get_short_ipv6

Full expanded IPv6 address

```php
<?php

$ip = 'fe80:0000:0000:0000::0226:2dff:fefa:dcba';

echo get_short_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcba

```


## get_next_ipv6

```php
<?php

$ip = 'fe80::226:2dff:fefa:dcba';


echo get_next_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcbb

```

## get_prev_ipv6


```php
<?php

$ip = 'fe80::226:2dff:fefa:dcbb';


echo get_prev_ipv6($ip);

// prints: fe80::226:2dff:fefa:dcba

```

## compare_ipv6


Return any one of following things.

| Return value | Description |
| :----------- | :---------- |
| -1 | First is less than second |
|  0 | both are equal |
|  1 | First is greater than second |
 

 
```php
<?php

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
<?php

$first = 'fe80::226:2dff:fefa:dcb1';
$second = 'fe80::226:2dff:fefa:dcb2';

// they have first 126 bits in common

echo compare_ipv6($first, $second);

// prints: 126

```

## is_valid_ipv6_network

Check given network ia valid.

```php
<?php

$ip = '2001:db8:fe18::/48';

if (is_valid_ipv6_network($ip)) {
    echo "This is valid IPv6 Network\n";
}

```


## is_ipv4_mapped_ipv6

is IPv4-Compatible and IPv4-Mapped Embedded IPv6 Addresses.

it would be represented as 0:0:0:0:0:FFFF:222.1.41.90, or ::FFFF:222.1.41.90.


```php
<?php

$ip = '::FFFF:222.1.41.90';

if (is_ipv4_mapped_ipv6($ip)) {
    echo "This is valid IPv4-Mapped Embedded IPv6 Address\n";
}

```



# Classes 
------------

This extension will provide following classes.

## IPv6Address

```php
<?php

$ip = IPv6Address::fromString('fe80::226:2dff:fefa:dcb1');

echo $ip->__toString();

echo $ip->getFullAddress();

```

#### Method Summary

| Signature    | Return | Description |
| :----------- | :------| :-----------| :---------- |
| public static `fromString( string $ipString )` | IPv6Address | create IPv6Address object from string |
| public `__toString()` | string | get short form of IPv6 string |
| public `getFullAddress()` | string | get short form of IPv6 string |
| public `nextAddress( $asString = false )` | IPv6Address  OR string when $asString is true | get next address |
| public `prevAddress( $asString = false )` | IPv6Address OR  string when $asString is true | get previous address |
| public `compare( IPv6Address|string $other )` | int | Compate other address with this one. see  compare_ipv6 for more info. |
| public `commonBits( IPv6Address|string $other )` | int | get common bits in both this and other objects |
| public `add( int $number )` | IPv6Address or null | Add to this address and return the IP object |
| public `subtract( $number )` | IPv6Address or null | Substract from this address and return the IP object  |


## IPv6Range

```php
<?php

$range = IPv6Range::fromString('fe80::226:2dff:fefa:dcb1', 'fe80::226:2dff:fefa:ffff');

echo $range->__toString();

```

#### Method Summary

| Signature    | Return | Description |
| :----------- | :------| :---------- |
| public static `fromString( string $ipStart, string $ipEnd )` | IPv6Address | create IPv6Range from start and end |
| public `__toString()` | string | - | get display form of IPv6 range |
| public `containsAddress( IPv6Address|string $address )` | bool | Check whether a IPv6 address exist in this range |
| public `containsRange( IPv6Range $range )` | bool | Check other range exist inside this range |
| public static `merge( IPv6Range $range1, IPv6Range $range2 )` | IPv6Range | return merged Range, or return null if they are completly disjoint. |
| public static `intersect( IPv6Range $range1, IPv6Range $range2 )` | IPv6Range | return intersected Range or null if there no intersection. |


## IPv6Network

All methods of IPv6Range are available here.

```php
<?php

class IPv6Network extends IPv6Range {
    // ----
}

$network = IPv6Network::fromString('fe80::226:2dff::/48');

echo $network->__toString();

```


#### Method Summary

| Signature    | Return | Description |
| :----------- | :------ | :---------- |
| public static `fromString( string $ipNetwork )` | IPv6Network | create IPv6Network object from network string |


## IPv6NetworkMask


All methods of IPv6Network are available here.

```php
<?php

class IPv6NetworkMask extends IPv6Network {
    // ----
}

$networkMask = IPv6NetworkMask::fromPrefixLength(40);

echo $networkMask->__toString();
// ffff:ffff:ff00::/40

```


#### Method Summary

| Signature    | Return | Description |
| :----------- | :------ | :---------- |
| public static `fromPrefixLength( int $prefix )` | IPv6NetworkMask | create IPv6NetworkMask from number of bits (1 to 128 ) |









