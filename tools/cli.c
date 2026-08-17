#define MAX_TEXT_BUFFER_LENGTH 2048

#if DEBUG
#define DEBUG_MEMORY_USAGE 1
#endif

#include "core/arena.h"
#include "core/defs.h"
#include "core/io.h"
#include "core/so.h"
#include "core/string.h"

#ifndef CLI_PROJECT
#include <unistd.h>
#else
#include "project.h"
#endif

#define CC "gcc"

void show_cmd_line_help()
{
  printn(
    "core/cli\n"
    " usage   : core/cli [options]\n"
    " options :\n"
    "    -h  --help          : show command line usage help\n"
    "    -s  --self          : rebuild the cli bootstrap (core/bin/cli)\n"
    "    -pi --project-init  : scaffold project files from project.h\n"
    "    -bb --backend-build : build the configured backend (shared dir)\n"
    "    -b  --build         : build the project\n"
    "    -r  --run           : run the project binary\n"
    "    -f  --force         : with -pi, regenerate include/app/defs.h\n"
  );
}

#ifndef CLI_PROJECT

// ---------------------------------------------------------------------------
// bootstrap: recompiles the real cli against the project's project.h so the
// config is read as C (no parser).  run from the project root.
// ---------------------------------------------------------------------------

int main(int argc, char **argv)
{
  if (argc > 1 && (str_eq(argv[1], "-s") || str_eq(argv[1], "--self"))) {
    so_exec(CC " core/tools/cli.c -I./core/include -std=c11 -o core/bin/cli");
    return 0;
  }

  if (!io_file_exists("project.h")) {
    printn(" - project.h not found, creating from template");
    so_exec("cp core/templates/project.h ./project.h");
  }

  so_exec("mkdir -p build");

  if (!so_exec(CC " core/tools/cli.c -I. -I./core/include -DCLI_PROJECT -std=c11 -o build/cli")) {
    printn("[error] failed to build cli");
    return 1;
  }

  execv("./build/cli", argv);
  printn("[error] failed to exec ./build/cli");
  return 1;
}

#else

// ---------------------------------------------------------------------------
// project cli: compiled with -I. so it includes the project's project.h and
// reads g_project directly.
// ---------------------------------------------------------------------------

static char g_cmd[MAX_TEXT_BUFFER_LENGTH];
static arena_t *g_arena = NULL;

bool str_replace_all(char *buffer, size_t buffer_size, const char *token, const char *repl)
{
  char out[MAX_TEXT_BUFFER_LENGTH];
  size_t token_len = strlen(token);
  size_t out_len = 0;
  const char *cursor = buffer;
  const char *found = NULL;

  while ((found = strstr(cursor, token)) != NULL) {
    size_t copy_len = (size_t)(found - cursor);
    if (out_len + copy_len + strlen(repl) >= buffer_size) {
      printn("[error] template too big");
      return false;
    }
    memcpy(out + out_len, cursor, copy_len);
    out_len += copy_len;
    memcpy(out + out_len, repl, strlen(repl));
    out_len += strlen(repl);
    cursor = found + token_len;
  }

  size_t tail_len = strlen(cursor);
  if (out_len + tail_len + 1 >= buffer_size) {
    printn("[error] template too big");
    return false;
  }
  memcpy(out + out_len, cursor, tail_len);
  out_len += tail_len;
  out[out_len] = '\0';

  strcpy(buffer, out);
  return true;
}

const char *backend_id_name(const char *backend)
{
  if (str_eq((char *)backend, "raylib")) return "BACKEND_RAYLIB";
  if (str_eq((char *)backend, "sdl"))    return "BACKEND_SDL";
  if (str_eq((char *)backend, "godot"))  return "BACKEND_GODOT";
  return NULL;
}

