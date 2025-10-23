#include "SDL_surface.h"
#include "sys.hpp"
#include <fstream>
#include <vector>


namespace render {


    // I'll be changing this later
    // Shader
    static uint32_t main_vertex_shader = 0;
    static uint32_t main_fragment_shader = 0;
    // Program
    static uint32_t main_program = 0;
    // Vertex Array
    static uint32_t main_vertex_array = 0;
    // Uniform
    static uint32_t main_u_proj = 0;
    static uint32_t main_u_view = 0;
    static uint32_t main_u_model = 0;
    static uint32_t main_u_tex0 = 0;
    static uint32_t main_u_renderMode = 0;

    // Attributes
    const static uint32_t MAIN_VERTICES = 0;
    const static uint32_t MAIN_TEXCOORD = 1;

    // Buffers
    // Vertices
    static std::vector<glm::vec3> vertices_list;
    static uint32_t vertices_buffer_id = 0;
    // Normal Texture Coords
    static std::vector<glm::vec2> normal_texCoord_list;
    static uint32_t normal_texCoord_id = 0;
    // Fliped Texture Coords
    static std::vector<glm::vec2> fliped_texCoord_list;
    static uint32_t fliped_texCoord_id = 0;


    // Texture Manager
    static std::map<std::string, uint32_t> g_textures;

    static uint32_t create_shader(GLenum type, std::string path);
    static uint32_t create_program(const std::vector<uint32_t>& shaders);
    static void delete_program(uint32_t& id, const std::vector<uint32_t>& shaders);

