#define DEBUG 1
#define DEBUG_MEMORY_USAGE 1
#define SINFL_IMPLEMENTATION
#define SDEFL_IMPLEMENTATION

#include "core/mem.h"
#include "core/io.h"
#include "core/arena.h"
#include "core/package.h"
#include "include/core/defs.h"

int main()
{
  arena_t *arena = arena_create(MB(32), "main");

  package_def_t def = {0};
  package_count_t caps = {
    .textures = 1,
    .fonts = 1,
    .sounds = 1,
    .atlas = 1,
    .musics = 1,
  };
  package_def_init(&def, caps, arena);

  package_def_append_texture(&def, "TEXTURE_001", "resources/texture/001.jpg");
  package_def_append_atlas(&def, "ATLAS_01", "resources/texture/atlas_01_64.png", (float[2]){ 64, 64});
  package_def_append_font(&def, "FONT_MONOGRAM", "resources/font/monogram.ttf");
  package_def_append_sound(&def, "SFX_POWERUP_01", "resources/sounds/sfx/sfx_powerup_01.wav", 0.5, 1);
  package_def_append_music(&def, "MUSC_MENU_01", "resources/sounds/music/menu_01.mp3", 0.5);

  package_def_write_header(&def, "core");

  package_t pkg = {0};
  arena_t *pkg_arena = arena_create_sub(arena, MB(10), "package");
  package_def_make(&def, &pkg, pkg_arena);

  package_write(&pkg, "core", pkg_arena);

  package_t pkg_read = { 0 };
  package_read("core", &pkg_read, arena);

  if (pkg_read.count.textures)
    printn(" - tex[0]   : ext=%s size=%d", pkg_read.textures[0].ext, pkg_read.textures[0].size);
  if (pkg_read.count.atlas)
    printn(" - atl[0]   : ext=%s size=%d cell=(%.0f, %.0f)", pkg_read.atlas[0].ext, pkg_read.atlas[0].size, pkg_read.atlas[0].cell_size[0], pkg_read.atlas[0].cell_size[1]);
  if (pkg_read.count.fonts)
    printn(" - fnt[0]   : ext=%s size=%d", pkg_read.fonts[0].ext, pkg_read.fonts[0].size);
  if (pkg_read.count.sounds)
    printn(" - snd[0]   : ext=%s size=%d vol=%.2f max_active=%d", pkg_read.sounds[0].ext, pkg_read.sounds[0].size, pkg_read.sounds[0].volume, pkg_read.sounds[0].max_active);
  if (pkg_read.count.musics)
    printn(" - mus[0]   : ext=%s size=%d vol=%.2f", pkg_read.musics[0].ext, pkg_read.musics[0].size, pkg_read.musics[0].volume);

  return 0;



  arena_t *compress_arena = arena_create_sub(arena, MB(10), "compress");

  int data_size = 0;
  unsigned char *data = io_load_file_data("compile_flags.txt", &data_size, arena);

  printn("\n[data]", data);
  printn("  data : %s", data);
  printn("  size : %d", data_size);
  int comp_data_size = 0;
  unsigned char *comp_data = io_compress_data(data, data_size, &comp_data_size, compress_arena);

  io_save_file_data("/tmp/save_test.txt", comp_data, comp_data_size);

  printn("\n[compress]");
  printn(" - size : %d", comp_data_size);

  int decomp_data_size = 0;
  unsigned char *decomp_data = io_decompress_data(comp_data, comp_data_size, &decomp_data_size, compress_arena);


  // char *string = arena_push(compress_arena, char, 64);
  // strcpy(string, "some data overide arena");
  // printn("\n[test arena after restore]:\n - string: %s", string);

  printn("\n[decompress]");
  printn("  size : %d", decomp_data_size);
  printn("  data : %s", decomp_data);
  printn("  data[last] : '%c'", decomp_data[decomp_data_size - 1]);

  printn("");
  arena_print_stats(arena->debug_id);
  // arena_print_track(compress_arena->debug_id, false);
  printn("");
  mem_print_stats();
  printn("");

  return 0;
}
