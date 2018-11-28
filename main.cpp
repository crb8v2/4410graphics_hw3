
// ##### Connor Baniak #####

#include <GL/glut.h>
#include <stdlib.h> // standard definitions
#include <math.h> // math definitions
#include <stdio.h> // standard I/O

#include "RGBpixmap.h"

// escape key (for exit)
#define ESC 27

// Camera position
float x = 0.0, y = -5.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction


void changeSize(int w, int h) {
    float ratio =  ((float) w) / ((float) h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

void drawSnowman() {
    // Draw body (a 20x20 spherical mesh of radius 0.75 at height 0.75)
    glColor3f(1.0, 1.0, 1.0); // set drawing color to white
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.75);
    glutSolidSphere(0.75, 20, 20);
    glPopMatrix();

    // Draw the head (a sphere of radius 0.25 at height 1.75)
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.75); // position head
    glutSolidSphere(0.25, 20, 20); // head sphere

    // Draw Eyes (two small black spheres)
    glColor3f(0.0, 0.0, 0.0); // eyes are black
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.10); // lift eyes to final position
    glPushMatrix();
    glTranslatef(-0.05, 0.0, 0.0);
    glutSolidSphere(0.05, 10, 10); // right eye
    glPopMatrix();
    glPushMatrix();
    glTranslatef(+0.05, 0.0, 0.0);
    glutSolidSphere(0.05, 10, 10); // left eye
    glPopMatrix();
    glPopMatrix();

    // Draw Nose (the nose is an orange cone)
    glColor3f(1.0, 0.5, 0.5); // nose is orange
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to point along -y
    glutSolidCone(0.08, 0.5, 10, 2); // draw cone
    glPopMatrix();
    glPopMatrix();

    // Draw a faux shadow beneath snow man (dark green circle)
    glColor3f(0.0, 0.5, 0.0);
    glPushMatrix();
    glTranslatef(0.2, 0.2, 0.001);	// translate to just above ground
    glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
    glutSolidSphere(0.75, 20, 20); // shadow same size as body
    glPopMatrix();
}

void update(void) {
    if (deltaMove) { // update camera position
        x += deltaMove * lx * 0.1;
        y += deltaMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

void renderScene(void) {
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
            x,      y,      1.0,
            x + lx, y + ly, 1.0,
            0.0,    0.0,    1.0);

    // Draw ground - 200x200 square colored green
    glColor3f(0.0, 0.7, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0, -100.0, 0.0);
    glVertex3f(-100.0,  100.0, 0.0);
    glVertex3f( 100.0,  100.0, 0.0);
    glVertex3f( 100.0, -100.0, 0.0);
    glEnd();

    // Draw 36 snow men
    for(i = -3; i < 3; i++)
        for(j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i*7.5, j*7.5, 0);
            drawSnowman();
            glPopMatrix();
        }

    glutSwapBuffers(); // Make it all visible
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    if (key == ESC || key == 'q' || key == 'Q') exit(0);
}

void pressSpecialKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP : deltaMove = 1.0; break;
        case GLUT_KEY_DOWN : deltaMove = -1.0; break;
        case GLUT_KEY_LEFT:
            angle += 0.1f;
            lx = -sin(angle);
            ly = cos(angle);
            break;

        case GLUT_KEY_RIGHT:
            angle -= 0.1f;
            lx = -sin(angle);
            ly = cos(angle);
            break;
    }
}

void releaseSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP : deltaMove = 0.0; break;
        case GLUT_KEY_DOWN : deltaMove = 0.0; break;
    }
}

int main(int argc, char **argv)
{

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

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}































