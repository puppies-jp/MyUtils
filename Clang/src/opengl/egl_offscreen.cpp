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

void InitialProc();

#define ARRAY_MAX 10000000

float vertex[ARRAY_MAX];
int lines[ARRAY_MAX];

int vertexDataSize = 0, lineDataSize = 0;
int width = 1000, height = 1000;

void disp()
{

	static int r = 0;
	glPushMatrix();						 //行列退避
	glRotated(double(r), 2.0, 1.0, 0.0); //[2].回転

	glClearColor(0, 0.0, 0, 0.7);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	// 頂点の描画
	glPointSize(3);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < vertexDataSize; i++)
			glArrayElement(i);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		glColor3f(0.6, 0.35, 0);
		for (int i = 0; i < lineDataSize; i++)
		{
			glArrayElement(lines[i * 3]);
			glArrayElement(lines[i * 3 + 1]);
			glArrayElement(lines[i * 3 + 2]);
		}
	}
	glEnd();

	glLineWidth(3);
	glColor3f(1.0, 0.75, 0);
	glBegin(GL_LINES);
	{
		for (int i = 0; i < lineDataSize; i++)
		{
			glArrayElement(lines[i * 3]);
			glArrayElement(lines[i * 3 + 1]);

			glArrayElement(lines[i * 3 + 1]);
			glArrayElement(lines[i * 3 + 2]);

			glArrayElement(lines[i * 3 + 2]);
			glArrayElement(lines[i * 3]);
		}
	}
	glEnd();

	r += 50;
	if (r > 360)
	{
		r = 0;
	}
}

int main()
{
	/*
	 * EGL initialization and OpenGL context creation.
	 */
	EGLDisplay display;
	EGLConfig config;
	EGLContext context;
	// EGLSurface surface;
	EGLint num_config;

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

	// surface = eglCreatePbufferSurface(display, config, nullptr);
	// assertEGLError("eglCreatePbufferSurface");

	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
	assertEGLError("eglMakeCurrent");

	/*
	 * Create an OpenGL framebuffer as render target.
	 */
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	assertOpenGLError("glBindFramebuffer");

	// デプスバッファ
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	assertOpenGLError("glFramebufferRenderbuffer");

	/*
	 * Create a texture as color attachment.
	 */
	GLuint t;
	glGenTextures(1, &t);

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
	 * GL_COLOR_ATTACHMENT0
	 * GL_DEPTH_COMPONENT
	 */
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
	assertOpenGLError("glFramebufferTexture2D");

	glEnable(GL_NORMALIZE);	 //法線の有効化
	glEnable(GL_DEPTH_TEST); // ZバッファON
	// gluPerspective(30.0, (double)height / (double)width, 1.0, 100.0);

	glRotatef(30, 1.0, 0, 0);
	glRotatef(50, 0, 1.0, 0);
	glRotatef(20, 0, 0.0, 1.0);
	glScalef(0.4, 0.4, 0.4);

	glViewport(0, 0, width, height);
	InitialProc();
	disp();

	/*
	 * Read the framebuffer's color attachment and save it as a PNG file.
	 */
	// glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	for (int i = 0; i < 10; i++)
	{
		disp();

		cv::Mat image(width, height, CV_8UC4);
		glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
		assertOpenGLError("glReadPixels");
		cv::imwrite("img" + std::to_string(i) + ".png", image);
	}
	/*
	 * Destroy context.
	 */
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &t);

	// eglDestroySurface(display, surface);
	// assertEGLError("eglDestroySurface");

	eglDestroyContext(display, context);
	assertEGLError("eglDestroyContext");

	eglTerminate(display);
	assertEGLError("eglTerminate");

	return 0;
}

void InitialProc()
{
	FILE *fpVData, *fpFData;
	fpVData = fopen("vData.txt", "r");
	fpFData = fopen("fData.txt", "r");

	if ((fpVData == NULL) || (fpFData == NULL))
	{
		printf("file error!!\n");
		return;
	}

	while (fscanf(fpVData, "%f, %f, %f", &vertex[vertexDataSize * 3], &vertex[vertexDataSize * 3 + 1], &vertex[vertexDataSize * 3 + 2]) != EOF)
		vertexDataSize += 1;

	while (fscanf(fpFData, "%d, %d, %d", &lines[lineDataSize * 3], &lines[lineDataSize * 3 + 1], &lines[lineDataSize * 3 + 2]) != EOF)
		lineDataSize += 1;
}
