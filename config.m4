dnl $Id$
dnl config.m4 for extension libipv6

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(libipv6, for libipv6 support,
dnl Make sure that the comment is aligned:
dnl [  --with-libipv6             Include libipv6 support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(libipv6, whether to enable libipv6 support,
dnl Make sure that the comment is aligned:
dnl [  --enable-libipv6           Enable libipv6 support])

if test "$PHP_LIBIPV6" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-libipv6 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/libipv6.h"  # you most likely want to change this
  dnl if test -r $PHP_LIBIPV6/$SEARCH_FOR; then # path given as parameter
  dnl   LIBIPV6_DIR=$PHP_LIBIPV6
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for libipv6 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LIBIPV6_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LIBIPV6_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the libipv6 distribution])
  dnl fi

  dnl # --with-libipv6 -> add include path
  dnl PHP_ADD_INCLUDE($LIBIPV6_DIR/include)

  dnl # --with-libipv6 -> check for lib and symbol presence
  dnl LIBNAME=libipv6 # you may want to change this
  dnl LIBSYMBOL=libipv6 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LIBIPV6_DIR/lib, LIBIPV6_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LIBIPV6LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong libipv6 lib version or lib not found])
  dnl ],[
  dnl   -L$LIBIPV6_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LIBIPV6_SHARED_LIBADD)

  PHP_NEW_EXTENSION(libipv6, libipv6.c, $ext_shared)
fi
