#include "core/string.h"

void show_cmd_line_help()
{
  const char help[] = {
    "core/cli\n"
    " usage   : core/cli [options]\n"
    " options :\n"
    "    -h --help     : show comand line usage help"
    "\n"
  };
  printn("%s", help);
}

bool parse_cmd_line(int argc, char **argv)
{
  for (int i = 0; i < argc; i++) {
    if (start_with(argv[i], "-h") || start_with(argv[i], "--help")) {
      show_cmd_line_help();
      return true;
    }
  }
  return false;
}

int main(int argc, char **argv)
{
  if (parse_cmd_line(argc, argv)) {
    return 0;
  }

  return 0;
}
