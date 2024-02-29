#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include "GameObject.h"

// A static singleton ResourceManager class that hosts several functions to load Textures and Shaders.
// Each loaded texture and/or shader is also stored for future reference by string handles.
// All functions and resources are static and no public constructor is defined.
class ResourceManager
{
public:
    static std::unordered_map<std::string, Shader>    Shaders;
    static std::unordered_map<std::string, Texture2D> Textures;
    static std::unordered_map<int, GameObject> GameObjects;
    
    // Loads all resource images inside the specified folder
    static void LoadResources(const std::string& folderPath);
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // Retrieves a stored shader
    static Shader GetShader(std::string name);
    // Loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha, const std::string name);
    // Retrieves a stored texture
    static Texture2D GetTexture(std::string name);

    //Retrieves a stored GameObject
    static GameObject GetGameObject(int index);

    //Loads a gameObject from file
    static std::vector<GameObject> LoadGameObjectsFromFile(const char* file);

    // Properly de-allocates all loaded resources
    static void Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { };
    // Loads and generates a shader from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};