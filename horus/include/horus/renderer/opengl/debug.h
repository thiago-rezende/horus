/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

void renderer_opengl_debug_callback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const void *user_parameter);
