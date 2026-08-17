#pragma once

#include "backend/api.h"
#include "core/defs.h"

API const char *platform_name(void);
API void        platform_help(void);
API bool        platform_build(backend_config_t *config);
API bool        platform_run(void);
API bool        platform_backend_build(backend_config_t *config);
API bool        platform_is_mobile(void);
API bool        platform_has_touch(void);
API bool        platform_is_ready(void);
