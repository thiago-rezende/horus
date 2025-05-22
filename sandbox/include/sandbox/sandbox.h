#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus application layer */
#include <horus/application/application.h>

application_t *application_create();
void application_destroy(application_t *application);

b8 on_event(event_t *event);
b8 on_render(void);
b8 on_update(f64 timestep);
