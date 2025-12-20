-- package definition
package('glad')
    -- package metadata
    set_homepage('https://gen.glad.sh/')
    set_description('Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator.')

    set_license('MIT')

    -- package remote origins
    add_urls('https://github.com/Dav1dde/glad/archive/refs/tags/$(version).tar.gz',
             'https://github.com/Dav1dde/glad.git')

    -- package versions
    add_versions('v2.0.8', '44f06f9195427c7017f5028d0894f57eb216b0a8f7c4eda7ce883732aeb2d0fc')

    -- package patches
    add_patches('v2.0.8', 'patches/v2.0.8/CMakeLists.patch', '1dc178c44c41b982b72f788dec926795cee95362a30cb3530ceb70a86219ea31')

    -- package default configs
    add_configs('api', {description = 'Apis to include in the generated glad library', default = '', type = 'string'})
    add_configs('extensions', {description = 'Extensions to include in the generated glad library', default = '', type = 'string'})

    -- package platform dependencies
    if is_plat('linux') then
      add_syslinks('dl')
    end

    -- package load porcedure
    on_load(function (package)
      if not package.is_built or package:is_built() then
        package:add('deps', 'cmake', 'python 3.x', {kind = 'binary'})
      end
    end)

    -- package install pricedure
    on_install('windows', 'linux', 'macosx', 'mingw', function (package)
      local configs = {}

      table.insert(configs, '-DCMAKE_BUILD_TYPE=' .. (package:debug() and 'Debug' or 'Release'))
      table.insert(configs, '-DBUILD_SHARED_LIBS=' .. (package:config('shared') and 'ON' or 'OFF'))

      if package:is_plat('windows') then
        table.insert(configs, '-DUSE_MSVC_RUNTIME_LIBRARY_DLL=' .. (package:runtimes():startswith('MT') and 'OFF' or 'ON'))
      end

      table.insert(configs, '-DGLAD_API=' .. package:config('api'))
      table.insert(configs, '-DGLAD_EXTENSIONS=' .. package:config('extensions'))

      import('package.tools.cmake').install(package, configs)
    end)

    on_test(function (package)
      assert(package:has_cfuncs('gladLoaderLoadGL', {includes = 'glad/gl.h'}))
    end)
