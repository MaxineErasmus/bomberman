#include "Font.hpp"
// #include "../../graphics/includes/Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GameBoi { namespace graphics {

    Fonts::Fonts(const char * file)
    {
        Load(file);
    }
    Fonts::~Fonts()
    {}

    Fonts::Fonts(const Fonts &src){
        static_cast<void>(src);
        return;
    }

    Fonts &Fonts::operator=(const Fonts &rhs){
        static_cast<void>(rhs);
        return *this;
    }
    void Fonts::Load(const char * file)
    {
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&_ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        // Load font as face
        if (FT_New_Face(_ft, file, 0, &_face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(_face, 0, 48);
        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                _face->glyph->bitmap.width,
                _face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                _face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                texture,
                glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
                glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
                static_cast<GLuint>(_face->glyph->advance.x)
            };
            _characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're finished
        FT_Done_Face(_face);
        FT_Done_FreeType(_ft);
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Fonts::Enable()
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Fonts::Disable()
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    void Fonts::RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int width, int height)
    {
        Enable();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(shader.ID(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(_VAO);
        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = _characters[*c];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        Disable();
    }
}}