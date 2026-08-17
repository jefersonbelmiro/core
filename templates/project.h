typedef struct {
  char *name;
  char *binary;
  char *version;
  char *backend_inc_path;
  char *backend_lib_path;
  char *backend;
} project_t;

static project_t g_project = {
  .name             = "main",
  .binary           = "main",
  .version          = "0.0.1",
  .backend          = "raylib",
  .backend_inc_path = "./../raylib/src/",
  .backend_lib_path = "./../raylib/build",
};
