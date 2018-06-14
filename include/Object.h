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
        int obj_type; /// 1=sphere, 2=AABB, 3=plane

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
            this->obj_type  = 2;
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

        void checkCollisionAABB(vector<Object *> vect){
            for(auto const a : vect){
                if(a->name != this->name){
                    bool col_x = false;
                    bool col_y = false;
                    bool col_z = false;

                    if(this->bbox_max.x * this->scale.x + this->pos.x >= a->bbox_min.x * a->scale.x + a->pos.x
                       && a->bbox_max.x * a->scale.x + a->pos.x >= this->bbox_min.x * this->scale.x + this->pos.x) {
                        col_x = true;
                    }

                    if(this->bbox_max.y * this->scale.y + this->pos.y >= a->bbox_min.y * a->scale.y + a->pos.y
                       && a->bbox_max.y * a->scale.y + a->pos.y >= this->bbox_min.y * this->scale.y + this->pos.y) {
                        col_y = true;
                    }

                    if(this->bbox_max.z * this->scale.z + this->pos.z >= a->bbox_min.z * a->scale.z + a->pos.z
                       && a->bbox_max.z * a->scale.z + a->pos.z >= this->bbox_min.z * this->scale.z + this->pos.z) {
                        col_z = true;
                    }

                    if(col_x && col_y && col_z){
                        a->destroyed = true;
                        printf("Destroyed %s\n", a->name.c_str());
                    }
                }
            }
        }

        //colisão com esfera?
        /*void checkCollisionSphere(vector<Object*> vect) {
            for(auto const a : vect){
                if(a->name != this->name && a->obj_type == 1){
                    bool col_x = false;
                    bool col_y = false;
                    bool col_z = false;

                    glm::vec3 closest_point(
                            max(this->bbox_min.x, min(a->pos.x, this->bbox_min.x)),
                            max(this->bbox_min.y, min(a->pos.y, this->bbox_min.y)),
                            max(this->bbox_min.z, min(a->pos.z, this->bbox_min.z))
                            );

                    float distance =
                            (closest_point.x - a->pos.x) * (closest_point.x - a->pos.x) +
                            (closest_point.y - a->pos.y) * (closest_point.y - a->pos.y) +
                            (closest_point.z - a->pos.z) * (closest_point.z - a->pos.z);

                    if (distance < radius?)
                }
            }
        }*/

//        void setPos(float x, float y, float z);
//        void setScale(float x, float y, float z);

        ///TO DO: fcg_util dfunctions that should probably be here

        /// void DrawVirtualObject(const char* object_name);
        /// void ComputeNormals(ObjModel* model);
        ///void BuildTrianglesAndAddToVirtualScene(ObjModel* model);


    protected:

};


#endif // _OBJECT_H
