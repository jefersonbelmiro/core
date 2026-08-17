#pragma once

#include "backend/api.h"

API backend_config_t backend_config(void)
{
  return (backend_config_t) {
    .src_path   = "../SDL",
    .build_path = "../SDL/build",
    .inc_path   = "../SDL/include",
    .lib_path   = "../SDL/build",
    .libs       = "-lSDL2 -lm",
    .impl       = "backend/sdl_impl.h",
  };
}
