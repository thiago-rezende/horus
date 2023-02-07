#pragma once

#include <stdio.h>

#define HORUS_ENTRYPOINT(application_create)                       \
  int main(int argc, char** argv, char** envp) {                   \
    printf("[ horus ] <linux> v0.1.0\n");                          \
                                                                   \
    void* application = application_create();                      \
                                                                   \
    printf("[ application ] <address:%p> created\n", application); \
                                                                   \
    return 0;                                                      \
  }
