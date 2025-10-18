# containers layer test sources
containers_sources = [
  'array.c',
]

# containers layer tests executables
foreach source : containers_sources
  name = source.replace('.c', '')

  exe = executable(
    name,
    source,
    dependencies: tests_dependencies,
  )

  test(name, exe)
endforeach
