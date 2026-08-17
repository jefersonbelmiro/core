# core

Header-only C library distributed as a git submodule. Provides arena memory,
math, input, timing, tweens and scene management, plus a C CLI that scaffolds,
builds and runs projects for multiple target platforms.

## Requirements

- Linux
- gcc
- a backend library (raylib, SDL2, ...) available as a shared checkout

## Install

```
git submodule add <repository> core
```

## Commands

Run from the project root.

| command            | description                                    |
|--------------------|------------------------------------------------|
| `core/bin/cli -pi` | scaffold project files (create once)          |
| `core/bin/cli -b`  | build the project                             |
| `core/bin/cli -r`  | run the project binary                        |
| `core/bin/cli -bb` | build the backend library for this platform   |
| `core/bin/cli -pl` | list the available platform implementations   |
| `core/bin/cli -p NAME` | select the target platform (default `linux`) |
| `core/bin/cli -h`  | usage + the current platform's help           |
| `core/bin/cli -s`  | rebuild the CLI launcher                      |

`core/bin/cli` is a launcher. Each invocation compiles the executor
(`core/tools/cli_project.c`) and runs it, so configuration changes apply on the
next call. Commands produce no output on success; failures print the command
that failed and an `[error]` line.

## Configuration

`include/app/defs.h` is the project configuration. It is created once from a
template with defaults; edit it freely.

| macro                | purpose                                  |
|----------------------|------------------------------------------|
| `PROJECT_NAME`       | project name                             |
| `PROJECT_BINARY`     | output binary name                       |
| `PROJECT_VERSION`    | project version                          |
| `APP_WINDOW_NAME`    | window title                             |

## Backends

Backends are split into interface (core), implementation (core) and per-project
config (template), following the platform pattern:

- `core/include/backend/api.h` — the interface: `backend_config()`,
  `backend_init()`, `backend_fini()`, `backend_main()`, `backend_main_loop()`,
  `get_screen_size()`, `is_window_resized()`.
- `core/include/backend/<name>_impl.h` — the implementation (shared engine glue).
- `include/backend/<name>.h` — the project config: `backend_config()` returns
  the source/build/include/library paths, link flags and the implementation
  header. Created from a template; edit the paths per project.
- `--backend <name>` selects which config header the launcher injects into the
  executor; the selected config's `impl` is `-include`d into the game build.

```
core/bin/cli -b --backend raylib
core/bin/cli -b --backend sdl
```

Backends are shared across projects (the checkout is not copied); `-bb` builds
once into the config's `build_path`.

## Platforms

Platforms mirror the backend pattern: an interface plus one implementation per
target, selected at build time.

- `core/include/platform/api.h` — the interface:
  `platform_name()`, `platform_help()`, `platform_build()`, `platform_run()`,
  `platform_backend_build()`, `platform_is_mobile()`, `platform_has_touch()`.
- Implementations live in the **project** at `include/platform/<name>.h`
  (copied from `core/templates/include/platform/`). Each defines the interface
  functions and reads the `defs.h` macros directly.
- `-p <name>` selects the implementation by filename: the launcher compiles the
  executor with `-include platform/<name>.h`, so `platform_build()` resolves to
  that target. The game is compiled with `-DPLATFORM=PLATFORM_<NAME>` by the
  platform's `build()`, so game code branches with `#if PLATFORM ==
  PLATFORM_WEB` etc.

### Adding a platform

1. Copy `include/platform/linux.h` to `include/platform/<name>.h` and implement
   the `platform_*` functions for the target (its `build()` must pass
   `-DPLATFORM=PLATFORM_<NAME>`).
2. Define the id (`PLATFORM_<NAME>`) in `core/include/core/defs.h`.
3. Build with `core/bin/cli -b -p <name>`.

No core edits are required to customize a platform; the build recipe is yours.

## Templates

`core/templates/` mirrors the project layout:

```
core/templates/
  compile_flags.txt            -> ./compile_flags.txt
  src/main.c                   -> src/main.c
  include/app/defs.h           -> include/app/defs.h
  include/app/scenes.h         -> include/app/scenes.h
  include/scenes/main.h        -> include/scenes/main.h
  include/scenes/menu.h        -> include/scenes/menu.h
  include/platform/linux.h     -> include/platform/linux.h
```

Files are copied once (`-n`, no clobber); edits in the project are never
overwritten.

## Project layout

```
include/app/defs.h            project configuration
include/backend/              backend configs (per-backend paths/libs)
include/platform/             platform implementations
include/app/scenes.h          scene dispatch
include/scenes/               scene implementations
src/main.c                    entry point
core/                         this library (submodule)
build/                        build output
```

## Adding a backend

1. Implement `core/include/backend/<name>_impl.h` against
   `core/include/backend/api.h` (init/fini/main/main_loop, screen size, window
   state). It must be self-contained (no app includes).
2. Add a config template `core/templates/include/backend/<name>.h` with
   `backend_config()` returning the paths/libs and `.impl` pointing to the
   implementation header.
3. Build with `core/bin/cli -b --backend <name>`.

`boot.h` only includes `backend/api.h` (the interface); the implementation is
injected by the CLI build, like platforms.
