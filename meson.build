# project description
project(
  'horus',
  ['c'],
  version: '0.1.0',
  license: 'BSD-3-Clause',
  meson_version: '>=1.0.0',
  default_options: [
    'c_std=c99',
    'buildtype=debug',
  ],
)

project_version_parts = meson.project_version().split('.')
project_version_major = project_version_parts[0].to_int()
project_version_minor = project_version_parts[1].to_int()
project_version_patch = project_version_parts[2].to_int()

project_description = 'A simple closs-platform application development framework'

# default properties
default_c_args = [
  '-Wunused-label',
  '-Wunused-value',
  '-Wunused-result',
  '-Wunused-function',
  '-Wunused-variable',
  '-Wunused-parameter',
  '-Wunused-local-typedefs',
  '-Wunused-but-set-variable',
]

default_override_options = [
  'c_std=c99',
  'werror=true',
  'warning_level=3',
]

# horus target
subdir('horus')

# horus pkg-config file generation
pkg_mod = import('pkgconfig')
pkg_mod.generate(
  libraries: horus_lib,
  version: meson.project_version(),
  name: 'lib' + meson.project_name(),
  filebase: meson.project_name(),
  description: project_description,
)

# sandbox target
if get_option('build_sandbox')
  subdir('sandbox')
endif

# tests targets
if get_option('build_tests')
  subdir('tests')
endif

# project options summary
summary(
  {
    'build_sandbox': get_option('build_sandbox'),
  },
  section: 'Options',
)

# project platform summary
summary(
  {
    'host': host_machine.system(),
    'target': target_machine.system(),
  },
  section: 'Platform',
)

# project directories summary
summary(
  {
    'bindir': get_option('bindir'),
    'libdir': get_option('libdir'),
    'datadir': get_option('datadir'),
    'includedir': get_option('includedir'),
    'localedir': get_option('localedir'),
    'localstatedir': get_option('localstatedir'),
    'mandir': get_option('mandir'),
    'sysconfdir': get_option('sysconfdir'),
    'prefix': get_option('prefix'),
  },
  section: 'Directories',
)
