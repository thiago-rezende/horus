-- sandbox target
target("sandbox")
    -- target kind
    set_kind("binary")

    -- target languages
    set_languages("c99")

    -- public include directories
    add_includedirs("include", { public = true })

    -- debugging utils
    if is_mode("debug") or is_mode("releasedbg") then
        -- debugging definitions
        add_defines("__sandbox_debug__")
    end

    -- dependencies
    add_deps("horus")

    -- runtime environment
    on_run(function (target)
        -- library directories
        os.addenv("LD_LIBRARY_PATH", target:targetdir())

        -- application execution
        os.exec(target:targetfile())
    end)
