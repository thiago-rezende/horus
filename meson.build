# project description
project('horus',
  ['c'],
  version : '0.1.0',
  license : 'BSD-3-Clause',
  meson_version : '1.0.0',
  default_options : ['c_std=c99',
                     'b_sanitize=leak',
                     'buildtype=debug'])

project_description = 'A simple closs-platform application development framework'

# horus target
subdir('horus')

# horus pkg-config file generation
pkg_mod = import ('pkgconfig')
pkg_mod.generate(libraries : horus_lib,
  version : meson.project_version(),
  name : 'lib' + meson.project_name(),
  filebase : meson.project_name(),
  description : project_description)

# sandbox target
if get_option('build_sandbox')
  subdir('sandbox')
endif

# project options summary
summary({'build_sandbox' : get_option('build_sandbox'),
        }, section : 'Options')

# project directories summary
summary({'bindir' : get_option('bindir'),
         'libdir' : get_option('libdir'),
         'datadir' : get_option('datadir'),
         'includedir' : get_option('includedir'),
         'localedir' : get_option('localedir'),
         'localstatedir' : get_option('localstatedir'),
         'mandir' : get_option('mandir'),
         'sysconfdir' : get_option('sysconfdir'),
         'prefix' : get_option('prefix'),
        }, section : 'Directories')
