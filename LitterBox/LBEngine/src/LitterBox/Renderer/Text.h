#pragma once
#include <glm.hpp>
#include "Camera.h"

namespace LB
{
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Offset;    // Offset from baseline to left/top of glyph
        unsigned int OffsetH;   // Horizontal offset to advance to next glyph
    };

    class Text
    {
    public:

        //Text();
        //~Text();

        void LoadFont(const std::string& fontpath, unsigned int fontsize = 24);
        void RenderText(const std::string& text, glm::vec2 position, float scale, glm::vec3 color = { 255.f, 255.f, 255.f });

        std::map<char, Character> characters{};

    private:
        unsigned int textShader{};
        unsigned int vao{}, vbo{};
    };

}

