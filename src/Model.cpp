#include "Model.h"
#include "stb_image.h"

Model::Model(std::string path)
{
	loadModel(path);
}

Model::~Model()
{
}

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp error" << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	std::cout << "Success! " << directory << std::endl;
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(curMesh, scene));
	}
	
	//std::cout << node->mName.data << std::endl;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

// Using for load texture images 
unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::cout << path << std::endl;
	std::string filename = std::string(path);
	filename = directory + '\\' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;
		GLint internalRGB;

		if (nrComponents == 1)
		{
			format = GL_RED;
			internalRGB = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
			internalRGB = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
			internalRGB = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> tempVertices;
	std::vector<unsigned int> tempIndices;
	std::vector<Texture> tempTextures;

	glm::vec3 tempVec;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex tempVertex;
		tempVertex.Position.x = mesh->mVertices[i].x;
		tempVertex.Position.y = mesh->mVertices[i].y;
		tempVertex.Position.z = mesh->mVertices[i].z;
		
		tempVertex.Normal.x = mesh->mNormals[i].x;
		tempVertex.Normal.y = mesh->mNormals[i].y;
		tempVertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			tempVertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			tempVertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else 
		{
			tempVertex.TexCoords = glm::vec2(0, 0);
		}

		tempVertices.push_back(tempVertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			tempIndices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	tempTextures.insert(tempTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	tempTextures.insert(tempTextures.end(), specularMaps.begin(), specularMaps.end());
	//// 3. normal maps
	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//tempTextures.insert(tempTextures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//tempTextures.insert(tempTextures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(tempVertices, tempIndices, tempTextures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	//std::cout << "hello material" << std::endl;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), this->directory);
		texture.type = typeName;
		texture.path = str.C_Str();
			
		textures.push_back(texture);
		//textures_loaded.push_back(texture);
		
	}
	return textures;
}

