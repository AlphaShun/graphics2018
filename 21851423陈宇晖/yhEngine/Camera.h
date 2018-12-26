#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	~Camera();

	glm::vec3 Position;	//���λ��
	glm::vec3 Forward;	//�������
	glm::vec3 Right;	//�������
	glm::vec3 Up;		//�������
	glm::vec3 WorldUp;	//��������

	float Pitch;		//����
	float Yaw;			//ˮƽת��
	float speedZ = 0;	//Z��ƽ���ٶ�
	float speedY = 0;	//Y��ƽ���ٶ�
	float speedX = 0;	//X��ƽ���ٶ�

	/*��õ�ǰλ�õ�view����*/
	glm::mat4 GetViewMatrix();

	/*����deltaXY�ƶ����*/
	void ProcessMouseMovement(float deltaX, float deltaY);

	/*�������λ��*/
	void UpdateCameraPos();

	/*Z���ƶ�*/
	void MoveZ(float z);

	/*X���ƶ�*/
	void MoveX(float x);
	
	/*X���ƶ�*/
	void MoveY(float y);

private:
	/*���������������*/
	void UpdateCameraVectors();
};