#include "../include/ui.hpp"

ui::HollowRect::HollowRect(qrk::vec2f size, float inset) {
    std::vector<qrk::vec2f> verticesTemp;

    size = qrk::vec2f({size.x() * 2, size.y() * 2});
    inset *= 2;

    verticesTemp.push_back(qrk::vec2f({0, 0}));
    verticesTemp.push_back(qrk::vec2f({0, -inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -inset}));

    verticesTemp.push_back(qrk::vec2f({0, 0}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), 0}));

    verticesTemp.push_back(qrk::vec2f({0, -inset}));
    verticesTemp.push_back(qrk::vec2f({0, -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({inset, -size.y() + inset}));

    verticesTemp.push_back(qrk::vec2f({0, -inset}));
    verticesTemp.push_back(qrk::vec2f({inset, -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({inset, -inset}));

    verticesTemp.push_back(qrk::vec2f({size.x() - inset, -inset}));
    verticesTemp.push_back(qrk::vec2f({size.x() - inset, -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -size.y() + inset}));

    verticesTemp.push_back(qrk::vec2f({size.x() - inset, -inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -inset}));

    verticesTemp.push_back(qrk::vec2f({0, -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -size.y()}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -size.y() + inset}));

    verticesTemp.push_back(qrk::vec2f({0, -size.y() + inset}));
    verticesTemp.push_back(qrk::vec2f({0, -size.y()}));
    verticesTemp.push_back(qrk::vec2f({size.x(), -size.y()}));


    std::vector<GLfloat> vertices;
    for (int i = 0; i < verticesTemp.size(); i++) {
        vertices.push_back(verticesTemp[i].x());
        vertices.push_back(verticesTemp[i].y());
        vertices.push_back(0.f);
        vertices.push_back(0.f);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = verticesTemp.size();

    this->originalSize = size;
}

void ui::HollowRect::SetSize(float x, float y) {
    size = qrk::vec2f({x / (originalSize.x() / 2), y / (originalSize.y() / 2)});
}

qrk::DrawData_2D ui::HollowRect::GetDrawData() {
    qrk::DrawData_2D drawData;
    drawData.VAO = VAO;
    drawData.VBO = VBO;
    drawData.position = position;
    drawData.size = size;
    drawData.texture = nullptr;
    drawData.color = qrk::ConvertToFloat(color);
    drawData.vertexCount = vertexCount;
    drawData.rotation = rotation;
    drawData.zLayer = zLayer;
    return drawData;
}

ui::Button::Button() {
    size = qrk::vec2f({0, 0});
    position = qrk::vec2f({0, 0});
    texture = nullptr;
    font = nullptr;
    buttonText = qrk::Text();
}

void ui::Button::SetTexture(qrk::Texture2D &_texture, State state) {
    switch (state) {
        case UNHOVERED:
            texture = &_texture;
            break;
        case HOVERED:
            textureHover = &_texture;
            break;
        case PRESSED:
            texturePress = &_texture;
            break;
        case ALL:
            texture = &_texture;
            textureHover = &_texture;
            texturePress = &_texture;
            break;
        default:
            qrk::debug::Warning("Invalid enum for Button::SetTexture");
            break;
    }
}

void ui::Button::SetColor(const qrk::Color &_color, State state) {
    switch (state) {
        case UNHOVERED:
            color = _color;
            break;
        case HOVERED:
            colorHover = _color;
            break;
        case PRESSED:
            colorPress = _color;
            break;
        case ALL:
            color = _color;
            colorHover = _color;
            colorPress = _color;
            break;
        default:
            qrk::debug::Warning("Invalid enum for Button::SetColor");
            break;
    }
}

void ui::Button::QueueDraw(qrk::RenderWindow &window) {
    if (!window.IsOpen()) { return; }
    window.QueueDraw(buttonRect.GetDrawData());
    window.QueueDraw(buttonText.GetDrawData());
}