/*
 * EGL headers.
 */
#include <EGL/egl.h>

/*
 * OpenGL headers.
 */
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>


typedef struct Contexts
{
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    // EGLSurface surface;
    EGLint num_config;
    Contexts();
    ~Contexts();

} Contexts;