//
//#include "RGBpixmap.h"
//#include "camera.h"
//
//void myInit();
//void idle();
//void myDisplay();
//void myKeyboard(int, int, int);
//void lightsAndCamera();
//void drawFloor();
//void backWall(double);
//
//
//Camera cam;
//
//float eye_x = 0.0;
//float eye_y = 5.0;
//float eye_z = 0.0;
//float turn = 0.0;
//
//int main(int argc, char **argv) {
//
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);
//    glutInitWindowSize(640,480);
//    glutInitWindowPosition(200, 200);
//    glutCreateWindow("Condog's Maze");
//
//
//
////    glutIdleFunc(idle);   // causes display func to be called over and over
//
//    glutDisplayFunc(myDisplay);
//
//    glutSpecialFunc(myKeyboard);
//
//    myInit();
//
//    glutMainLoop();
//
//}
//
//void myInit(){
//    glClearColor(1.0, 1.0, 1.0, 0.0); //background color white
//    glColor3f(0.0f, 0.0f, 0.0f);
//    glPointSize(4.0);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0.0, 640.0, 0.0,  480.0);
//    glEnable(GL_LIGHTING); // enable the light source
//    glEnable(GL_LIGHT0);
//    glShadeModel(GL_SMOOTH);
//    //glShadeModel(GL_FLAT);
//    glEnable(GL_DEPTH_TEST); // for hidden surface removal
//    glEnable(GL_NORMALIZE); // normalize vectors for proper shading
//    glClearColor(0.1f,0.1f,0.1f,0.0f);  // background is light gray
//    glViewport(0, 0, 640, 480);
//
//    Point3 eye(eye_x, eye_y, eye_z);
//    Point3 look(5.0, 5.0, 0.0);
//    Vector3 up(0.0, 1.0, 0.0);
//
//    cam.set(eye, look, up);
//}
//
//void idle(){
//    glutPostRedisplay();
//}
//
//void myDisplay(){
//
//    //set properties of the surface material
//    GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f}; // gray
//    GLfloat mat_diffuse[] = {1.0f, 1.0f, 0.3f, 1.0f};
//    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
//    GLfloat mat_shininess[] = {50.0f};
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//
//    // lights and camera config
//    lightsAndCamera();
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // draw maze
//    mat_diffuse[0] = 0.5f, mat_diffuse[1] = 0.5f, mat_diffuse[2] = 0.5f, mat_diffuse[3] = 0.0f;
//    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
//    drawFloor();
//
//    glFlush();
//    glutSwapBuffers();
//}
//
//void myKeyboard(int key, int x, int y){
//    switch (key) {
//        case GLUT_KEY_UP:
//            cam.slide(0.0, 0.0, eye_z - 0.5);
//            break;
//        case GLUT_KEY_DOWN:
//            cam.slide(0.0, 0.0, eye_z + 0.5);
//            break;
//        case GLUT_KEY_LEFT:
//            cam.yaw(2.5);
//            break;
//        case GLUT_KEY_RIGHT:
//            cam.yaw(-2.5);
//            break;
//        default:
//            break;
//    }
//    glutPostRedisplay();
//}
//
//void lightsAndCamera(){
//    // set the light source properties
//    GLfloat lightIntensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
//    GLfloat light_position[] = {0.5f, 0.7f, 3.0f, 1.0f};
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
//
//
//    // set the camera
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(70.0, 640.0 / 480, 1.0, 100.0);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0, 5, 0);
//
//}
//
//void wall(double thickness) { // draw thin wall with top = xz-plane, corner at origin
//    glPushMatrix();
////    glTranslated(00, 0.5 * thickness,  0.5);
//    glScaled(2.0, thickness, 10.0);
//    glutSolidCube(1.0);
//    glPopMatrix();
//}
//
//void drawFloor(){
//
//    glPushMatrix();
//    glRotated(-90.0,0.5, 0.0, 0.0);
//    backWall(0.02);                  //backwall
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslated(0.0, -1.0, 0.0);
//    wall(0.02);                     // bottom
//    glPopMatrix();
//}
//
//void backWall(double thickness) { // draw thin wall with top = xz-plane, corner at origin
//    glPushMatrix();
////    glTranslated(00, 0.5 * thickness,  0.5);
//    glScaled(2.0, thickness, 2.0);
//    glutSolidCube(1.0);
//    glPopMatrix();
//}
//

//----------------------------------------------------------------------
//	Best if viewed with tab stops set every 2 columns.
//----------------------------------------------------------------------
//	File: opengl-3D-sample.cpp - Sample 3D OpenGL/GLUT program
//	Origin: Lighthouse3D (heavily modified by Dave Mount)
//
//	This is a sample program that illustrates OpenGL and GLUT. It
//	renders a picture of 36 snowmen. The camera can be moved by dragging
//	the mouse. The camera moves forward by hitting the up-arrow key and
//	back by moving the down-arrow key. Hit ESC, 'q' or 'Q' to exit.
//
//	Warning #1: This program uses the function glutSpecialUpFunc, which
//	may not be available in all GLUT implementations. If your system
//	does not have it, you can comment this line out, but the up arrow
//	processing will not be correct.
//
//	Warning #2: This is a minimalist program. Very little attention has
//	been paid to good programming technique.
//----------------------------------------------------------------------
