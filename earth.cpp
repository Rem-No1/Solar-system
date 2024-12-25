#define GL_SILENCE_DEPRECATION
#include<stdio.h>
#include<stdlib.h>
#include <gl/glut.h>
#include<math.h>
#include<time.h>
#include<errno.h>

#define PI 3.14  // ����е�ֵ
float angleMoon = 0.0, angleEarth = 0.0, angleAstroid = 0.0,
angleMars = 0.0,
angleMercury = 0.0,
angleVenus = 0.0,
angleJupiter = 0.0,
angleSaturn = 0.0,
angleUranus = 30.0,
angleNeptune = 60.0,
anglePluto = 90.0,  // ��ʼ��ڤ���ǵ���ת�Ƕ�
angleComet = 0.0;  // ��ʼ�����ǵ���ת�Ƕ�

GLfloat sx = 0.2, sy = 0.2, sz = 0.2;  // ����̫�������ű���

GLfloat black[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat white[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat blue[] = { 0.0f,0.0f,0.9f,1.0f };
GLfloat er[] = { 0.0f,5.0f,0.9f,1.0f };
GLfloat yellow[] = { 0.7f,0.2f,0.0f,1.0f };
GLfloat qAmb[] = { 0.1,0.1,0.1,1.0 };
GLfloat qDif[] = { 1.0,1.0,1.0,1.0 };
GLfloat qSpec[] = { .50,.50,.50,.10 };
GLfloat qPos[] = { 0,0,0,0.1 };
GLfloat sc[8] = { 0.295 , 0.40,0.50, 0.60,0.80,1.0,1.05,1.13 };
double ang = 2 * PI / 300;
double angular = 2 * PI / 50;

GLuint textures[9];  // ��������

// ��������
void loadTexture(const char* filename, int index) {
    FILE* file;
    unsigned char* data;
    int width, height;

    // ʹ�� fopen_s �滻 fopen
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0) {
        printf("�޷��������ļ�: %s\n", filename);
        return;
    }

    width = 256;
    height = 256;
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    glBindTexture(GL_TEXTURE_2D, textures[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
}

// ��ʼ������
void initTextures() {
    glGenTextures(9, textures);
    loadTexture("D:/C++opengl/KS/earth/BMP/sun.bmp", 0);
    loadTexture("D:/C++opengl/KS/earth/BMP/mercury.bmp", 1);
    loadTexture("D:/C++opengl/KS/earth/BMP/venus.bmp", 2);
    loadTexture("D:/C++opengl/KS/earth/BMP/earth.bmp", 3);
    loadTexture("D:/C++opengl/KS/earth/BMP/mars.bmp", 4);
    loadTexture("D:/C++opengl/KS/earth/BMP/jupiter.bmp", 5);
    loadTexture("D:/C++opengl/KS/earth/BMP/saturn.bmp", 6);
    loadTexture("D:/C++opengl/KS/earth/BMP/uranus.bmp", 7);
    loadTexture("D:/C++opengl/KS/earth/BMP/neptune.bmp", 8);
}


// ��ʼ������
void initLighting()
{
    glEnable(GL_LIGHTING);  // ���ù���
    glEnable(GL_LIGHT7);  // �����ض��Ĺ�Դ

    glLightfv(GL_LIGHT7, GL_AMBIENT, qAmb);  // ���û�����
    glLightfv(GL_LIGHT7, GL_DIFFUSE, qDif);  // ���������
    glLightfv(GL_LIGHT7, GL_SPECULAR, qSpec);  // ���þ����
}

// ��ʼ����������������
void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);  // ���ñ�����ɫΪ��ɫ
    glPointSize(1.0);  // ���õ�Ĵ�С
    glLineWidth(2.0);  // �����������
    glEnable(GL_TEXTURE_2D);  // ��������
    initTextures();  // ��ʼ������
}

// ���Ʊ���
void background()
{
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.00, 0.00);
    glVertex3f(-01.00, 01.00, 1);
    glColor3f(.20, 0.0, 0.70);
    glVertex3f(01.00, 1.00, 1);
    glColor3f(0, 0.0, 0.0);
    glVertex3f(1.00, -1.00, 1);
    glColor3f(.70, .10, .20);
    glVertex3f(-1.00, -1.00, 1);
    glEnd();
}

