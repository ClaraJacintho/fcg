//
// Created by ynniv on 12/06/18.
//

#ifndef FCG_SHADER_H
#define FCG_SHADER_H


#include <glad/glad.h>
#include "fcg_util.hpp"

class Shader {
public:
    GLuint shader_id;
    Shader(const char* vertex_path, const char* fragment_path) {
        GLuint vtx_shader_id, frg_shader_id;
        vtx_shader_id = LoadShader_Vertex(vertex_path);
        frg_shader_id = LoadShader_Fragment(fragment_path);

        shader_id = CreateGpuProgram(vtx_shader_id, frg_shader_id);
    }

    void activate() {
        glUseProgram(shader_id);
    }

    void deactivate() {
        glUseProgram(0);
    }

    void passValue(const std::string &attrib, int value) const {
        glUniform1i(glGetUniformLocation(shader_id, attrib.c_str()), value);
    }

    void passValue(const std::string &attrib, float value) const {
        glUniform1f(glGetUniformLocation(shader_id, attrib.c_str()), value);
    }

    GLint getUniformLocation(const std::string &uniform) {
        return glGetUniformLocation(shader_id, uniform.c_str());
    }

private:
    GLuint LoadShader_Vertex(const char* filename) {
        // Criamos um identificador (ID) para este shader, informando que o mesmo
        // ser� aplicado nos v�rtices.
        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

        // Carregamos e compilamos o shader
        LoadShader(filename, vertex_shader_id);

        // Retorna o ID gerado acima
        return vertex_shader_id;
    }

    GLuint LoadShader_Fragment(const char* filename) {
        // Criamos um identificador (ID) para este shader, informando que o mesmo
        // ser� aplicado nos fragmentos.
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Carregamos e compilamos o shader
        LoadShader(filename, fragment_shader_id);

        // Retorna o ID gerado acima
        return fragment_shader_id;
    }

    void LoadShader(const char* filename, GLuint shader_id) {
        // Lemos o arquivo de texto indicado pela vari�vel "filename"
        // e colocamos seu conte�do em mem�ria, apontado pela vari�vel
        // "shader_string".
        std::ifstream file;
        try {
            file.exceptions(std::ifstream::failbit);
            file.open(filename);
        } catch ( std::exception& e ) {
            fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
            std::exit(EXIT_FAILURE);
        }
        std::stringstream shader;
        shader << file.rdbuf();
        std::string str = shader.str();
        const GLchar* shader_string = str.c_str();
        const GLint   shader_string_length = static_cast<GLint>( str.length() );

        // Define o c�digo do shader GLSL, contido na string "shader_string"
        glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

        // Compila o c�digo do shader GLSL (em tempo de execu��o)
        glCompileShader(shader_id);

        // Verificamos se ocorreu algum erro ou "warning" durante a compila��o
        GLint compiled_ok;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos mem�ria para guardar o log de compila��o.
        // A chamada "new" em C++ � equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];
        glGetShaderInfoLog(shader_id, log_length, &log_length, log);

        // Imprime no terminal qualquer erro ou "warning" de compila��o
        if ( log_length != 0 )
        {
            std::string  output;

            if ( !compiled_ok )
            {
                output += "ERROR: OpenGL compilation of \"";
                output += filename;
                output += "\" failed.\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            }
            else
            {
                output += "WARNING: OpenGL compilation of \"";
                output += filename;
                output += "\".\n";
                output += "== Start of compilation log\n";
                output += log;
                output += "== End of compilation log\n";
            }

            fprintf(stderr, "%s", output.c_str());
        }

        // A chamada "delete" em C++ � equivalente ao "free()" do C
        delete [] log;
    }

    GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
        // Criamos um identificador (ID) para este programa de GPU
        GLuint program_id = glCreateProgram();

        // Defini��o dos dois shaders GLSL que devem ser executados pelo programa
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Linkagem dos shaders acima ao programa
        glLinkProgram(program_id);

        // Verificamos se ocorreu algum erro durante a linkagem
        GLint linked_ok = GL_FALSE;
        glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

        // Imprime no terminal qualquer erro de linkagem
        if ( linked_ok == GL_FALSE )
        {
            GLint log_length = 0;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

            // Alocamos mem�ria para guardar o log de compila��o.
            // A chamada "new" em C++ � equivalente ao "malloc()" do C.
            GLchar* log = new GLchar[log_length];

            glGetProgramInfoLog(program_id, log_length, &log_length, log);

            std::string output;

            output += "ERROR: OpenGL linking of program failed.\n";
            output += "== Start of link log\n";
            output += log;
            output += "\n== End of link log\n";

            // A chamada "delete" em C++ � equivalente ao "free()" do C
            delete [] log;

            fprintf(stderr, "%s", output.c_str());
        }

        // Os "Shader Objects" podem ser marcados para dele��o ap�s serem linkados
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        // Retornamos o ID gerado acima
        return program_id;
    }
};


#endif //FCG_SHADER_H
