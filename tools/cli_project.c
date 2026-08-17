#include "core/defs.h"
#include "core/io.h"
#include "core/so.h"
#include "core/string.h"
#include "platform/api.h"
#include <dirent.h>

void show_cmd_line_help()
{
  printn(
    "core/cli\n"
    " usage   : core/cli [options]\n"
    " options :\n"
    "    -h  --help           : show command line usage help\n"
    "    -s  --self           : rebuild the cli bootstrap (core/bin/cli)\n"
    "    -pi --project-init   : scaffold project files (create once)\n"
    "    -bb --backend-build  : build the backend for the current platform\n"
    "    -b  --build          : build the project\n"
    "    -r  --run            : run the project binary\n"
    "    -pl --platforms      : list available platforms\n"
    "    -p  --platform NAME  : target platform (default linux)\n"
    "    --backend NAME       : target backend (default raylib)\n"
  );
}

void copy_if_missing(const char *src, const char *dst)
{
  if (!io_file_exists(dst)) {
    so_exec("cp %s %s", src, dst);
  }
}

void ensure_skeleton()
{
  if (!io_file_exists("include")) {
    so_exec("mkdir -p include/app include/scenes include/platform");
  }
  if (!io_file_exists("src")) {
    so_exec("mkdir -p src");
  }

  so_exec("cp -rn core/templates/include/. include/");
  so_exec("cp -rn core/templates/src/. src/");
  copy_if_missing("core/templates/compile_flags.txt", "compile_flags.txt");
}

void project_init()
{
  ensure_skeleton();
  printn(" - project initialized");
  printn(" - edit include/app/defs.h (name, binary, ...)");
}

void project_build()
{
  ensure_skeleton();
  if (!platform_build()) {
    printn("[error] build failed");
  }
}

void project_run()
{
  if (!platform_run()) {
    printn("[error] run failed");
  }
}

void project_backend_build()
{
  if (!platform_backend_build()) {
    printn("[error] backend build failed");
  }
}

void project_platforms()
{
  printn("platform: %s", platform_name());

  DIR *dir = opendir("include/platform");
  if (!dir) {
    printn("[error] no include/platform directory");
    return;
  }

  printn("files in include/platform/:");
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] == '.') continue;
    printn(" - %s", entry->d_name);
  }
  closedir(dir);
}

int main(int argc, char **argv)
{
  if (argc == 1) {
    show_cmd_line_help();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (str_eq(argv[i], "-h") || str_eq(argv[i], "--help")) {
      show_cmd_line_help();
      platform_help();
      return 0;
    }
    if (str_eq(argv[i], "-pi") || str_eq(argv[i], "--project-init")) { project_init(); }
    if (str_eq(argv[i], "-bb") || str_eq(argv[i], "--backend-build")) { project_backend_build(); }
    if (str_eq(argv[i], "-b") || str_eq(argv[i], "--build")) { project_build(); }
    if (str_eq(argv[i], "-r") || str_eq(argv[i], "--run")) { project_run(); }
    if (str_eq(argv[i], "-pl") || str_eq(argv[i], "--platforms")) { project_platforms(); }
  }

  return 0;
}
