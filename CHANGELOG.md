# CHANGELOG
> This changelog is automaticaly generated.
> If you find any issues with this file, please report to the developers.

## [Unreleased]

 - [a3e1b7d] feat(events): add polling mechanism for keyboard state via keycodes (Thiago Rezende)
 - [f8e9420] chore(input): remove current keysymbol abstraction for a better and more functional design in the future (Thiago Rezende)
 - [3f5d5e6] fix(events|linux): update the event loop logic to allow state propagation even without an on_event callback (Thiago Rezende)
 - [6c43e65] feat(events): add initial polling mechanism with mouse state and improved some functions naming (Thiago Rezende)
 - [2b29704] chore(meson): update meson.build for header files and add platform detection (Thiago Rezende)
 - [ccbebf6] feat(events): add mouse scroll events and its linux implementation (Thiago Rezende)
 - [b2b434e] fix(window|linux): check if xcb reply error is NULL before dereferencing and free its resources when it fails (Thiago Rezende)
 - [501e671] feat(window|linux): add xcb-xkb extension and update the key repeat behaviour to prevent virtual release (Thiago Rezende)
 - [e4fc136] feat(input): add inital input layer with keyboard events and platform keycode mapping (Thiago Rezende)
 - [f66e332] chore: move mouse_position_t to input layer and set default build type to debug in settings.json (Thiago Rezende)
 - [a67454f] chore: improve include organization and move mouse button to the input layer (Thiago Rezende)
 - [8799c6b] feat(events): add mouse button press and release events (Thiago Rezende)
 - [8d16462] feat(events): add initial event propagation system (Thiago Rezende)
 - [465f0a7] refactor(window): replace 'strlen' calls with 'string_length' (Thiago Rezende)
 - [050a580] feat(window): add platform_window_is_fullscreen function to gather the window fullscreen state (Thiago Rezende)
 - [6aef1ee] style(window|linux): remove unnecessary comment blocks in favor of trailing commas (Thiago Rezende)
 - [5bfa57a] ci: update vulkan-validation-layers package name on system dependencies step for all jobs (Thiago Rezende)
 - [d59e3b5] ci: update vulkan-validation-layers package name on system dependencies step (Thiago Rezende)
 - [c7df296] feat(window): add platform_window_set_fullscreen function to control the full-screen state of the window (Thiago Rezende)
 - [e642081] chore: add .clangd and update changelog.sh (Thiago Rezende)
 - [767b257] feat(debugger): add platform dependent debugger break and rename some assertion functions (Thiago Rezende)
 - [737ddc5] refactor(logger): update logger function naming (Thiago Rezende)
 - [8dd2630] feat(horus): add assertion macros and debugger break (Thiago Rezende)
 - [21644c8] ci: add vulkan dependencies (Thiago Rezende)
 - [d64b9bb] build(horus): add vulkan dependency (Thiago Rezende)
 - [5443a66] feat(horus): add initial strings utilities implementation (Thiago Rezende)
 - [2636eef] feat(horus): add time and window headers to horus.h (Thiago Rezende)
 - [6014f1f] refactor(sandbox): clean some logger calls that was used for testing purpose (Thiago Rezende)
 - [41a2f30] feat(platform|linux): add absolute time and sleep functions (Thiago Rezende)
 - [ad2a31f] feat(window|linux): add on_update and on_render declarations and calls (Thiago Rezende)
 - [eb24f3b] build: set the required meson version (Thiago Rezende)
 - [dbf4c29] style(application): add spacing between includes and definitions (Thiago Rezende)
 - [b0fe2fb] feat(window|linux): add window title update function (Thiago Rezende)
 - [24fbf3e] feat(window|linux): add window size update function (Thiago Rezende)
 - [3c66276] feat(window|linux): add window size acquire function (Thiago Rezende)
 - [98b268e] chore(git): add git hooks setup script (Thiago Rezende)
 - [b11c1b3] chore(git): add pre-commit hook script (Thiago Rezende)
 - [5979d09] refactor(window|linux): remove unnecessary logger call on focus change event (Thiago Rezende)
 - [fea9cdb] style(window|linux): update xcb protocol and clase cookies naming (Thiago Rezende)
 - [57c3d5b] refactor(window|linux): remove unnecessary xcb dependencies (Thiago Rezende)
 - [8923939] ci(linux): add system dependencies setup to other steps (Thiago Rezende)
 - [1c3fb21] ci(linux): add system dependencies setup step (Thiago Rezende)
 - [f36d6c9] feat(window|linux): add xcb utility libraries (Thiago Rezende)
 - [1cca392] refactor(window|linux): remove unnecessary events masking (Thiago Rezende)
 - [bb9a73c] docs(changelog): update CHANGELOG.md (Thiago Rezende)
 - [4f140eb] feat(window|linux): add initial full-screen configuration (Thiago Rezende)
 - [66a16f2] feat(application): add initial resolution configuration (Thiago Rezende)
 - [f9a15d3] refactor(window): simplify debug logger calls (Thiago Rezende)
 - [7709b35] feat(horus): add compile time definitions (Thiago Rezende)
 - [3f85c3a] chore: add initial setup.sh script (Thiago Rezende)
 - [49bb655] docs(changelog): add initial markdown changelog generation (Thiago Rezende)
 - [7f315c9] refactor(logger): update the level order and make the level prefix indice more consistent (Thiago Rezende)
 - [75321c2] feat(application): move the application instance ownership to the client (Thiago Rezende)
 - [df5682f] feat(application): add initial application layer definition (Thiago Rezende)
 - [04b1c52] revert(ec05701): bring c version back to c99 (Thiago Rezende)
 - [2687856] style: change pointer alignment to right side (Thiago Rezende)
 - [4e28e73] build: move xcb dependency declaration to linux platform checking boundary (Thiago Rezende)
 - [b21c389] refactor(window|linux): use stdlib's free on objects allocated by xcb (Thiago Rezende)
 - [a97ca67] fix(window|linux): remove memory leak at window destruction (Thiago Rezende)
 - [55101ea] feat(window): add focus events checking (Thiago Rezende)
 - [8e613e6] feat(window): add initial window implementation for linux (Thiago Rezende)
 - [ec05701] build: update c version from c99 to c11 (Thiago Rezende)
 - [6aac15f] refactor(logger): change trace level color to white instead of gray (Thiago Rezende)
 - [b74e514] docs: add initial changelog generator script (Thiago Rezende)
 - [2920e70] refactor(logger): removed unnecessary macro declaration (Thiago Rezende)
 - [f06efe3] feat(logger): flush the streams after message write (Thiago Rezende)
 - [8f745b7] style: update indentation in some meson.build files (Thiago Rezende)
 - [484d6b0] docs: update project description (Thiago Rezende)
 - [743cb5a] refactor(logger): automatically add new line after logger message (Thiago Rezende)
 - [952ce66] feat(logger): add ansi escape colors for all logger levels (Thiago Rezende)
 - [a8a0b79] refactor(memory): update memory clearing function to platform_memory_clear (Thiago Rezende)
 - [fd0642f] feat(logger): add simple console logger (Thiago Rezende)
 - [2d26218] feat(platform): add simple memory allocation interface (Thiago Rezende)
 - [eafea93] style: add .editorconfig file (Thiago Rezende)
 - [80f8d1f] refactor(entrypoint): update entrypoint to a declaration instead of a macro (Thiago Rezende)
 - [ea5174a] feat(horus): add platform detection with predefined macros (Thiago Rezende)
 - [e52cdc1] feat(platform): add console abstraction (Thiago Rezende)
 - [f749f74] feat(horus): add core macro definitions (Thiago Rezende)
 - [6928bb7] feat(horus): add core types definitions (Thiago Rezende)
 - [f3c029d] feat(horus): add entrypoint declaration with HORUS_ENTRYPOINT macro (Thiago Rezende)
 - [0447a42] docs: update badges in README.md (Thiago Rezende)
 - [ed544a0] build: add pkg-config file generation (Thiago Rezende)
 - [b91d400] ci: remove setup-python cache configuration (Thiago Rezende)
 - [c1b3b74] ci: add bulid directory caching (Thiago Rezende)
 - [2f6d266] docs: add ci badge to README.md (Thiago Rezende)
 - [17f6151] ci: fix workflow syntax error (Thiago Rezende)
 - [f55b2c7] ci: add linux workflow (Thiago Rezende)
 - [3e8b3f3] build(horus): add headers installation target (Thiago Rezende)
 - [6b53447] build(horus): update machine checking to use host_machine instead of target_machine (Thiago Rezende)
 - [9bc1e19] build(sandbox): update target variable name (Thiago Rezende)
 - [2e419fe] build: add threads dependency (Thiago Rezende)
 - [c14dc5a] build: add clang-tidy config file (Thiago Rezende)
 - [e562076] build: move to meson build system (Thiago Rezende)
 - [757c65a] build: add sandbox target and initial xmake config (Thiago Rezende)
 - [50722a8] build: add horus target and initial xmake config (Thiago Rezende)
 - [f6f6bb6] docs: update README.md description (Thiago Rezende)
 - [97ad2e1] chore: initial commit (Thiago Rezende)
