# horus source files
horus_sources = [
  'src/definitions.c',
  'src/core/strings.c',
  'src/logger/logger.c',
  'src/events/base.c',
]

# horus source files [ linux ]
if host_machine.system() == 'linux'
  horus_sources += [
    'src/platform/linux/time.c',
    'src/platform/linux/memory.c',
    'src/platform/linux/window.c',
    'src/platform/linux/console.c',
  ]
endif

# horus include directories
horus_includes = include_directories('include')

# horus dependencies
threads_dep = dependency('threads')

horus_dependencies = [
  threads_dep,
]

# horus dependencies [ linux ]
if host_machine.system() == 'linux'
  xcb_dep = dependency('xcb')

  vulkan_dep = dependency('vulkan')

  horus_dependencies += [
    xcb_dep,
    vulkan_dep,
  ]
endif

# horus configuration file
horus_configuration = configuration_data(
  {
    'HORUS_PROJECT_NAME': meson.project_name(),
    'HORUS_PROJECT_VERSION': meson.project_version(),
    'HORUS_PROJECT_DESCRIPTION': project_description,
    'HORUS_PROJECT_LICENSE': meson.project_license()[0],
    'HORUS_PLATFORM': host_machine.system(),
    'HORUS_GETTEXT_PACKAGE': meson.project_name(),
    'HORUS_BINDIR': get_option('bindir'),
    'HORUS_LIBDIR': get_option('libdir'),
    'HORUS_DATADIR': get_option('datadir'),
    'HORUS_INCLUDEDIR': get_option('includedir'),
    'HORUS_LOCALEDIR': get_option('localedir'),
    'HORUS_LOCALSTATEDIR': get_option('localstatedir'),
    'HORUS_MANDIR': get_option('mandir'),
    'HORUS_SYSCONFDIR': get_option('sysconfdir'),
    'HORUS_PREFIX': get_option('prefix'),
  },
)

configure_file(
  input: 'include/horus/config.h.in',
  output: 'config.h',
  configuration: horus_configuration,
)

# horus library
horus_lib = shared_library(
  'horus',
  sources: horus_sources,
  include_directories: horus_includes,
  dependencies: horus_dependencies,
  install: true,
)

# dorus dependency declaration
horus_dep = declare_dependency(
  link_with: horus_lib,
  include_directories: horus_includes,
)

# horus headers target
subdir('include')
