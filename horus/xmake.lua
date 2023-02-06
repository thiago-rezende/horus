-- project information
set_project("horus")
set_description("cross-platform application development framework")

-- project version
set_version("0.1.0", { build = "%Y%m%d%H%M" })

-- default rules
add_rules("mode.debug", "mode.check", "mode.releasedbg", "mode.release")


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

    -- linux toolchain
    if is_plat("linux") then
        set_toolchains("clang")
    end