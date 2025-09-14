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
    dependencies: tests_dependencies,
  )

  test(name, exe)
endforeach
