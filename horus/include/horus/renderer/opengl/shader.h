#pragma once

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>

#define RENDERER_DEFAULT_SHADER_MODULE_VERTEX_STAGE_ENTRYPOINT "vertex_entrypoint"
#define RENDERER_DEFAULT_SHADER_MODULE_COMPUTE_STAGE_ENTRYPOINT "compute_entrypoint"
#define RENDERER_DEFAULT_SHADER_MODULE_FRAGMENT_STAGE_ENTRYPOINT "fragment_entrypoint"

struct __shader_module {};
