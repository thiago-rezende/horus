#pragma once

#include <stdio.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/window.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

/* horus input layer */
#include <horus/input/mouse.h>
#include <horus/input/keyboard.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

extern application_t *application_create(void);
extern b8 application_destroy(application_t *application);

static b8 __console_create();
static b8 __console_enable_virtual_terminal_processing(HANDLE hConsoleHandle);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
  (void)hInst;     /* unused */
  (void)hInstPrev; /* unused */
  (void)cmdline;   /* unused */
  (void)cmdshow;   /* unused */

  /* create a console since using 'windows' subsystem */
  if (!__console_create()) {
    logger_error_format("<%s:v%s> <platform:%s> console creation failed", horus_project_name(), horus_project_version(),
                        horus_platform());
  }

  logger_info_format("<%s:v%s> <platform:%s> initializing", horus_project_name(), horus_project_version(),
                     horus_platform());

  application_t *application = application_create();

  if (!application) {
    logger_critical_format("<application:%p> creation failed", (void *)application);

    return 1;
  }

  logger_info_format("<application:%p> <name:%s> created", (void *)application, application->name);

  if (!application_setup(application)) {
    logger_critical_format("<application:%p> setup failed", (void *)application);

    application_destroy(application);

    return 1;
  }

  if (!application_run(application)) {
    logger_critical_format("<application:%p> execution failed", (void *)application);
  }

  if (!application_teardown(application)) {
    logger_critical_format("<application:%p> teardown failed", (void *)application);

    application_destroy(application);

    return 1;
  }

  if (!application_destroy(application)) {
    logger_critical_format("<application:%p> destruction failed", (void *)application);

    return 1;
  }

  logger_info_format("<application:%p> destroyed", (void *)application);

  logger_info_format("<%s:v%s> <platform:%s> terminating", horus_project_name(), horus_project_version(),
                     horus_platform());

  return 0;
}

b8 __console_create() {
  if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
    if (!AllocConsole()) {
      return false;
    }
  }

  FILE *output;

  freopen_s(&output, "CONIN$", "r", stdin);
  freopen_s(&output, "CONOUT$", "w", stdout);
  freopen_s(&output, "CONOUT$", "w", stderr);

  HANDLE hConIn = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  HANDLE hConOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  SetStdHandle(STD_INPUT_HANDLE, hConIn);
  SetStdHandle(STD_ERROR_HANDLE, hConOut);
  SetStdHandle(STD_OUTPUT_HANDLE, hConOut);

  __console_enable_virtual_terminal_processing(hConOut);

  return true;
}

b8 __console_enable_virtual_terminal_processing(HANDLE hConsoleHandle) {
  DWORD dwMode = 0;

  if (hConsoleHandle == INVALID_HANDLE_VALUE || hConsoleHandle == NULL) {
    return false;
  }

  if (!GetConsoleMode(hConsoleHandle, &dwMode)) {
    return false;
  }

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  if (!SetConsoleMode(hConsoleHandle, dwMode)) {
    return false;
  }

  return true;
}
