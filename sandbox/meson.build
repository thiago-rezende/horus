# sandbox source files
sandbox_sources = [
  'source/sandbox.c',
]

# sandbox include directories
sandbox_includes = include_directories('include')

# sandbox executable
sandbox_exe = executable(
  'sandbox',
  c_args: default_c_args,
  sources: sandbox_sources,
  dependencies: horus_dep,
  override_options: default_override_options,
  include_directories: sandbox_includes,
  install: false,
)
