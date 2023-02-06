project('horus',
  ['c'],
  version : '0.1.0',
  license: 'BSD-3-Clause',
  default_options : ['c_std=c99'])

subdir('horus')

if get_option('build_sandbox')
  subdir('sandbox')
endif

summary({'build_sandbox': get_option('build_sandbox'),
        }, section: 'Options')

summary({'bindir': get_option('bindir'),
         'libdir': get_option('libdir'),
         'datadir': get_option('datadir'),
         'includedir': get_option('includedir'),
         'localedir': get_option('localedir'),
         'localstatedir': get_option('localstatedir'),
         'mandir': get_option('mandir'),
         'sysconfdir': get_option('sysconfdir'),
         'prefix': get_option('prefix'),
        }, section: 'Directories')
