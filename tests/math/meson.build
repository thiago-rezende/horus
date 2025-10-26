# math layer test sources
math_sources = [
  'matrix.c',
  'vector.c',
  'quaternion.c',
]

# math layer tests executables
foreach source : math_sources
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
