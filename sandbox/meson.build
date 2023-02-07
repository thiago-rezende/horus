sandbox_sources = [
    'src/sandbox.c'
]

sandbox_includes = include_directories('include')

sandbox_lib = executable('sandbox',
    sources : sandbox_sources,
    include_directories : sandbox_includes,
    dependencies : horus_dep,
    install : false)
