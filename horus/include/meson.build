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

  # horus platform input layer
  'horus/platform/input/mouse.h',
  'horus/platform/input/keyboard.h',

  # horus renderer layer
  'horus/renderer/general.h',
  'horus/renderer/renderer.h',

  # horus vulkan renderer layer
  'horus/renderer/vulkan/renderer.h',
  'horus/renderer/vulkan/platform.h',
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
endif

# horus headers installation
install_headers(horus_headers, preserve_path: true)
