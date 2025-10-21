# horus header files
horus_headers = [
  # horus global header
  'horus/horus.h',

  # horus base layer
  'horus/definitions.h',

  # horus core layer
  'horus/core/types.h',
  'horus/core/assert.h',
  'horus/core/macros.h',
  'horus/core/numeric.h',
  'horus/core/strings.h',

  # horus containers layer
  'horus/containers/array.h',

  # horus application layer
  'horus/application/application.h',
  'horus/application/configuration.h',

  # horus logger layer
  'horus/logger/ansi.h',
  'horus/logger/logger.h',

  # horus input layer
  'horus/input/mouse.h',
  'horus/input/keyboard.h',

  # horus events layer
  'horus/events/base.h',
  'horus/events/mouse.h',
  'horus/events/keyboard.h',

  # horus platform layer
  'horus/platform/time.h',
  'horus/platform/memory.h',
  'horus/platform/window.h',
  'horus/platform/console.h',
  'horus/platform/debugger.h',
  'horus/platform/detection.h',
  'horus/platform/entrypoint.h',
  'horus/platform/filesystem.h',

  # horus platform input layer
  'horus/platform/input/mouse.h',
  'horus/platform/input/keyboard.h',

  # horus math layer
  'horus/math/matrix.h',
  'horus/math/vector.h',

  # horus types layer
  'horus/types/vertex.h',

  # horus renderer layer
  'horus/renderer/shader.h',
  'horus/renderer/general.h',
  'horus/renderer/renderer.h',
  'horus/renderer/pipeline.h',

  # horus renderer buffers layer
  'horus/renderer/buffers/index.h',
  'horus/renderer/buffers/vertex.h',
  'horus/renderer/buffers/uniform.h',
  'horus/renderer/buffers/instance.h',
]

horus_renderer_vulkan_headers = [
  # horus renderer layer [ vulkan ]
  'horus/renderer/vulkan/debug.h',
  'horus/renderer/vulkan/device.h',
  'horus/renderer/vulkan/loader.h',
  'horus/renderer/vulkan/shader.h',
  'horus/renderer/vulkan/command.h',
  'horus/renderer/vulkan/instance.h',
  'horus/renderer/vulkan/renderer.h',
  'horus/renderer/vulkan/pipeline.h',
  'horus/renderer/vulkan/platform.h',
  'horus/renderer/vulkan/swapchain.h',
  'horus/renderer/vulkan/descriptors.h',
  'horus/renderer/vulkan/synchronization.h',
]

# horus header files [ linux ]
if host_machine.system() == 'linux'
  horus_headers += [
    # horus platform layer [ linux ]
    'horus/platform/linux/window.h',
    'horus/platform/linux/entrypoint.h',

    # horus input layer [ linux ]
    'horus/platform/linux/input/mouse.h',
    'horus/platform/linux/input/keyboard.h',
  ]

  horus_headers += horus_renderer_vulkan_headers
endif

# horus header files [ windows ]
if host_machine.system() == 'windows'
  horus_headers += [
    # horus platform layer [ windows ]
    'horus/platform/windows/window.h',
    'horus/platform/windows/windows.h',
    'horus/platform/windows/entrypoint.h',

    # horus input layer [ windows ]
    'horus/platform/windows/input/mouse.h',
    'horus/platform/windows/input/keyboard.h',
  ]

  horus_headers += horus_renderer_vulkan_headers
endif

# horus headers installation
install_headers(horus_headers, preserve_path: true)
