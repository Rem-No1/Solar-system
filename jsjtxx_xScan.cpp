#include <gl/glut.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <set>
using namespace std;

struct VERTEX { int x, y; };	// 点结构
vector<VERTEX> _polyline;	// 折线顶点列
vector<VERTEX> _polygon;	// 多边形顶点列

int winWidth = 800, winHeight = 600;     //窗口的宽度和高度

// 改进的Bresenham算法绘制直线段
void  Bhline(int  x0, int  y0, int  x1, int  y1);
// X-扫描线法实现多边形转换
void ScanTransferX(vector<VERTEX> polygon, COLORREF color);

// 窗口大小变化响应函数
void ChangeSize(int w, int h)
{
	winWidth = w;	winHeight = h;
	glViewport(0, 0, w, h);                 //指定窗口显示区域
	glMatrixMode(GL_PROJECTION);      //设置投影参数
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

// 初始化OpenGL场景
void Initial() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				//设置窗口背景颜色为白色
}

// 显示图形
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);   //用当前背景色填充窗口
	glColor3f(0.0f, 0.0f, 0.0f);

	// 此处需增加调用基本图形生成函数
	Bhline(0, 0, 100, 300);
	Bhline(0, 0, 300, 100);
	ScanTransferX(_polygon, RGB(255, 0, 0));

	glFlush();
}

// 第2个窗口中的图形绘制
void Displayw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	glFlush();
}

// 键盘响应函数
void KeyEvent(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
	{
		glutPostRedisplay();
	}
	break;
	default: break;
	}
}

int main(int argc, char* argv[]) {
	VERTEX p[7] = { {210, 160}, {90, 240}, {30, 140}, {90, 20}, {180, 100}, {240, 20}, {360, 180} };
	for (int i = 0; i < 7; i++)	_polygon.push_back(p[i]);

	glutInit(&argc, argv);			// glut初始化
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);		//初始化窗口的显示模式
	glutInitWindowSize(winWidth, winHeight);				//设置窗口的尺寸
	glutInitWindowPosition(100, 200);		//设置窗口的位置
	glutCreateWindow("原始图形");	//创建一个窗口
	glutDisplayFunc(Display);					//设置当前窗口的显示回调函数
	glutReshapeFunc(ChangeSize);							//指定窗口大小变化响应函数
	glutKeyboardFunc(KeyEvent);								//指定键盘响应函数
	Initial();					//完成窗口初始化

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);	//设置窗口的尺寸
	glutInitWindowPosition(150 + winWidth, 200);		//设置窗口的位置
	glutCreateWindow("裁剪后的图形");
	glutDisplayFunc(Displayw);					//设置当前窗口的显示回调函数
	glutReshapeFunc(ChangeSize);			//指定窗口大小变化响应函数
	Initial();

	glutMainLoop();		//启动主GLUT事件处理循环
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 改进的Bresenham算法绘制直线段
void  Bhline(int  x0, int  y0, int  x1, int  y1) {
	int  x, y, dx, dy, e;
	bool bswap = abs(y1 - y0) > abs(x1 - x0);
	if (bswap) {
		e = x0, x0 = y0, y0 = e;
		e = x1, x1 = y1, y1 = e;
	}
	if (x0 > x1) {
		x = x1; x1 = x0; x0 = x; y = y1; y1 = y0; y0 = y;
	}
	dx = x1 - x0;  dy = y1 - y0;  x = x0;  y = y0;
	e = -dx;
	glBegin(GL_POINTS);
	while (x <= x1) {
		if (bswap)
			glVertex2i(y, x);
		else
			glVertex2i(x, y);
		x++;
		e = e + 2 * dy;
		if (e > 0) {
			y++;
			e = e - 2 * dx;
		}
	}
	glEnd();
}

// X-扫描线法实现多边形转换
void ScanTransferX(vector<VERTEX> polygon, COLORREF color) {
	// 获取最小和最大扫描线
	int ymin = 10000, ymax = -10000, nVertex = polygon.size();
	for (int i = 0; i < nVertex; i++) {
		if (polygon[i].y < ymin) ymin = polygon[i].y;
		if (polygon[i].y > ymax) ymax = polygon[i].y;
	}
	polygon.push_back(polygon[0]);	// 插入首点，便于后续扫描线按边求交点

	// 从ymin到ymax扫描转换
	for (int yi = ymin; yi <= ymax; yi++)
	{
		multiset<int> vaXs;	// 扫描线与多边形边的交点集的x坐标
		// 判断扫描线和哪些边相交，如相交，则求交点，并排序
		for (int i = 0, xi; i < nVertex; i++)
		{
			VERTEX p1 = polygon[i], p2 = polygon[i + 1];
			if ((yi >= p1.y && yi < p2.y) || (yi >= p2.y && yi < p1.y))
			{// 将每条边的最大y值缩短一个单位，判断是否和扫描线相交，如相交，则求交点，插入排序到交点集合中
				if (yi == p1.y)
				{
					xi = p1.x;
				}
				else if (yi == p2.y)
				{
					xi = p2.x;
				}
				else
				{
					xi = (int)((double)((yi - p1.y) * p2.x + (p2.y - yi) * p1.x) / (double)(p2.y - p1.y) + 0.5);
				}
				vaXs.insert(xi);
			}
			else if (yi == p1.y && yi == p2.y)
			{// 是水平线，则将两个端点加入点集
				vaXs.insert(p1.x);
				vaXs.insert(p2.x);
			}
		}

		BYTE r = GetRValue(color), g = GetGValue(color), b = GetBValue(color);
		glColor3ub(r, g, b);
		glBegin(GL_POINTS);
		// 区间交点对间进行填充着色
		for (multiset<int>::iterator it = vaXs.begin(); it != vaXs.end(); ++it)
		{
			int nBeginX = (*it), nEndX = *(++it);
			for (int j = nBeginX; j <= nEndX; j++)
			{
				glVertex2i(j, yi);
			}
		}
		glEnd();
	}
}
