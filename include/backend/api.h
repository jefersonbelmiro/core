#pragma once

#include "core/defs.h"

typedef struct {
  const char *src_path;
  const char *build_path;
  const char *inc_path;
  const char *lib_path;
  const char *libs;
  const char *impl;
} backend_config_t;

API backend_config_t backend_config();
API void             backend_init();
API void             backend_fini();
API void             backend_main();
API void             backend_main_loop();
API screen_size_t    get_screen_size();
API bool             is_window_resized();
