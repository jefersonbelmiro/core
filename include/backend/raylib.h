#pragma once

#include "backend/api.h"

typedef vec2_t Vector2;

int  GetScreenWidth(void);
int  GetScreenHeight(void);
bool IsWindowResized(void);

API screen_size_t backend_get_screen_size()
{
  return (screen_size_t) {
    .x = GetScreenWidth(),
    .y = GetScreenHeight(),
  };
}

API bool backend_is_window_resized()
{
  return IsWindowResized();
}

API void backend_init()
{

}

API void backend_fini()
{

}
