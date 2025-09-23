# horus source files
horus_sources = [
  # horus base layer
  'source/definitions.c',

  # horus core layer
  'source/core/strings.c',

  # horus containers layer
  'source/containers/array.c',

  # horus logger layer
  'source/logger/logger.c',

  # horus input layer
  'source/input/mouse.c',
  'source/input/keyboard.c',

  # horus events layer
  'source/events/base.c',

  # horus application layer
  'source/application/application.c',

  # horus renderer layer
  'source/renderer/general.c',

  # horus renderer layer [ vulkan ]
  'source/renderer/vulkan/debug.c',
  'source/renderer/vulkan/device.c',
  'source/renderer/vulkan/loader.c',
  'source/renderer/vulkan/instance.c',
  'source/renderer/vulkan/renderer.c',
]

# horus source files [ linux ]
if host_machine.system() == 'linux'
  horus_sources += [
    # horus platform layer [ linux ]
    'source/platform/linux/time.c',
    'source/platform/linux/memory.c',
    'source/platform/linux/window.c',
    'source/platform/linux/console.c',

    # horus input layer [ linux ]
    'source/platform/linux/input/mouse.c',
    'source/platform/linux/input/keyboard.c',

    # horus renderer layer [ vulkan ]
    'source/renderer/vulkan/linux/platform.c',
  ]
endif

# horus source files [ windows ]
if host_machine.system() == 'windows'
  horus_sources += [
    # horus platform layer [ windows ]
    'source/platform/windows/time.c',
    'source/platform/windows/memory.c',
    'source/platform/windows/window.c',
    'source/platform/windows/console.c',

    # horus input layer [ windows ]
    'source/platform/windows/input/mouse.c',
    'source/platform/windows/input/keyboard.c',

    # horus renderer layer [ vulkan ]
    'source/renderer/vulkan/windows/platform.c',
  ]
endif

# horus include directories
horus_includes = include_directories('include')

# horus dependencies
threads_dep = dependency('threads')

horus_dependencies = [
  threads_dep,
]

# horus dependencies [ linux ]
if host_machine.system() == 'linux'
  # horus window dependencies [ linux ]
  xcb_dep = dependency('xcb')
  xcb_keysyms_dep = dependency('xcb-keysyms')

  # horus renderer dependencies [ linux ]
  vulkan_dep = dependency('vulkan')

  horus_dependencies += [
    # horus window dependencies [ linux ]
    xcb_dep,
    xcb_keysyms_dep,

    # horus renderer dependencies [ linux ]
    vulkan_dep,
  ]
endif

# horus dependencies [ windows ]
if host_machine.system() == 'windows'
  # horus renderer dependencies [ windows ]
  vulkan_dep = dependency('vulkan')

  horus_dependencies += [
    # horus renderer dependencies [ windows ]
    vulkan_dep,
  ]
endif

# horus configuration file
horus_configuration = configuration_data(
  {
    'HORUS_PROJECT_NAME': meson.project_name(),
    'HORUS_PROJECT_VERSION': meson.project_version(),
    'HORUS_PROJECT_VERSION_MAJOR': project_version_major,
    'HORUS_PROJECT_VERSION_MINOR': project_version_minor,
    'HORUS_PROJECT_VERSION_PATCH': project_version_patch,
    'HORUS_PROJECT_DESCRIPTION': project_description,
    'HORUS_PROJECT_LICENSE': meson.project_license()[0],
    'HORUS_PLATFORM': host_machine.system(),
    'HORUS_GETTEXT_PACKAGE': meson.project_name(),
    'HORUS_BINDIR': get_option('bindir'),
    'HORUS_LIBDIR': get_option('libdir'),
    'HORUS_DATADIR': get_option('datadir'),
    'HORUS_INCLUDEDIR': get_option('includedir'),
    'HORUS_LOCALEDIR': get_option('localedir'),
    'HORUS_LOCALSTATEDIR': get_option('localstatedir'),
    'HORUS_MANDIR': get_option('mandir'),
    'HORUS_SYSCONFDIR': get_option('sysconfdir'),
    'HORUS_PREFIX': get_option('prefix'),
  },
)

configure_file(
  input: 'include/horus/config.h.in',
  output: 'config.h',
  configuration: horus_configuration,
)

# horus library
horus_lib = static_library(
  'horus',
  c_args: default_c_args,
  sources: horus_sources,
  dependencies: horus_dependencies,
  override_options: default_override_options,
  include_directories: horus_includes,
  install: true,
)

# dorus dependency declaration
horus_dep = declare_dependency(
  link_with: horus_lib,
  include_directories: horus_includes,
)

# horus headers target
subdir('include')
