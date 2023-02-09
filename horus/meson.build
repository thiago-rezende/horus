# horus source files
horus_sources = [
  'src/logger/logger.c'
]

# horus source files [ linux ]
if host_machine.system() == 'linux'
  horus_sources += [
    'src/platform/linux/memory.c',
    'src/platform/linux/window.c',
    'src/platform/linux/console.c'
  ]
endif

# horus include directories
horus_includes = include_directories('include')

# horus dependencies
threads_dep = dependency('threads')
xcb_dep = dependency('xcb')

horus_dependencies = [
  threads_dep,
  xcb_dep
]

# horus library
horus_lib = shared_library('horus',
  sources : horus_sources,
  include_directories : horus_includes,
  dependencies : horus_dependencies,
  install : true)

# dorus dependency declaration
horus_dep = declare_dependency(
  link_with : horus_lib,
  include_directories : horus_includes)

# horus headers target
subdir('include')
