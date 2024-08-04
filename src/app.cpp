#include "../include/app.hpp"
#include <../include/rect.hpp>

qrk::RenderWindowSettings app::settings;
qrk::RenderWindow *app::window = nullptr;
qrk::Event *app::event = nullptr;
bool app::closedFlag = false;

qrk::Font *app::font_aribkl160 = nullptr;
qrk::Font *app::font_aribkl80 = nullptr;
qrk::Font *app::font_aribkl40 = nullptr;
qrk::Text *app::text_title = nullptr;
qrk::Text *app::text_titleBottom = nullptr;
ui::HollowRect *app::hr_visualizerOutline = nullptr;

int app::run() {
    Initialize();

    while (window->IsOpen()) {
        GetInput();
        if (closedFlag) { break; }
        Update();
        Draw();
    }

    Terminate();
    return 0;
}

void app::Initialize() {
    //window creation//
    qrk::debug::Log("Window creation started");
    qrk::vec2u primaryScreenSize = qrk::GetPrimaryScreenSize();
    qrk::vec2f windowSize({(float) primaryScreenSize.x() * 0.66f,
                           (float) primaryScreenSize.y() * 0.66f});
    qrk::debug::Log("Primary screen size detected: " +
                    std::to_string(primaryScreenSize.x()) + ", " +
                    std::to_string(primaryScreenSize.y()));
    settings.windowSettings.clearColor = {41, 53, 102, 255};
    settings.windowSettings.windowStyle = Q_WINDOW_NONRESIZABLE;
    settings.windowSettings.multisamplingLevel = 16;
    settings.renderSettings.cullFaces = true;
    window = new qrk::RenderWindow(qrk::vec2u({(unsigned int) windowSize.x(),
                                               (unsigned int) windowSize.y()}),
                                   "ES 2t camshaft generator | 0.3a", settings);

    if (!window->IsOpen()) {
        qrk::debug::Error("Failed to create window",
                          qrk::debug::Q_FAILED_TO_CREATE_WINDOW);
    }

    event = new qrk::Event(window->GetWindow());
    //window creation end//
    //initialize the UI//
    //UI text //
    font_aribkl160 = new qrk::Font("resources/fonts/ariblk.ttf",
                                   windowSize.x() * 0.16f, 2000);
    font_aribkl80 =
            new qrk::Font("resources/fonts/ariblk.ttf", windowSize.x() * 0.08f);
    font_aribkl40 =
            new qrk::Font("resources/fonts/ariblk.ttf", windowSize.x() * 0.04f);
    text_title = new qrk::Text(*font_aribkl160);
    text_titleBottom = new qrk::Text(*font_aribkl40);
    text_title->SetText("ES 2t Generator");
    text_title->SetPosition(20, 20);
    text_titleBottom->SetText("By Midnight Blue");
    text_titleBottom->SetPosition(23, 20 + text_title->GetHeight());
    // !UI text //
    // UI user input

    // !UI user input
    // UI misc//
    float windowSizeX = window->GetWindow().GetSize().x();
    float windowSizeY = window->GetWindow().GetSize().y();

    hr_visualizerOutline = new ui::HollowRect(
            qrk::vec2f({windowSizeX * 0.4f, windowSizeY * 0.8f}),
            windowSizeX * 0.005f);

    hr_visualizerOutline->SetPosition(
            windowSizeX - hr_visualizerOutline->GetSize().x() -
                    (windowSizeX * 0.05f),
            -windowSizeY + hr_visualizerOutline->GetSize().y() +
                    (windowSizeY - hr_visualizerOutline->GetSize().y()) / 2);
    hr_visualizerOutline->SetLayer(-0.1f);
    // !UI misc //
    //UI initialization end//
}

void app::GetInput() {
    event->UpdateWindow();
    if (event->KeyDown(qrk::ESCAPE)) {
        window->Close();
        closedFlag = true;
    }
}

void app::Update() {}

void app::Draw() {
    window->ClearWindow();

    //draw UI//
    window->QueueDraw(text_title->GetDrawData());
    window->QueueDraw(text_titleBottom->GetDrawData());
    window->QueueDraw(hr_visualizerOutline->GetDrawData());
    //end UI draw//

    window->Draw();
}

void app::Terminate() {
    qrk::debug::Log("Terminating app");

    delete window;
    delete event;
    delete font_aribkl160;
    delete font_aribkl80;
    delete font_aribkl40;
    delete text_title;
    delete text_titleBottom;
}
