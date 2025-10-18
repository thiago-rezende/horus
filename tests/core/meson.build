# core layer test sources
core_sources = [
  'strings.c',
]

# core layer tests executables
foreach source : core_sources
  name = source.replace('.c', '')

  exe = executable(
    name,
    source,
    c_args: tests_c_args,
    dependencies: tests_dependencies,
    override_options: tests_override_options,
  )

  test(name, exe)
endforeach
