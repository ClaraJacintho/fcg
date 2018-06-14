//
// Created by Ynniv Siralos on 12/06/2018.
//

#ifndef FCG_CAMERA_H
#define FCG_CAMERA_H

#include <glm/vec4.hpp>

#define NEAR -0.1f
#define FAR -10.0f

class Camera {
public:
    glm::vec4 camera_position_c;
    glm::vec4 camera_lookat_l;
    glm::vec4 camera_view_vector;
    glm::vec4 camera_up_vector;
private:
};


#endif //FCG_CAMERA_H
