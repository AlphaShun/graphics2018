#ifndef GLCAMERA_H
#define GLCAMERA_H
#include "eigen.h"
#include "GL/glu.h"
#include "iostream"

using namespace Eigen;
class GLCamera {
public:
	GLCamera();
	GLCamera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);
	void setModelViewMatrix(); //���ص�ǰMV����

	//����������ӽ�
	void setShape(float viewAngle, float aspect, float Near, float Far);
	void slide(float du, float dv, float dn);

	//����������ת  �ֱ�Ϊ N����ͷע�ӷ��򣩣�V����ͷ�Ϸ���������U(��ͷ�Ҳ�������
	void roll(float angle);
	void yaw(float angele);
	void pitch(float angle);
	float getDist();
private:
	Vector3d m_pos;
	Vector3d m_target;
	Vector3d m_up;
	Vector3d u, v, n;
};
#endif // !GLCAMERA_H
