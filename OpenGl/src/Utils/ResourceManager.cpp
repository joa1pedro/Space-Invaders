#include "ResourceManager.h"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Instantiate static variables
std::unordered_map<std::string, Texture2D> ResourceManager::Textures;
std::unordered_map<std::string, Shader> ResourceManager::Shaders;
std::unordered_map<int, GameObject> ResourceManager::GameObjects;
namespace fs = std::filesystem;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, const std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

std::vector<GameObject> ResourceManager::LoadGameObjectsFromFile(const char* filename)
{
	std::vector<GameObject> gameObjects;
	std::fstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return gameObjects;
	}

	std::string line;
	GameObject gameObject;

	while (std::getline(file, line)) {
		if (line.empty()) continue;
		std::string tag = line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1);
		std::string value = line.substr(line.find(']') + 1, line.find('[', line.find(']') + 1) - line.find(']') - 1);

		if (tag == "/GameObject") {
			gameObject.UniqueId = static_cast<int>(GameObjects.size());
			gameObject.Sprite = ResourceManager::GetTexture(gameObject.SpritePath);
			gameObjects.push_back(gameObject);
            GameObjects[static_cast<int>(GameObjects.size())] = gameObject;
			gameObject = GameObject(); // Reset gameObject
		}
		else {
			gameObject.SetValues(tag, value);
		}
	}

	file.close();
    return gameObjects;
}

GameObject ResourceManager::GetGameObject(const int index) {
    return GameObjects[index];
}

void ResourceManager::Clear()
{
    // Delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);

    // Delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile){
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        // Close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha){
    // Create texture object
    Texture2D texture;  
        
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    
    // Load the image
    int width, height, nrChannels;

    //stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 4);
    
    // Now generate the texture
    texture.Generate(width, height, data);
    
    // And free the image data
    stbi_image_free(data);
    return texture;
}

void ResourceManager::LoadResources(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            const std::string& filePath = entry.path().string();
            LoadTexture(filePath.c_str(), true, entry.path().filename().string());
        }
    }
}