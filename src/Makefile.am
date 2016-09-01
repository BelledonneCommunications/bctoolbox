

lib_LTLIBRARIES=libbctoolbox.la


dnl quick& dirty fix to add a conditional build on bctoolbox tester Sept 1s, 2016 - SAV 
if BUILD_TESTS
lib_LTLIBRARIES +=libbctoolbox-tester.la
libbctoolbox_tester_la_SOURCES = tester.c utils.h
libbctoolbox_tester_la_LIBADD = $(CUNIT_LIBS)
libbctoolbox_tester_la_LDFLAGS= -version-info $(BCTOOLBOX_SO_VERSION) -no-undefined
endif 


libbctoolbox_la_SOURCES= vfs.c utils/port.c logging/logging.c containers/list.c containers/map.cc

if ENABLE_POLARSSL

libbctoolbox_la_SOURCES += crypto/polarssl.c

endif

if ENABLE_POLARSSL12

libbctoolbox_la_SOURCES += crypto/polarssl1.2.c

endif

if ENABLE_MBEDTLS

libbctoolbox_la_SOURCES += crypto/mbedtls.c

endif

AM_CFLAGS = $(POLARSSL_CFLAGS) $(MBEDTLS_CFLAGS)

libbctoolbox_la_LIBADD = $(POLARSSL_LIBS) $(MBEDTLS_LIBS)

libbctoolbox_la_LDFLAGS = -version-info $(BCTOOLBOX_SO_VERSION) -no-undefined



AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src $(CUNIT_CFLAGS)
