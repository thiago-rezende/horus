-- xmake minimal version
set_xmakever("2.7.0")

-- project information
set_project("horus")
set_description("cross-platform application development framework")

-- project version
set_version("0.1.0", { build = "%Y%m%d%H%M" })

-- default rules
add_rules("mode.debug", "mode.check", "mode.releasedbg", "mode.release")

-- linux toolchain
if is_plat("linux") then
    set_toolchains("clang")
end

-- horus target
includes("horus")

-- sandbox target
includes("sandbox")