# sandbox source files
sandbox_sources = [
  'source/sandbox.c',
]

# sandbox include directories
sandbox_includes = include_directories('include')

# sandbox executable
sandbox_exe = executable(
  'sandbox',
  sources: sandbox_sources,
  dependencies: horus_dep,
  include_directories: sandbox_includes,
  install: false,
)
