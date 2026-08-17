#if DEBUG
#define DEBUG_MEMORY_USAGE 1
#endif

#include "core/arena.h"
#include "core/defs.h"
#include "core/io.h"
#include "core/so.h"
#include "core/string.h"

#define CC "gcc"
#define INC "-I./include -I./core/include -I./../../raylib/src/"
#define DEPS "-L./build/raylib/raylib -lraylib -lm -lX11"

static arena_t *g_arena = NULL;

void show_cmd_line_help()
{
  char help[] = {
    "core/cli\n"
    " usage   : core/cli [options]\n"
    " options :\n"
    "    -h  --help          : show comand line usage help\n"
    "    -pi --project-init  : create project.cfg\n"
    "    -bb --backend-build : build current backend\n"
    "    -b  --build         : build src/main.c\n"
    "    -r  --run           : run build/main\n"
  };
  if (help[strlen(help) - 1] == '\n') {
    help[strlen(help) - 1] = 0x0;
  }
  printn("%s", help);
}

void compile(const char *source, const char *output)
{
  const char format[] = {
    CC " %s " INC " " DEPS " -o %s"
  };
  so_exec(format, source, output);
}

void project_init()
{
  if (io_file_exists("./project.h")) {
    printn("file exits ./project.h");

    if (!io_file_exists("include")) {
      io_mkdir("include");
    }

    if (!io_file_exists("include/scenes")) {
      so_exec("cp -rv core/templates/scenes include/scenes");
    }
    if (!io_file_exists("include/app")) {
      so_exec("cp -rv core/templates/app include/app");
    }
    if (!io_file_exists("compile_flags.txt")) {
      so_exec("cp -rv core/templates/compile_flags.txt ./");
      // const char *content = format_text(
      //   "-std=c11\n"
      //   "-I./core/include\n"
      //   "-I./include\n"
      //   "-I%s\n"
      //   "-DDEBUG=1\n"
      //   "-DLOG_LEVEL=5\n"
      //   "-DDEBUG_MEMORY_USAGE=1\n"
      //   "-DARENA_FALLBACK_MALLOC=1\n"
      //   "-DHOT_RELOAD=1\n"
      //   "-Wall\n"
      //   "-Wextra\n", 
      //   g_project.backend_inc_path
      // );
      // printn("content:\n%s", content);
      // io_save_file_data("compile_flags.txt", content, strlen(content));
    }

    if (!io_file_exists("./src")) {
      so_exec("cp -rv core/templates/src ./");
    }

    return;
  }

  so_exec("cp -rv core/templates/project.h ./");
  if (!io_file_exists("compile_flags.txt")) {
    so_exec("cp -rv core/templates/compile_flags.txt ./");
  }
  printn(
      "project.h created\n"
      " - edit and run core/cli again to create initial files"
  );
}

void backend_build()
{
  if (io_file_exists("libs/raylib")) {
    printn(" - backend raylib already exists in: libs/raylib");
  } else {
    printn(" - adding backend raylib...");
    so_exec("git submodule add --force https://github.com/raysan5/raylib.git "
        "libs/raylib");
  }
  so_exec("mkdir -p build/raylib");
  so_exec("cmake -S libs/raylib -B build/raylib "
      "  -DBUILD_EXAMPLES=OFF        "
      "  -DBUILD_GAMES=OFF           "
      "  -DCMAKE_BUILD_TYPE=Release  ");
  so_exec("make -C build/raylib -j$(nproc)");

  printn("raylib built");
}

void build()
{
  compile("src/main.c", "build/main");
}

void run()
{
  so_exec("build/main");
}

int main(int argc, char **argv)
{
  g_arena = arena_create(KB(64), "main");

  if (argc == 1) {
    show_cmd_line_help();
    return 0;
  }
  for (int i = 0; i < argc; i++) {
    if (str_eq(argv[i], "-h") || str_eq(argv[i], "--help")) {
      show_cmd_line_help();
      return 0;
    }
    if (str_eq(argv[i], "--project-init") || str_eq(argv[i], "-pi")) {
      project_init();
    }
    if (str_eq(argv[i], "--backend-build") || str_eq(argv[i], "-bb")) {
      backend_build();
    }
    if (str_eq(argv[i], "--build") || str_eq(argv[i], "-b")) {
      build();
    }
    if (str_eq(argv[i], "--run") || str_eq(argv[i], "-r")) {
      run();
    }
  }

#if DEBUG
  arena_print_stats(g_arena->debug_id);
  // arena_print_track(app->arena->debug_id, false);
  mem_print_stats();
#endif

  return 0;
}
