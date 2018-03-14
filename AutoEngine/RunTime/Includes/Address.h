#ifndef ADDRESS_H_
#define ADDRESS_H_

#include "model.h"
#include "stb_image.h"




//Load Shader Address

//Load Resource Address 
#define TEXTURE_LOAD( path , width , height , nrChannels ) stbi_load("../Resource/" ## path, width, height, nrChannels, 0);




#endif // !ADDRESS_H_
