#include <horus/horus.h>

#include <sandbox/sandbox.h>

application_t *application_create() {
  application_t *application = platform_memory_allocate(sizeof(application_t));
  platform_memory_clear(application, sizeof(application_t));

  application->name = "Sandbox Application";

  application->on_update = on_update;
  application->on_render = on_render;

  application->configuration = (configuration_t){.windowed = true, /* */
                                                 .resolution = (resolution_t){
                                                     .width = 1280, /* */
                                                     .height = 720  /* */
                                                 }};

  return application;
}

void application_destroy(application_t *application) {
  platform_memory_deallocate(application);
}

b8 on_render(void) {
  return true;
}

b8 on_update(f64 timestep) {
  return true;
}
