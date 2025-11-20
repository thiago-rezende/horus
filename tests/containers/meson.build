# containers layer test sources
containers_sources = [
  'array.c',
  'hash_map.c',
  'linked_list.c',
]

# containers layer tests executables
foreach source : containers_sources
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
