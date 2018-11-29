
// ##### Connor Baniak #####

#include <GL/glut.h>
#include <stdlib.h> // standard definitions
#include <math.h> // math definitions
#include <stdio.h> // standard I/O

#include "RGBpixmap.h"

// escape key (for exit)
#define ESC 27

using namespace std;

// Camera position
float x = 0.0, z = 0.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
// Camera direction
float lx = 0.0, lz = 1.0; // camera points initially along z-axis
float angle = 0.0; // angle of rotation for the camera direction

RGBpixmap floors;
RGBpixmap wall;
RGBpixmap ceiling;

int mazeMap[12][12] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void changeSize(int w, int h) {
    float ratio = ((float) w) / ((float) h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

void update(void) {
    if (deltaMove) { // update camera position
        x += deltaMove * lx * 0.1;
        z += deltaMove * lz * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

void wallCube(float x, float y, float z, int texture) {

    // top
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x, y + 1.0f, z + 1.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z);
    glTexCoord2f(1.0, 0.0); glVertex3f(x, y + 1.0f, z);
    glEnd();

    // bottom
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z + 1.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(x + 1.0f, y, z + 1.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(x + 1.0f, y, z);
    glTexCoord2f(1.0, 0.0); glVertex3f(x, y, z);
    glEnd();

    // north
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x + 1.0f, y, z + 1.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(x, y + 1.0f, z + 1.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(x, y, z + 1.0f);
    glEnd();

    // east
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z);
    glTexCoord2f(1.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(x + 1.0f, y, z + 1.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x + 1.0f, y, z);
    glEnd();

    // south
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z);
    glTexCoord2f(0.0, 1.0); glVertex3f(x, y + 1.0f, z);
    glTexCoord2f(1.0, 1.0); glVertex3f(x + 1.0f, y + 1.0f, z);
    glTexCoord2f(1.0, 0.0); glVertex3f(x + 1.0f, y, z);
    glEnd();

    // west
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z);
    glTexCoord2f(0.0, 1.0); glVertex3f(x, y + 1.0f, z);
    glTexCoord2f(1.0, 1.0);glVertex3f(x, y + 1.0f, z + 1.0f);
    glTexCoord2f(1.0, 0.0);glVertex3f(x, y, z + 1.0f);
    glEnd();
}

void drawFloorCubes(){
    int xx,zz;

    for(xx = 0; xx < 12; xx++) {
        for (zz = 0; zz < 12; zz++) {
            int holdxx, holdzz;
            holdxx = xx;
            holdzz = zz;
            holdxx -= 6;
            holdzz -= 6;
            wallCube(holdxx, -1.0f, holdzz, 1);
        }
    }

}

void drawRoofCubes(){
    int xx,zz;

    for(xx = 0; xx < 12; xx++) {
        for (zz = 0; zz < 12; zz++) {
            int holdxx, holdzz;
            holdxx = xx;
            holdzz = zz;
            holdxx -= 6;
            holdzz -= 6;
            wallCube(holdxx, 1.0f, holdzz, 3);
        }
    }

}

void drawMazeCubes(){
    int xx,zz;

    for(xx = 0; xx < 12; xx++){
        for(zz = 0; zz < 12; zz++){
            if(mazeMap[xx][zz] == 1){
                int holdxx, holdzz;
                holdxx = xx;
                holdzz = zz;
                holdxx -= 6;
                holdzz -= 6;
                wallCube(holdxx, 0, holdzz, 2);
            }
        }
    }
}

void renderScene(void) {
    int xx, zz;

    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
            x, 0.5, z,
            x + lx, 0.5, z + lz,
            0.0, 0.5, 0.0);

    drawFloorCubes();

    drawRoofCubes();

    drawMazeCubes();

    glutSwapBuffers(); // Make it all visible
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    if (key == ESC || key == 'q' || key == 'Q') exit(0);
}

void pressSpecialKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP :
            deltaMove = 0.02f;
            break;
        case GLUT_KEY_DOWN :
            deltaMove = -0.02f;
            break;
        case GLUT_KEY_LEFT:
            angle -= 0.2f;
            lx = sin(angle);
            lz = -cos(angle);
            break;

        case GLUT_KEY_RIGHT:
            angle += 0.2f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
    }
}

void releaseSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP :
            deltaMove = 0.0;
            break;
        case GLUT_KEY_DOWN :
            deltaMove = 0.0;
            break;
    }
}

void myInit(){
    //bring in textures
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    floors.readBMPFile("grass.bmp");
    floors.setTexture(1);
    wall.readBMPFile("wall.bmp");
    wall.setTexture(2);
    ceiling.readBMPFile("roofy.bmp");
    ceiling.setTexture(3);
}

int main(int argc, char **argv) {

    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 400);
    glutCreateWindow("Connor's Maze");

    // register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
    glutKeyboardFunc(processNormalKeys); // process standard key clicks
    glutSpecialFunc(pressSpecialKey); // process special key pressed
    // Warning: Nonstandard function! Delete if desired.
    glutSpecialUpFunc(releaseSpecialKey); // process special key release

    myInit();

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}