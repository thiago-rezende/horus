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

  f64 timestep = 0;
  f64 current_absolute_time = platform_absolute_time();
  f64 previous_absolute_time = platform_absolute_time();

  input_mouse_clear_state();
  input_keyboard_clear_state();

  while (!platform_window_should_close(application->window)) {
    platform_window_process_events(application->window);

    /* check window state before proceeding to prevent renderer problems */
    if (platform_window_should_close(application->window)) {
      break;
    }

    previous_absolute_time = current_absolute_time;
    current_absolute_time = platform_absolute_time();

    timestep = current_absolute_time - previous_absolute_time;

    if (application->on_update) {
      if (!application->on_update(timestep)) {
        logger_error_format("<application:%p> <on_update> failed", (void *)application);
      }
    }

    /* TODO: improve for multiple windows support */
    if (renderer_record_commands(application->renderer)) {
      if (application->on_render) {
        if (!application->on_render(application->renderer)) {
          logger_error_format("<application:%p> <on_render> failed", (void *)application);
        }
      }

      renderer_submit_commands(application->renderer);
    }
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
