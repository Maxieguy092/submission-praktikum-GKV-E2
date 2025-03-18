#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>

struct timeval lastTime;
float deltaTime;
int frameCount = 0;
float fpsTime = 0, fps = 0;
char fpsString[32];
const float targetFPS = 60.0f, targetFrameTime = 1.0f / targetFPS;

void calculateDeltaTime() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    deltaTime = (currentTime.tv_sec - lastTime.tv_sec) +
                (currentTime.tv_usec - lastTime.tv_usec) / 1000000.0f;
    lastTime = currentTime;
}

void updateFPSCounter() {
    frameCount++;
    fpsTime += deltaTime;
    if (fpsTime >= 1.0) {
        fps = frameCount / fpsTime;
        frameCount = 0;
        fpsTime = 0;
        sprintf(fpsString, "FPS: %.1f", fps);
    }
}

void capFPS(struct timeval frameStartTime) {
    struct timeval frameEndTime;
    gettimeofday(&frameEndTime, NULL);
    float frameTime = (frameEndTime.tv_sec - frameStartTime.tv_sec) +
                      (frameEndTime.tv_usec - frameStartTime.tv_usec) / 1000000.0f;
    if (frameTime < targetFrameTime) {
        usleep((unsigned int)((targetFrameTime - frameTime) * 1000000));
    }
}

void setupProjection(int width, int height) {
    if (height == 0) height = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

float camX = 0.0f, camY = 0.0f, camZ = 5.0f;
float yaw = 0.0f, pitch = 0.0f, speed = 0.4f;

void setupCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camX, -camY, -camZ);
}

// IGNORE ABOVE FUNCTIONS

const int SEGMENTS = 36;
const float RADIUS = 0.5f;
const float HEIGHT = 0.5f;

void drawFilledTube(int SEGMENTS, float RADIUS, float HEIGHT) {
    std::vector<float> sideVertices;
    std::vector<float> topCapVertices;
    std::vector<float> bottomCapVertices;
    std::vector<float> sideNormals;
    
    topCapVertices.push_back(0.0f);
    topCapVertices.push_back(HEIGHT / 2);
    topCapVertices.push_back(0.0f);

    bottomCapVertices.push_back(0.0f);
    bottomCapVertices.push_back(-HEIGHT / 2);
    bottomCapVertices.push_back(0.0f);

    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = (2.0f * M_PI * i) / SEGMENTS;
        float x = cos(angle);
        float z = sin(angle);

        sideVertices.push_back(x * RADIUS);
        sideVertices.push_back(-HEIGHT / 2);
        sideVertices.push_back(z * RADIUS);

        sideVertices.push_back(x * RADIUS);
        sideVertices.push_back(HEIGHT / 2);
        sideVertices.push_back(z * RADIUS);

        sideNormals.push_back(x);
        sideNormals.push_back(0);
        sideNormals.push_back(z);

        sideNormals.push_back(x);
        sideNormals.push_back(0);
        sideNormals.push_back(z);

        topCapVertices.push_back(x * RADIUS);
        topCapVertices.push_back(HEIGHT / 2);
        topCapVertices.push_back(z * RADIUS);

        bottomCapVertices.push_back(x * RADIUS);
        bottomCapVertices.push_back(-HEIGHT / 2);
        bottomCapVertices.push_back(z * RADIUS);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glNormalPointer(GL_FLOAT, 0, sideNormals.data());
    glVertexPointer(3, GL_FLOAT, 0, sideVertices.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sideVertices.size() / 3);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertexPointer(3, GL_FLOAT, 0, topCapVertices.data());
    glDrawArrays(GL_TRIANGLE_FAN, 0, topCapVertices.size() / 3);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertexPointer(3, GL_FLOAT, 0, bottomCapVertices.data());
    glDrawArrays(GL_TRIANGLE_FAN, 0, bottomCapVertices.size() / 3);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void drawCar() {
    glPushMatrix();
        glPushMatrix();
            glScalef(2.0, 0.5, 1.0);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glScalef(1.0, 0.5, 1.0);
            glTranslatef(0.0, 1.0, 0.0);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glScalef(0.5,0.5,0.5);
            glRotatef(90,1.0,0.0,0.0); 
            glTranslatef(-1.0,0.75,1.0);  
            drawFilledTube(SEGMENTS, RADIUS, HEIGHT);
        glPopMatrix();

        glPushMatrix();
            glScalef(0.5,0.5,0.5);
            glRotatef(90,1.0,0.0,0.0); 
            glTranslatef(1.0,0.75,1.0);  
            drawFilledTube(SEGMENTS, RADIUS, HEIGHT);
        glPopMatrix();

        glPushMatrix();
            glScalef(0.5,0.5,0.5);
            glRotatef(90,1.0,0.0,0.0); 
            glTranslatef(-1.0,-0.75,1.0);  
            drawFilledTube(SEGMENTS, RADIUS, HEIGHT);
        glPopMatrix();

        glPushMatrix();
            glScalef(0.5,0.5,0.5);
            glRotatef(90,1.0,0.0,0.0); 
            glTranslatef(1.0,-0.75,1.0);  
            drawFilledTube(SEGMENTS, RADIUS, HEIGHT);
        glPopMatrix();

    glPopMatrix();
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat high_shininess[] = { 100.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glShadeModel(GL_SMOOTH);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCamera();
    drawCar();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    setupProjection(w, h);
}

void Idle() {
    struct timeval frameStartTime;
    gettimeofday(&frameStartTime, NULL);
    calculateDeltaTime();
    updateFPSCounter();
    display();
    capFPS(frameStartTime);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': camZ -= speed; break;
        case 's': camZ += speed; break;
        case 'a': camX -= speed; break;
        case 'd': camX += speed; break;
        case 'q': camY += speed; break;
        case 'e': camY -= speed; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: yaw -= 2.0f; break;
        case GLUT_KEY_RIGHT: yaw += 2.0f; break;
        case GLUT_KEY_UP: pitch += 2.0f; break;
        case GLUT_KEY_DOWN: pitch -= 2.0f; break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solid Cube Car with Lighting");

    glEnable(GL_DEPTH_TEST);
    setupLighting(); 

    gettimeofday(&lastTime, NULL);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}
