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
    dependencies: tests_dependencies,
  )

  test(name, exe)
endforeach
