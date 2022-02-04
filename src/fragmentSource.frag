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


//	float dist = length(lightPos - FragPos);
//	float attenuation = 1.0 / (lightP.constant + lightP.linear * dist + lightP.quadratic * (dist * dist));
//	
//	vec3 lightDir = normalize(lightPos - FragPos);
//	vec3 reflectVec = reflect(-lightDir, Normal);
////	vec3 reflectVec = 2 * Normal - (lightDir); // test
//
//	vec3 cameraVec = normalize(cameraPos - FragPos);
//
//	// specular
//	float specularAmount = pow(max(dot(reflectVec, cameraVec), 0), material.shininess );
//	vec3 specular = texture(material.specular, TexCoord).rgb * specularAmount  * lightColor;
//
//	// diffuse
//	vec3 diffuse = texture(material.diffuse, TexCoord).rgb * max(dot(lightDir, Normal), 0) * lightColor;
//	//vec3 diffuse = texture(material.diffuse, TexCoord).rgb;
//
//	// ambient
//	vec3 ambient = texture(material.diffuse, TexCoord).rgb * material.ambient * ambientColor;
//
//	//emission 
//	vec3 emission = texture(material.emission, vec2(TexCoord.x, TexCoord.y + offset * 0.1f)).rgb;
//
//	float cosTheta = dot( normalize(FragPos - lightPos), -1.0f * lightDirUniform );
//	
//	float spotRatio;
//	if(cosTheta > lightS.cosPhyInner){
//		// inside
//		spotRatio = 1.0f;
//	}else if(cosTheta > lightS.cosPhyOuter){
//		// middle
//		spotRatio = (cosTheta - lightS.cosPhyOuter) / (lightS.cosPhyInner - lightS.cosPhyOuter);
//	}else{
//		// outside
//		spotRatio = 0.0f;
//	}

	//FragColor = vec4((ambient + (diffuse + specular) * spotRatio) * objColor, 1.0f);
	
//	if(cosTheta > lightS.cosPhy){
//		// inside the spotlight
//		FragColor = vec4((ambient + (diffuse + specular) ) * objColor, 1.0f);
//	}else{
//		// outside 
//		FragColor = vec4(ambient  * objColor, 1.0f);
//	}

//	FragColor = vec4((ambient + (diffuse + specular) * attenuation ) * objColor, 1.0f);
//	FragColor = vec4((ambient + diffuse + specular + emission) * objColor, 1.0f);

	