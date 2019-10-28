#include <GLUT/glut.h>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include<iostream>
#include<fstream>
#define PI 3.141592653

using namespace std;

struct Point
{
    float x, y,z;
};
struct Sight {
    float x_eye, y_eye, z_eye, x_centre, y_centre, z_centre, up_x, up_y, up_z;
};

struct Perspective {
    float angle, aspect, z_near, z_far;
};

struct Ortho {
    float left, right, bottom, top, near, far;
};

Ortho ortho = {
        -100, 100, -100, 100,-100,100
};

Perspective perspective = {
        65, 2, -10, 100
};

Sight sight = {
        0,0,-100,0,0,-1,0,1,0
};

vector<Point> v;
vector<vector<int> > faces;
int point_count, face_count;

int light_sample = 1;
int width = 800;
int height = 600;
bool perspectiveProjection = false;
bool orthoProjection = true;

int red = rand()%255 + 1;
int green = rand()%255 + 1;
int blue = rand()%255 + 1;
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void drawScene();

void init(void) {
    glClearColor(0.3, 0.3, 0.3, 0.0);

    glEnable(GL_LIGHTING);

    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_NORMALIZE);
}

void reshape(int width, int height)
{
}

void readFromFile()
{
    float x, y,z;
    Point p;
    ifstream f("object_data", ios::in);
    f >> point_count >> face_count;
    for (int i = 0; i < point_count; i++)
    {
        f >> p.x >> p.y >> p.z;
        v.push_back(p);
    }
    faces.resize(face_count);
    for (int i = 0; i < face_count; i++)
    {
        int n;
        f >> n;
        for (int j = 0; j < n; j++)
        {
            int p;
            f >> p;
            faces[i].push_back(p);
        }
    }
    f.close();
}

void display(void)
{
    glViewport(0, 0, 2*width, 2*height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

    switch (light_sample)
    {
        case 1:
        {
            GLfloat light0_diffuse[] = { 0.4, 0.7, 0.2 };
            GLfloat light0_direction[] = { 0.0, 0.0, 100.0, 0.0 };
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
            glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
            break; }
        case 2:
        {
            GLfloat light1_diffuse[] = { 0.4, 0.7, 0.2 };
            GLfloat light1_position[] = { 0.0, 0.0, 1.0, 1.0 };
            glEnable(GL_LIGHT1);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            break; }
        case 3:
        {
            GLfloat light2_diffuse[] = { 0.4, 0.7, 0.2 };
            GLfloat light2_position[] = { 0.0, 0.0, 1.0, 1.0 };
            glEnable(GL_LIGHT2);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
            glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
            glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
            glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.2);
            glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.4);
            break; }
        case 4:
        {
            GLfloat light3_diffuse[] = { 0.4, 0.7, 0.2 };
            GLfloat light3_position[] = { 0.0, 0.0, 1.0, 1.0 };
            GLfloat light3_spot_direction[] = { 0.0, 0.0, -1.0 };
            glEnable(GL_LIGHT3);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
            glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30);
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_spot_direction);
            break;
        }
        case 5:
        {
            GLfloat light4_diffuse[] = { 1.0, 1.0, 1.0 }; // light color
            GLfloat light4_position[] = { 0.0, 0.0, -100.0, 1.0 }; // light position
            GLfloat light4_spot_direction[] = { 0.0, 1.0, -100.0 };
            glEnable(GL_LIGHT4);
            glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
            glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
            glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30);
            glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light4_spot_direction);
            glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0);
            break;
        }
        case 6:
            {
                GLfloat light5_diffuse[] = { 1.0, 0.0, 0.0 };
                GLfloat light5_position[] = { GLfloat(0.5 * cos(0.0)), GLfloat (0.5 * sin(0.0)), 1.0, 1.0 };
                glEnable(GL_LIGHT5);
                glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
                glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
                glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.0);
                glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.4);
                glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.8);
                GLfloat light6_diffuse[] = { 0.0, 1.0, 0.0 };
                GLfloat light6_position[] = { GLfloat(0.5 * cos(2 * PI / 3)), GLfloat(0.5 * sin(2 * PI / 3)),
                                              1.0, 1.0 };
                glEnable(GL_LIGHT6);
                glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
                glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
                glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.0);
                glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.4);
                glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.8);
                GLfloat light7_diffuse[] = { 0.0, 0.0, 1.0 };
                GLfloat light7_position[] = { GLfloat(0.5 * cos(4 * PI / 3)), GLfloat(0.5 * sin(4 * PI / 3)),
                                              1.0, 1.0 };
                glEnable(GL_LIGHT7);
                glLightfv(GL_LIGHT7, GL_DIFFUSE, light7_diffuse);
                glLightfv(GL_LIGHT7, GL_POSITION, light7_position);
                glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 0.0);
                glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.4);
                glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.8);
                break;
            }
        }
        GLfloat x, y;
        drawScene();

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
        glutSwapBuffers();
}

void keyboard_function(unsigned char key, int x, int y) {
    switch (key)
    {
        case '1':light_sample = 1;break;
        case '2':light_sample = 2; break;
        case '3':light_sample = 3; break;
        case '4':light_sample = 4; break;
        case '5':light_sample = 5; break;
        case '6':light_sample = 6; break;
        case 27:
            exit(0);
    }
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    srand(time(NULL));
    readFromFile();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Пример установки источников света в OpenGL");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();
}

