#ifndef Q_UI
#define Q_UI

#include <../include/glyph_renderer.hpp>
#include <../include/rect.hpp>
#include <../include/render_window.hpp>

namespace ui {
class HollowRect {
public:
    HollowRect(){};
    HollowRect(qrk::vec2f size, float inset);

    void SetSize(float x, float y);
    qrk::vec2f GetSize() { return qrk::vec2f({size.x() * originalSize.x() / 2, size.y() * originalSize.y() / 2});
    }

    void SetPosition(float x, float y) {
        position = qrk::vec2f({x * 2, y * 2});
    }
    qrk::vec2f GetPosition() { return position; }

    void SetColor(const qrk::Color &_color) { color = _color; }
    qrk::Color GetColor() { return color; }

    void SetLayer(float layer) { zLayer = -std::clamp(layer, -0.999f, 0.999f); }

    qrk::DrawData_2D GetDrawData();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLsizei vertexCount = 0;

    qrk::vec2f position = qrk::vec2f({0, 0});
    qrk::vec2f size = qrk::vec2f({1, 1});
    qrk::vec2f originalSize;
    float rotation = 0;
    float zLayer = 0;
    qrk::Color color = {255, 255, 255, 255};
};
class Button {
public:
    enum State { UNHOVERED, HOVERED, PRESSED, ALL };

    Button();

    void SetFont(qrk::Font &_font) { font = &_font; }
    void SetText(const std::string &text) { buttonText.SetText(text); }
    void SetTexture(qrk::Texture2D &_texture, State state);
    void SetColor(const qrk::Color &_color, State state);

    void SetSize(const qrk::vec2f &_size) { size = _size; }
    qrk::vec2f GetSize() { return size; }
    void SetPosition(const qrk::vec2f &_position) { position = _position; }
    qrk::vec2f GetPosition() { return position; }
    void SetLayer(float _layer) {
        buttonRect.SetLayer(_layer - 0.01f);
        buttonText.SetLayer(_layer + 0.01f);
    }

    void QueueDraw(qrk::RenderWindow &window);

private:
    qrk::Font *font;
    qrk::Text buttonText;

    qrk::Rect buttonRect = qrk::Rect();
    qrk::Texture2D *texture;
    qrk::Texture2D *textureHover;
    qrk::Texture2D *texturePress;

    qrk::Color color;
    qrk::Color colorHover;
    qrk::Color colorPress;

    qrk::vec2f position;
    qrk::vec2f size;
    float layer;
};
}// namespace ui

#endif// !Q_UI
