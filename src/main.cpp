//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                  TRABALHO FINAL
//
//               Maria Clara Jacintho
//                 Vinicius Chagas
//

#include <iostream>
#include <Shader.h>
#include <Skybox.h>
#include <Tunnel.h>
#include "fcg_util.hpp"
// Headers locais, definidos na pasta "include/"


#include "Object.h"
#include "utils.h"
#include "Player.h"
#include "Shader.h"
#include "Sphere.h"
#include "Obstacle.h"


#define MIN 0
#define MAX 3
#define PLANE_HEIGHT 0.5
#define PLANE_WIDTH  0.7
#define OBSTACLE_SPEED 0.03

glm::vec3 randomize_position(float camera_z);
void initialize_position(vector<Object *> planes);
void update_plane(vector<Object *> planes, float camera_z);

bool free_camera = false;
Player player;
double dt = 0.0f;
double lastFrame = 0.0f;
int g_Walking = 0, g_Strafing = 0;
int main(int argc, char* argv[]) {
    int success = glfwInit();
    if (!success) {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Configuramos o OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); //compatibilidade com macOS

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "INF01047 - PROJETO FINAL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos as funções de callback de input
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização.
    Shader shader("../../src/shader_vertex.glsl","../../src/shader_fragment.glsl");
    Shader tunnel_shd("../../src/tunnel_vertex.glsl","../../src/tunnel_fragment.glsl");

    model_uniform      = shader.getUniformLocation("model");
    view_uniform       = shader.getUniformLocation("view");
    projection_uniform = shader.getUniformLocation("projection");
    object_id_uniform  = shader.getUniformLocation("object_id");
    bbox_min_uniform   = shader.getUniformLocation("bbox_min");
    bbox_max_uniform   = shader.getUniformLocation("bbox_max");

    shader.activate();
        shader.passValue("TextureImage0", 0);
        shader.passValue("TextureImage1", 1);
        shader.passValue("TextureImage2", 2);
    shader.deactivate();

    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");      // TextureImage0
    LoadTextureImage("../../data/tc-earth_nightmap_citylights.gif"); // TextureImage1
    LoadTextureImage("../../data/metal.png"); //TextureImage2
    Skybox skybox("../../data/mp_jasper");  //não funciona ainda

    vector<Object*> objects;
    vector<Object*> planes;
    vector<Obstacle*> obstacles;

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ComputeNormals(&(player.model));
    BuildTrianglesAndAddToVirtualScene(&player);
    player.update_bbox();
    player.setRotation(glm::vec3(-22.5f,180.0f,0.0f));
    player.setScale(glm::vec3(0.05f,0.05f,0.05f));
    player.setPos(glm::vec3(0.0f,0.0f,0.0f));
    player.fix_bbox();
    objects.push_back(&player);
    player.printBBox();

    Object horse (2,6,"horse","../../data/horse.obj");
    ComputeNormals(&(horse.model));
    BuildTrianglesAndAddToVirtualScene(&horse);
    //sun.setScale(glm::vec3(0.005f,0.005f,0.005f));
   // sun.setPos(glm::vec3(1.0f,1.0f,-2.0f));
   // objects.push_back(&sun);
   // sun.printBBox();

    Sphere earth(1,"sphere","../../data/sphere.obj");
    ComputeNormals(&(earth.model));
    BuildTrianglesAndAddToVirtualScene(&earth);
    earth.setScale(1.0f);
    earth.setPos(glm::vec3(1.0f,1.0f,-4.0f));
    objects.push_back(&earth);
    Obstacle s(&earth);
    s.movement = glm::vec3(OBSTACLE_SPEED,0.0f,0.0f);
    obstacles.push_back(&s);
    earth.printBBox();


    Sphere mars(4,"sphere","../../data/sphere.obj");
    ComputeNormals(&(mars.model));
    BuildTrianglesAndAddToVirtualScene(&mars);
    mars.setScale(1.0f);
    mars.setPos(glm::vec3(1.0f,2.0f,-9.0f));
    objects.push_back(&mars);
    Obstacle s2(&mars);
    s2.movement = glm::vec3(-OBSTACLE_SPEED,0.0f,0.0f);
    obstacles.push_back(&s2);
    mars.printBBox();

    Tunnel tunnel;


    Object cow(2,5, "cow", "../../data/cow.obj");
    ComputeNormals(&(cow.model));
    BuildTrianglesAndAddToVirtualScene(&cow);
    cow.setScale(glm::vec3(1,1,1));
    cow.setPos(glm::vec3(0.0f,-3,-2));
     objects.push_back(&cow);
    cow.printBBox();

    Object plane(3,3, "plane","../../data/plane.obj");
    ComputeNormals(&(plane.model));
    BuildTrianglesAndAddToVirtualScene(&plane);
    plane.setRotation(glm::vec3(90.0f,00.0f,0.0f));
    plane.setScale(glm::vec3( PLANE_WIDTH, PLANE_HEIGHT,1.0f));
    objects.push_back(&plane);
    planes.push_back(&plane);

    Object plane2(3,3, "plane","../../data/plane.obj");
    ComputeNormals(&(plane2.model));
    BuildTrianglesAndAddToVirtualScene(&plane2);
    plane2.setRotation(glm::vec3(90.0f,0.0f,0.0f));
    plane2.setScale(glm::vec3( PLANE_WIDTH, PLANE_HEIGHT,1.0f));
    objects.push_back(&plane2);
    planes.push_back(&plane2);

    Object plane3(3,3, "plane","../../data/plane.obj");
    ComputeNormals(&(plane.model));
    BuildTrianglesAndAddToVirtualScene(&plane3);
    plane3.setRotation(glm::vec3(90.0f,00.0f,0.0f));
    plane3.setScale(glm::vec3( PLANE_WIDTH, PLANE_HEIGHT,1.0f));
    objects.push_back(&plane3);
    planes.push_back(&plane3);

    Object plane4(3,3, "plane","../../data/plane.obj");
    ComputeNormals(&(plane.model));
    BuildTrianglesAndAddToVirtualScene(&plane4);
    plane4.setRotation(glm::vec3(90.0f,0.0f,0.0f));
    plane4.setScale(glm::vec3( PLANE_WIDTH, PLANE_HEIGHT,1.0f));
    objects.push_back(&plane4);
    planes.push_back(&plane4);


    Object plane5(3,3, "plane","../../data/plane.obj");
    ComputeNormals(&(plane.model));
    BuildTrianglesAndAddToVirtualScene(&plane5);
    plane5.setRotation(glm::vec3(90.0f,0.0f,0.0f));
    plane5.setScale(glm::vec3( PLANE_WIDTH, PLANE_HEIGHT,1.0f));
    objects.push_back(&plane5);
    planes.push_back(&plane5);

//    Object plane(3,3, "plane","../../data/plane.obj");
//    ComputeNormals(&(plane.model));
//    BuildTrianglesAndAddToVirtualScene(&plane);
//    plane.setScale(glm::vec3(2.0f,2.0f,2.0f));
//    plane.setPos(glm::vec3(0.0f, -2.0f, 0.0f));
//    objects.push_back(&plane);
//    plane.printBBox();
//
//    Object plane2(3,3, "plane","../../data/plane.obj");
//    ComputeNormals(&(plane2.model));
//    BuildTrianglesAndAddToVirtualScene(&plane2);
//    plane2.setRotation(glm::vec3(0.0f,0.0f,90.0f));
//    plane2.setScale(glm::vec3(2.0f,2.0f,2.0f));
//    plane2.setPos(glm::vec3(2.0f, 0.0f, 0.0f));
//    objects.push_back(&plane2);
//    plane2.printBBox();


    initialize_position(planes);


    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slide 66 do documento "Aula_13_Clipping_and_Culling.pdf".
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 22 à 34 do documento "Aula_13_Clipping_and_Culling.pdf".
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Variáveis auxiliares utilizadas para chamada à função
    // TextRendering_ShowModelViewProjection(), armazenando matrizes 4x4.
    glm::mat4 the_projection;
    glm::mat4 the_model;
    glm::mat4 the_view;

    int number_of_objects = objects.size();
    glm::vec4 camera_position_c;
    glm::vec4 camera_view_vector;
    glm::vec4 camera_up_vector;
    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window)) {

        double curTime = glfwGetTime();
        dt = curTime - lastFrame;
        lastFrame = curTime;

        // Definimos a cor do "fundo" do framebuffer como branco.
        // https://www.tug.org/pracjourn/2007-4/walden/color.pdf
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        shader.activate();

        for(int i=0; i < obstacles.size();i++){
            obstacles[i]->update(player.pos.z + 3.0f, dt);
        }

        player.update(dt, objects);

        if(free_camera){

            glm::vec4 camera_lookat_l    = glm::vec4(0.0f,0.0f,-1.0f,1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
            // Vetor "view", sentido para onde a câmera está virada
            camera_view_vector = glm::vec4(cos(g_CameraPhi)*sin(g_CameraTheta),
                                                     -sin(g_CameraPhi),
                                                     cos(g_CameraPhi)*cos(g_CameraTheta),
                                                     0.0f);
            camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

            camera_position_c = camera_position_c + 0.05f * g_Walking * camera_view_vector;
            camera_position_c = camera_position_c + 0.05f * g_Strafing * crossproduct(camera_view_vector, camera_up_vector);

        }else{

            camera_position_c = glm::vec4(player.pos.x, player.pos.y + 2.0f, player.pos.z + 3.0f, 1.0f);
            // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
            glm::vec4 camera_lookat_l = glm::vec4(player.pos.x, player.pos.y, player.pos.z, 1.0f);
            camera_view_vector = camera_lookat_l - camera_position_c;
            camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        }

        update_plane(planes, camera_position_c.z);

        glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

        glm::mat4 projection;

        float nearplane = -0.1f;
        float farplane = -100.0f;

        if (g_UsePerspectiveProjection) {
            float field_of_view = M_PI / 3.0f;
            projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        } else {
            float t = 1.5f * g_CameraDistance / 2.5f;
            float b = -t;
            float r = t * g_ScreenRatio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
        }

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

        shader.passValue("view", view);
        shader.passValue("projection", projection);

        if(player.destroyed == false) {

            for (int i = 0; i < number_of_objects; i++) {
                if (objects[i]->destroyed == false) {
                    model = Matrix_Translate(objects[i]->pos.x, objects[i]->pos.y, objects[i]->pos.z)
                            * Matrix_Scale(objects[i]->scale.x, objects[i]->scale.y, objects[i]->scale.z)
                            * Matrix_Rotate_X(objects[i]->rad.x)
                            * Matrix_Rotate_Y(objects[i]->rad.y)
                            * Matrix_Rotate_Z(objects[i]->rad.z);
                    shader.passValue("model", model);
                    shader.passValue("object_id", objects[i]->obj_type);
                    DrawVirtualObject(objects[i]->name.c_str());
                }
            }
            //TODO: Movimento dos obstaculos de acordo com o tempo nao frames! Que nem personagem!
            //tunnel.draw(tunnel_shd);

            // sun.setScale(glm::vec3(0.005f,0.005f,0.005f));
            // sun.setPos(glm::vec3(1.0f,1.0f,-2.0f));

            model = Matrix_Identity();
            switch(player.lives){

                case 3:
                    PushMatrix(model);
                        model = model * Matrix_Translate(player.pos.x  -4.0 ,player.pos.y-3.5f,player.pos.z);
                        PushMatrix(model);
                            model = model *  Matrix_Scale(0.001f,0.001,0.001f);
                            shader.passValue("model", model);
                            shader.passValue("object_id", horse.obj_type);
                            DrawVirtualObject(horse.name.c_str());
                        PopMatrix(model);
                    PopMatrix(model);
                case 2:
                    PushMatrix(model);
                        model = model *  Matrix_Translate(player.pos.x  -3.5 ,player.pos.y-3.5f,player.pos.z);
                        PushMatrix(model);
                            model = model *  Matrix_Scale(0.001f,0.001,0.001f);
                            shader.passValue("model", model);
                            shader.passValue("object_id", horse.obj_type);
                            DrawVirtualObject(horse.name.c_str());
                        PopMatrix(model);
                    PopMatrix(model);
                case 1:
                    PushMatrix(model);
                        model = model *  Matrix_Translate(player.pos.x  -3.0 ,player.pos.y-3.5f,player.pos.z);
                        PushMatrix(model);
                            model = model *  Matrix_Scale(0.001f,0.001,0.001f);
                            shader.passValue("model", model);
                            shader.passValue("object_id", horse.obj_type);
                            DrawVirtualObject(horse.name.c_str());
                        PopMatrix(model);
                    PopMatrix(model);
                default:break;
        }

            /*glDepthFunc(GL_LEQUAL);
            skybox.shader.activate();
            skybox.shader.passValue("view", glm::mat4(glm::mat3(view)));
            skybox.shader.passValue("projection", projection);
            glBindVertexArray(skybox.VAO);
            glActiveTexture(GL_TEXTURE0 + 128);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture_map);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glBindVertexArray(0);
            glDepthFunc(GL_LESS);
            skybox.shader.deactivate();*/

            TextRendering_ShowFramesPerSecond(window);

            glm::vec4 pos4(player.pos.x, player.pos.y, player.pos.z, 0.0f);
            glm::vec4 bbmin4(player.bbox_min.x, player.bbox_min.y, player.bbox_min.z, 0.0f);
            glm::vec4 bbmax4(player.bbox_max.x, player.bbox_max.y, player.bbox_max.z, 0.0f);
//
            TextRendering_PrintVector(window, pos4, 0, 0);
//            TextRendering_PrintVector(window, bbmax4, -0.9, 0);
//            TextRendering_PrintVector(window, bbmin4, -0.9, 0.9);

            //TextRendering_PrintString(window, "Elle est pas belle, la vie?", -0.9, 0.9);

        }
        else{
            TextRendering_PrintString(window, "GAME OVER!", 0, 0);
        }

        shader.deactivate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    float delta = 3.141592 / 16; // 22.5 graus, em radianos.

    // Se o usuário apertar a tecla P, utilizamos projeção perspectiva.
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        g_UsePerspectiveProjection = true;
    }

    // Se o usuário apertar a tecla O, utilizamos projeção ortográfica.
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        g_UsePerspectiveProjection = false;
    }

    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }

    // Se o usuário apertar a tecla R, recarregamos os shaders dos arquivos "shader_fragment.glsl" e "shader_vertex.glsl".
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        //TODO: REFATORAR
        Shader shader("../../src/shader_vertex.glsl","../../src/shader_fragment.glsl");

        model_uniform           = shader.getUniformLocation("model");
        view_uniform            = shader.getUniformLocation("view");
        projection_uniform      = shader.getUniformLocation("projection");
        object_id_uniform       = shader.getUniformLocation("object_id");
        bbox_min_uniform        = shader.getUniformLocation("bbox_min");
        bbox_max_uniform        = shader.getUniformLocation("bbox_max");

        shader.activate();
        shader.passValue("TextureImage0", 0);
        shader.passValue("TextureImage1", 1);
        shader.passValue("TextureImage2", 2);
        shader.deactivate();

        fprintf(stdout,"Shaders recarregados!\n");
        fflush(stdout);
    }
    if (key == GLFW_KEY_D  && action == GLFW_PRESS){
       player.move(glm::vec3(1.0f,0.0f,0.0f));
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        player.move(glm::vec3(-1.0f,0.0f,0.0f));
    }
    if (key == GLFW_KEY_W  && action == GLFW_PRESS){
        player.move(glm::vec3(0.0f,1.0f,0.0f));
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        player.move(glm::vec3(0.0f,-1.0f,0.0f));
    }
    if (key == GLFW_KEY_Q  && action == GLFW_PRESS){
        player.move(glm::vec3(0.0f,0.0f,1.0f));
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        player.move(glm::vec3(0.0f,0.0f,-1.0f));
    }
    if (key == GLFW_KEY_D  && action == GLFW_RELEASE){
        player.move(glm::vec3(0.0f,0.0f,0.0f));
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE){
        player.move(glm::vec3(0.0f,0.0f,0.0f));
    }
    if (key == GLFW_KEY_W  && action == GLFW_RELEASE){
        player.move(glm::vec3(0.0f,0.0f,0.0f));
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE){
        player.move(glm::vec3(0.0f,0.0f,0.0f));
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        free_camera = !free_camera;
    }
    if (key == GLFW_KEY_UP){
        g_Walking = (action != GLFW_RELEASE) ? 1 : 0;
    }

    if (key == GLFW_KEY_DOWN){
        g_Walking = (action != GLFW_RELEASE) ? -1 : 0;
    }

    if (key == GLFW_KEY_LEFT){
        g_Strafing = (action != GLFW_RELEASE) ? -1 : 0;
    }

    if (key == GLFW_KEY_RIGHT){
        g_Strafing = (action != GLFW_RELEASE) ? 1 : 0;
    }
}

