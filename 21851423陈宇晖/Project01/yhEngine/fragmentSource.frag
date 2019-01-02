#version 330 core									
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

/* ���� */
struct Material {
    vec3 ambient;		//���廷����
    sampler2D diffuse;	//��������ͼ
    sampler2D specular;	//������ͼ
    float shininess;	//����߹ⱶ��
};

/* ƽ�й� */
struct LightDirectional{
	vec3 pos;		//�����ڵ�λ��
	vec3 color;		//�����ɫ
	vec3 dirToLight;//ƽ�й⡤����
};

/* ���Դ */
struct LightPoint {
	vec3 pos;		//�����ڵ�λ��
	vec3 color;		//�����ɫ
	vec3 dirToLight;//�ⷽ��

	float constant;	//������
	float linear;	//һ����
	float quadratic;//������
};

/* �۹�� */
struct LightSpot{
	vec3 pos;		//�����ڵ�λ��
	vec3 color;		//�����ɫ
	vec3 dirToLight;//�ⷽ��

	float constant;	//������
	float linear;	//һ����
	float quadratic;//������

	float cosPhyInner;
	float cosPhyOutter;
};

uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;
uniform LightSpot lightS;

uniform vec3 objColor;
uniform vec3 ambientColor;

uniform vec3 lightDirUniform;
uniform vec3 cameraPos;

out vec4 FragColor;

/* ����ƽ�й�����λ�ü����ķ��� */
vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){

	// diffuse = max( dot(uL, uN), 0) 
	/* ���ߺͷ��ߵļн�Խ�� ������ԽС ��֮Խ�� */
	float diffuseIntensity = max( dot( light.dirToLight, uNormal), 0);
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

	// specular = max( dot(uR, uCam��, 0)
	/* ������ߺ͵㵽����ļн�Խ�� �����ԽС ��֮Խ�� */
	vec3 uR = normalize( reflect(-light.dirToLight, uNormal));
	float specularIntensity = pow( max( dot( uR, dirToCamera), 0), material.shininess);
	vec3 specularColor = specularIntensity * light.color * texture(material.specular, TexCoord).rgb;

	vec3 result = diffuseColor + specularColor;
	return result;
}

/* ���ݵ��Դ�����ķ��� */
vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){

	// attenuation
	/* �����˥�� */
	float dist = length(light.pos -  FragPos);
	float attenuation =  1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// diffuse
	/* ��Դ�ķ������ǹ̶��� ����ͨ���������ó� */
	vec3 dirToLight = normalize(light.pos - FragPos);
	float diffuseIntensity = max( dot( dirToLight, uNormal), 0);
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 uR = normalize( reflect(-dirToLight, uNormal));
	float specularIntensity = pow( max( dot( uR, dirToCamera), 0), material.shininess);
	vec3 specularColor = specularIntensity * light.color * texture(material.specular, TexCoord).rgb;

	vec3 result = attenuation * (diffuseColor + specularColor);
	return result;
}


vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){

	/* �����Ե�ữ */
	float spotRatio;
	vec3 dirToLight = normalize(light.pos - FragPos);
	float cosTheta = dot( -dirToLight, -light.dirToLight);
	if(cosTheta > light.cosPhyInner){
		//inside
		spotRatio = 1.0;
	}
	else if (cosTheta > light.cosPhyOutter){
		//middle
		spotRatio=(cosTheta - light.cosPhyOutter) / (light.cosPhyInner - light.cosPhyOutter);
	}
	else{
		//outside
		spotRatio=0;
	}
	
	// attenuation
	/* �����˥�� */
	float dist = length(light.pos -  FragPos);
	float attenuation =  1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// diffuse
	float diffuseIntensity = max( dot( dirToLight, uNormal), 0);
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 uR = normalize( reflect(-dirToLight, uNormal));
	float specularIntensity = pow( max( dot( uR, dirToCamera), 0), material.shininess);
	vec3 specularColor = specularIntensity * light.color * texture(material.specular, TexCoord).rgb;

	vec3 result = attenuation * (diffuseColor + specularColor) * spotRatio;
	return result;
}

void main(){

	vec3 finalResult = vec3(0, 0, 0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);
	finalResult += CalcLightDirectional(lightD, uNormal, dirToCamera);
	finalResult += CalcLightPoint(lightP0, uNormal, dirToCamera);
	finalResult += CalcLightPoint(lightP1, uNormal, dirToCamera);
	finalResult += CalcLightPoint(lightP2, uNormal, dirToCamera);
	finalResult += CalcLightPoint(lightP3, uNormal, dirToCamera);
	finalResult += CalcLightSpot(lightS, uNormal, dirToCamera);

	FragColor = vec4(finalResult, 1.0);
//	else{
//		//outside
//		spotRatio=0;
//		FragColor = vec4((ambient + (diffuse + specular)*spotRatio ) * objColor, 1.0f);
//	}
}