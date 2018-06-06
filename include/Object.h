#ifndef _OBJECT_H
#define _OBJECT_H
#include <string>
#include <tiny_obj_loader.h>

using namespace std;

struct ObjModel
{
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
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

struct ls_cord {
    float x,y,z;
} typedef cord;

class Object
{
    string name;
    cord scale;
    cord pos;   //Position X, Y and Z
    cord rad;   //Angles of rotation X, Y and Z



    public:
        ObjModel model; //Vertices etc
        Object(string name, const char *file_name, const char *mtl_basepath = NULL);
        virtual ~Object();


    protected:

    private:
};

#endif // _OBJECT_H
