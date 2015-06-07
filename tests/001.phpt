--TEST--
Check for libipv6 functions
--SKIPIF--
<?php if (!extension_loaded("libipv6")) print "skip"; ?>
--FILE--
<?php 
echo "libipv6 extension is available";

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
    if (get_short_ipv6($testIp) != $expected) {
        echo "\nTest Failed, get_short_ipv6() \nExpected: $expected\nActual:   " . get_short_ipv6($testIp) . "\n";
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
    if (get_short_ipv6($wrongIp) !== false) {
        echo "Negative \nTest Failed, get_short_ipv6() \nExpected: false \nActual:   " . get_short_ipv6($wrongIp) . "\n";
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
    if (get_full_ipv6($testIp) != $expected) {
        echo "\nTest Failed, get_full_ipv6() \nExpected: $expected\nActual:   " . get_full_ipv6($testIp) . "\n";
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
    'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff' => false,
);

foreach ($ips as $testIp => $expected) {
    if (get_next_ipv6($testIp) !== $expected) {
        echo "\nTest Failed, get_next_ipv6() of $testIp,  \nExpected: $expected\nActual:   " . get_next_ipv6($testIp) . "\n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10c',
    'e03d::5fff' => 'e03d::5ffe',
    '::200' => '::1ff',
    '::' => false,
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
    if (get_prev_ipv6($testIp) !== $expected) {
        echo "\nTest Failed, get_prev_ipv6() of $testIp,  \nExpected: $expected\nActual:   " . get_prev_ipv6($testIp) . "\n";
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
    if (compare_ipv6($testIp, $expected) !== 0) {
        echo "\nTest Failed, compare_ipv6() should match $testIp == $expected \n";
    }
    
    if (compare_ipv6($expected, $testIp) !== 0) {
        echo "\nTest Failed, ulta compare_ipv6() should match $expected == $testIp \n";
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
    if (compare_ipv6($testIp, $expected) !== -1) {
        echo "\nTest Failed, 2 compare_ipv6() should be lesser, $testIp < $expected \n";
    }
    
    if (compare_ipv6($expected, $testIp) !== 1) {
        echo "\nTest Failed, 2 ulta compare_ipv6() should be greater $expected > $testIp \n";
    }
}


$ips = array(
    '2015:2:10::0001' => '2015:2:10::',
    '8245:0397:59af:c47f:dafb:e920:d08f:010d' => '8245:397:59af:c47f:dafb:e920:d08f:10c',
    'e03d::5fff' => 'e03d::5ffe',
    '::200' => '::1ff',
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
    if (compare_ipv6($testIp, $expected) !== 1) {
        echo "\nTest Failed, 3 compare_ipv6() should be greater, $testIp > $expected \n";
    }
    
    if (compare_ipv6($expected, $testIp) !== -1) {
        echo "\nTest Failed, 3 ulta compare_ipv6() should be lesser $expected < $testIp \n";
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
    if (get_common_bits($testIp, $expected[0]) !== $expected[1]) {
        echo "\nTest Failed, get_common_bits($testIp, $expected[0]):  \nExpected: $expected[1]\nActual:   " . get_common_bits($testIp, $expected[0]) . "\n";
    }
}


error_reporting(E_ERROR);
//ini_set('display_errors', 1);

$ips = array(
    '2001:cdba:9abc:5678::/64' => true,
    '2001:cdba:9abc:5678:f::/64' => false,
);

foreach ($ips as $testIp => $expected) {
    if (is_valid_ipv6_network($testIp) !== $expected) {
        echo "\nTest Failed, is_valid_ipv6_network($testIp) \nExpected: $expected\nActual:   " . is_valid_ipv6_network($testIp) . "\n";
    }
}



?>
--EXPECT--
libipv6 extension is available
