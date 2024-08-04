#ifndef Q_APP
#define Q_APP

#include <../include/render_window.hpp>
#include <../include/event.hpp>
#include <../include/glyph_renderer.hpp>
#include "../include/ui.hpp"

class app {
public:
    static int run();
    static void Terminate();

private:
    static void Initialize();
    static void GetInput();
    static void Update();
    static void Draw();

    //basic elements
    static qrk::RenderWindowSettings settings;
    static qrk::RenderWindow *window;
    static qrk::Event *event;

    static bool closedFlag;

    //UI elements
    static qrk::Font *font_aribkl160;
    static qrk::Font *font_aribkl80;
    static qrk::Font *font_aribkl40;
    static qrk::Text *text_title;
    static qrk::Text *text_titleBottom;

    static ui::HollowRect *hr_visualizerOutline;
};

#endif