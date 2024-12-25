#define GL_SILENCE_DEPRECATION
#include<stdio.h>
#include<stdlib.h>
#include <gl/glut.h>
#include<math.h>
#include<time.h>
#include<errno.h>

#define PI 3.14  // 定义π的值
float angleMoon = 0.0, angleEarth = 0.0, angleAstroid = 0.0,
angleMars = 0.0,
angleMercury = 0.0,
angleVenus = 0.0,
angleJupiter = 0.0,
angleSaturn = 0.0,
angleUranus = 30.0,
angleNeptune = 60.0,
anglePluto = 90.0,  // 初始化冥王星的旋转角度
angleComet = 0.0;  // 初始化彗星的旋转角度

GLfloat sx = 0.2, sy = 0.2, sz = 0.2;  // 定义太阳的缩放比例

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

GLuint textures[9];  // 纹理数组

// 加载纹理
void loadTexture(const char* filename, int index) {
    FILE* file;
    unsigned char* data;
    int width, height;

    // 使用 fopen_s 替换 fopen
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0) {
        printf("无法打开纹理文件: %s\n", filename);
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

// 初始化纹理
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


// 初始化光照
void initLighting()
{
    glEnable(GL_LIGHTING);  // 启用光照
    glEnable(GL_LIGHT7);  // 启用特定的光源

    glLightfv(GL_LIGHT7, GL_AMBIENT, qAmb);  // 设置环境光
    glLightfv(GL_LIGHT7, GL_DIFFUSE, qDif);  // 设置漫射光
    glLightfv(GL_LIGHT7, GL_SPECULAR, qSpec);  // 设置镜面光
}

// 初始化背景和其他设置
void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);  // 设置背景颜色为黑色
    glPointSize(1.0);  // 设置点的大小
    glLineWidth(2.0);  // 设置线条宽度
    glEnable(GL_TEXTURE_2D);  // 启用纹理
    initTextures();  // 初始化纹理
}

// 绘制背景
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

// 绘制行星轨道
void orbit()
{
    glColor3f(0.5, 0.5, 0.5);

    int i = 0;
    for (i = 0; i < 8; i++) // 八大行星
    {
        glPushMatrix();  // 保存当前变换矩阵
        if (i == 5)
        {
            glRotatef(45, 1.0, 0.0, 0.0);  // 特殊处理土星的倾斜角度
        }
        else
        {
            glRotatef(63, 1.0, 0.0, 0.0);  // 其他行星的轨道倾斜角度
        }
        glScalef(sc[i], sc[i], sc[i]);  // 按比例缩放轨道
        glBegin(GL_POINTS);
        double ang1 = 0.0;
        int i = 0;
        for (i = 0; i < 300; i++)
        {
            glVertex2d(cos(ang1), sin(ang1));  // 绘制轨道点
            ang1 += ang;
        }
        glEnd();
        glPopMatrix();  // 恢复之前保存的变换矩阵
    }
}

