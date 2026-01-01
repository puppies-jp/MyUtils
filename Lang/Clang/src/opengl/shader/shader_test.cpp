#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <cstdlib>

#define GL_SILENCE_DEPRECATION 1
#define GL_GLEXT_PROTOTYPES 1
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

/*
 * g++ shader_test.cpp -o sample01.out -Wall -framework GLUT -framework OpenGL
 */
int width = 300;
int height = 300;

GLuint VertexArrayID;
GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
int initWindow(int argc, char **argv)
{

    glutInit(&argc, argv);
    // offscreen();

    // どこにwindowを表示するか
    glutInitWindowPosition(100, 100);
    // windowのサイズ
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Obj Test");
    return 0;
}

// 3頂点を表す3つのベクトルの配列
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,
    -1.0f,
    0.0f,
    1.0f,
    -1.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
};
GLuint vertexbuffer;
GLuint g_programID;

void disp()
{
    // これが頂点バッファを指し示すものとなります。
    // バッファのクリア
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //  色指定
    glColor4f(1.0, 0.0, 0.0, 1.0);

    // 3つの頂点座標をGPUに転送
    // GL_POINTS
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);

    glVertex2f(0.3, -0.3);
    glVertex2f(-0.3, -0.8);
    glVertex2f(0.3, -0.8);

    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, -0.5);

    glEnd();
    glFlush();
}

void timer(int x)
{
    glutPostRedisplay();
    glutTimerFunc(500, timer, 0); // 定期実行関数にtimerをセットする
}

int main(int argc, char **argv)
{
    initWindow(argc, argv);
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // ポイントスプライトの設定
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    g_programID = LoadShaders("SimpleVertexShader.vrtx", "SimpleFragmentShader.flag");
    glUseProgram(g_programID);
    glutDisplayFunc(disp);        // display関数をセットする
    glutTimerFunc(500, timer, 0); // 定期実行関数にtimerをセットする
    glViewport(0, 0, width, height);
    printf("this ?\n");
    // シェーダからGLSLプログラムを作りコンパイルする。
    glutMainLoop();
    // sleep(1000);
}

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
{

    // シェーダを作ります。
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER_EXT);

    // ファイルから頂点シェーダのコードを読み込みます。
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }

    // ファイルからフラグメントシェーダを読み込みます。
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // 頂点シェーダをコンパイルします。
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // 頂点シェーダをチェックします。
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<GLchar> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "ProgramErrorMsg: %s\n", &VertexShaderErrorMessage[0]);

    // フラグメントシェーダをコンパイルします。
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // フラグメントシェーダをチェックします。
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<GLchar> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "ProgramErrorMsg: %s\n", &FragmentShaderErrorMessage[0]);

    // プログラムをリンクします。
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // プログラムをチェックします。
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<GLchar> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "ProgramErrorMsg: %s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    printf("ProgramID: %d\n", ProgramID);
    return ProgramID;
}