void write_template(const char *template_path, const char *dst, bool force)
{
  if (io_file_exists((char *)dst) && !force) {
    printn(" - %s already exists (delete it or use -f to regenerate)", dst);
    return;
  }

  int size = 0;
  char *content = (char *)io_load_file_data(template_path, &size, g_arena);
  if (!content || size <= 0) {
    printn("[error] failed to read template %s", template_path);
    return;
  }

  const char *backend_id = backend_id_name(g_project.backend);
  if (!backend_id) {
    printn("[error] unsupported backend '%s'", g_project.backend);
    return;
  }

  char buffer[MAX_TEXT_BUFFER_LENGTH];
  snprintf(buffer, sizeof buffer, "%s", content);

  str_replace_all(buffer, sizeof buffer, "@BACKEND@", backend_id);
  str_replace_all(buffer, sizeof buffer, "@BACKEND_INC_PATH@", g_project.backend_inc_path);
  str_replace_all(buffer, sizeof buffer, "@BACKEND_LIB_PATH@", g_project.backend_lib_path);
  str_replace_all(buffer, sizeof buffer, "@APP_WINDOW_NAME@", g_project.name);

  io_save_file_data(dst, buffer, (int)strlen(buffer));
  printn(" - wrote %s", dst);
}

void project_build()
{
  char inc[MAX_TEXT_BUFFER_LENGTH];
  char deps[MAX_TEXT_BUFFER_LENGTH];

  snprintf(inc, sizeof inc, "-I./include -I./core/include -I%s", g_project.backend_inc_path);

  if (str_eq(g_project.backend, "raylib")) {
    snprintf(deps, sizeof deps,
        "-L%s -lraylib -lm -lX11 -lGL -lpthread -ldl -lrt",
        g_project.backend_lib_path);
  } else {
    printn("[error] backend '%s' deps not configured", g_project.backend);
    return;
  }

  snprintf(g_cmd, sizeof g_cmd,
      CC " -std=c11 %s src/main.c %s -o build/%s",
      inc, deps, g_project.binary);
  so_exec("%s", g_cmd);
}

void project_run()
{
  so_exec("build/%s", g_project.binary);
}

void project_init(bool force)
{
  so_exec("mkdir -p include/app include/scenes src");

  if (!io_file_exists("include/app/scenes.h")) {
    so_exec("cp core/templates/app/scenes.h include/app/scenes.h");
  }
  if (!io_file_exists("include/scenes/menu.h")) {
    so_exec("cp core/templates/scenes/menu.h include/scenes/menu.h");
  }
  if (!io_file_exists("include/scenes/main.h")) {
    so_exec("cp core/templates/scenes/main.h include/scenes/main.h");
  }
  if (!io_file_exists("src/main.c")) {
    so_exec("cp core/templates/src/main.c src/main.c");
  }

  write_template("core/templates/compile_flags.txt", "compile_flags.txt", true);
  write_template("core/templates/app/defs.h", "include/app/defs.h", force);

  printn(" - project initialized");
  printn(" - edit project.h to point to your shared backend, then re-run");
}

void backend_build()
{
  if (str_eq(g_project.backend, "raylib")) {
    so_exec("mkdir -p %s", g_project.backend_build_path);
    so_exec("cmake -S %s -B %s "
        "-DBUILD_EXAMPLES=OFF "
        "-DBUILD_GAMES=OFF "
        "-DCMAKE_BUILD_TYPE=Release",
        g_project.backend_src_path, g_project.backend_build_path);
    so_exec("make -C %s -j$(nproc)", g_project.backend_build_path);
    printn(" - backend '%s' built", g_project.backend);
  } else {
    printn(" - backend '%s' build not implemented yet", g_project.backend);
  }
}

int main(int argc, char **argv)
{
  g_arena = arena_create(KB(64), "cli");
  bool force = false;

  if (argc == 1) {
    show_cmd_line_help();
    arena_fini(g_arena);
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (str_eq(argv[i], "-f") || str_eq(argv[i], "--force")) {
      force = true;
    }
  }

  for (int i = 1; i < argc; i++) {
    if (str_eq(argv[i], "-h") || str_eq(argv[i], "--help")) { show_cmd_line_help(); return 0; }
    if (str_eq(argv[i], "-pi") || str_eq(argv[i], "--project-init")) { project_init(force); }
    if (str_eq(argv[i], "-bb") || str_eq(argv[i], "--backend-build")) { backend_build(); }
    if (str_eq(argv[i], "-b") || str_eq(argv[i], "--build")) { project_build(); }
    if (str_eq(argv[i], "-r") || str_eq(argv[i], "--run")) { project_run(); }
  }

  return 0;
}

#endif
