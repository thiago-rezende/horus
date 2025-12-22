/* horus types layer */
#include <horus/types/vertex.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/shader.h>
#include <horus/renderer/opengl/pipeline.h>

graphics_pipeline_t *graphics_pipeline_create(renderer_t *renderer, shader_module_t *module) {
  (void)renderer; /* unused */

  graphics_pipeline_t *pipeline = platform_memory_allocate(sizeof(graphics_pipeline_t));

  *pipeline = (graphics_pipeline_t){
      .module = module,
  };

  return pipeline;
}

b8 graphics_pipeline_destroy(graphics_pipeline_t *pipeline) {
  platform_memory_deallocate(pipeline);

  return true;
}

b8 graphics_pipeline_bind(graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)renderer; /* unused */

  glUseProgram(pipeline->module->shader_program);

  return true;
}
