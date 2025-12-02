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

# core layer tests
subdir('core')

# math layer tests
subdir('math')

# containers layer tests
subdir('containers')
