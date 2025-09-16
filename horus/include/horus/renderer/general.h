#pragma once

typedef enum __renderer_implementation {
  RENDERER_IMPLEMENTATION_NONE = 0,
  RENDERER_IMPLEMENTATION_VULKAN,
  RENDERER_IMPLEMENTATION_COUNT
} renderer_implementation_t;

const char *__renderer_implementation_string(renderer_implementation_t renderer_implementation);
