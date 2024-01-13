#pragma once
#include "font.h"
#include "fonts.hpp"

class Theme {
public:
    ImFont *font, *font1, *font2, *font3, *font4, *fontsp, *arial;
    void Register(ImGuiIO& io) {

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
        icons_config.FontDataOwnedByAtlas = false;

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;

        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;

        arial = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16);
        //font1 = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 16.f, &font_cfg);
        //font2 = io.Fonts->AddFontFromMemoryCompressedTTF(poppinsFont, 158240, 19.0f, &icons_config, icons_ranges);
        //font3 = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 17.5, &CustomFont);

        font = io.Fonts->AddFontFromMemoryTTF((void*)fontAwesome, sizeof(fontAwesome), 16.f, &icons_config, icons_ranges);

    }
    
};