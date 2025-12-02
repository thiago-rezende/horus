/* horus base layer */
#include <horus/definitions.h>

const char *horus_project_name(void) {
  return HORUS_PROJECT_NAME;
}

const char *horus_project_version(void) {
  return HORUS_PROJECT_VERSION;
}

const char *horus_project_license(void) {
  return HORUS_PROJECT_LICENSE;
}

const char *horus_project_description(void) {
  return HORUS_PROJECT_DESCRIPTION;
}

unsigned int horus_project_version_major(void) {
  return HORUS_PROJECT_VERSION_MAJOR;
}

unsigned int horus_project_version_minor(void) {
  return HORUS_PROJECT_VERSION_MINOR;
}

unsigned int horus_project_version_patch(void) {
  return HORUS_PROJECT_VERSION_PATCH;
}

const char *horus_platform(void) {
  return HORUS_PLATFORM;
}
