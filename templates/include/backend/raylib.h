#pragma once

#include "backend/api.h"

API backend_config_t backend_config(void)
{
  return (backend_config_t) {
    .src_path   = "../raylib",
    .build_path = "../raylib/build",
    .inc_path   = "../raylib/src",
    .lib_path   = "../raylib/build/raylib",
    .libs       = "-lraylib -lm -lX11 -lGL -lpthread -ldl -lrt",
    .impl       = "backend/raylib_impl.h",
  };
}
