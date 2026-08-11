#pragma once

#include "core/defs.h"
#include "backend/raylib.h"

API void backend_start()
{
  backend_init();
}

API void backend_stop()
{
  backend_fini();
}
