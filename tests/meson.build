# testing framework
cmocka_dep = dependency(
  'cmocka',
  fallback: ['cmocka', 'cmocka_dep'],
)

# default tests dependencies
tests_dependencies = [
  horus_dep,
  cmocka_dep,
]

# default test compiler options
tests_c_args = default_c_args

# default override options
tests_override_options = default_override_options

# core layer tests
subdir('core')

# math layer tests
subdir('math')

# containers layer tests
subdir('containers')
