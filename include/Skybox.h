//
// Created by Ynniv Siralos on 12/06/2018.
//

#ifndef FCG_SKYBOX_H
#define FCG_SKYBOX_H


#include <string>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

class Skybox {
public:
    GLuint texture_map;
    Skybox(std::string skybox) {
        texture_map = loadCubemapTexture(skybox);
    }

private:
    GLuint loadCubemapTexture(std::string skybox) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        std::vector<std::string> images;
        images.push_back(skybox.append("/rt.tga"));
        images.push_back(skybox.replace(skybox.find("rt"),2,"lf"));
        images.push_back(skybox.replace(skybox.find("lf"),2,"up"));
        images.push_back(skybox.replace(skybox.find("up"),2,"dn"));
        images.push_back(skybox.replace(skybox.find("dn"),2,"ft"));
        images.push_back(skybox.replace(skybox.find("ft"),2,"bk"));

        int width, height, nrChannels;
        for (unsigned int i = 0; i < images.size(); i++) {
            unsigned char *data = stbi_load(images[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            } else {
                std::cout << "Cubemap texture failed to load at path: " << images[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
};


#endif //FCG_SKYBOX_H
