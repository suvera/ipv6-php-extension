--TEST--
Check for libipv6 IPv6Address class
--SKIPIF--
<?php if (!extension_loaded("libipv6")) print "skip"; ?>
--FILE--
<?php 
echo "libipv6 IPv6Address class is available";

$ips = array(
    '2015:2:10::0001' => '2015:2:10::1',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10d',
    'e03d::5fff' => 'e03d::5fff',
    '::200' => '::200',
    '::' => '::',
    '::1' => '::1',
    '1::' => '1::',
    '1::1' => '1::1',
    '1::2' => '1::2',
    '::1:2' => '::1:2',
    '1:2::' => '1:2::',
    '1:2::3' => '1:2::3',
    '1:2::3:4' => '1:2::3:4',
    '1:2:3:4:5:6:7:8' => '1:2:3:4:5:6:7:8',
);

foreach ($ips as $testIp => $expected) {
    $obj = IPv6Address::fromString($testIp);
    if ($obj->__toString() != $expected) {
        echo "\nTest Failed, IPv6Address->__toString() \nExpected: $expected\nActual:   " . $obj->__toString() . "\n";
    }
}

$wrongIps = array(
    ':',
    'a',
    '1',
    ':::',
    ':::1',
    '::1:',
    '1:::',
    ':1::',
    '1:::1',
    ':1::2',
    '::1:2:',
    '1:2:::',
    '1:2:::3',
    '1:2::3::4',
    '1:2:3:4:5:6:7:8:',
    ':1:2:3:4:5:6:7:8',
    '1:2:3:4::5:6:7:8',
    '1:2:3:4:5:6:7:8::',
    '::1:2:3:4:5:6:7:8',
    'y',
    '1::k',
    '2015:2:10::00001',
    '8245:0397:59af:0c47f:dafb:e920:d08f:010d',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d*8245:0397:59af:c47f:dafb:e920:d08f:0200',
    '::-e03d::5fff:',
    '::50000-::200',
);

foreach ($wrongIps as $wrongIp) {
    $obj = IPv6Address::fromString($wrongIp);
    
    if ($obj !== null) {
        echo "\nNegative Test Failed, IPv6Address::fromString() \nExpected: null \nActual:   " . $obj . "\n";
    }
}



$ips = array(
    '2015:2:10::0001' => '2015:0002:0010:0000:0000:0000:0000:0001',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:0397:59af:c47f:dafb:e920:d08f:010d',
    'e03d::5fff' => 'e03d:0000:0000:0000:0000:0000:0000:5fff',
    '::200' => '0000:0000:0000:0000:0000:0000:0000:0200',
    '::' => '0000:0000:0000:0000:0000:0000:0000:0000',
    '::1' => '0000:0000:0000:0000:0000:0000:0000:0001',
    '1::' => '0001:0000:0000:0000:0000:0000:0000:0000',
    '1::1' => '0001:0000:0000:0000:0000:0000:0000:0001',
    '1::2' => '0001:0000:0000:0000:0000:0000:0000:0002',
    '::1:2' => '0000:0000:0000:0000:0000:0000:0001:0002',
    '1:2::' => '0001:0002:0000:0000:0000:0000:0000:0000',
    '1:2::3' => '0001:0002:0000:0000:0000:0000:0000:0003',
    '1:2::3:4' => '0001:0002:0000:0000:0000:0000:0003:0004',
    '1:2:3:4:5:6:7:8' => '0001:0002:0003:0004:0005:0006:0007:0008',
);

