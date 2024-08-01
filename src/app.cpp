#include "../include/app.hpp"

qrk::RenderWindowSettings app::settings;
qrk::RenderWindow *app::window = nullptr;
qrk::Event *app::event = nullptr;
bool app::closedFlag = false;

int app::run() {
    Initialize();

    do {
        GetInput();
        if (closedFlag) { break; }
        Update();
        Draw();
    } while (window->IsOpen());

    Terminate();
    return 1;
}

void app::Initialize() {
    settings.windowSettings.clearColor = {41, 53, 102, 255};
    settings.windowSettings.windowStyle = Q_WINDOW_NONRESIZABLE;
    window = new qrk::RenderWindow(qrk::vec2u({1280, 720}), "ES 2 stroke camshaft generator | 0.3a", settings);

    event = new qrk::Event(window->GetWindow());
}

void app::GetInput() {
    event->UpdateWindow();
    if (event->KeyDown(qrk::ESCAPE)) {
        window->Close();
        closedFlag = true;
    }
}

void app::Update() {

}

void app::Draw() {
    window->ClearWindow();

    window->Draw();
}

void app::Terminate() {
    delete window;
    delete event;
}
