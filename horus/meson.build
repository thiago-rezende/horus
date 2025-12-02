# horus source files
horus_sources = [
  # horus base layer
  'source/definitions.c',

  # horus core layer
  'source/core/hash.c',
  'source/core/strings.c',
  'source/core/numeric.c',

  # horus containers layer
  'source/containers/array.c',
  'source/containers/hash_map.c',
  'source/containers/linked_list.c',

  # horus logger layer
  'source/logger/logger.c',

  # horus input layer
  'source/input/mouse.c',
  'source/input/keyboard.c',

  # horus events layer
  'source/events/base.c',

  # horus math layer
  'source/math/matrix.c',
  'source/math/vector.c',
  'source/math/quaternion.c',

  # horus application layer
  'source/application/application.c',

  # horus camera layer
  'source/camera/camera.c',

  # horus renderer layer
  'source/renderer/texture.c',
  'source/renderer/renderer.c',
]

horus_renderer_vulkan_sources = [
  # horus renderer layer [ vulkan ]
  'source/renderer/vulkan/debug.c',
  'source/renderer/vulkan/image.c',
  'source/renderer/vulkan/device.c',
  'source/renderer/vulkan/loader.c',
  'source/renderer/vulkan/shader.c',
  'source/renderer/vulkan/command.c',
  'source/renderer/vulkan/texture.c',
  'source/renderer/vulkan/instance.c',
  'source/renderer/vulkan/pipeline.c',
  'source/renderer/vulkan/renderer.c',
  'source/renderer/vulkan/swapchain.c',
  'source/renderer/vulkan/descriptors.c',
  'source/renderer/vulkan/synchronization.c',

  # horus renderer buffers layer [ vulkan ]
  'source/renderer/vulkan/buffers/index.c',
  'source/renderer/vulkan/buffers/vertex.c',
  'source/renderer/vulkan/buffers/uniform.c',
  'source/renderer/vulkan/buffers/instance.c',
]

# horus source files [ linux ]
if host_machine.system() == 'linux'
  horus_sources += [
    # horus platform layer [ linux ]
    'source/platform/linux/time.c',
    'source/platform/linux/memory.c',
    'source/platform/linux/window.c',
    'source/platform/linux/console.c',
    'source/platform/linux/filesystem.c',

    # horus input layer [ linux ]
    'source/platform/linux/input/mouse.c',
    'source/platform/linux/input/keyboard.c',

    # horus renderer layer [ vulkan ]
    'source/renderer/vulkan/linux/platform.c',
  ]

  horus_sources += horus_renderer_vulkan_sources
endif

# horus source files [ windows ]
if host_machine.system() == 'windows'
  horus_sources += [
    # horus platform layer [ windows ]
    'source/platform/windows/time.c',
    'source/platform/windows/memory.c',
    'source/platform/windows/window.c',
    'source/platform/windows/console.c',
    'source/platform/windows/filesystem.c',

    # horus input layer [ windows ]
    'source/platform/windows/input/mouse.c',
    'source/platform/windows/input/keyboard.c',

    # horus renderer layer [ vulkan ]
    'source/renderer/vulkan/windows/platform.c',
  ]

  horus_sources += horus_renderer_vulkan_sources
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
  cc = meson.get_compiler('c')

  # horus system dependencies [ linux ]
  m_dep = cc.find_library('m', required: true)

  # horus window dependencies [ linux ]
  xcb_dep = dependency('xcb')
  xcb_xkb_dep = dependency('xcb-xkb')
  xcb_keysyms_dep = dependency('xcb-keysyms')

  # horus renderer dependencies [ linux ]
  vulkan_dep = dependency('vulkan')

  # horus renderer textures dependencies [ linux ]
  ktx_dep = cc.find_library('ktx', required: true)

  horus_dependencies += [
    # horus system dependencies [ linux ]
    m_dep,

    # horus window dependencies [ linux ]
    xcb_dep,
    xcb_xkb_dep,
    xcb_keysyms_dep,

    # horus renderer dependencies [ linux ]
    vulkan_dep,

    # horus renderer textures dependencies [ linux ]
    ktx_dep,
  ]
endif

# horus dependencies [ windows ]
if host_machine.system() == 'windows'
  cc = meson.get_compiler('c')

  # horus window dependencies [ windows ]
  dwmapi_dep = cc.find_library('dwmapi', required: true)
  uxtheme_dep = cc.find_library('uxtheme', required: true)

  # horus renderer dependencies [ windows ]
  vulkan_dep = dependency('vulkan')

  # horus renderer textures dependencies [ windows ]
  ktx_dep = cc.find_library('ktx', required: true)

  horus_dependencies += [
    # horus window dependencies [ windows ]
    dwmapi_dep,
    uxtheme_dep,

    # horus renderer dependencies [ windows ]
    vulkan_dep,

    # horus renderer textures dependencies [ windows ]
    ktx_dep,
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
  sources: horus_sources,
  dependencies: horus_dependencies,
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
