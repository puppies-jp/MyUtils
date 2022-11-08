#include <iostream>
#include <sstream>
#include <stdexcept>

/*
 * OpenCV for saving the render target as an image file.
 */
#include <opencv2/opencv.hpp>

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

using namespace std;

void assertOpenGLError(const std::string &msg)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        stringstream s;
        s << "OpenGL error 0x" << std::hex << error << " at " << msg;
        throw runtime_error(s.str());
    }
}

void assertEGLError(const std::string &msg)
{
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        stringstream s;
        s << "EGL error 0x" << std::hex << error << " at " << msg;
        throw runtime_error(s.str());
    }
}

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
Contexts processContexts;

int makeContext(Contexts &src);
void endContext(Contexts &src);

Contexts::Contexts()
{
    makeContext(*this);
}

Contexts::~Contexts()
{
    endContext(*this);
}

int makeContext(Contexts &src)
{
    /*
     * EGL initialization and OpenGL context creation.
     */
    EGLContext display;
    EGLContext config;
    EGLContext context;
    // EGLSurface surface;
    EGLint num_config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assertEGLError("eglGetDisplay");

    eglInitialize(display, nullptr, nullptr);
    assertEGLError("eglInitialize");

    eglChooseConfig(display, nullptr,
                    &config, 1,
                    &num_config);
    assertEGLError("eglChooseConfig");

    eglBindAPI(EGL_OPENGL_API);
    assertEGLError("eglBindAPI");

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    assertEGLError("eglCreateContext");

    // surface = eglCreatePbufferSurface(display, config, nullptr);
    // assertEGLError("eglCreatePbufferSurface");

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
    assertEGLError("eglMakeCurrent");

    src.display = display;
    src.config = config;
    src.context = context;
    src.num_config = num_config;
    return 0;
}

void endContext(Contexts &src)
{
    eglDestroyContext(src.display, src.context);
    assertEGLError("eglDestroyContext");

    eglTerminate(src.display);
    assertEGLError("eglTerminate");
}