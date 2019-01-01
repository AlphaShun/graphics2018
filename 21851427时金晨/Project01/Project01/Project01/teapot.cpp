#include <stdlib.h>  
#include <GL/glut.h>  
#include <Windows.h>
#include <math.h>
#include<iostream>
#pragma comment(lib, "glut32.lib")  
using namespace std;
GLfloat roate = 0.0;// set rote of roate ying yu bu hao  bu zhuang le ������ת����
GLfloat rote = 0.0;//shezhi��ת�Ƕ�
GLfloat anglex = 0.0;//X ����ת
GLfloat angley = 0.0;//Y ����ת
GLfloat anglez = 0.0;//Z ����ת
GLint WinW = 400;
GLint WinH = 400;
GLfloat oldx;//���������ʱ��¼�������  
GLfloat oldy;

//��ʼ��
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //������ɫ  
}

//��ʾ
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); //ʹ����ȼ��

	glPushMatrix();

	glColor3f(1.0, 0.0, 0.0); //���ʺ�ɫ
	glEnable(GL_NORMALIZE);  //��һ��������

	glLoadIdentity();  //���ص�λ����  
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(rote, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);
	glutSolidTeapot(2.0);
	rote += roate;

	glPopMatrix();
	glutSwapBuffers();
}

//
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
//���ù���
void SetupLights()
{
	GLfloat ambientLight[] = { 0.2f,  0.2f,  0.2f,  1.0f };//������
	GLfloat diffuseLight[] = { 0.9f,  0.9f,  0.9f,  1.0f };//������
	GLfloat specularLight[] = { 1.0f,  1.0f,  1.0f,  1.0f };//�����
	GLfloat lightPos[] = { 50.0f, 80.0f, 60.0f, 1.0f };//��Դλ��

	glEnable(GL_LIGHTING);								//���ù���
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		//���û�����Դ
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		//�����������Դ
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);	//���þ����Դ
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);		//���õƹ�λ��
	glEnable(GL_LIGHT0);								//�򿪵�һ���ƹ�

	glEnable(GL_COLOR_MATERIAL);						//���ò��ʵ���ɫ����
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//ָ��������ɫ����
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //ָ�����϶Ծ����ķ�Ӧ
	glMateriali(GL_FRONT, GL_SHININESS, 100);           //ָ������ϵ��
}

//������
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate = 0;
			rote = 0;
			oldx = x;//���������ʱ��¼�������  
			oldy = y;
		}

	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate = 1.0f;
		}
	}

}

//�˶�
void motion(int x, int y)
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	angley += 108 * (GLfloat)deltax / (GLfloat)WinW;//������Ļ����껬���ľ�����������ת�ĽǶ�  
	anglex += 108 * (GLfloat)deltay / (GLfloat)WinH;
	//anglez += 360 * (GLfloat)deltay / (GLfloat)WinH;
	oldx = x;//��¼��ʱ��������꣬�����������  
	oldy = y;//����û����������䣬��������ת���ò��ɿ�  
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(display);

	SetupLights();

	glutMainLoop();
	return 0;
}
