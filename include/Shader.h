//
// Created by ynniv on 12/06/18.
//

#ifndef FCG_SHADER_H
#define FCG_SHADER_H


#include <glad/glad.h>
#include "fcg_util.hpp"

class Shader {
public:
    GLuint shader_id;
    Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr){
        GLuint vtx_shader_id, frg_shader_id, geo_shader_id;
        vtx_shader_id = LoadShader_Vertex(vertex_path);
        frg_shader_id = LoadShader_Fragment(fragment_path);

        shader_id = glCreateProgram();
        glAttachShader(shader_id, vtx_shader_id);
        glAttachShader(shader_id, frg_shader_id);
        glLinkProgram(shader_id);
        glDeleteShader(vtx_shader_id);
        glDeleteShader(frg_shader_id);
    }

    void activate(){
        glUseProgram(shader_id);
    }
};


#endif //FCG_SHADER_H
