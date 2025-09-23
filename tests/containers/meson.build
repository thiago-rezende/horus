# containers layer test sources
core_sources = [
  'array.c',
]

# containers layer tests executables
foreach source : core_sources
  name = source.replace('.c', '')

  exe = executable(
    name,
    source,
    c_args: default_c_args,
    dependencies: tests_dependencies,
    override_options: default_override_options,
  )

  test(name, exe)
endforeach
