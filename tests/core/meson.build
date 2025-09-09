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
    dependencies: tests_dependencies,
  )

  test(name, exe)
endforeach
