#include "core/arena.h"
#include "core/cfg_parser.h"
#include "core/defs.h"
#include "core/io.h"
#include "core/so.h"
#include "core/string.h"
#include <string.h>

#define CC "gcc"
#define INC "-I./core/include -I./include -I./../../raylib/src/"
#define DEPS "-L./build/raylib/raylib -lraylib -lm -lX11"

typedef struct {
  const char *name;
  const char *version;
  const char *backend;
  const char *backend_inc_path;
  const char *backend_lib_path;
} project_config_t;

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
  if (io_file_exists("./project.cfg")) {
    cfg_tokens_t tokens = {0};
    arena_t *arena = arena_create(KB(10), "backend/build");
    cfg_file_parse("./project.cfg", &tokens, arena);

    project_config_t cfg = {0};
    for (u16 i = 0; i < tokens.key_value_count; i++) {
      if (strcmp(tokens.key_value[i].name, "name") == 0) {
        cfg.name = tokens.key_value[i].value;
      }
      else if (strcmp(tokens.key_value[i].name, "version") == 0) {
        cfg.version = tokens.key_value[i].value;
      }
      else if (strcmp(tokens.key_value[i].name, "backend") == 0) {
        cfg.backend = tokens.key_value[i].value;
      }
      else if (strcmp(tokens.key_value[i].name, "backend_inc_path") == 0) {
        cfg.backend_inc_path = tokens.key_value[i].value;
      }
      else if (strcmp(tokens.key_value[i].name, "backend_lib_path") == 0) {
        cfg.backend_lib_path = tokens.key_value[i].value;
      }
    }

    printn("[project]");
    printn(" name: %s", cfg.name);
    printn(" version: %s", cfg.version);
    printn(" backend: %s", cfg.backend);
    printn(" backend_inc_path: %s", cfg.backend_inc_path);
    printn(" backend_lib_path: %s", cfg.backend_lib_path);

    // create fiels
    if (!io_file_exists("scenes")) {
      so_exec("cp core/scenes scenes");
    }

    return;
  }

  const char content[] = {
    "name: main\n"
    "version: 0.0.1:\n"
    "backend: raylib\n"
    "backend_inc_path  : ../raylib/src/\n"
    "backend_lib_path  : ../raylib/build/raylib\n"
    "backend_build_path: ../raylib/build\n"
  };
  io_save_file_data("./project.cfg", content, strlen(content));
  printn(
      "project.cfg created\n"
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
  if (argc == 1) {
    show_cmd_line_help();
    return 0;
  }
  for (int i = 0; i < argc; i++) {
    if (start_with(argv[i], "-h") || start_with(argv[i], "--help")) {
      show_cmd_line_help();
      return 0;
    }
    if (start_with(argv[i], "--project-init") || start_with(argv[i], "-pi")) {
      project_init();
    }
    if (start_with(argv[i], "--backend-build") || start_with(argv[i], "-bb")) {
      backend_build();
    }
    if (start_with(argv[i], "--build") || start_with(argv[i], "-b")) {
      build();
    }
    if (start_with(argv[i], "--run") || start_with(argv[i], "-r")) {
      run();
    }
  }
  return 0;
}
