#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpola��o da cor de cada v�rtice, definidas em "shader_vertex.glsl" e
// "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posi��o do v�rtice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// gouraud shading, calculado no vertex shader
in vec3 gouraud_color;

// Matrizes computadas no c�digo C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto est� sendo desenhado no momento
#define EARTH 1
#define JET 2
#define PLANE  3
#define MARS 4
#define COW 5
#define SUN 6
uniform int object_id;

// Par�metros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Vari�veis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;

// O valor de sa�da ("out") de um Fragment Shader � a cor final do fragmento.
out vec3 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posi��o da c�mera utilizando a inversa da matriz que define o
    // sistema de coordenadas da c�mera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual � coberto por um ponto que percente � superf�cie de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posi��o no
    // sistema de coordenadas global (World coordinates). Esta posi��o � obtida
    // atrav�s da interpola��o, feita pelo rasterizador, da posi��o de cada
    // v�rtice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada v�rtice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em rela��o ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da c�mera em rela��o ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    float lambert = max(0,dot(n,l));

    vec3 Kd;
    vec3 Ks;
    vec3 Ka;
    float q;

    vec4 r = (l*-1) + (2*n)*(dot(n,l));
    vec3 Ia = vec3(0.5f,0.5f,0.5f);
    vec3 I = vec3(1.0,1.0,1.0);

    if ( object_id == EARTH )
    {
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        vec3 ro  = vec3(position_model.x,position_model.y, position_model.z);
        float ro_value = length(ro);
        float thetha = atan(position_model.x, position_model.z);
        float phi = asin(position_model.y/ro_value);

        U = (thetha+M_PI)/(2*M_PI);
        V = (phi + M_PI_2)/M_PI;

        Kd = texture(TextureImage0, vec2(U,V)).rgb;
        Ks = vec3(0.5, 0.5, 0.7);
        Ka = Kd;

        vec3 diffuse = Kd *I * lambert + 0.01;
        vec3 ambient = Ka * Ia;
        color = diffuse + ambient;
    }
    else if ( object_id == JET )
    {
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;
        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = (position_model.x - minx)/(maxx - minx);
        V = (position_model.z - minz)/(maxz - minz);

        Kd = texture(TextureImage2, vec2(U,V)).rgb * vec3(0.64, 0.64, 0.64);
        color = Kd * (lambert + 0.01);
    }
    else if ( object_id == PLANE )
    {
        Kd = vec3(0.0,1,0.5);
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.3,0.3,0.3);
        q = 1.0;
        vec3 diffuse = Kd * (lambert + 0.01);
        vec3 ambient = Ka * Ia;
        vec3 phong =  Ks*I*(pow(max(0,dot(r,v)),q));

        color = diffuse + ambient + phong;
    }
    else if ( object_id == MARS ){
           Kd = vec3(0.8,0.4,0.08);
            Ks = vec3(0.0,0.0,0.0);
            Ka = vec3(0.4,0.2,0.04);
            q = 1.0;
            vec3 diffuse = Kd * (lambert + 0.01);
            vec3 ambient = Ka * Ia;
            vec3 phong =  Ks*I*(pow(max(0,dot(r,v)),q));
            color = diffuse + ambient + phong;
    }
    else if ( object_id == COW ){
          Kd = vec3(0.5,0.5,0.5);
//          Ks = vec3(0.8,0.8,0.8);
//          Ka = vec3(0.04,0.2,0.4);
//          q = 32.0;
//          vec3 diffuse = Kd * (lambert + 0.01);
//          vec3 ambient = Ka * Ia;
//          vec3 phong =  Ks*I*(pow(max(0,dot(r,v)),q));
//          color = diffuse + ambient + phong;
          color = gouraud_color * Kd;
    }
    else if ( object_id == SUN ){
          Kd = vec3(0.08,0.4,0.8);
          Ks = vec3(0.8,0.8,0.8);
          Ka = vec3(1,1,1);
          q = 32.0;
          vec3 diffuse = Kd * (lambert + 0.01);
          vec3 ambient = Ka * Ia;
          vec3 phong =  Ks*I*(pow(max(0,dot(r,v)),q));
          color = diffuse + ambient + phong;
    }
    //color = Kd0 * (lambert + 0.01);

    // Cor final com corre��o gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
}
