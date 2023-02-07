# horus source files
horus_sources = []

# horus source files [ linux ]
if host_machine.system() == 'linux'
    horus_sources += [
        'src/platform/linux/console.c'
    ]
endif

# horus include directories
horus_includes = include_directories('include')

# horus dependencies
threads_dep = dependency('threads')

# horus library
horus_lib = shared_library('horus',
    sources : horus_sources,
    include_directories : horus_includes,
    dependencies : [threads_dep],
    install : true)

# dorus dependency declaration
horus_dep = declare_dependency(
    link_with : horus_lib,
    include_directories : horus_includes)

# horus headers target
subdir('include')
