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
        echo "Test Failed, toString() \nExpected: $expected\nActual:   " . get_short_ipv6($testIp) . "\n\n";
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
        echo "Test Failed, toString() \nExpected: false \nActual:   " . get_short_ipv6($wrongIp) . "\n\n";
    }
}


?>
--EXPECT--
libipv6 extension is available
