#ifndef _OBJECT_H
#define _OBJECT_H

#include <iostream>
#include <string>
#include <tiny_obj_loader.h>
#include <glm/vec3.hpp>
#include <math.h>
#include <glad/glad.h>
#include "matrices.h"

#define EARTH 1
#define JET 2
#define PLANE  3
#define MARS 4
#define COW 5

#define SPHERE 1
#define AABB 2
#define PLANE 3
#define M_PI   3.14159265358979323846
using namespace std;

// Estrutura que representa um modelo geom�trico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel {
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor l� o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true)
    {
        printf("Carregando modelo \"%s\"... ", filename);

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

        if (!err.empty())
            fprintf(stderr, "\n%s\n", err.c_str());

        if (!ret)
            throw std::runtime_error("Erro ao carregar modelo.");

        printf("OK.\n");
    }
};

//std::map<std::string, SceneObject> g_VirtualScene;

class Object {
public:
    string name;
    glm::vec3 scale;
    glm::vec3 pos;   //Position X, Y and Z
    glm::vec3 rad;   //Angles of rotation X, Y and Z
    ObjModel model; //Vertices etc
    bool destroyed;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    int obj_type;
    int hitbox_type;

    Object(int hitbox_type, int obj_type, string name, const char *file_name, const char *mtl_basepath = nullptr)
            : model(file_name, mtl_basepath, true) {

        this->name      = name;
        this->scale     = glm::vec3(1.0f,1.0f,1.0f);
        this->pos       = glm::vec3(0.0f,0.0f,0.0f);
        this->rad       = glm::vec3(0.0f,0.0f,0.0f);
        this->destroyed = false;
        this->obj_type  = obj_type;
        this->hitbox_type = hitbox_type;
    }

    Object(string name, const char *file_name, const char *mtl_basepath = nullptr)
            : model(file_name, mtl_basepath, true) {

        this->name      = name;
        this->scale     = glm::vec3(1.0f,1.0f,1.0f);
        this->pos       = glm::vec3(0.0f,0.0f,0.0f);
        this->rad       = glm::vec3(0.0f,0.0f,0.0f);
        this->destroyed = false;
        this->hitbox_type = AABB;
        this->obj_type  = JET;
    }

    void setPos(glm::vec3 pos_v){
        glm::vec3 pos_delta = pos_v - this->pos;
        this->pos += pos_delta;

        this->bbox_max = translateBBox(this->bbox_max, pos_delta);
        this->bbox_min = translateBBox(this->bbox_min, pos_delta);
    }

    void setScale(glm::vec3 scale_v){
        this->scale = scale_v;

        this->bbox_max = scaleBBox(this->bbox_max, this->scale);
        this->bbox_min = scaleBBox(this->bbox_min, this->scale);
    }

    void setRotation(glm::vec3 angles_v){
        this->rad.x = (float)(angles_v.x * M_PI / 180);
        this->rad.y = (float)(angles_v.y * M_PI / 180);
        this->rad.z = (float)(angles_v.z * M_PI / 180);

        this->bbox_max = rotateBBox(this->bbox_max, this->rad);
        this->bbox_min = rotateBBox(this->bbox_min, this->rad);
    }

    glm::vec3 rotateBBox(glm::vec3 bbox, glm::vec3 rotation){
        glm::vec4 bbox4(bbox.x,bbox.y,bbox.z,1.0f);

        glm::mat4 rotx = Matrix_Rotate_X(rotation.x);
        glm::mat4 roty = Matrix_Rotate_Y(rotation.y);
        glm::mat4 rotz = Matrix_Rotate_Z(rotation.z);

        bbox4 = rotz * roty * rotx * bbox4;

        glm::vec3 bboxfinal(bbox4);

        return bboxfinal;
    }

    glm::vec3 scaleBBox(glm::vec3 bbox, glm::vec3 scaling){
        glm::vec3 bbox_final;

        //rotaciona ponto em Z
        bbox_final = bbox * scaling;

        return bbox_final;
    }

    glm::vec3 translateBBox(glm::vec3 bbox, glm::vec3 position){
        glm::vec3 bbox_final;

        //rotaciona ponto em Z
        bbox_final = bbox + position;

        return bbox_final;
    }

    void printBBox(){
        std::cout << this->name
                  << " bbox max: "
                  << this->bbox_max.x << " "
                  << this->bbox_max.y << " "
                  << this->bbox_max.z << " "
                  << std::endl;

        std::cout << this->name
                  << " bbox min: "
                  << this->bbox_min.x << " "
                  << this->bbox_min.y << " "
                  << this->bbox_min.z << " "
                  << std::endl;
    }

};


#endif // _OBJECT_H