// �������ǹ��
void orbit()
{
    glColor3f(0.5, 0.5, 0.5);

    int i = 0;
    for (i = 0; i < 8; i++) // �˴�����
    {
        glPushMatrix();  // ���浱ǰ�任����
        if (i == 5)
        {
            glRotatef(45, 1.0, 0.0, 0.0);  // ���⴦�����ǵ���б�Ƕ�
        }
        else
        {
            glRotatef(63, 1.0, 0.0, 0.0);  // �������ǵĹ����б�Ƕ�
        }
        glScalef(sc[i], sc[i], sc[i]);  // ���������Ź��
        glBegin(GL_POINTS);
        double ang1 = 0.0;
        int i = 0;
        for (i = 0; i < 300; i++)
        {
            glVertex2d(cos(ang1), sin(ang1));  // ���ƹ����
            ang1 += ang;
        }
        glEnd();
        glPopMatrix();  // �ָ�֮ǰ����ı任����
    }
}

// ���Ƴ���
void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ����Ȼ�����
    glLoadIdentity();  // ���ñ任����
    background();  // ���Ʊ���
    orbit();  // ���ƹ��
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);  // ������Ȳ���
    glEnable(GL_COLOR_MATERIAL);  // ������ɫ����
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[0]);  // ��̫������
    glColor3f(0.7, 0.5, 0.0);  // ������ɫ
    glScalef(sx, sy, sz);  // ����̫��
    glLightfv(GL_LIGHT7, GL_POSITION, qPos);  // ���ù�Դλ��
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);  // ���÷������
    glutSolidSphere(1, 50, 50);  // ����̫��
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);  // ���ò���
    glPopMatrix();

    glScalef(0.2, 0.2, 0.2);  // ���ű���
    // ����ˮ��
    glPushMatrix();
    glRotatef(angleMercury, 0.0, 1.0, -0.5);
    glTranslatef(1.5, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[1]);  // ��ˮ������
    glColor3f(1.0, 0.9, 0.0);
    glScalef(0.08, 0.08, 0.08);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // ���ƽ���
    glPushMatrix();
    glRotatef(angleVenus, 0.0, 1.0, -0.5);
    glTranslatef(2.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[2]);  // �󶨽�������
    glColor3f(0.9, 0.1, 0.0);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // ���Ƶ��������
    glPushMatrix();
    glRotatef(angleEarth, 0.0, 1.0, -0.5);
    glTranslatef(2.5, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[3]);  // �󶨵�������
    glColor3f(0.0, 0.1, 0.7);
    glScalef(0.23, 0.23, 0.23);
    glutSolidSphere(1, 50, 50);
    glPushMatrix();
    glRotatef(angleMoon, 0.0, 0.1, 0.05);
    glTranslatef(1.3, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, 0);  // �������
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix(); // ����������
    glPopMatrix(); // ����������

    // ���ƻ���
    glPushMatrix();
    glRotatef(angleMars, 0.0, 1.0, -0.5);
    glTranslatef(-3.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[4]);  // �󶨻�������
    glColor3f(0.05, 0.05, 0.01);
    glScalef(0.17, 0.17, 0.17);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // ����С���Ǵ�
    glPushMatrix();
    glColor3f(3.30, 3.30, 3.30);
    glRotatef(63, 1.0, 0.0, 0.0);
    int j = 0, i = 0, div = 90; float siz = 2;
    float scl[4] = { 3.3,3.4,3.35,3.2 };
    for (j = 0; j < 4; j++)
    {
        glPushMatrix(); siz -= 0.3;
        glPointSize(siz);
        glScalef(scl[j], scl[j], scl[j]);
        glBegin(GL_POINTS);
        double ang1 = 0.0 - angleAstroid, a = (2 * PI) / div;
        for (i = 0; i < div; i++)
        {
            glVertex2d(cos(ang1), sin(ang1));
            ang1 += a;
        }
        div += 10;
        glEnd();
        glPopMatrix();
    }
    glPopMatrix(); // С���Ǵ��������

    // ����ľ�Ǽ�������
    glPushMatrix();
    glRotatef(angleJupiter, 0.0, 1.0, -0.5);
    glTranslatef(-4.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[5]);  // ��ľ������
    glColor3f(0.4, 0.2, 0.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(1, 50, 50);
    glPushMatrix();
    glRotatef(angleMoon, 1.0, -0.5, 0.0);
    glTranslatef(0.0, 0, 1.1);
    glBindTexture(GL_TEXTURE_2D, 0);  // �������
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix(); // ľ�����ǻ������
    glPopMatrix();

    // �������Ǽ���⻷
    glPushMatrix();
    glRotatef(angleSaturn, 0.0, 1.0, -1.0);
    glTranslatef(-5.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[6]);  // ����������
    glColor3f(0.9, 0.0, 0.0);
    glScalef(0.4, 0.4, 0.4);
    glutSolidSphere(1, 50, 50);
    glPushMatrix();
    glRotatef(45, 1.0, 0.0, 0.0);
    glPointSize(3);
    glColor3f(5.0, 3.0, 1.0);
    glScalef(1.2, 1.2, 1.2);
    glBegin(GL_POINTS);
    double ang1 = 0.0;
    i = 0;
    for (i = 0; i < 50; i++)
    {
        glVertex2d(cos(ang1), sin(ang1));
        ang1 += angular;
    }
    glEnd();

    glPointSize(2);
    glPopMatrix(); // �⻷�������
    glPopMatrix();

    // ����������
    glPushMatrix();
    glRotatef(angleUranus, 0.0, 1.0, -0.5);
    glTranslatef(5.2, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[7]);  // ������������
    glColor3f(0.0, 0.5, 0.9);
    glScalef(0.23, 0.23, 0.23);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // ���ƺ�����
    glPushMatrix();
    glRotatef(angleNeptune, 0.0, 1.0, -0.5);
    glTranslatef(-5.7, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[8]);  // �󶨺���������
    glColor3f(0.0, 0.0, 0.9);
    glScalef(0.2, 0.2, 0.2);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // ����ڤ����
    glPushMatrix();
    glRotatef(anglePluto, 0.0, 1.0, -0.5);
    glTranslatef(6.2, 0.0, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();  // ڤ���ǻ������

    // ��������
    glPushMatrix();
    glRotatef(angleComet, 0.0, 1.0, -0.5);
    glTranslatef(-6.7, 0.0, 0.0);
    glColor3f(0.6, 0.0, 0.6);  // ����ɫ����Ϊ�Ϻ�ɫ
    glScalef(0.05, 0.05, 0.05);
    glutSolidSphere(1, 50, 50);  // �������Ǻ���
    glBegin(GL_LINES);  // ������β
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(-2.0, 0.5, 0.0);
    glEnd();
    glPopMatrix();  // ���ǻ������

    glPopMatrix();
    glFlush();  // ǿ�ƽ�����������Ⱦ����
}

// ��������λ��
void update(int value) {
    // ������������ű���
    if ((angleMoon >= 0 && angleMoon < 180))
    {
        sx -= 0.0003; sy -= 0.0003; sz -= 0.0003;
    }
    else { sx += 0.0003; sy += 0.0003; sz += 0.0003; }

    // ���¸����ǵ���ת�Ƕ�
    angleMoon += 2;
    if (angleMoon > 360) {
        angleMoon -= 360;
    }
    angleEarth += 0.7;
    if (angleEarth > 360) {
        angleEarth -= 360;
    }
    angleMercury += 2;
    if (angleMercury > 360) {
        angleMercury -= 360;
    }
    angleVenus += 0.9;
    if (angleVenus > 360) {
        angleVenus -= 360;
    }
    angleMars += 0.5;
    if (angleMars > 360) {
        angleMars -= 360;
    }
    angleJupiter += 0.2;
    if (angleJupiter > 360) {
        angleJupiter -= 360;
    }
    angleSaturn += 0.1;
    if (angleSaturn > 360) {
        angleSaturn -= 360;
    }
    angleUranus += 0.05;
    if (angleUranus > 360) {
        angleUranus -= 360;
    }
    angleNeptune += 0.02;
    if (angleNeptune > 360) {
        angleNeptune -= 360;
    }
    anglePluto += 0.01;
    if (anglePluto > 360) {
        anglePluto -= 360;
    }
    angleComet += 1;
    if (angleComet > 360) {
        angleComet -= 360;
    }
    angleAstroid += 0.002;
    if (angleAstroid > 360) {
        angleAstroid -= 360;
    }

    // �����ػ�
    glutPostRedisplay();
    // �趨��ʱ���Զ��ڵ��ø��º���
    glutTimerFunc(20, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // ��ʼ��GLUT��
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // ������ʾģʽ
    glutInitWindowPosition(0, 0);  // ���ô���λ��
    glutInitWindowSize(700, 700);  // ���ô��ڴ�С
    glutCreateWindow("Solar System");  // ��������
    initLighting();  // ��ʼ������
    myinit();  // ��ʼ����������
    glutDisplayFunc(draw);  // ������ʾ�ص�����
    glutTimerFunc(25, update, 0);  // ���ö�ʱ���Ը�������λ��
    glutMainLoop();  // ����GLUT�¼�����ѭ��
    return 0;
}
