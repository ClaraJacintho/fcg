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
#include "fcg_util.hpp"
// Headers locais, definidos na pasta "include/"


#include "Object.h"
#include "utils.h"
#include "Player.h"
#include "Shader.h"


Player player;
double dt = 0.0f;
double lastFrame = 0.0f;

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

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    player.setPos(glm::vec3(0.0f,-1.0f,0.0f));
    player.setScale(glm::vec3(0.05f,0.05f,0.05f));
    player.setRotation(glm::vec3(-22.5f,180.0f,0.0f));
    ComputeNormals(&(player.model));
    BuildTrianglesAndAddToVirtualScene(&player);
    objects.push_back(&player);

    Object sphere(1, "sphere","../../data/sphere.obj");
    sphere.setPos(glm::vec3(1.0f,1.0f,-1.0f));
    sphere.setScale(glm::vec3(1.0f,1.0f,1.0f));
    ComputeNormals(&(sphere.model));
    BuildTrianglesAndAddToVirtualScene(&sphere);
    objects.push_back(&sphere);

    Object plane(3, "plane","../../data/plane.obj");
    plane.setPos(glm::vec3(0.0f, -2.0f, 0.0f));
    plane.setScale(glm::vec3(2.0f,2.0f,2.0f));
    ComputeNormals(&(plane.model));
    BuildTrianglesAndAddToVirtualScene(&plane);
    objects.push_back(&plane);

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

    int number_of_objects = g_VirtualScene.size();

    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window)) {
        double curTime = glfwGetTime();
        dt = curTime - lastFrame;
        lastFrame = curTime;

        // Definimos a cor do "fundo" do framebuffer como branco.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        shader.activate();

        player.update_player(dt, objects);

        // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
        glm::vec4 camera_position_c  = glm::vec4(player.pos.x,player.pos.y+2.0f,player.pos.z+3.0f,1.0f);
        glm::vec4 camera_lookat_l    = glm::vec4(player.pos.x,player.pos.y,player.pos.z,1.0f);
        glm::vec4 camera_view_vector = camera_lookat_l - camera_position_c;
        glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f);

        glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);
        glm::mat4 projection;

        float nearplane = -0.1f;
        float farplane  = -10.0f;

        if (g_UsePerspectiveProjection) {
            float field_of_view = M_PI / 3.0f;
            projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        } else {
            float t = 1.5f*g_CameraDistance/2.5f;
            float b = -t;
            float r = t*g_ScreenRatio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
        }

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

        shader.passValue("view", view);
        shader.passValue("projection", projection);

        for(int i = 0; i < number_of_objects; i++){
            if(objects[i]->destroyed == false){
                model = Matrix_Translate(objects[i]->pos.x,objects[i]->pos.y,objects[i]->pos.z)
                    * Matrix_Scale(objects[i]->scale.x,objects[i]->scale.y,objects[i]->scale.z)
                    * Matrix_Rotate_X(objects[i]->rad.x)
                    * Matrix_Rotate_Y(objects[i]->rad.y)
                    * Matrix_Rotate_Z(objects[i]->rad.z);
                shader.passValue("model", model);
                shader.passValue("object_id", objects[i]->obj_type);
                DrawVirtualObject(objects[i]->name.c_str());
            }
        }

        shader.deactivate();

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

        TextRendering_ShowEulerAngles(window);
        TextRendering_ShowProjection(window);
        TextRendering_ShowFramesPerSecond(window);

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
        player.move_right();
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        player.move_left();
    }
       if (key == GLFW_KEY_W  && action == GLFW_PRESS){
        player.move_up();
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        player.move_down();
    }
       if (key == GLFW_KEY_Q  && action == GLFW_PRESS){
        player.move_foward();
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        player.move_backwards();
    }
    if (key == GLFW_KEY_D  && action == GLFW_RELEASE){
        player.unturn_right();
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE){
        player.unturn_left();
    }
       if (key == GLFW_KEY_W  && action == GLFW_RELEASE){
        player.unturn_up();
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE){
        player.unturn_down();
    }


}
