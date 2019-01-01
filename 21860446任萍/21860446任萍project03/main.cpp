#include<windows.h>
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
GLfloat *currentCoeff;
GLenum currentPlane;
GLint currentGenMode;
float roangles = 45.0;
#define stripeImageWidth 32
GLubyte stripeImage[4 * stripeImageWidth];
//�Զ����ʼ��opengl����

void makeStripeImage(void) { //��������
	for (int j = 0; j < stripeImageWidth; j++) {
		stripeImage[4 * j + 0] = (GLubyte)((j <= 4) ? 255 : 0);
		stripeImage[4 * j + 1] = (GLubyte)((j > 4) ? 255 : 0);
		stripeImage[4 * j + 2] = (GLubyte)0;
		stripeImage[4 * j + 3] = (GLubyte)255;
	}
}
void init(void)
{
	GLfloat xequalzero[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat slanted[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	makeStripeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//����ӳ��
	glTexImage1D(GL_TEXTURE_1D, 0, 4, stripeImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//������
	currentCoeff = xequalzero;
	currentGenMode = GL_OBJECT_LINEAR;
	currentPlane = GL_OBJECT_PLANE;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);//�Զ�������������,�趨���������ƽ��ο���ʽ
	glTexGenfv(GL_S, currentPlane, currentCoeff);//���趨���ƽ��
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_1D);
	//glShadeModel(GL_SMOOTH);
	// ���ù�Դ
	GLfloat sun_light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat sun_light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);//��0�Ź�Դλ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);//0�Ź�Դ����������
	//�����ܶ�η�������������ڻ����еĹ���ǿ�ȣ���ɫ��
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);//ɢ�������
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);//���淴�������

	//OPENGL�ں������Ⱦ��ʹ�ù��գ�Ĭ��������ǹرյ�
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CW);
	// ���ò���
	GLfloat earth_mat_ambient[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	GLfloat earth_mat_diffuse[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	GLfloat earth_mat_specular[] = { 0.0f, 0.5f, 0.5f, 1.0f };
	GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat earth_mat_shininess = 50.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);//���ʻ�����ɫ
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);//����ɢ����ɫ
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);//���ʾ��淴����ɫ
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);//���ʷ������ɫ
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);//���淴��ָ��
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(roangles, 0.0, 0.0, 1.0);
	//glTranslatef(Move_x, Move_y, Move_z);
	glutSolidSphere(1.0, 32, 32);
	glPopMatrix();

	//glEnable/glDisable ���� �����͹رշ�������GL����
	glDisable(GL_DEPTH_TEST);//������Ȳ���
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_1D);//�Ի�Ĳ��ʽ�����ͼ
	glDisable(GL_LIGHTING);//�رչ�Դ
	glDisable(GL_LIGHT0);//�رչ�Դ��
	glDisable(GL_AUTO_NORMAL);//ִ�к�ͼ���ܰѹⷴ�䵽��������
	glDisable(GL_NORMALIZE);//��ת��֮��͹���֮ǰ������������׼���ɵ�λ����
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	//����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//����ͶӰ
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h / (GLfloat)w, 1.5*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	//����ģ�Ͳ���--���������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("��");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}
