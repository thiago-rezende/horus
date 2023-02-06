horus_sources = []

if target_machine.system() == 'linux'
    horus_sources += [
        'src/platform/linux/entrypoint.c'
    ]
endif

horus_includes = include_directories('include')

horus_lib = shared_library('horus',
    sources: horus_sources,
    include_directories: horus_includes,
    install: true)

horus_dep = declare_dependency(
    link_with: horus_lib,
    include_directories: horus_includes)
