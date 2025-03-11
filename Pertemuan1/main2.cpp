#include <GL/glut.h>

void drawLineStrip() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.8, 0.8);
        glVertex2f(-0.6, 0.6);
        glVertex2f(-0.4, 0.8);
        glVertex2f(-0.2, 0.6);
    glEnd();
}

void drawLineLoop() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.8, 0.4);
        glVertex2f(-0.6, 0.2);
        glVertex2f(-0.4, 0.4);
        glVertex2f(-0.6, 0.6);
    glEnd();
}

void drawTriangleFan() {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.2, 0.8);
        glVertex2f(0.4, 0.6);
        glVertex2f(0.6, 0.8);
        glVertex2f(0.8, 0.6);
        glVertex2f(0.6, 0.4);
        glVertex2f(0.4, 0.6);
    glEnd();
}

void drawTriangleStrip() {
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(0.2, 0.2);
        glVertex2f(0.4, 0.4);
        glVertex2f(0.6, 0.2);
        glVertex2f(0.8, 0.4);
    glEnd();
}

void drawQuads() {
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(-0.8, -0.2);
        glVertex2f(-0.6, -0.2);
        glVertex2f(-0.6, -0.4);
        glVertex2f(-0.8, -0.4);
    glEnd();
}

void drawQuadStrip() {
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUAD_STRIP);
        glVertex2f(-0.2, -0.2);
        glVertex2f(0.0, -0.2);
        glVertex2f(-0.2, -0.4);
        glVertex2f(0.0, -0.4);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawLineStrip();
    drawLineLoop();
    drawTriangleFan();
    drawTriangleStrip();
    drawQuads();
    drawQuadStrip();
    
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Primitives with GLUT");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}