foreach ($ips as $testIp => $expected) {
    $obj = IPv6Address::fromString($testIp);

    if ($obj->getFullAddress() != $expected) {
        echo "\nTest Failed, IPv6Address->getFullAddress() \nExpected: $expected\nActual:   " . $obj->getFullAddress() . "\n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::2',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10e',
    'e03d::5fff' => 'e03d::6000',
    '::200' => '::201',
    '::' => '::1',
    '::1' => '::2',
    '1::' => '1::1',
    '1::1' => '1::2',
    '1::2' => '1::3',
    '::1:2' => '::1:3',
    '1:2::' => '1:2::1',
    '1:2::3' => '1:2::4',
    '1:2::3:4' => '1:2::3:5',
    '1:2:3:4:5:6:7:8' => '1:2:3:4:5:6:7:9',
    '0:ffff:ffff:ffff::' => '0:ffff:ffff:ffff::1',
    '0:ffff:ffff:ffff:ffff:ffff:ffff:ffff' => '1::',
    '1:1:ffff:ffff:ffff:ffff:ffff:ffff' => '1:2::',
    'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff' => '',
);



foreach ($ips as $testIp => $expected) {
    $obj = IPv6Address::fromString($testIp);

    if ('' . $obj->nextAddress() !== $expected) {
        echo "\nTest Failed, IPv6Address->nextAddress() \nExpected: $expected\nActual:   " . $obj->nextAddress() . "\n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10c',
    'e03d::5fff' => 'e03d::5ffe',
    '::200' => '::1ff',
    '::' => '',
    '::1' => '::',
    '1::' => '0:ffff:ffff:ffff:ffff:ffff:ffff:ffff',
    '1::1' => '1::',
    '1::2' => '1::1',
    '::1:2' => '::1:1',
    '1:2::' => '1:1:ffff:ffff:ffff:ffff:ffff:ffff',
    '1:2::3' => '1:2::2',
    '1:2::3:4' => '1:2::3:3',
    '1:2:3:4:5:6:7:8' => '1:2:3:4:5:6:7:7',
);

foreach ($ips as $testIp => $expected) {
    $obj = IPv6Address::fromString($testIp);

    if ('' . $obj->prevAddress() !== $expected) {
        echo "\nTest Failed, IPv6Address->prevAddress() \nExpected: $expected\nActual:   " . $obj->prevAddress() . "\n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::1',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10d',
    'e03d::5fff' => 'e03d::5fff',
    '::200' => '::200',
    '::' => '::',
    '::1' => '::1',
    '1::' => '1::',
    '1::1' => '1::1',
    '1::2' => '1::2',
    '::1:2' => '::1:2',
    '1:2::' => '1:2::',
    '1:2::3' => '1:2::3',
    '1:2::3:4' => '1:2::3:4',
    '1:2:3:4:5:6:7:8' => '1:2:3:4:5:6:7:8',
    '2015:2:10::0001' => '2015:0002:0010:0000:0000:0000:0000:0001',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:0397:59af:c47f:dafb:e920:d08f:010d',
    'e03d::5fff' => 'e03d:0000:0000:0000:0000:0000:0000:5fff',
    '::200' => '0000:0000:0000:0000:0000:0000:0000:0200',
    '::' => '0000:0000:0000:0000:0000:0000:0000:0000',
    '::1' => '0000:0000:0000:0000:0000:0000:0000:0001',
    '1::' => '0001:0000:0000:0000:0000:0000:0000:0000',
    '1::1' => '0001:0000:0000:0000:0000:0000:0000:0001',
    '1::2' => '0001:0000:0000:0000:0000:0000:0000:0002',
    '::1:2' => '0000:0000:0000:0000:0000:0000:0001:0002',
    '1:2::' => '0001:0002:0000:0000:0000:0000:0000:0000',
    '1:2::3' => '0001:0002:0000:0000:0000:0000:0000:0003',
    '1:2::3:4' => '0001:0002:0000:0000:0000:0000:0003:0004',
    '1:2:3:4:5:6:7:8' => '0001:0002:0003:0004:0005:0006:0007:0008',
);

foreach ($ips as $testIp => $expected) {

    $obj = IPv6Address::fromString($testIp);

    if ($obj->compare($expected) !== 0) {
        echo "\nTest Failed, IPv6Address->compare($expected) should match $testIp == $expected \n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::2',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10e',
    'e03d::5fff' => 'e03d::6000',
    '::200' => '::201',
    '::' => '::1',
    '::1' => '::2',
    '1::' => '1::1',
    '1::1' => '1::2',
    '1::2' => '1::3',
    '::1:2' => '::1:3',
    '1:2::' => '1:2::1',
    '1:2::3' => '1:2::4',
    '1:2::3:4' => '1:2::3:5',
    '1:2:3:4:5:6:7:8' => '1:2:3:4:5:6:7:9',
    '0:ffff:ffff:ffff::' => '0:ffff:ffff:ffff::1',
    '0:ffff:ffff:ffff:ffff:ffff:ffff:ffff' => '1::',
    '1:1:ffff:ffff:ffff:ffff:ffff:ffff' => '1:2::',
);

foreach ($ips as $testIp => $expected) {

    $obj = IPv6Address::fromString($testIp);

    if ($obj->compare($expected) !== -1) {
        echo "\nTest Failed, IPv6Address->compare($expected) should be lesser $testIp < $expected \n";
    }
    
    $obj = IPv6Address::fromString($expected);
    
    if ($obj->compare($testIp) !== 1) {
        echo "\nTest Failed, IPv6Address->compare($testIp) should be greater $testIp > $expected \n";
    }
    
}



$ips = array(
    '2015:2:10::0001' => array('2015:2:10::1', 128),
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => array('8245:397:59af:c47f:dafb:e920:d08f:10c', 127),
    'e03d::5fff' => array('e03d::5ffe', 127),
    '::' => array('::', 128),
    '1:ffff::' => array('1::', 16),
    '1:f::' => array('1::', 28),
);

foreach ($ips as $testIp => $expected) {
    $obj = IPv6Address::fromString($testIp);
    
    if ($obj->commonBits($expected[0]) !== $expected[1]) {
        echo "\nTest Failed, $obj->commonBits($testIp, $expected[0]):  \nExpected: $expected[1]\nActual:   " . $obj->commonBits($expected[0]) . "\n";
    }
}



?>
--EXPECT--
libipv6 IPv6Address class is available
