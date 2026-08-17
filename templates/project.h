#pragma once

typedef struct {
  char *name;
  char *binary;
  char *version;
  char *backend;
  char *backend_src_path;
  char *backend_build_path;
  char *backend_inc_path;
  char *backend_lib_path;
} project_t;

// all paths are relative to the project root (or absolute).
// point them to a SHARED backend dir so multiple projects reuse it.
static project_t g_project = {
  .name               = "main",
  .binary             = "main",
  .version            = "0.0.1",
  .backend            = "raylib",
  .backend_src_path   = "../raylib",
  .backend_build_path = "../raylib/build",
  .backend_inc_path   = "../raylib/src",
  .backend_lib_path   = "../raylib/build/raylib",
};
