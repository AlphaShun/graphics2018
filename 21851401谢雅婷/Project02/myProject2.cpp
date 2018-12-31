#define BITMAP_ID 0x4D42
#define PI 3.1415926

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

GLuint sun,earth,moon;

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����

/*����power_of_two
*�ж�һ�������ǲ���2����������
*/
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// ���ļ������ʧ�ܣ�����
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
	free(pixels);
	return texture_ID;
}

void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
	GLfloat ds = 1.0f / (GLfloat)iSlices;
	GLfloat dt = 1.0f / (GLfloat)iStacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
	GLint i, j;

	for (i = 0; i < iStacks; i++)
	{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (j = 0; j <= iSlices; j++)
		{
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

			glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
		}
		glEnd();

		t -= dt;
	}
}

void drawCircle(GLfloat r)
{
	int j = 0;
	glBegin(GL_POINTS);
	for (j = 0; j < 100; j++)
	{
		GLfloat theta = j * 2 * PI / 100;
		glVertex3f(r*cos(theta), 0.0f, r*sin(theta));
	}
	glEnd();
}

void Initial()
{

	GLfloat light0_diffuse[] = { 1.0f,0.5f,0.0f,1.0f };//light0������������

	GLfloat light0_position[] = { 10.0f,20.0f,-150.0f,1.0f };//light0������λ��

	GLfloat light0_direction[] = { 0.0f,0.0f,-1.0f };//light0�ľ۹�Ʒ����

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);//light0��̫������

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

	glEnable(GL_DEPTH_TEST);// ������Ȳ���

	glEnable(GL_LIGHTING);// ���ù�Դ

	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);//����2D����ģʽ

	glFrontFace(GL_CCW);// ָ����ʱ���Ʒ���ʾ���������

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//����

	//gluLookAt(100.0, 100.0, 100.0, .0, .0, .0, .0, 1.0, 0.0);

	//gluPerspective(40.0, 1.0, 150.0, 300.0);

}

void ChangeSize(int w, int h)
{

	if (h == 0)h = 1;

	glViewport(0, 0, w, h);// ���������ߴ�

	glMatrixMode(GL_PROJECTION);//ָ����ǰ����ͶӰ�����ջ

	glLoadIdentity();//����ͶӰ����

	gluPerspective(35.0, (float)w / (float)h, 1.0, 500.0);//ָ��͸��ͶӰ�Ĺ۲�ռ�

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

}

void Display(void)
{

	static float angle1 = 0.0f, angle2 = 0.0f;//angle1������̫����ת�ĽǶȣ�angle2�����Ƶ�����ת�ĽǶ�

	

	//GLfloat sun_emission[] = { 0.6f,0.0f,0.0f,1.0f };//̫�����������

	GLfloat earth_emission[] = { 0.4f,0.4f,0.8f,50.0f };//�������������

	GLfloat moon_emission[] = { 0.98f,0.625f,0.12f,1.0f };//�������������

	GLfloat sun_ambient[] = { 1.0f, 1.0f, 1.0f, 0.0f };//̫����������

	GLfloat earth_moon_ambient[] = { .0f, .0f, .0f, 1.0f };//����������������

	GLfloat light1_diffuse[] = { 0.5f,0.8f,0.0f,1.0f };//��Դlight1������������

	GLfloat light1_position[] = { 50.0f,100.0f,100.0f,1.0f };//��Դlight1������λ��

	GLfloat light1_direction[] = { 0.0f,0.0f,-1.0f };//��Դlight1�ľ۹�Ʒ����

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����Ȼ�����
	//glPushMatrix();

	//glRotatef(10, 0.0, 1.0, 0.0);

	//glPopMatrix();

	glMatrixMode(GL_MODELVIEW);//ָ����ǰ����ģ����ͼ�����ջ

	glLoadIdentity();//����ģ����ͼ����

	glTranslatef(0.0f, 0.0f, -150.0f);//��ͼ����z�Ḻ���ƶ�150.0f

	glRotatef(angle1, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, sun);

	glEnable(GL_LIGHT1);


	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);

	//glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);

	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);

	gltDrawSphere(12.0f, 30, 30);//����̫��

	glDisable(GL_LIGHT1);



	glPushMatrix();//���浱ǰ�ľ�����ͼģ��

	glBindTexture(GL_TEXTURE_2D, earth);

	glRotatef(angle1, 0.0f, 10.0f, 1.0f);//��תһ���Ƕ�

	glTranslatef(40.0f, 0.0f, 0.0f);//��x�������ƶ�40.0f

	glMaterialfv(GL_FRONT, GL_EMISSION, earth_emission);

	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_moon_ambient);

	gltDrawSphere(6.0f, 20, 20);//���Ƶ���

								  
	glBindTexture(GL_TEXTURE_2D, moon);

	glRotatef(angle2, 0.0f, 1.0f, 0.0f);

	glTranslatef(15.0f, 0.0f, 0.0f);//��x�᷽���ƶ�15.0f

	glMaterialfv(GL_FRONT, GL_EMISSION, moon_emission);

	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_moon_ambient);

	gltDrawSphere(3.0f, 20, 20);//��������



	glPopMatrix();//�ָ�������ͼģ��

	angle1 += 4.0f;//������ת��������������Ч��

	if (angle1 == 360.0f)

		angle1 = 0.0f;

	angle2 += 40.0f;

	if (angle2 == 360.0f)

		angle2 = 0.0f;

	glutSwapBuffers();

}

void TimerFunc(int x)
{

	glutPostRedisplay();

	glutTimerFunc(100, TimerFunc, 1);

}

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(640, 480);

	glutInitWindowPosition(400, 200);

	glutCreateWindow("myOpenGLProject2");

	glutReshapeFunc(ChangeSize);

	glutDisplayFunc(Display);

	glutTimerFunc(500, TimerFunc, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);    // ��������
	sun = load_texture("sun.bmp");  //��������
	earth = load_texture("earth.bmp");
	moon = load_texture("moon.bmp");

	Initial();

	glutMainLoop();

	return 0;

}