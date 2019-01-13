#version 440 core

out vec4 FragColor;

struct Material
{
 sampler2D diffuse;     //��������ͼ
 sampler2D specular;
 float  shininess;

};

uniform Material material;
struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;



uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
void main()
{



	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	
	//������
	vec3 lightDir = normalize(light.position- FragPos);       //���߷�����Ƭ��ָ���Դ
	vec3 norm = normalize(Normal);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	//������
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//�����
	
	vec3 viewDir = normalize(viewPos - FragPos);        //���߷�����Ƭ��ָ�������
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);   //32�Ƿ���ȣ������߹���С
	vec3 specular = light.specular *vec3(texture(material.specular,TexCoords)) *spec;


	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/ epsilon, 0.0, 1.0);
	
	diffuse *= attenuation;
	specular *= attenuation;
	diffuse *= intensity;
	specular *= intensity;

	vec3 result = ambient+diffuse+specular;
	FragColor = vec4(result,1.0f);
	

}