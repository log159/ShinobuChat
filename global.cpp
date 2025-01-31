﻿#include "global.h"
#include <vector>
#include <string>


//平台支持
#define IS_WINDOWS
//#define IS_WCHAR
#ifdef IS_WINDOWS
#include <windows.h>
#include <io.h>
#endif // IS_WINDOWS

bool GlobalTemp::ShowStyleEditor = false;
HWND GlobalTemp::WindowMainHandle;
WNDCLASSEXW GlobalTemp::WindowMainWc;
int GlobalTemp::CubismFrameCount = 0;
std::string GlobalTemp::LunarCalendar = "";
bool GlobalTemp::RefreshTable = true;

void GlobalConfig::GlobalConfigInit(GlobalConfig* gc) {
    static bool initHas = false;if (initHas == true) return;initHas = true;
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    std::cout << u8"全局配置初始化" << std::endl;

    gc->select_lan = (LAN)FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::LANGUAGESEL_ID], INITINT);
    gc->user_num = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::USER_NUM], INITINT);
    gc->select_theme_id = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::THEMESEL_ID], INITINT);
    gc->defaultStyle = ImGui::GetStyle();
    FILE* fpp;
    if (fopen_s(&fpp, STYLEWAY, "rb") == 0) {
        ImGuiStyle st;
        fread(&st, sizeof(ImGuiStyle), 1, fpp);
        ImGui::GetStyle() = st;
        fclose(fpp);
    }
    gc->select_font = (std::string)FileSetting::GetValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::FONTSEL_NAME], INITSTR);
    gc->window_cubism_style_id = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_CUBISM_STYLE_ID], INITINT);

    gc->window_main_style_id = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_STYLE_ID], INITINT);
    gc->window_main_dock_id = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_DOCK_ID], INITINT);
    gc->window_main_transparent_id = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_TRANSPARENT_ID], INITINT);

    if (gc->window_main_style_id == 0)
        ::SetWindowPos(GlobalTemp::WindowMainHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else if (gc->window_main_style_id == 1)
        ::SetWindowPos(GlobalTemp::WindowMainHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    if (gc->window_main_dock_id == 0)  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    else if (gc->window_main_dock_id == 1) io.ConfigFlags &= (~ImGuiConfigFlags_DockingEnable);
    if (gc->window_main_transparent_id == 0)  io.ConfigDockingTransparentPayload = true;
    else if (gc->window_main_transparent_id == 1) io.ConfigDockingTransparentPayload = false;

    gc->window_main_forecastfps = (float)FileSetting::GetDoubleValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_FORECASTFPS], 120.0f);
    gc->window_main_addtimefps = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_ADDTIMEFPS], 0);

    gc->window_cubism_addtimefps = FileSetting::GetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_CUBISM_ADDTIMEFPS], 0);

#ifdef IS_WINDOWS
    static std::vector<std::pair<std::string,std::string>> temp_fonts;
    std::string basePath = "./Fonts/";
    std::string searchPattern = basePath + "*.ttf";
    wchar_t widePattern[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, searchPattern.c_str(), -1, widePattern, MAX_PATH);
    struct _wfinddata_t fdata;
    intptr_t fh = _wfindfirst(widePattern, &fdata);
    if (fh == -1) {
        std::cout << "No .ttf files found.\n";
    }
    temp_fonts.clear();
    do {
        if (!(fdata.attrib & _A_SUBDIR)) {
            char fileName[MAX_PATH];
            WideCharToMultiByte(CP_UTF8, 0, fdata.name, -1, fileName, MAX_PATH, NULL, NULL);

            temp_fonts.push_back(std::pair<std::string,std::string>(fileName,basePath + fileName));
        }
    } while (_wfindnext(fh, &fdata) == 0);
    _findclose(fh);
    for (const auto& file : temp_fonts) {
        std::cout << file.second << std::endl;
    }
    std::cout << "TTF file search end\n";
    gc->fonts_size = temp_fonts.size();
    gc->fonts_list = new const char* [gc->fonts_size];
    gc->fonts_name_list = new const char* [gc->fonts_size];
    for (int i = 0; i < gc->fonts_size; ++i) {
        gc->fonts_list[i] = temp_fonts[i].second.c_str();
        gc->fonts_name_list[i] = temp_fonts[i].first.c_str();
        //cout << i << " " << gc->fonts_list[i] << endl;
    }
#endif //IS_WINDOWS

    //设置字体
    ImFont* font_current = ImGui::GetFont();
    for (int i = 0; i < gc->fonts_size; ++i) {
        ImFont* font = io.Fonts->AddFontFromFileTTF(gc->fonts_list[i],30.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        if (strcmp(font->GetDebugName(), gc->select_font.c_str()) == 0) {
            io.FontDefault = font;
        }
    }
    io.FontGlobalScale =(float)FileSetting::GetDoubleValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::FONT_GLOBAL_SCALE], 1.0);


}

void GlobalConfig::GlobalConfigSave(GlobalConfig* gc)
{

    std::cout << u8"全局配置保存" << std::endl;
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::LANGUAGESEL_ID], (int)::GlobalConfig::getInstance()->select_lan);
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::USER_NUM], (int)::GlobalConfig::getInstance()->user_num);
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::THEMESEL_ID], (int)::GlobalConfig::getInstance()->select_theme_id);

    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_STYLE_ID], (int)::GlobalConfig::getInstance()->window_main_style_id);
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_DOCK_ID], (int)::GlobalConfig::getInstance()->window_main_dock_id);
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_TRANSPARENT_ID], (int)::GlobalConfig::getInstance()->window_main_transparent_id);

    FILE* fp;
    fopen_s(&fp, STYLEWAY, "wb");
    if (fp != NULL) {
        fwrite(&ImGui::GetStyle(), sizeof(ImGuiStyle), 1, fp);
        fclose(fp);
    }
    FileSetting::SetValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::FONTSEL_NAME],::GlobalConfig::getInstance()->select_font.c_str());
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_CUBISM_STYLE_ID], ::GlobalConfig::getInstance()->window_cubism_style_id);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    FileSetting::SetDoubleValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::FONT_GLOBAL_SCALE], (double)io.FontGlobalScale);
    FileSetting::SetDoubleValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_FORECASTFPS], (double)::GlobalConfig::getInstance()->window_main_forecastfps);
    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_MAIN_ADDTIMEFPS],::GlobalConfig::getInstance()->window_main_addtimefps);

    FileSetting::SetLongValue(0, INIGROUPMARKSTR, inifreemark_map[::FREEMARK::WINDOW_CUBISM_ADDTIMEFPS], ::GlobalConfig::getInstance()->window_cubism_addtimefps);

}

GlobalConfig::~GlobalConfig()
{
    if (fonts_size == 0 && fonts_list == NULL) { return; }
    for (int i = 0; i < fonts_size; ++i) {
        free((void*)fonts_list[i]);
        free((void*)fonts_name_list[i]);
    }
    delete[] fonts_list;
    delete[] fonts_name_list;
}
