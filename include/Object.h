#ifndef OBJECT_H
#define OBJECT_H
#include <string>

using namespace std;

class Object
{
    string name;
    float x,y,z;
    public:
        Object();
        virtual ~Object();

    protected:

    private:
};

#endif // OBJECT_H
