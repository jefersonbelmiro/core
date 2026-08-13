#pragma once

#include "app/defs.h"
#include "core/app.h"
#include "core/defs.h"
#include "scenes/main.h"
#include "scenes/menu.h"

API void app_scene_setup()
{
  app_set_scene(SCENE_MENU);
}

API bool app_scene_entering() 
{
  app_t *app = app_ptr();
  switch (app->scene) {
  case SCENE_MAIN:
    return main_scene_entering(app->scene_state);
  case SCENE_MENU:
    return menu_scene_entering(app->scene_state);
  default: return true;
  }
}

API bool app_scene_exiting() 
{
  app_t *app = app_ptr();
  switch (app->scene) {
    case SCENE_MAIN:
      return main_scene_exiting(app->scene_state);
    case SCENE_MENU:
      return menu_scene_exiting(app->scene_state);
    default: return true;
  }
}

API void app_scene_init() 
{
  app_t *app = app_ptr();
  switch (app->scene) {
    case SCENE_MAIN:
      app->scene_state = main_scene_init();
    break;
    case SCENE_MENU:
      app->scene_state = menu_scene_init();
    break;
    default: break;
  }
}

API void app_scene_process(float delta) 
{
  app_t *app = app_ptr();
  switch (app->scene) {
    case SCENE_MAIN: 
      main_scene_process(app->scene_state, delta);
    break;
    case SCENE_MENU: 
      menu_scene_process(app->scene_state, delta);
    break;
    default: break;
  }
}

API void app_scene_draw() 
{
  app_t *app = app_ptr();
  switch (app->scene) {
    case SCENE_MAIN: 
      main_scene_draw(app->scene_state);
    break;
    case SCENE_MENU: 
      menu_scene_draw(app->scene_state);
    break;
    default: break;
  }
}

API void app_scene_free() 
{
  app_t *app = app_ptr();
  switch (app->scene) {
    case SCENE_MAIN: 
      main_scene_free(app->scene_state);
    break;
    case SCENE_MENU: 
      menu_scene_free(app->scene_state);
    break;
    default: break;
  }
}

API void app_scene_sync(scene_type_t scene, sync_signal_type_t signal)
{
  switch (scene) {
    case SCENE_MAIN:
      main_scene_sync(app_ptr()->scene_state, signal);
    break;
    case SCENE_MENU:
      menu_scene_sync(app_ptr()->scene_state, signal);
    break;
    default: break;
  }
}
