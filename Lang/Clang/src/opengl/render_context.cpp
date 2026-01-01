#include <iostream>
#include <sstream>
#include <stdexcept>

/*
 * OpenCV for saving the render target as an image file.
 */
#include <opencv2/opencv.hpp>
#include "render_context.h"
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

Contexts::Contexts()
{
    /* EGL initialization and OpenGL context creation.*/
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assertEGLError("eglGetDisplay");

    eglInitialize(display, nullptr, nullptr);
    assertEGLError("eglInitialize");

    eglChooseConfig(display, nullptr, &config, 1, &num_config);
    assertEGLError("eglChooseConfig");

    eglBindAPI(EGL_OPENGL_API);
    assertEGLError("eglBindAPI");

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    assertEGLError("eglCreateContext");

    // src.surface = eglCreatePbufferSurface(src.display, src.config, nullptr);
    // assertEGLError("eglCreatePbufferSurface");

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
    assertEGLError("eglMakeCurrent");
}

Contexts::~Contexts()
{
    /* Destroy Context */
    eglDestroyContext(display, context);
    assertEGLError("eglDestroyContext");

    eglTerminate(display);
    assertEGLError("eglTerminate");
}

OffscreenBuffer::OffscreenBuffer(int width,int height)
{
    this->width = width;
    this->height = height;
    /* Create an OpenGL framebuffer as render target.*/
    glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	assertOpenGLError("glBindFramebuffer");

	glGenTextures(1, &t);
	// デプスバッファ
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	assertOpenGLError("glFramebufferRenderbuffer");

	/* Create a texture as color attachment.*/
	glBindTexture(GL_TEXTURE_2D, t);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	assertOpenGLError("glTexImage2D");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	/*
	 * Attach the texture to the framebuffer.
	 * GL_COLOR_ATTACHMENT0 , GL_DEPTH_COMPONENT
	 */
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
	assertOpenGLError("glFramebufferTexture2D");

    anySettings();
}

void OffscreenBuffer::anySettings()
{
    printf("Width: %d Height: %d\n",width,height);
	glViewport(0, 0, this->width, this->height);
    // バッファをクリアしておく
	glClearColor(0, 0.0, 0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // settings
    glEnable(GL_NORMALIZE);	 // 法線の有効化
	glEnable(GL_DEPTH_TEST); // ZバッファON

}

OffscreenBuffer::~OffscreenBuffer()
{
	/* Destroy Frame buffer. */
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &t);

}

int OffscreenBuffer::saveImage(std::string imagefile)
{
    // read buffer設定
    glReadBuffer(GL_COLOR_ATTACHMENT0);
	cv::Mat image(this->width, this->height, CV_8UC4);
	glReadPixels(0, 0, this->width, this->height, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
	assertOpenGLError("glReadPixels");
	cv::imwrite(imagefile, image);
    return 0;
}
