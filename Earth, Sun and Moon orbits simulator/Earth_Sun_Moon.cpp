#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

// Constants for orbit speeds (relative)
float sunRotation = 0.0f;
float earthOrbit = 0.0f;
float earthRotation = 0.0f;
float moonOrbit = 0.0f;

void drawSphere(float radius, int segments, float r, float g, float b) {
    glColor3f(r, g, b);
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, radius, segments, segments);
    gluDeleteQuadric(quad);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera position
    gluLookAt(0.0, 20.0, 50.0,  // Camera is high up looking down
              0.0, 0.0, 0.0,   // Looking at the Sun
              0.0, 1.0, 0.0);  // Up vector

    // 1. THE SUN
    glPushMatrix();
        glRotatef(sunRotation, 0, 1, 0); // Sun spinning on axis
        drawSphere(3.0f, 32, 1.0f, 0.8f, 0.0f); // Yellow Sun
    glPopMatrix();

    // 2. THE EARTH
    glPushMatrix();
        glRotatef(earthOrbit, 0, 1, 0);    // Orbit around Sun
        glTranslatef(15.0f, 0.0f, 0.0f);   // Distance from Sun
        
        // Save Earth's position for the Moon
        glPushMatrix(); 
            glRotatef(earthRotation, 0, 1, 0); // Earth spinning on axis
            drawSphere(1.2f, 24, 0.2f, 0.2f, 1.0f); // Blue Earth
        glPopMatrix();

        // 3. THE MOON (Inside Earth's Matrix)
        glPushMatrix();
            glRotatef(moonOrbit, 0, 1, 0);  // Orbit around Earth
            glTranslatef(4.0f, 0.0f, 0.0f); // Distance from Earth
            drawSphere(0.4f, 16, 0.7f, 0.7f, 0.7f); // Grey Moon
        glPopMatrix();

    glPopMatrix(); // Exit Earth's coordinate system

    // Update angles for next frame
    sunRotation += 0.1f;
    earthOrbit += 0.5f;    // Earth moves
    earthRotation += 2.0f; // Earth spins faster than it orbits
    moonOrbit += 1.5f;     // Moon orbits Earth faster than Earth orbits Sun
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Solar System: Moon Movement", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST); // Ensure planets don't overlap weirdly

    // Set up Projection
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1024.0/768.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(window)) {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}