void drawScene(){

    for (int i = 0; i < face_count; i++)
    {
        glColor3f(red/255.0, green/255.0, blue/255.0);
        glBegin(GL_POLYGON);
        for (int j = 0; j < faces[i].size(); j++)
        {
            glVertex3f(v[faces[i][j]].x, v[faces[i][j]].y, v[faces[i][j]].z);
        }


        glEnd();
    }
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            glMatrixMode(GL_MODELVIEW);
            glTranslated(0, 1, 0);
            display();
            break;
        case GLUT_KEY_DOWN:
            glMatrixMode(GL_MODELVIEW);
            glTranslated(0, -1, 0);
            display();
            break;
        case GLUT_KEY_LEFT:
            glMatrixMode(GL_MODELVIEW);
            glTranslated(-1, 0, 0);
            display();
            break;
        case GLUT_KEY_RIGHT:
            glMatrixMode(GL_MODELVIEW);
            glTranslated(1, 0, 0);
            display();
            break;
    }
}

void processNormalKeys(unsigned char key, int x, int y) {
    //printf("Key %d\n", key);
    switch (key) {
        case '1':{
            light_sample = 1;
            glutPostRedisplay();
            break;
        }
        case '2':{
            light_sample = 2;
            glutPostRedisplay();
            break;
        }
        case '3':{
            light_sample = 3;
            glutPostRedisplay();
            break;
        }
        case '4':{
            light_sample = 4;
            glutPostRedisplay();
            break;
        }
        case '5':{
            light_sample = 5;
            glutPostRedisplay();
            break;
        }
        case '6':{
            light_sample = 6;
            glutPostRedisplay();
            break;
        }
        case 27:
            exit(0);
        case 61:
            perspective.z_near += 1;
            perspective.z_far -= 1;
            glMatrixMode(GL_MODELVIEW);
            glScaled(1.1,1.1,1.1);
            glutPostRedisplay();
            break;
        case 45:
            perspective.z_near -= 1;
            perspective.z_far += 1;
            glMatrixMode(GL_MODELVIEW);
            glScaled(0.9,0.9,0.9);
            glutPostRedisplay();
            break;
        case 120:
            glMatrixMode(GL_MODELVIEW);
            glRotatef(6,1,0,0);
            glutPostRedisplay();
            break;
        case 121:
            glMatrixMode(GL_MODELVIEW);
            glRotatef(6,0,1,0);
            glutPostRedisplay();
            break;
        case 122:
            glMatrixMode(GL_MODELVIEW);
            glRotatef(6,0,0,1);
            glutPostRedisplay();
            break;
        case 119: // w
            sight.y_eye += 2;
            glutPostRedisplay();
            break;
        case 97: // a
            sight.x_eye -= 2;
            glutPostRedisplay();
            break;
        case 115: // s
            sight.y_eye -= 2;
            glutPostRedisplay();
            break;
        case 100: // d
            sight.x_eye += 2;
            glutPostRedisplay();
            break;
        case 87: // W
            sight.y_centre += 2;
            glutPostRedisplay();
            break;
        case 65: // A
            sight.x_centre -= 2;
            glutPostRedisplay();
            break;
        case 83: // S
            sight.y_centre -= 2;
            glutPostRedisplay();
            break;
        case 68: // D
            sight.x_centre += 2;
            glutPostRedisplay();
            break;
        case 23: // Ctrl - W
            sight.up_y += 2;
            glutPostRedisplay();
            break;
        case 1: // Ctrl - A
            sight.up_x -= 2;
            glutPostRedisplay();
            break;
        case 19: // Ctrl - S
            sight.up_y -= 2;
            glutPostRedisplay();
            break;
        case 4: // Ctrl - D
            sight.up_x += 2;
            glutPostRedisplay();
            break;
        case 113: // q
            sight.z_eye += 2;
            glutPostRedisplay();
            break;
        case 101: // e
            sight.z_eye -= 2;
            glutPostRedisplay();
            break;
        case 81: // Q
            sight.z_centre += 2;
            glutPostRedisplay();
            break;
        case 69: // E
            sight.z_centre -= 2;
            glutPostRedisplay();
            break;
        case 17: // Ctrl - Q
            sight.up_z += 2;
            glutPostRedisplay();
            break;
        case 5: // Ctrl - E
            sight.up_z -= 2;
            glutPostRedisplay();
            break;

        case 114: // r
            perspective = {
                    60, 1, -20, 20
            };

            sight = {
                    0,0,-30,0,0,0,0,1,0
            };

            ortho = {
                    -100, 100, -100, 100,-100,100
            };
            red = rand()%255 + 1;
            green = rand()%255 + 1;
            blue = rand()%255 + 1;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glutPostRedisplay();
            break;
        case 44: // <
            perspective.angle -= 2;
            glutPostRedisplay();
            break;
        case 46: // >
            perspective.angle += 2;
            glutPostRedisplay();
            break;
        case 111: // o
            orthoProjection = true;
            perspectiveProjection = false;
            glutPostRedisplay();
            break;
        case 112: // p
            orthoProjection = false;
            perspectiveProjection = true;
            glutPostRedisplay();
            break;
        case 88: // X
            glMatrixMode(GL_MODELVIEW);
            glRotatef(-6,1,0,0);
            glutPostRedisplay();
            break;
        case 89: // Y
            glMatrixMode(GL_MODELVIEW);
            glRotatef(-6,0,1,0);
            glutPostRedisplay();
            break;
        case 90: // Z
            glMatrixMode(GL_MODELVIEW);
            glRotatef(-6,0,0,1);
            glutPostRedisplay();
            break;
        case 107: // k
            glMatrixMode(GL_MODELVIEW);
            glRotatef(6,1,1,1);
            glutPostRedisplay();
            break;
        case 106: // j
            glMatrixMode(GL_MODELVIEW);
            glRotatef(-6,1,1,1);
            glutPostRedisplay();
            break;
    }


}
