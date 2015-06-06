dnl $Id$
dnl config.m4 for extension libipv6


PHP_ARG_ENABLE(libipv6, whether to enable libipv6 support,
[  --enable-libipv6           Enable libipv6 support])

if test "$PHP_LIBIPV6" != "no"; then
  PHP_NEW_EXTENSION(libipv6, libipv6.c, $ext_shared, "-DHAVE_CONFIG_H ")
fi