void initialize_position(vector<Object *> planes){
    srand(time(NULL));
    for (int i = 0; i < planes.size(); ++i) {
        float x = MIN + (rand() % (MAX +1) + MIN);
        cout << x; cout << " ";
        float y = MIN + (rand() % (MAX +1) + MIN);
        cout << y; cout << " ";
        float z = -i*5 - 2/*MIN + (rand() % (MAX +1) + MIN)*/;
        cout << z << endl;

        planes[i]->setPos(glm::vec3(x,y,z));
        planes[i]->printBBox();
        //plane.setPos(glm::vec3(0.0f, -2.0f, 0.0f));
    }
}

glm::vec3 randomize_position(float camera_z){
    srand(time(NULL));
    float x = MIN + (rand() % (MAX +1) + MIN);
    //cout << x; cout << " ";
    float y = MIN + (rand() % (MAX +1) + MIN);
    cout << y; cout << " ";
    float z =  camera_z - 15;
    //cout << z << endl;

    return glm::vec3(x,y,z);
    //plane.setPos(glm::vec3(0.0f, -2.0f, 0.0f));
}


void update_plane(vector<Object *> planes, float camera_z){

    for (int i = 0; i < planes.size(); ++i) {
        if(planes[i]->pos.z > camera_z){
            // player.destroyed = true;
            cout << "killer: ";
            cout << i << endl; cout << " ";
            cout << camera_z;
        }
        if(planes[i]->destroyed){
            planes[i]->setPos(randomize_position(camera_z));
            planes[i]->destroyed = false;
        }
    }

}

