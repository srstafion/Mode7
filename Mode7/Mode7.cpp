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
    float angle;
public:
    Viewer() {
        viewer.setPosition(154, 140);
        viewer.setSize(Vector2f(10, 10));
        viewer.setFillColor(Color::Green);
        viewer.setOrigin(viewer.getSize() / 2.0f);
        angle = 0;
    }
    void moveForward(float distance) {
        float angleRad = angle * PI / 180.0f;
        float deltaX = cos(angleRad) * distance;
        float deltaY = sin(angleRad) * distance;
        viewer.move(deltaX, deltaY);
    }
    void moveBack(float distance) {
        moveForward(-distance);
    }
    void rotate(float degrees) {
        angle -= degrees;
        viewer.setRotation(angle);
    }
    Vector2f getViewPosition() {
        return viewer.getPosition();
    }

    float getAngle() {
        return angle;
    }
    void show(RenderWindow& window) {
        window.draw(viewer);
    }
};

// Function to handle SFML events
void handleEvents(Viewer& viewer, RenderWindow& window) {
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
        }
    }
}

// Function to draw the scene
void drawScene(Viewer& viewer, RenderWindow& window, Sprite& floorMap) {
    window.clear();
    window.draw(floorMap);
    viewer.show(window);
}

void drawLinesAndSquares(Viewer& viewer, RenderWindow& window) {
    Vector2f viewerPos = viewer.getViewPosition();
    float angle = viewer.getAngle();

    for (int i = -1; i <= 1; i += 2) {
        float newAngle = angle + i * 60.0f;

        float dx = cos(newAngle * PI / 180.0f);
        float dy = sin(newAngle * PI / 180.0f);

        float endpointX, endpointY;
        if (abs(dx) > abs(dy)) {
            endpointX = (dx > 0) ? WINDOW_SIZE : 0;
            endpointY = viewerPos.y + (endpointX - viewerPos.x) * dy / dx;
            if (endpointY < 0 || endpointY > WINDOW_SIZE) {
                endpointY = min(max(endpointY, 0.0f), static_cast<float>(WINDOW_SIZE));
                endpointX = viewerPos.x + (endpointY - viewerPos.y) * dx / dy;
            }
        }
        else {
            endpointY = (dy > 0) ? WINDOW_SIZE : 0;
            endpointX = viewerPos.x + (endpointY - viewerPos.y) * dx / dy;
            if (endpointX < 0 || endpointX > WINDOW_SIZE) {
                endpointX = min(max(endpointX, 0.0f), static_cast<float>(WINDOW_SIZE));
                endpointY = viewerPos.y + (endpointX - viewerPos.x) * dy / dx;
            }
        }

        // Draw the ray
        VertexArray line(Lines, 2);
        line[0].position = viewerPos;
        line[1].position = Vector2f(endpointX, endpointY);
        line[0].color = Color::White;
        line[1].color = Color::White;
        window.draw(line);

        // Draw a square at the endpoint
        RectangleShape square;
        square.setSize(Vector2f(10, 10));
        square.setFillColor(Color::Red);
        square.setOrigin(square.getSize() / 2.0f);
        square.setPosition(endpointX, endpointY);
        window.draw(square);

        Vector2f startPoint2 = viewerPos + Vector2f(dx * 40, dy * 40);

        // Draw startpoints
        RectangleShape startPointMarker;
        startPointMarker.setSize(Vector2f(10, 10));
        startPointMarker.setFillColor(Color::Blue);
        startPointMarker.setOrigin(startPointMarker.getSize() / 2.0f);

        startPointMarker.setPosition(startPoint2);
        window.draw(startPointMarker);
    }
}

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
        handleEvents(viewer, window);
        drawScene(viewer, window, floorMap);
        drawLinesAndSquares(viewer, window);
        window.display();
    }
}
