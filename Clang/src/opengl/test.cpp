#include <GLUT/glut.h>
/*
 * g++ test.cpp -o sample01.out -Wall -framework GLUT -framework OpenGL
 */
//----------- プロトタイプ宣言 --------------//
void display();
void reshape(int w, int h);
void timer(int value);
void LIGHT_INIT();

//------------- OpenGLの初期設定 ------------------//
void GLUT_INIT()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); //ダブルバッファ、Zバッファ
    glutInitWindowSize(640, 480);
    glutCreateWindow("glut Primitive");
}

void GLUT_CALL_FUNC()
{
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 17);
}

void MY_INIT()
{
    // glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    LIGHT_INIT();
    glEnable(GL_NORMALIZE);  //法線の有効化
    glEnable(GL_DEPTH_TEST); // ZバッファON
}

//光源の設定
void LIGHT_INIT()
{
    float Light0Pos[] = {1, 1, 1, 0};             //光源の位置
    glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos); //位置だけ設定(あとはデフォルト)

    glEnable(GL_LIGHTING); //光源の有効化
    glEnable(GL_LIGHT0);
}

//------------- メイン関数 ----------------//
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    GLUT_INIT();
    GLUT_CALL_FUNC();
    MY_INIT();

    glutMainLoop();

    return 0;
}

//------------ ここからコールバック関数 ------------------//
void display()
{
    static int r = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();                      //行列退避
    glRotated(double(r), 2.0, 1.0, 0.0); //[2].回転
    // glutSolidCube(1.0);
    // glutWireCone(1.0, 2.0, 30, 30);
    // glutSolidSphere(1.0,50,50);
    glutSolidTorus(0.3, 0.5, 30, 30);
    glutWireTeapot(1.0);
    glPopMatrix(); //行列を戻す

    glutSwapBuffers();

    if (++r > 360)
    {
        r = 0;
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); //行列モード切替
    glLoadIdentity();            //行列初期化
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW); //行列モード切替
    glLoadIdentity();
    gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(t, timer, 17); //タイマー関数
}
