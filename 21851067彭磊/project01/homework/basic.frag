#version 330 core

out vec4 FragColor;           //Ƭ����ɫ
in vec3 Normal;				  //Ƭ�η�����
in vec3 FragPos;             //Ƭ��λ��
uniform vec3 lightColor;    //������ɫ
uniform vec3 objectColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
uniform vec3 lightPos;    // ��Դλ��
uniform vec3 viewPos;     //�۲���λ��

vec3 norm = normalize(Normal);              //��������׼��
vec3 lightDir = normalize(lightPos - FragPos);     //��λ���߷���������Ƭ��ָ���Դ
vec3 viewDir = normalize(viewPos - FragPos);       //�۲������ߵ�λ����,Ƭ��ָ��۲���
vec3 reflectDir = reflect(-lightDir, norm);        //����ⷽ������

void main()
{
    float specularStrength = 0.5;                    //����ǿ��
	float spec = pow(max(dot(viewDir, reflectDir),0.0),256); //�������
	vec3 specular = specularStrength * spec *lightColor;   //���淴���
	float diff = max(dot(norm, lightDir),0.0);       //����������
	vec3 diffuse = lightColor * diff;                //�������ǿ
	float ambientStrength = 0.1;                     //����������
	vec3 ambient = ambientStrength * lightColor;     //������ǿ
    FragColor = vec4((ambient+diffuse+specular) * objectColor, 1.0);   //������ɫ
}