// 绘制场景
void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色和深度缓冲区
    glLoadIdentity();  // 重置变换矩阵
    background();  // 绘制背景
    orbit();  // 绘制轨道
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glEnable(GL_COLOR_MATERIAL);  // 启用颜色材质
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[0]);  // 绑定太阳纹理
    glColor3f(0.7, 0.5, 0.0);  // 设置颜色
    glScalef(sx, sy, sz);  // 缩放太阳
    glLightfv(GL_LIGHT7, GL_POSITION, qPos);  // 设置光源位置
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);  // 设置发光材质
    glutSolidSphere(1, 50, 50);  // 绘制太阳
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);  // 重置材质
    glPopMatrix();

    glScalef(0.2, 0.2, 0.2);  // 缩放比例
    // 绘制水星
    glPushMatrix();
    glRotatef(angleMercury, 0.0, 1.0, -0.5);
    glTranslatef(1.5, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[1]);  // 绑定水星纹理
    glColor3f(1.0, 0.9, 0.0);
    glScalef(0.08, 0.08, 0.08);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // 绘制金星
    glPushMatrix();
    glRotatef(angleVenus, 0.0, 1.0, -0.5);
    glTranslatef(2.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[2]);  // 绑定金星纹理
    glColor3f(0.9, 0.1, 0.0);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // 绘制地球和月球
    glPushMatrix();
    glRotatef(angleEarth, 0.0, 1.0, -0.5);
    glTranslatef(2.5, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[3]);  // 绑定地球纹理
    glColor3f(0.0, 0.1, 0.7);
    glScalef(0.23, 0.23, 0.23);
    glutSolidSphere(1, 50, 50);
    glPushMatrix();
    glRotatef(angleMoon, 0.0, 0.1, 0.05);
    glTranslatef(1.3, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, 0);  // 解绑纹理
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix(); // 月球绘制完成
    glPopMatrix(); // 地球绘制完成

    // 绘制火星
    glPushMatrix();
    glRotatef(angleMars, 0.0, 1.0, -0.5);
    glTranslatef(-3.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[4]);  // 绑定火星纹理
    glColor3f(0.05, 0.05, 0.01);
    glScalef(0.17, 0.17, 0.17);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // 绘制小行星带
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
    glPopMatrix(); // 小行星带绘制完成

    // 绘制木星及其卫星
    glPushMatrix();
    glRotatef(angleJupiter, 0.0, 1.0, -0.5);
    glTranslatef(-4.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[5]);  // 绑定木星纹理
    glColor3f(0.4, 0.2, 0.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(1, 50, 50);
    glPushMatrix();
    glRotatef(angleMoon, 1.0, -0.5, 0.0);
    glTranslatef(0.0, 0, 1.1);
    glBindTexture(GL_TEXTURE_2D, 0);  // 解绑纹理
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix(); // 木星卫星绘制完成
    glPopMatrix();

    // 绘制土星及其光环
    glPushMatrix();
    glRotatef(angleSaturn, 0.0, 1.0, -1.0);
    glTranslatef(-5.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[6]);  // 绑定土星纹理
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
    glPopMatrix(); // 光环绘制完成
    glPopMatrix();

    // 绘制天王星
    glPushMatrix();
    glRotatef(angleUranus, 0.0, 1.0, -0.5);
    glTranslatef(5.2, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[7]);  // 绑定天王星纹理
    glColor3f(0.0, 0.5, 0.9);
    glScalef(0.23, 0.23, 0.23);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // 绘制海王星
    glPushMatrix();
    glRotatef(angleNeptune, 0.0, 1.0, -0.5);
    glTranslatef(-5.7, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, textures[8]);  // 绑定海王星纹理
    glColor3f(0.0, 0.0, 0.9);
    glScalef(0.2, 0.2, 0.2);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    // 绘制冥王星
    glPushMatrix();
    glRotatef(anglePluto, 0.0, 1.0, -0.5);
    glTranslatef(6.2, 0.0, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    glScalef(0.1, 0.1, 0.1);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();  // 冥王星绘制完成

    // 绘制彗星
    glPushMatrix();
    glRotatef(angleComet, 0.0, 1.0, -0.5);
    glTranslatef(-6.7, 0.0, 0.0);
    glColor3f(0.6, 0.0, 0.6);  // 将颜色设置为紫红色
    glScalef(0.05, 0.05, 0.05);
    glutSolidSphere(1, 50, 50);  // 绘制彗星核心
    glBegin(GL_LINES);  // 绘制彗尾
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(-2.0, 0.5, 0.0);
    glEnd();
    glPopMatrix();  // 彗星绘制完成

    glPopMatrix();
    glFlush();  // 强制将命令送至渲染队列
}

// 更新行星位置
void update(int value) {
    // 更新月球的缩放比例
    if ((angleMoon >= 0 && angleMoon < 180))
    {
        sx -= 0.0003; sy -= 0.0003; sz -= 0.0003;
    }
    else { sx += 0.0003; sy += 0.0003; sz += 0.0003; }

    // 更新各行星的旋转角度
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

    // 请求重绘
    glutPostRedisplay();
    // 设定定时器以定期调用更新函数
    glutTimerFunc(20, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // 初始化GLUT库
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // 设置显示模式
    glutInitWindowPosition(0, 0);  // 设置窗口位置
    glutInitWindowSize(700, 700);  // 设置窗口大小
    glutCreateWindow("Solar System");  // 创建窗口
    initLighting();  // 初始化光照
    myinit();  // 初始化其他设置
    glutDisplayFunc(draw);  // 设置显示回调函数
    glutTimerFunc(25, update, 0);  // 设置定时器以更新行星位置
    glutMainLoop();  // 进入GLUT事件处理循环
    return 0;
}
