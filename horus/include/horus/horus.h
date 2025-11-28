#pragma once

/* horus base layer */
#include <horus/definitions.h>

/* platform detection */
#include <horus/platform/detection.h>

/* horus core layer */
#include <horus/core/hash.h>
#include <horus/core/types.h>
#include <horus/core/assert.h>
#include <horus/core/macros.h>
#include <horus/core/strings.h>

/* horus containers layer */
#include <horus/containers/array.h>
#include <horus/containers/common.h>
#include <horus/containers/hash_map.h>
#include <horus/containers/linked_list.h>

/* horus application layer */
#include <horus/application/application.h>
#include <horus/application/configuration.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/window.h>
#include <horus/platform/memory.h>
#include <horus/platform/console.h>
#include <horus/platform/debugger.h>
#include <horus/platform/filesystem.h>

#if !defined(HORUS_ENTRYPOINT_DISABLE)
#include <horus/platform/entrypoint.h>
#endif

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus input layer */
#include <horus/input/mouse.h>
#include <horus/input/keyboard.h>

/* horus events layer */
#include <horus/events/base.h>
#include <horus/events/mouse.h>
#include <horus/events/keyboard.h>

/* horus math layer */
#include <horus/math/matrix.h>
#include <horus/math/vector.h>
#include <horus/math/quaternion.h>

/* horus types layer */
#include <horus/types/vertex.h>

/* horus camera layer */
#include <horus/camera/camera.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>
#include <horus/renderer/texture.h>
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

/* horus renderer buffers layer */
#include <horus/renderer/buffers/index.h>
#include <horus/renderer/buffers/vertex.h>
#include <horus/renderer/buffers/uniform.h>
#include <horus/renderer/buffers/instance.h>
