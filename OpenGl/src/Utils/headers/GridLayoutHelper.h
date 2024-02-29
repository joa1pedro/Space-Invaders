#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//Originally set to 800x600
const unsigned int SCREEN_WIDTH2 = 800;
const unsigned int SCREEN_HEIGHT2 = 600;

class GridLayoutHelper
{
public:
	static void CreateGridLayoutForEnemies();
private:
	static glm::vec2 scalePosition(const glm::vec2& position);
};

