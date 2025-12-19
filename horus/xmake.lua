-- horus dependencies
if is_plat('linux') then
  add_requires('ktx')

  add_requires('glad', {configs = {spec = 'gl,glx', api = 'gl=4.6,glx=1.4', profile = 'core'}})
  add_requires('opengl')
end

if is_plat('windows') then
  add_requires('ktx')

  add_requires('glad', {configs = {spec = 'gl,wgl', api = 'gl=4.6,wgl=1.0', profile = 'core'}})
  add_requires('opengl')
end

-- horus target
target('horus')
    -- kind
    set_kind('static')

    -- languages
    set_languages('c11')

    -- dependencies
    if is_plat('linux') then
      add_packages('ktx')

      add_packages('glad')
      add_packages('opengl')

      add_links('xcb', 'xcb-xkb', 'xcb-keysyms')
    end

    if is_plat('windows') then
      add_packages('ktx')

      add_packages('glad')
      add_packages('opengl')

      add_links('gdi32', 'user32', 'shell32', 'kernel32', 'dwmapi', 'uxtheme')
    end

    -- definitions
    add_defines('HORUS_PLATFORM="$(plat)"')

    add_defines('HORUS_PROJECT_NAME="horus"')
    add_defines('HORUS_PROJECT_VERSION="0.1.0"')
    add_defines('HORUS_PROJECT_LICENSE="BSD-3-Clause"')
    add_defines('HORUS_PROJECT_DESCRIPTION="A simple closs-platform application development framework"')

    add_defines('HORUS_PROJECT_VERSION_MAJOR=0')
    add_defines('HORUS_PROJECT_VERSION_MINOR=1')
    add_defines('HORUS_PROJECT_VERSION_PATCH=0')

    -- debugging utilities
    if is_mode('debug') or is_mode('releasedbg') then
        -- debugging definitions
        add_defines('__horus_debug__')
    end

    -- include directories
    add_includedirs('include', { public = true })

    -- base layer
    add_files('source/*.c')

    -- core layer
    add_files('source/core/*.c')

    -- math layer
    add_files('source/math/*.c')

    -- input layer
    add_files('source/input/*.c')

    -- camera layer
    add_files('source/camera/*.c')

    -- events layer
    add_files('source/events/*.c')

    -- logger layer
    add_files('source/logger/*.c')

    -- platform layer
    if is_plat('linux') then
      -- platform layer [ linux ]
      add_files('source/platform/linux/*.c')

      -- platform input layer [ linux ]
      add_files('source/platform/linux/input/*.c')
    end

    if is_plat('windows') then
      -- platform layer [ windows ]
      add_files('source/platform/windows/*.c')

      -- platform input layer [ windows ]
      add_files('source/platform/windows/input/*.c')
    end

    -- renderer layer
    add_files('source/renderer/*.c')

    if is_plat('linux', 'windows') then
      -- platform renderer layer
      add_files('source/renderer/opengl/*.c')

      -- platform renderer buffers layer
      add_files('source/renderer/opengl/buffers/*.c')

      if is_plat('linux') then
        -- platform renderer layer [ linux ]
        add_files('source/renderer/opengl/linux/*.c')
      end

      if is_plat('windows') then
        -- platform renderer layer [ windows ]
        add_files('source/renderer/opengl/windows/*.c')
      end
    end

    -- containers layer
    add_files('source/containers/*.c')

    -- application layer
    add_files('source/application/*.c')


