#define MAX_TEXT_BUFFER_LENGTH 2048

#include "core/defs.h"
#include "core/io.h"
#include "core/so.h"
#include "core/string.h"
#include <unistd.h>

#define CC "gcc"

// Launcher. Compiles the executor (cli_project.c) with the selected platform
// implementation forced in via -include, so platform_build() etc. resolve to
// the target's implementation. -p selects it by filename (default linux).
int main(int argc, char **argv)
{
  if (argc > 1 && (str_eq(argv[1], "-s") || str_eq(argv[1], "--self"))) {
    so_exec(CC " core/tools/cli.c -I./core/include -std=c11 -o core/bin/cli");
    return 0;
  }

  bool fresh = !io_file_exists("include/app/defs.h");
  so_exec("mkdir -p include/app include/scenes include/platform include/backend src");
  so_exec("cp -rn core/templates/include/. include/");
  so_exec("cp -rn core/templates/src/. src/");
  so_exec("cp -n core/templates/compile_flags.txt compile_flags.txt");
  if (fresh) {
    printn(" - created project skeleton from templates (edit include/app/defs.h)");
  }

  if (!io_file_exists("build")) {
    so_exec("mkdir -p build");
  }

  const char *platform = "linux";
  const char *backend = "raylib";
  for (int i = 1; i < argc - 1; i++) {
    if (str_eq(argv[i], "-p") || str_eq(argv[i], "--platform")) {
      platform = argv[i + 1];
    }
    if (str_eq(argv[i], "--backend")) {
      backend = argv[i + 1];
    }
  }

  if (!so_exec(CC " -include platform/%s.h -include backend/%s.h -DMAX_TEXT_BUFFER_LENGTH=2048 "
      "core/tools/cli_project.c -I. -I./include -I./core/include -std=c11 -o build/cli",
      platform, backend)) {
    printn("[error] failed to build cli");
    return 1;
  }

  execv("./build/cli", argv);
  printn("[error] failed to exec ./build/cli");
  return 1;
}
