#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include <tiny_obj_loader.h>
#include <glm.hpp>

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
        int proj_type; /// 1=sphere, 2=AABB, 3=plane

        Object(string name, const char *file_name, const char *mtl_basepath = nullptr)
                : model(file_name, mtl_basepath, true) {
            ///TO DO: Actually call this functions here instead of in main
            ///ComputeNormals(&(this->model));
            ///BuildTrianglesAndAddToVirtualScene(&(this->model));

            this->name    = name;
            this->scale   = glm::vec3(1.0f,1.0f,1.0f);
            this->pos     = glm::vec3(0.0f,0.0f,0.0f);
            this->rad.x   = 0.0f;
            this->rad.y   = 0.0f;
            this->rad.z   = 0.0f;
            this->destroyed = false;
            this->proj_type = 2;

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
        }

//        void setPos(float x, float y, float z);
//        void setScale(float x, float y, float z);

        ///TO DO: fcg_util dfunctions that should probably be here

        /// void DrawVirtualObject(const char* object_name);
        /// void ComputeNormals(ObjModel* model);
        ///void BuildTrianglesAndAddToVirtualScene(ObjModel* model);


    protected:

};


#endif // _OBJECT_H
