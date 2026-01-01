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

void assertEGLError(const std::string &msg);
void assertOpenGLError(const std::string &msg);

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

class OffscreenBuffer{
public:
    OffscreenBuffer(int width,int height);
    ~OffscreenBuffer();
    int saveImage(std::string imagefile);
    virtual void anySettings();

protected:
   	GLuint frameBuffer;
	GLuint depthrenderbuffer;
 	GLuint t;
    int width,height;

};

