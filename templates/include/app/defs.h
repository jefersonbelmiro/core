#pragma once

#include "core/defs.h"

#define PROJECT_NAME      "main"
#define PROJECT_BINARY    "main"
#define PROJECT_VERSION   "0.0.1"

#define APP_WINDOW_NAME   PROJECT_NAME

typedef enum scene_type_t {
  SCENE_NONE,
  SCENE_MENU,
  SCENE_MAIN,
  SCENE_COUNT,
} scene_type_t;

