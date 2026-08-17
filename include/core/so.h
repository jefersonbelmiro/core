#pragma once

#include "core/defs.h"
#include "core/string.h"
#include <assert.h>
#include <stdlib.h>

API bool so_exec(const char *format, ...)
{
  assert(format);

  static char buffer[MAX_TEXT_BUFFER_LENGTH];

  va_list args;
  va_start(args, format);
  int required_byte_count = vsnprintf(buffer, MAX_TEXT_BUFFER_LENGTH, format, args);
  if (required_byte_count >= MAX_TEXT_BUFFER_LENGTH) {
    printn("[ERROR] cmd(%d chars) >= MAX_TEXT_BUFFER_LENGTH(%d chars)", required_byte_count, MAX_TEXT_BUFFER_LENGTH);
    return false;
  }
  va_end(args);

  int error = system(buffer);
  if (error) {
    printn("cmd: %s", buffer);
    return false;
  }

  return true;
}
