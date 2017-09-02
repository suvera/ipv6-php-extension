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

echo get_common_bits($first, $second);

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

This extension will provide following classes.

## IPv6Address

```php
<?php

$ip = IPv6Address::fromString('fe80::226:2dff:fefa:dcb1');

echo $ip->__toString();

echo $ip->getFullAddress();

```

#### Method Summary

| Signature    | Description |
| :----------- | :---------- |
| public static `fromString( string $ipString )` | Return: `IPv6Address`<br>Create IPv6Address object from string |
| public `__toString()` | Return: `string`<br>Get short form of IPv6 string |
| public `getFullAddress()` | Return: `string`<br>Get full/expanded form of IPv6 string |
| public `nextAddress( $asString = false )` | Return: `IPv6Address` by default<br>Return: `string` when $asString is true<br>Get next address |
| public `prevAddress( $asString = false )` | Return: `IPv6Address` by default<br>Return: `string` when $asString is true<br>get previous address |
| public `compare( IPv6Address|string $other )` | Return: `int` <br>Compate other address with this one.<br>`-1`: First is less than second<br>`0` : both are equal<br>`1` : First is greater than second|
| public `commonBits( IPv6Address|string $other )` |  Return: `int` <br>Get common bits in both `this` and `other` object |
| public `add( int $number )` | Return: `IPv6Address` or `null` <br>Add to this address and<br> return the result IPv6Address object |
| public `subtract( $number )` |  Return: `IPv6Address` or `null` <br>Substract from this address and<br> return the result IPv6Address object  |
| public `maskWithNetworkMask( IPv6NetworkMask|int $mask, bool $returnString=false )` |  Return: `IPv6Address` or `null` <br> $mask can be either IPv6NetworkMask or int prefix 1 - 128<br>Return the address after netwrok mask applied.   |


## IPv6Range

```php
<?php

$range = IPv6Range::fromString('fe80::226:2dff:fefa:dcb1', 'fe80::226:2dff:fefa:ffff');

echo $range->__toString();

```

#### Method Summary

| Signature    | Description |
| :----------- | :---------- |
| public static `fromString( string $ipStart, string $ipEnd )` | Return: `IPv6Address`<br> create IPv6Range from start and end |
| public `__toString()` |  Return: `string`<br>Get string of IPv6 range |
| public `containsAddress( IPv6Address|string $address )` | Return: `bool`<br> Check whether a IPv6 address exist <br>in this range |
| public `containsRange( IPv6Range $range )` | Return: `bool`<br>Check other range exist inside this range |
| public `getMinAddress($returnString = false)` | Return: `IPv6Address` or string if $returnString is true<br>return Start address of this range or network. |
| public `getMaxAddress($returnString = false)` | Return: `IPv6Address` or string if $returnString is true<br>return End address of this range or network. |
| public static `merge( IPv6Range $range1, IPv6Range $range2 )` | Return: `IPv6Range`<br>return merged Range, <br>or return null if they are completly disjoint. |
| public static `intersect( IPv6Range $range1, IPv6Range $range2 )` | Return: `IPv6Range`<br>return intersected Range <br>or null if there no intersection. |


## IPv6Network

All methods of IPv6Range are available here because `IPv6Network extends IPv6Range`

```php
<?php
$network = IPv6Network::fromString('fe80::226:2dff::/48');

echo $network->__toString();

```


#### Method Summary

| Signature    | Description |
| :----------- | :---------- |
| public static `fromString( string $ipNetwork )` | Return: `IPv6Network`<br>Create IPv6Network object from network string |


## IPv6NetworkMask


All methods of IPv6Network are available here becuase `IPv6NetworkMask extends IPv6Network`

```php
<?php
$networkMask = IPv6NetworkMask::fromPrefixLength(40);

echo $networkMask->__toString();
// ffff:ffff:ff00::/40

```


#### Method Summary

| Signature    | Description |
| :----------- | :---------- |
| public static `fromPrefixLength( int $prefix )` | Return: `IPv6NetworkMask`<br>Create IPv6NetworkMask from number of bits (1 to 128 ) |
