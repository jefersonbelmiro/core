#pragma once

#include "core/defs.h"

API const char *platform_name(void);
API void        platform_help(void);
API bool        platform_build(void);
API bool        platform_run(void);
API bool        platform_backend_build(void);
API bool        platform_is_mobile(void);
API bool        platform_has_touch(void);
