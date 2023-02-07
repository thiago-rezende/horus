#include <horus/horus.h>

#include <sandbox/sandbox.h>

void* sandbox_create() {
  return (void*)0;
}

HORUS_ENTRYPOINT(sandbox_create);
