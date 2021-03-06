#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>

namespace GameBoi { namespace graphics {

    class Texture {
    private:
        std::string		m_FileName;
        GLuint			m_TID;
        GLuint          load();
    public:
        Texture(const std::string& filename);
        ~Texture(void);
        Texture(const Texture &src);
        Texture &operator=(const Texture &rhs);
        void            bind()const ;
    };
}}