#pragma once

#define APP_WINDOW_TRANSPARENT 1

#ifndef DEBUG
#define DEBUG 1
#endif

#define BACKEND BACKEND_RAYLIB
#define BACKEND_INC_PATH "./../../raylib/src/"
#define BACKEND_LIB_PATH "./../../raylib/build/"

#ifndef DEBUG_MEMORY_USAGE
#define DEBUG_MEMORY_USAGE 0
#endif

typedef enum scene_type_t {
  SCENE_NONE,
  SCENE_MENU,
  SCENE_MAIN,
  SCENE_COUNT,
} scene_type_t;

#if BACKEND == BACKEND_RAYLIB
#include "raylib.h"
typedef Vector2 vec2_t;
#elif BACKEND == BACKEND_SDL
#include <SDL.h>
typedef SDL_FPoint vec2_t;
#else
#error "unsupported backend"
#endif
