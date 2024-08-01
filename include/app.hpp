#ifndef Q_APP
#define Q_APP

#include <../include/render_window.hpp>
#include <../include/event.hpp>

class app {
public:
    static int run();
private:
    static void Initialize();
    static void GetInput();
    static void Update();
    static void Draw();
    static void Terminate();

    static qrk::RenderWindowSettings settings;
    static qrk::RenderWindow *window;
    static qrk::Event *event;

    static bool closedFlag;
};

#endif