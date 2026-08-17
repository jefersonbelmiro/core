#pragma once

#include "app/defs.h"

#if BACKEND == BACKEND_RAYLIB
#include "backend/raylib_impl.h"
#elif BACKEND == BACKEND_SDL
#include "backend/sdl_impl.h"
#else
#error "unsupported backend"
#endif

#include "core/defs.h"

API void core_init()
{
  backend_init();
  app_init();
  app_scene_setup();
  app_start();
  backend_main();
}

