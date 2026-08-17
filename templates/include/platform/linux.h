#pragma once

#include "app/defs.h"
#include "backend/api.h"
#include "core/so.h"
#include "platform/api.h"

API const char *platform_name(void)
{
  return "linux";
}

API bool platform_is_mobile(void)
{
  return false;
}

API bool platform_has_touch(void)
{
  return false;
}

API void platform_help(void)
{
  printn("linux: gcc + raylib (-lX11 -lGL), native window");
}

API bool platform_is_ready(void)
{
  return true;
}

API bool platform_build(backend_config_t *b)
{
  return so_exec(
      "gcc -std=c11 "
      "-DPLATFORM=PLATFORM_LINUX "
      "-include platform/linux.h "
      "-include %s "
      "-I./include -I./core/include -I%s "
      "src/main.c "
      "-L%s %s "
      "-o build/%s",
      b->impl, b->inc_path, b->lib_path, b->libs, PROJECT_BINARY);
}

API bool platform_run(void)
{
  return so_exec("build/%s", PROJECT_BINARY);
}

API bool platform_backend_build(backend_config_t *b)
{
  if (!so_exec("mkdir -p %s", b->build_path)) return false;
  if (!so_exec("cmake -S %s -B %s "
      "-DBUILD_EXAMPLES=OFF "
      "-DBUILD_GAMES=OFF "
      "-DCMAKE_BUILD_TYPE=Release",
      b->src_path, b->build_path)) return false;
  return so_exec("make -C %s -j$(nproc)", b->build_path);
}
