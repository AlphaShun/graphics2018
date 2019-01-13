#include <stdio.h>
#include <stdio.h>
//#include <GL\glut.h>
#include <math.h>
#include "myLoader.h"

using namespace std;

#define  GLUT_WHEEL_UP		3  
#define  GLUT_WHEEL_DOWN	4
const int winWeight = 600, winHeight = 600;
string filePath = "teddy.obj";
ObjLoader objModel = ObjLoader(filePath);
GLfloat defaultMatrix[16];
GLfloat currentMatrix[16];
GLfloat distance_z;
bool mouseLefeDown = false, mouseRightDown = false;
int bufX = 0, bufY = 0;

void setLightRes() {

	GLfloat mat_specular[] = { 1.0, 0.5, 0.0, 1.0 };  //���淴�����
	GLfloat mat_shininess[] = { 10.0 };               //�߹�ָ��
													  //��������
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	const GLfloat glfLightAmbient1[] = { 0.1, 0.1, 0.1, 1.0 };
	const GLfloat glfLightAmbient2[] = { 0.4, 0.4, 0.4, 1.0 };
	const GLfloat glfLightDiffuse1[] = { 0, 0.8, 0.8, 1.0 };
	const GLfloat glfLightDiffuse2[] = { 0.8, 0.8, 0.8, 1.0 };
	const GLfloat glfLightSpecular1[] = { 0, 0.8, 0.8, 1.0 };
	const GLfloat glfLightSpecular2[] = { 0.8, 0.8, 0.8, 1.0 };
	const GLfloat glPosition1[] = { 0,0,1,0 };
	const GLfloat glPosition2[] = { 0.6,0.6,-0.6,1 };
	const GLfloat light2_diffuse[] = { 1.0f,0.5f,0.0f,1.0f };
	const GLfloat light2_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	const GLfloat light2_direction[] = { 0.0f,0.0f,.0f,.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, glfLightAmbient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glfLightDiffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glfLightSpecular1);
	glLightfv(GL_LIGHT0, GL_POSITION, glPosition1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, glfLightAmbient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, glfLightDiffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, glfLightSpecular2);
	glLightfv(GL_LIGHT1, GL_POSITION, glPosition2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);//��������  
	glEnable(GL_LIGHTING);//���ù���  
	glEnable(GL_LIGHT0);//�򿪹�Դ
	glEnable(GL_LIGHT1);  
	glEnable(GL_LIGHT2);
	glEnable(GL_COLOR_MATERIAL);//������ɫ׷��  
	glFrontFace(GL_CCW);// ָ����ʱ���Ʒ���ʾ���������
}
/*��ʼ��
*/
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 10.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(100.0, 100.0, 100.0, .0, 0.0, 0.0, .0, 1.0, 0.0);
	distance_z = 100.0f * sqrt(3.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, defaultMatrix);
	memcpy(currentMatrix, defaultMatrix, sizeof(defaultMatrix));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
}

/* ��ʾ
*/
void display()
{
	static int z = 0;
	GLfloat earth_moon_ambient[] = { .0f, .0f, .0f, 1.0f };//����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(currentMatrix);
	setLightRes();
	//glutSolidTeapot(8);
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_moon_ambient);
	objModel.Draw();
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, h - winWeight, winWeight, winHeight);
}

/* �ھ�������ϵ�½���ƽ�ơ���ת�任
*/
void addTranslate(GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();																		// ���ص�λ����
	glTranslatef(x, y, z);																	// ƽ��
	glMultMatrixf(currentMatrix); 															// �任������˵�ǰ����
	glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);
	glPopMatrix();
}
void addRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();																		// ���ص�λ����
	glTranslatef(.0, .0, -distance_z);														// z��ƽ�Ƹ�ԭ
	glRotatef(angle, x, y, z);																// ��ת
	glTranslatef(.0, .0, distance_z);														// z��ƽ������	
	glMultMatrixf(currentMatrix);															// �任������˵�ǰ����
	glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);
	glPopMatrix();
}

void mouse(int button, int state, int x, int y)
{
	y = winHeight - y;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLefeDown = true;
			bufX = x; bufY = y;
		}
		else {
			mouseLefeDown = false;
			bufX = bufY = 0;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseRightDown = true;
			bufX = x; bufY = y;
		}
		else {
			mouseRightDown = false;
			bufX = bufY = 0;
		}
	}
	/*�����ڵ�ǰģ�������½���
	*/
	else if (button == GLUT_WHEEL_UP) {
		if (state == GLUT_UP) {
			glScalef(.9f, .9f, .9f);
			glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);
			glutPostRedisplay();
		}
	}
	else if (button == GLUT_WHEEL_DOWN) {
		if (state == GLUT_UP) {
			glScalef(1.1f, 1.1f, 1.1f);
			glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);
			glutPostRedisplay();
		}
	}
}
void motion(int x, int y)
{
	y = winHeight - y;
	if (bufX >= 0 && bufY >= 0 && (bufX != x || bufY != y)) {
		GLfloat deltax = GLfloat(x - bufX);
		GLfloat deltay = GLfloat(y - bufY);
		if (mouseRightDown) {
			addTranslate(deltax * .2f, deltay * .2f, .0f);
			glutPostRedisplay();
		}
		else if (mouseLefeDown) {
			GLfloat dis = sqrt(deltax*deltax + deltay * deltay);
			addRotate(dis*.5f, -deltay / dis, deltax / dis, .0);
			glutPostRedisplay();
		}
	}
	bufX = x;
	bufY = y;
}

/*������ʾ
*/
void printInstrument() {
	printf("������ʾ��\n����ת���壺������꡾�������ק\n���ƶ����壺������꡾�Ҽ�����ק\n���������壺������꡾���֡�");
}

/*������
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);																		//��ʼ��GLUT
	glutInitWindowSize(winWeight, winHeight);													//��ʼ���ڴ�С
	glutInitWindowPosition(300, 0);																//��ʼ����λ��
	glutCreateWindow("myOpenGL_Lab_03");														//�������ڣ�ָ����������
	printInstrument();																			//�ն���ʾ������ʾ
	init();																						//���ó�ʼ������
	glutReshapeFunc(reshape);																	//ָ���ػ溯��
	glutDisplayFunc(display);																	//ָ����ʾ���ݺ���
	glutMouseFunc(mouse);																		//ָ����꺯��
	glutMotionFunc(motion);																		//ָ����궯������
	glutMainLoop();																				//������ѭ���������¼�

	return 0;
}