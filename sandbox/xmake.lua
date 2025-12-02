-- sandbox target
target('sandbox')
    -- kind
    set_kind('binary')

    -- languages
    set_languages('c11')

    -- working directory
    set_rundir('$(projectdir)')

    -- windows subsystem
    if is_plat('windows') then
      add_rules('win.sdk.application')
    end

    -- dependencies
    add_deps('horus')

    -- include directories
    add_includedirs('include', { public = false })

    -- source files
    add_files('source/*.c')