    void init() {
        glDisable(GL_DEPTH_TEST);

        // Create Main Shaders
        main_vertex_shader = create_shader(GL_VERTEX_SHADER, "data/shaders/main.vs.glsl");
        main_fragment_shader = create_shader(GL_FRAGMENT_SHADER, "data/shaders/main.fs.glsl");

        // Program
        main_program = create_program({main_vertex_shader, main_fragment_shader});

        glGenVertexArrays(1, &main_vertex_array);

        bind();
        // Uniform
        main_u_proj = glGetUniformLocation(main_program, "proj");
        main_u_view = glGetUniformLocation(main_program, "view");
        main_u_model = glGetUniformLocation(main_program, "model");
        main_u_tex0 = glGetUniformLocation(main_program, "tex0");
        glUniform1i(main_u_tex0, 0);
        main_u_renderMode = glGetUniformLocation(main_program, "renderMode");
        glUniform1i(main_u_renderMode, RenderMode::RENDER_MODE_NO_TEXTURES);

        // Attribute
        glBindVertexArray(main_vertex_array);
        glEnableVertexAttribArray(MAIN_VERTICES);
        glEnableVertexAttribArray(MAIN_TEXCOORD);
        glBindVertexArray(0);
        unbind();

        // Buffers
        // vertices
        vertices_list.clear();
        vertices_list.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        vertices_list.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        vertices_list.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        vertices_list.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        vertices_list.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        vertices_list.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

        glGenBuffers(1, &vertices_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices_list.size() * sizeof(glm::vec3), vertices_list.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Normal Texture Coords
        normal_texCoord_list.clear();
        normal_texCoord_list.push_back(glm::vec2(0.0f, 0.0f));
        normal_texCoord_list.push_back(glm::vec2(1.0f, 0.0f));
        normal_texCoord_list.push_back(glm::vec2(0.0f, 1.0f));

        normal_texCoord_list.push_back(glm::vec2(0.0f, 1.0f));
        normal_texCoord_list.push_back(glm::vec2(1.0f, 0.0f));
        normal_texCoord_list.push_back(glm::vec2(1.0f, 1.0f));

        glGenBuffers(1, &normal_texCoord_id);
        glBindBuffer(GL_ARRAY_BUFFER, normal_texCoord_id);
        glBufferData(GL_ARRAY_BUFFER, normal_texCoord_list.size() * sizeof(glm::vec2), normal_texCoord_list.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Flipped Texture Coords
        fliped_texCoord_list.clear();
        fliped_texCoord_list.push_back(glm::vec2(1.0f, 0.0f));
        fliped_texCoord_list.push_back(glm::vec2(0.0f, 0.0f));
        fliped_texCoord_list.push_back(glm::vec2(1.0f, 1.0f));

        fliped_texCoord_list.push_back(glm::vec2(1.0f, 1.0f));
        fliped_texCoord_list.push_back(glm::vec2(0.0f, 0.0f));
        fliped_texCoord_list.push_back(glm::vec2(0.0f, 1.0f));

        glGenBuffers(1, &fliped_texCoord_id);
        glBindBuffer(GL_ARRAY_BUFFER, fliped_texCoord_id);
        glBufferData(GL_ARRAY_BUFFER, fliped_texCoord_list.size() * sizeof(glm::vec2), fliped_texCoord_list.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void release() {
        // Remove Textures
        for(std::map<std::string, uint32_t>::iterator it = g_textures.begin(); it != g_textures.end(); it++) {
            glDeleteTextures(1, &it->second);
        }

        g_textures.clear();

        // Regular Removes
        glDeleteBuffers(1, &fliped_texCoord_id);
        glDeleteBuffers(1, &normal_texCoord_id);
        glDeleteBuffers(1, &vertices_buffer_id);
        glDeleteVertexArrays(1, &main_vertex_array);
        delete_program(main_program, {main_vertex_shader, main_fragment_shader});
        glDeleteShader(main_vertex_shader);
        glDeleteShader(main_fragment_shader);
    }

    void clear(const glm::vec4& color) {
        glViewport(0, 0, app::getWidth(), app::getHeight());
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void bind() {
        glUseProgram(main_program);
    }

    void unbind() {
        glUseProgram(0);
    }

    void setProjection(const glm::mat4& m) {
        glUniformMatrix4fv(main_u_proj, 1, GL_FALSE, &m[0][0]);
    }

    void setView(const glm::mat4& m) {
        glUniformMatrix4fv(main_u_view, 1, GL_FALSE, &m[0][0]);
    }

    void setModel(const glm::mat4& m) {
        glUniformMatrix4fv(main_u_model, 1, GL_FALSE, &m[0][0]);
    }

    void setRenderMode(RenderMode renderMode) {
        glUniform1i(main_u_renderMode, renderMode);
    }

    void draw(TextureFlip textureFlip) {
        glBindVertexArray(main_vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_id);
        glVertexAttribPointer(MAIN_VERTICES, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if(textureFlip == TextureFlip::TF_NORMAL) {
            glBindBuffer(GL_ARRAY_BUFFER, normal_texCoord_id);
        } else if(textureFlip == TextureFlip::TF_FLIPPED) {
            glBindBuffer(GL_ARRAY_BUFFER, fliped_texCoord_id);
        }
        glVertexAttribPointer(MAIN_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, vertices_list.size());

        glBindVertexArray(0);
    }


    // Texture Manager
    void addTextureFromFile(std::string name, std::string path) {
        SDL_Surface* surf = IMG_Load(path.c_str());

        if(surf == nullptr) {
            std::cout << path << " doesn't exist\n";
            return;
        }

        glGenTextures(1, &g_textures[name]);

        glBindTexture(GL_TEXTURE_2D, g_textures.at(name));

        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            GL_RGBA, 
            surf->w, 
            surf->h, 
            0, 
            GL_RGBA, 
            GL_UNSIGNED_BYTE, 
            surf->pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);

        SDL_FreeSurface(surf);
    }

    void bindTexture(GLenum active, std::string name) {
        glActiveTexture(active);
        glBindTexture(GL_TEXTURE_2D, g_textures.at(name));
    }

    void unbindTexture(GLenum active) {
        glActiveTexture(active);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void enableAlphaBlend() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void disableAlphaBlend() {
        glDisable(GL_BLEND);
    }


    // Internal render functions
    static uint32_t create_shader(GLenum type, std::string path) {
        uint32_t temp = glCreateShader(type);

        // Load Shader Source
        std::ifstream in(path);
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::string src;
        src.resize(size);
        in.read(src.data(), src.size());
        std::cout << src << "\n";

        const char* c_src = src.c_str();
        glShaderSource(temp, 1, &c_src, nullptr);
        // Compile Shader
        glCompileShader(temp);

        // Check for errors
        int32_t len = 0;
        glGetShaderiv(temp, GL_INFO_LOG_LENGTH, &len);

        if(len > 0) {
            std::string log;
            log.resize(len);
            glGetShaderInfoLog(temp, log.size(), nullptr, log.data());
            std::cout << log << "\n";
        }

        return temp;
    }

    static uint32_t create_program(const std::vector<uint32_t>& shaders) {
        uint32_t temp = glCreateProgram();

        std::for_each(shaders.begin(), shaders.end(), [&](uint32_t shader) {
            glAttachShader(temp, shader);
        });

        glLinkProgram(temp);

        int32_t len = 0;

        glGetProgramiv(temp, GL_INFO_LOG_LENGTH, &len);

        if(len > 0) {
            std::string log;
            log.resize(len);
            glGetProgramInfoLog(temp, log.size(), nullptr, log.data());
            std::cout << log << "\n";
        }

        return temp;
    }

    static void delete_program(uint32_t& id, const std::vector<uint32_t>& shaders) {
        std::for_each(shaders.begin(), shaders.end(), [&](uint32_t shader) {
            glDetachShader(id, shader);
        });
        glDeleteProgram(id);
        id = 0;
    }
}