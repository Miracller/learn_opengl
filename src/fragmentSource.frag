#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D ourTexture;
//uniform sampler2D ourFace;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
};

struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};

struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;

	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;

	float constant;
	float linear;
	float quadratic;

	float cosPhyInner;
	float cosPhyOuter;
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
//uniform vec3 lightPos;
//uniform vec3 lightDirUniform;
//uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform float offset;

out vec4 FragColor;

vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){
	
	// diffuse  max(dot(L, N), 0)
	float diffIntensity = max(dot(light.dirToLight, uNormal), 0);
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;
	
	// specular  pow(max(dot(Reflect, Cameraview), 0), shininess)
	vec3 R = normalize(reflect(-light.dirToLight, uNormal));
	float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess);
	vec3 specColor = specIntensity * light.color * texture(material.specular, TexCoord).rgb;

	vec3 result = diffColor + specColor;

	return result;

}

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
	// attenuation
	float dist = length(light.pos - FragPos);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist*dist));

	// diffuse
	float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal), 0) * attenuation;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 R = normalize(reflect(-normalize(light.pos - FragPos), uNormal));
	float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation;
	vec3 specColor = specIntensity * light.color * texture(material.specular, TexCoord).rgb;

	
	vec3 result = diffColor + specColor;

	return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
	// attenuation
	float dist = length(light.pos - FragPos);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist*dist));
	float spotRation;
	float cosTheta = dot(normalize(FragPos - light.pos), -light.dirToLight);

	if(cosTheta > light.cosPhyInner){
		spotRation = 1.0f;
	}else if(cosTheta > light.cosPhyOuter){
		spotRation = (cosTheta - light.cosPhyOuter) / (light.cosPhyInner - light.cosPhyOuter);
	}else{
		spotRation = 0;
	}

	// diffuse
	float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal), 0) * attenuation * spotRation ;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 R = normalize(reflect(-normalize(light.pos - FragPos), uNormal));
	float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation * spotRation;
	vec3 specColor = specIntensity * light.color * texture(material.specular, TexCoord).rgb;

	
	vec3 result = diffColor + specColor;

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

	//ambient 
	finalResult += texture(material.diffuse, TexCoord).rgb * material.ambient * ambientColor;

	FragColor = vec4(finalResult, 1.0f);
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);  //test, white box
}



	
