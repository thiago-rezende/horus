# sandbox source files
sandbox_sources = [
  'src/sandbox.c'
]

# sandbox include directories
sandbox_includes = include_directories('include')

# sandbox executable
sandbox_exe = executable('sandbox',
  sources : sandbox_sources,
  include_directories : sandbox_includes,
  dependencies : horus_dep,
  install : false)
