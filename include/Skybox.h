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
#include "Object.h"
#include "Shader.h"
#include <fcg_util.hpp>

class Skybox {
public:
    GLuint texture_map;
    GLuint VBO;
    GLuint VAO;
    Shader shader;
    //Object object;

    Skybox(const char * skybox) : shader("../../src/skybox_vertex.glsl","../../src/skybox_fragment.glsl") {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_cube), &skybox_cube, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

        texture_map = loadCubemapTexture(skybox);

        shader.activate();
            shader.passValue("skybox", 0);
        shader.deactivate();
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

    float skybox_cube [216] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
};


#endif //FCG_SKYBOX_H
