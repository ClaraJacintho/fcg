#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include <tiny_obj_loader.h>
#include <glm.hpp>

#define SPHERE 1
#define AABB 2
#define PLANE 3
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

        bool checkCollision(vector<Object *> vect){
            for(auto const obj : vect){
                if(obj->name != this->name){
                    switch (obj->obj_type) {
                        case SPHERE:
                        case PLANE:
                        case AABB:
                            return checkCollisionAABB(obj);
                        default:
                            break;
                    }
                }
            }
        }

        bool checkCollisionAABB(Object* obj){
            bool col_x = false;
            bool col_y = false;
            bool col_z = false;

            if(this->bbox_max.x * this->scale.x + this->pos.x >= obj->bbox_min.x * obj->scale.x + obj->pos.x
               && obj->bbox_max.x * obj->scale.x + obj->pos.x >= this->bbox_min.x * this->scale.x + this->pos.x) {
                col_x = true;
            }

            if(this->bbox_max.y * this->scale.y + this->pos.y >= obj->bbox_min.y * obj->scale.y + obj->pos.y
               && obj->bbox_max.y * obj->scale.y + obj->pos.y >= this->bbox_min.y * this->scale.y + this->pos.y) {
                col_y = true;
            }

            if(this->bbox_max.z * this->scale.z + this->pos.z >= obj->bbox_min.z * obj->scale.z + obj->pos.z
               && obj->bbox_max.z * obj->scale.z + obj->pos.z >= this->bbox_min.z * this->scale.z + this->pos.z) {
                col_z = true;
            }

            if(col_x && col_y && col_z){
//                obj->destroyed = true;
                printf("touched %s\n", obj->name.c_str());
                return true;
            }

            return false;
        }

        void checkCollisionAABB(vector<Object *> vect){
            for(auto const obj : vect){
                if(obj->name != this->name){
                    bool col_x = false;
                    bool col_y = false;
                    bool col_z = false;

                    if(this->bbox_max.x * this->scale.x + this->pos.x >= obj->bbox_min.x * obj->scale.x + obj->pos.x
                       && obj->bbox_max.x * obj->scale.x + obj->pos.x >= this->bbox_min.x * this->scale.x + this->pos.x) {
                        col_x = true;
                    }

                    if(this->bbox_max.y * this->scale.y + this->pos.y >= obj->bbox_min.y * obj->scale.y + obj->pos.y
                       && obj->bbox_max.y * obj->scale.y + obj->pos.y >= this->bbox_min.y * this->scale.y + this->pos.y) {
                        col_y = true;
                    }

                    if(this->bbox_max.z * this->scale.z + this->pos.z >= obj->bbox_min.z * obj->scale.z + obj->pos.z
                       && obj->bbox_max.z * obj->scale.z + obj->pos.z >= this->bbox_min.z * this->scale.z + this->pos.z) {
                        col_z = true;
                    }

                    if(col_x && col_y && col_z){
                        obj->destroyed = true;
                        printf("Destroyed %s\n", obj->name.c_str());
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
