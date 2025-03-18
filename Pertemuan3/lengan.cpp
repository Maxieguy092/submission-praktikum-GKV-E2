#include <GL/glut.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h> // For usleep function

// Variables to track time
struct timeval lastTime;
float deltaTime; // Time in seconds since last frame

// FPS counter variables
int frameCount = 0;
float fpsTime = 0;
float fps = 0;
char fpsString[32];

// Target framerate cap
const float targetFPS = 60.0f;
const float targetFrameTime = 1.0f / targetFPS; // Time per frame in seconds


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
        sprintf(fpsString, "FPS: %.1f (Capped at 60)", fps);
    }
}

void capFPS(struct timeval frameStartTime) {
    struct timeval frameEndTime;
    gettimeofday(&frameEndTime, NULL);
    float frameTime = (frameEndTime.tv_sec - frameStartTime.tv_sec) +
                      (frameEndTime.tv_usec - frameStartTime.tv_usec) / 1000000.0f;
    if (frameTime < targetFrameTime) {
        float sleepTime = (targetFrameTime - frameTime) * 1000000;
        usleep((unsigned int)sleepTime);
    }
}

void renderBitmapString(float x, float y, void *font, char *string) {
    char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void renderFPSCounter() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_HELVETICA_12, fpsString);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
}

void setupProjection(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

float camX = 0.0f, camY = 0.0f, camZ = 5.0f;
float yaw = 0.0f, pitch = 0.0f, speed = 0.4f;

void setupCamera(float camX, float camY, float camZ, float yaw, float pitch) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-pitch, 1.0f, 0.0f, 0.0f); 
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f); 

    glTranslatef(-camX, -camY, -camZ);
}

void updateCamera() {
    setupCamera(camX, camY, camZ, yaw, pitch);
}

int shoulderAngle, armAngle = 0, fingerAngle = 0, fingerAngle1 = 0, fingerAngle2 = 0, fingerAngle3 = 0;

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT);
    renderFPSCounter();
    setupCamera(camX, camY, camZ, yaw, pitch);

    glPushMatrix();
        glTranslatef (-1.0, 0.0, 0.0);
        glRotatef ((GLfloat) shoulderAngle, 0.0, 0.0, 1.0); 
        glTranslatef (1.0, 0.0, 0.0); 
        glPushMatrix();
            glScalef (2.0, 0.2, 0.2); 
            glutWireCube (1.0); 
        glPopMatrix();

        glTranslatef (1.0, 0.0, 0.0);
        glRotatef ((GLfloat) armAngle, 0.2, 0.0, 1.0); 
        glTranslatef (1.0, 0.0, 0.0); 

        glPushMatrix();
            glScalef (2.0, 0.2, 0.2); 
            glutWireCube (1.0); 
        glPopMatrix(); 

        glTranslatef (1.0, 0.0, 0.0);
        
        
        glPushMatrix();
            glRotatef ((GLfloat) fingerAngle, 0.0, 0.0, -1.0); 
            glScalef (0.2, 0.05, 0.05); 
            glTranslatef (0.5, 0.0, 1.5); 
            glutWireCube (1.0); 
        glPopMatrix(); 
        
        glTranslatef (0.0, 0.0, 0.0);
        
        glPushMatrix();
            glRotatef ((GLfloat) fingerAngle1, 0.0, 0.0, -1.0); 
            glScalef (0.2, 0.05, 0.05); 
            glTranslatef (0.5, 0.0, 0.0); 
            glutWireCube (1.0); 
        glPopMatrix(); 

        glTranslatef (0.0, 0.0, 0.0);
        
        glPushMatrix();
            glRotatef ((GLfloat) fingerAngle2, 0.0, 0.0, -1.0);     
            glScalef (0.2, 0.05, 0.05); 
            glTranslatef (0.5, 0.0, -1.5); 
            glutWireCube (1.0); 
        glPopMatrix(); 

        glTranslatef (0.0, 0.0, 0.0);
        
        glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0); 
        
        glPushMatrix();
            glRotatef ((GLfloat) fingerAngle3, 0.0, 1.0, 0.0); 
            glScalef (0.2, 0.05, 0.05); 
            glTranslatef (-1.0, 0.0, -1.5); 
            glutWireCube (1.0); 
        glPopMatrix(); 

    glPopMatrix(); 
    glutSwapBuffers();
    }

void reshape (int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    setupProjection(w,h);
}

void Idle(void) {
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
        case 'j': shoulderAngle = (shoulderAngle-5) % 360; break;
        case 'l': shoulderAngle = (shoulderAngle+5) % 360; break;
        case 'i': armAngle = (armAngle-5) % 360; break;
        case 'k': armAngle = (armAngle+5) % 360; break;
        
        case '1': fingerAngle = (fingerAngle-5) % 360; break;
        case '2': fingerAngle = (fingerAngle+5) % 360; break;
        case '!': fingerAngle = (fingerAngle-5) % 360; break;
        case '@': fingerAngle = (fingerAngle+5) % 360; break;

        case '3': fingerAngle1 = (fingerAngle1-5) % 360; break;
        case '4': fingerAngle1 = (fingerAngle1+5) % 360; break;
        case '#': fingerAngle1 = (fingerAngle1-5) % 360; break;
        case '$': fingerAngle1 = (fingerAngle1+5) % 360; break;

        case '5': fingerAngle2 = (fingerAngle2-5) % 360; break;
        case '6': fingerAngle2 = (fingerAngle2+5) % 360; break;
        case '%': fingerAngle2 = (fingerAngle2-5) % 360; break;
        case '^': fingerAngle2 = (fingerAngle2+5) % 360; break;
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube example with FPS counter (capped at 60 FPS)");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    gettimeofday(&lastTime, NULL);
    strcpy(fpsString, "FPS: 0.0 (Capped at 60)");
    
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}
