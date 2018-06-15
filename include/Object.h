#ifndef _OBJECT_H
#define _OBJECT_H

#include <iostream>
#include <string>
#include <tiny_obj_loader.h>
#include <glm/vec3.hpp>
#include <math.h>
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

class Object
{
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

        Object(int obj_type, string name, const char *file_name, const char *mtl_basepath = nullptr)
                : model(file_name, mtl_basepath, true) {
            ///TO DO: Actually call this functions here instead of in main
            ///ComputeNormals(&(this->model));
            ///BuildTrianglesAndAddToVirtualScene(&(this->model));

            this->name      = name;
            this->scale     = glm::vec3(1.0f,1.0f,1.0f);
            this->pos       = glm::vec3(0.0f,0.0f,0.0f);
            this->rad       = glm::vec3(0.0f,0.0f,0.0f);
            this->destroyed = false;
            this->obj_type  = obj_type;
        }

        Object(string name, const char *file_name, const char *mtl_basepath = nullptr)
                : model(file_name, mtl_basepath, true) {
            ///TO DO: Actually call this functions here instead of in main
            ///ComputeNormals(&(this->model));
            ///BuildTrianglesAndAddToVirtualScene(&(this->model));

            this->name      = name;
            this->scale     = glm::vec3(1.0f,1.0f,1.0f);
            this->pos       = glm::vec3(0.0f,0.0f,0.0f);
            this->rad       = glm::vec3(0.0f,0.0f,0.0f);
            this->destroyed = false;
            this->obj_type  = AABB;
        }

        void setPos(glm::vec3 pos_v){
            this->pos = pos_v;
        }

        void setScale(glm::vec3 scale_v){
            this->scale = scale_v;
        }

        void setRotation(glm::vec3 angles_v){
            this->rad.x = (float)(angles_v.x * M_PI / 180);
            this->rad.y = (float)(angles_v.y * M_PI / 180);
            this->rad.z = (float)(angles_v.z * M_PI / 180);

            this->bbox_max = rotateBBox(this->bbox_max, this->rad);
            this->bbox_min = rotateBBox(this->bbox_min, this->rad);

        }

        glm::vec3 rotateBBox(glm::vec3 bbox, glm::vec3 rotation){
            glm::vec3 bbox_final;

            //rotaciona ponto em Z
            bbox_final.x = bbox.x * cos(rotation.z) - bbox.y * sin(rotation.z);
            bbox_final.y = bbox.x * sin(rotation.z) + bbox.y * cos(rotation.z);

            //rotaciona ponto em Y
            bbox_final.x = bbox.x * cos(rotation.y) + bbox.z * sin(rotation.y);
            bbox_final.z = -bbox.x * sin(rotation.y) + bbox.z * cos(rotation.y);

            //rotaciona ponto em X
            bbox_final.y = bbox.y * cos(rotation.x) - bbox.z * sin(rotation.x);
            bbox_final.z = bbox.y * sin(rotation.x) + bbox.z * cos(rotation.x);

            return bbox_final;
        }
};


#endif // _OBJECT_H
