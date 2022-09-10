#include <stdio.h>
#include <vector>
#include <iostream>
#include <cstdlib>

//#include <OpenGL/gl.h>
//#include <GL/gl.h>
#include <GLUT/glut.h>
#include <opencv2/opencv.hpp>

/*
 * g++ test.cpp -o sample01.out -Wall -framework GLUT -framework OpenGL
 */

#define ARRAY_MAX 10000000

float vertex[ARRAY_MAX];
int lines[ARRAY_MAX];

int vertexDataSize = 0, lineDataSize = 0;
int width = 200, height = 200;
int isFirst = true;

void GL2Png()
{
    // 取得画像エリアの確保(width*height)
    unsigned char *buffer =
        (unsigned char *)malloc(width * height * 4 * sizeof(unsigned char));
    // フロントを読み込む様に設定する
    glReadBuffer(GL_FRONT);
    // 描画内容の読込(画像で出力する場合、RGB,BGRなどに合わせて取得するといい)
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, buffer);
    // glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    cv::Mat image = cv::Mat(height, width, CV_8UC3, buffer);
    // cv::Mat image = cv::Mat(height, width, CV_8UC4, buffer);
    cv::imwrite("output.bmp", image);
    free(buffer);
}

void disp()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

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

    glColor3f(0.6, 0.35, 0);
    glBegin(GL_TRIANGLES);
    {
        for (int i = 0; i < lineDataSize; i++)
        {
            glArrayElement(lines[i * 3]);
            glArrayElement(lines[i * 3 + 1]);
            glArrayElement(lines[i * 3 + 2]);
        }
    }
    glEnd();

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

    if (isFirst)
    {
        isFirst = false;
        GL2Png();
    }
    glFlush();
}

void timer(int x)
{
    glRotatef(1, 1.0, 1.0, 0);
    // glRotatef(1, 0, 1.0, 0);
    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
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

int main(int argc, char **argv)
{
    InitialProc();
    printf("dbug1");
    glutInit(&argc, argv);
    // どこにwindowを表示するか
    glutInitWindowPosition(100, 100);
    // windowのサイズ
    glutInitWindowSize(width, height);
    GLUT_RGB;
    GLUT_SINGLE;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Obj Test");

    /*
    // create object
    unsigned int objectId = 0;
    glGenObject(1, &objectId);
    // bind/assign object to context
    glBindObject(GL_WINDOW_TARGET, objectId);
    // set options of object currently bound to GL_WINDOW_TARGET
    glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
    glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
    // set context target back to default
    glBindObject(GL_WINDOW_TARGET, 0);
    */

    glRotatef(50, 1.0, 0, 0);
    glRotatef(30, 0, 1.0, 0);
    glScalef(0.6, 0.6, 0.6);
    // glScalef(0.8, 0.8, 0.8);
    glutDisplayFunc(disp);        // display関数をセットする
    glutTimerFunc(100, timer, 0); // 定期実行関数にtimerをセットする

    glutMainLoop();
    return 0;
}