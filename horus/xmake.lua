-- linux dependencies
if is_plat("linux") then
    add_requires("pthread", { system = true, configs = { shared = true } })
end

-- horus target
target("horus")
    -- target kind
    set_kind("shared")

    -- target languages
    set_languages("c99")

    -- public include directories
    add_includedirs("include", { public = true })

    -- debugging utils
    if is_mode("debug") or is_mode("releasedbg") then
        -- debugging definitions
        add_defines("__horus_debug__")

        -- debugging source files
    end

    -- linux platform
    if is_plat("linux") then
        -- platform dependent source files
        add_files("src/platform/linux/*.c")

        -- platform dependencies
        add_packages("pthread")
    end
