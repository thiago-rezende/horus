-- xmake minimal version
set_xmakever('3.0.0')

-- project information
set_project('horus')

set_description('cross-platform application development framework')

-- project version
set_version('0.1.0', { build = '%Y%m%d%H%M' })

-- default rules
add_rules('mode.debug', 'mode.releasedbg', 'mode.release')

-- default toolchains
if is_plat('linux') then
  set_toolchains('clang')
end

if is_plat('windows') then
  set_toolchains('clang-cl')

  -- default msvc runtime
  set_runtimes('MD')
end

-- default options
option('tests', {default = true, description = 'build tests'})
option('sandbox', {default = true, description = 'build sandbox'})

-- custom configurations
includes('xmake')

-- assets target
includes('assets')

-- horus target
includes('horus')

-- sandbox target
if has_config('sandbox') then
  includes('sandbox')
end

-- test targets
if has_config('tests') then
  includes('tests')
end
