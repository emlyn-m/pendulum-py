#include <GLFW/glfw3.h>

#include <math.h>
#include <cstdint>


constexpr uint32_t RGBA_RED   = 0xff000000;
constexpr uint32_t RGBA_GREEN = 0x00ff0000;
constexpr uint32_t RGBA_BLUE  = 0x0000ff00;
constexpr uint32_t RGBA_ALPHA = 0x000000ff;

void drawCircle(float cx, float cy, float r, uint32_t rgba) {
    int num_segments = 100;
    
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(((rgba & RGBA_RED)>>24)/255.0, ((rgba & RGBA_GREEN)>>16)/255.0, ((rgba & RGBA_BLUE)>>8)/255.0, (rgba & RGBA_ALPHA)/255.0);
    for (int ii=0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        
        float x = r * std::cos(theta);
        float y = r * std::sin(theta);

        glVertex2f(x+cx, y+cy);
    }

    glEnd();

}

void drawLine(float rx, float ry, float dx, float dy) {
    // todo: include width
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(rx, ry);
    glVertex2f(dx, dy);
    glEnd();
}