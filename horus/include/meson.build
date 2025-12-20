# horus header files
horus_headers = [
  # horus global header
  'horus/horus.h',

  # horus base layer
  'horus/definitions.h',

  # horus core layer
  'horus/core/hash.h',
  'horus/core/types.h',
  'horus/core/assert.h',
  'horus/core/macros.h',
  'horus/core/numeric.h',
  'horus/core/strings.h',

  # horus containers layer
  'horus/containers/array.h',
  'horus/containers/common.h',
  'horus/containers/hash_map.h',
  'horus/containers/linked_list.h',

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
  'horus/math/quaternion.h',

  # horus types layer
  'horus/types/vertex.h',

  # horus camera layer
  'horus/camera/camera.h',

  # horus renderer layer
  'horus/renderer/shader.h',
  'horus/renderer/texture.h',
  'horus/renderer/material.h',
  'horus/renderer/pipeline.h',
  'horus/renderer/renderer.h',

  # horus renderer buffers layer
  'horus/renderer/buffers/index.h',
  'horus/renderer/buffers/vertex.h',
  'horus/renderer/buffers/uniform.h',
  'horus/renderer/buffers/instance.h',

]

horus_renderer_opengl_headers = [
  # horus renderer layer [ opengl ]
  'horus/renderer/opengl/shader.h',
  'horus/renderer/opengl/texture.h',
  'horus/renderer/opengl/renderer.h',
  'horus/renderer/opengl/pipeline.h',
  'horus/renderer/opengl/platform.h',

  # horus renderer loader layer [ opengl ]
  'horus/renderer/opengl/glad/gl.h',

  # horus renderer buffers layer [ opengl ]
  'horus/renderer/opengl/buffers/index.h',
  'horus/renderer/opengl/buffers/vertex.h',
  'horus/renderer/opengl/buffers/uniform.h',
  'horus/renderer/opengl/buffers/instance.h',
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

    # horus renderer loader layer [ opengl ]
    'horus/renderer/opengl/glad/egl.h',
  ]

  horus_headers += horus_renderer_opengl_headers
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

    # horus renderer loader layer [ opengl ]
    'horus/renderer/opengl/glad/wgl.h',
  ]

  horus_headers += horus_renderer_opengl_headers
endif

# horus headers installation
install_headers(horus_headers, preserve_path: true)
