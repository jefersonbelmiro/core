#pragma once

#include "core/defs.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define BACKEND            @BACKEND@
#define BACKEND_INC_PATH   "@BACKEND_INC_PATH@"
#define BACKEND_LIB_PATH   "@BACKEND_LIB_PATH@"
#define APP_WINDOW_NAME    "@APP_WINDOW_NAME@"

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
#define COLOR_PRIMARY_DARK_2 (Color){ 24, 24, 32, 255 }
#elif BACKEND == BACKEND_SDL
#include <SDL.h>
typedef SDL_FPoint vec2_t;
#else
#error "unsupported backend"
#endif
