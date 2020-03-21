project('bctoolbox', 'c', version : '4.4.0')

#get mbedtls libraries from the system:

mbedtls_dep = declare_dependency(link_with : 'mbedtls',
  include_directories : '/usr/include')
  
cc = meson.get_compiler('c')
mbedtls_lib = cc.find_library('mbedtls', has_headers : ['mbedtls/ssl.h'])

mbedcrypto_lib = cc.find_library('mbedcrypto')
mbedx509_lib = cc.find_library('mbedx509')

threads = dependency('threads')

headers = [
	'include/bctoolbox/vfs.h',
	'include/bctoolbox/port.h',
	'include/bctoolbox/list.h',
	'include/bctoolbox/parser.h',
	'include/bctoolbox/charconv.h',
	'include/bctoolbox/defs.h',
	'include/bctoolbox/logging.h',
	'include/bctoolbox/crypto.h',
	'include/bctoolbox/compiler.h',
	'include/bctoolbox/regex.h',
	'include/bctoolbox/tester.h',
	'include/bctoolbox/map.h',
	'include/bctoolbox/vconnect.h'
	]

src = [ 'src/vconnect.c',
	'src/utils/port.c' ,
	'src/vfs.c',
	'src/logging/logging.c',
	'src/crypto/mbedtls.c',
	'src/crypto/crypto.c',
	'src/containers/list.c',
	'src/parser.c',
	'src/utils.h'
	]

incdirs = include_directories('include')

bctboolbox_library = build_target('bctoolbox', src, 
	dependencies : [ mbedtls_lib, mbedcrypto_lib, mbedx509_lib, threads],  
	include_directories : [ incdirs, 'src'],
	target_type : 'both_libraries',
	install : true)

install_headers(headers, subdir : 'bctoolbox')

configuration_data_object = configuration_data()
configuration_data_object.set('version' , meson.project_version())

configure_file(input : 'bctoolbox.spec.in',
               output : 'bctoolbox.spec',
               configuration: configuration_data_object,
               install : true,
               install_dir : 'share/SPECS')
               
bctoolbox_dep = declare_dependency(include_directories : incdirs, link_with : bctboolbox_library)


#rpm = import('rpm')
#rpm.generate_spec_template()
