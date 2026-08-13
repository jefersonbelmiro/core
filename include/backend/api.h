#pragma once

#include "core/defs.h"

API void          backend_init();
API void          backend_fini();
API void          backend_main();
API void          backend_main_loop();
API screen_size_t backend_get_screen_size();
API bool          backend_is_window_resized();
