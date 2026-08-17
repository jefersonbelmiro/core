# core

Shared C engine used as a git submodule. Header-only (`static inline` API), plus a C CLI
that scaffolds and builds projects — no shell, no build scripts.

## add to a project

```
git submodule add <core-url> core
core/bin/cli -pi          # scaffold project (project.h, include/, src/, compile_flags.txt)
```

`core/bin/cli` is a bootstrap: on every call it compiles `core/tools/cli.c` against the
project's `project.h` (read as C, no parser) and runs it. Edit `project.h` and the next
command picks it up automatically.

## commands

```
core/bin/cli -s            rebuild the cli bootstrap (core/bin/cli)
core/bin/cli -pi           scaffold project files from project.h
core/bin/cli -pi -f        regenerate include/app/defs.h too
core/bin/cli -bb           build the configured backend (shared dir, one time)
core/bin/cli -b            build the project
core/bin/cli -r            run the project binary
```

## shared backend

`project.h` points to a backend dir reused by several projects (no per-project copy):

```c
static project_t g_project = {
  .name               = "main",
  .backend            = "raylib",
  .backend_src_path   = "../raylib",          // checkout (used by -bb)
  .backend_build_path = "../raylib/build",    // cmake output (used by -bb)
  .backend_inc_path   = "../raylib/src",      // -I for editor and build
  .backend_lib_path   = "../raylib/build/raylib", // -L dir containing libraylib.a
};
```

Paths are relative to the project root (or absolute).

## adding a backend

1. add `core/include/backend/<name>_impl.h` implementing `backend/api.h`.
2. extend `core/include/core/boot.h` dispatch (`#elif BACKEND == BACKEND_<NAME>`).
3. extend `core/tools/cli.c`: `project_build()` deps and `backend_build()`.
4. define `BACKEND_<NAME>` in `core/include/core/defs.h`, map it in `backend_id_name()`,
   and add its `vec2_t` to the `#if/#elif` in `core/templates/app/defs.h`.

Generated files (`include/app/defs.h`, `compile_flags.txt`) are rendered from templates
with `@TOKEN@` substitution (`@BACKEND@`, `@BACKEND_INC_PATH@`, `@BACKEND_LIB_PATH@`,
`@APP_WINDOW_NAME@`).
