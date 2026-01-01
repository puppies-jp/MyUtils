#include <iostream>
#include <sstream>
#include <stdexcept>

/*
 * OpenCV for saving the render target as an image file.
 */
#include "render_context.h"

using namespace std;

#define ARRAY_MAX 10000000

float vertex[ARRAY_MAX];
int lines[ARRAY_MAX];

int vertexDataSize = 0, lineDataSize = 0;
int width = 1000, height = 1000;

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

	printf("vertex: %d Line Size: %d\n", vertexDataSize, lineDataSize);
}

void disp()
{

	static int r = 0;
	glPushMatrix();							 // 行列退避
	glRotated(double(r * 1), 2.0, 1.0, 0.0); //[2].回転

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
	printf("Make Context\n");
	Contexts processContexts;
	printf("Make frame buffer\n");
	OffscreenBuffer frameBuffer(width, height);

	glScalef(0.4, 0.4, 0.4);
	/* Render something.*/
	printf("InitialProc\n");
	InitialProc();
	disp();
	for (int i = 0; i < 10; i++)
	{
		disp();
		frameBuffer.saveImage("img" + std::to_string(i) + ".png");
	}
}