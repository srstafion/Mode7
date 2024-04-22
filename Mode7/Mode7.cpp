#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;
using namespace sf;
static int WINDOW_SIZE = 1024;
static float PI = 3.141592;

class Viewer {
private:
    RectangleShape viewer;
    Vector2f position;
    float angle;
public:
    Viewer() {
        viewer.setPosition(154, 140);
        viewer.setSize(Vector2f(10, 10));
        viewer.setFillColor(Color::Green);
        angle = 0;
    }
    void moveForward(float distance) {
        float angleRad = angle * PI / 180.0f;
        float deltaX = cos(angleRad) * distance;
        float deltaY = sin(angleRad) * distance;
        viewer.move(deltaX, -deltaY);
    }
    void moveBack(float distance) {
        moveForward(-distance);
    }
    void rotate(float degrees) {
        angle += degrees;
    }
    Vector2f getViewPosition() {
        return viewer.getPosition() + Vector2f(viewer.getSize().x / 2, viewer.getSize().y / 2);
    }

    float getAngle() {
        return angle;
    }
    void show(RenderWindow& window) {
        window.draw(viewer);
    }
};

int main() {
    Viewer viewer;
    Sprite floorMap;
    floorMap.scale(0.5, 0.5);
    Texture floorTexture;
    floorTexture.loadFromFile("SCDSS1.png");
    floorMap.setTexture(floorTexture);

    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mode7 Test");
    window.setPosition(Vector2i(60, 12.5));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Left)
                    viewer.rotate(1);
                else if (event.key.code == Keyboard::Right)
                    viewer.rotate(-1);
                else if (event.key.code == Keyboard::Up)
                    viewer.moveForward(3);
                else if (event.key.code == Keyboard::Down)
                    viewer.moveBack(3);
                else if (event.key.code == Keyboard::C)
                    cout << viewer.getViewPosition().x << " " << viewer.getViewPosition().y;
            }
        }

        window.clear();
        window.draw(floorMap);
        Vector2f viewerPos = viewer.getViewPosition();
        float angle = viewer.getAngle();
        float rayLength = 200.0f;

        for (int i = -1; i <= 1; i += 1) {
            float newAngle = angle + i * 60.0f;
            float endpointX = viewerPos.x + rayLength * cos(newAngle * PI / 180.0f);
            float endpointY = viewerPos.y - rayLength * sin(newAngle * PI / 180.0f);

            VertexArray line(Lines, 2);
            line[0].position = viewerPos;
            line[1].position = Vector2f(endpointX, endpointY);
            line[0].color = Color::White;
            line[1].color = Color::White;

            window.draw(line);
        }

        viewer.show(window);
        window.display();
    }
}