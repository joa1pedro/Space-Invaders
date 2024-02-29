#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "texture.h"

Texture2D::Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGBA8), Image_Format(GL_RGBA), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR){
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data){
    this->Height = height;
    this->Width = width;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, this->Width, this->Height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
