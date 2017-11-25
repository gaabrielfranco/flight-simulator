#include <Model/OBJ.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <math.h>

using namespace std;

struct Flying_machines
{
    Model model;
    bool moving = false;
    bool inside = false;
    float angle = 270.0;
    float position[3] = {0.0, 2.0, 0.0};
    float facing[3] = {1.0, 0.0, 0.0};
};

Flying_machines airplane, air_balloon, helicopter;
Model tower, building, road, farmhouse, mountain;
int width = 500, height = 500, fm_selected = 1;

GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};

GLfloat sLightPos1[] = {0.0, 100.0, 0.0, 0.0};

GLfloat sLightPos2Airplane[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2Airplane[] = {0.0, -1.0, 0.0};
GLfloat sLightPos2Ballon[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2Ballon[] = {0.0, -1.0, 0.0};
GLfloat sLightPos2Helicopter[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2Helicopter[] = {0.0, -1.0, 0.0};

bool active_light = false, light0 = false, light1 = false;

void rotate_y(float angle)
{
    angle *= 0.0174533;
    switch (fm_selected)
    {
        float x, z;
    case 1:
        x = airplane.facing[0];
        z = airplane.facing[2];
        airplane.facing[0] = x * cos(angle) - z * sin(angle);
        airplane.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    case 2:
        x = air_balloon.facing[0];
        z = air_balloon.facing[2];
        air_balloon.facing[0] = x * cos(angle) - z * sin(angle);
        air_balloon.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    case 3:
        x = helicopter.facing[0];
        z = helicopter.facing[2];
        helicopter.facing[0] = x * cos(angle) - z * sin(angle);
        helicopter.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    }
}

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -1.225, 1.225, 2.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

    //Movimentação
    if (airplane.moving || air_balloon.moving || helicopter.moving)
    {
        switch (fm_selected)
        {
        case 1:
            airplane.position[0] -= 5 * airplane.facing[0];
            airplane.position[2] -= 5 * airplane.facing[2];
            break;

        case 2:
            air_balloon.position[0] -= 5 * air_balloon.facing[0];
            air_balloon.position[2] -= 5 * air_balloon.facing[2];
            break;

        case 3:
            helicopter.position[0] -= 5 * helicopter.facing[0];
            helicopter.position[2] -= 5 * helicopter.facing[2];
            break;
        }
    }

    //Iluminação airplane
    sLightPos2Airplane[0] = airplane.position[0] + (airplane.facing[0] * 100);
    sLightPos2Airplane[1] = airplane.position[1] + 300;
    sLightPos2Airplane[2] = airplane.position[2] + (airplane.facing[2] * 100);
    sLightDir2Airplane[0] = (airplane.position[0] + (airplane.facing[0] * 50)) - sLightPos2Airplane[0];
    sLightDir2Airplane[1] = (airplane.position[1] + (airplane.facing[1] * 50)) - sLightPos2Airplane[1];
    sLightDir2Airplane[2] = (airplane.position[2] + (airplane.facing[2] * 50)) - sLightPos2Airplane[2];

    //Iluminação ballon
    sLightPos2Ballon[0] = air_balloon.position[0] + (air_balloon.facing[0] * 100);
    sLightPos2Ballon[1] = air_balloon.position[1] + 300;
    sLightPos2Ballon[2] = air_balloon.position[2] + (air_balloon.facing[2] * 100);
    sLightDir2Ballon[0] = (air_balloon.position[0] + (air_balloon.facing[0] * 50)) - sLightPos2Ballon[0];
    sLightDir2Ballon[1] = (air_balloon.position[1] + (air_balloon.facing[1] * 50)) - sLightPos2Ballon[1];
    sLightDir2Ballon[2] = (air_balloon.position[2] + (air_balloon.facing[2] * 50)) - sLightPos2Ballon[2];

    //Iluminação helicopter
    sLightPos2Helicopter[0] = helicopter.position[0] + (helicopter.facing[0] * 100);
    sLightPos2Helicopter[1] = helicopter.position[1] + 300;
    sLightPos2Helicopter[2] = helicopter.position[2] + (helicopter.facing[2] * 100);
    sLightDir2Helicopter[0] = (helicopter.position[0] + (helicopter.facing[0] * 50)) - sLightPos2Helicopter[0];
    sLightDir2Helicopter[1] = (helicopter.position[1] + (helicopter.facing[1] * 50)) - sLightPos2Helicopter[1];
    sLightDir2Helicopter[2] = (helicopter.position[2] + (helicopter.facing[2] * 50)) - sLightPos2Helicopter[2];

    //Iluminação
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_POSITION, sLightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightfv(GL_LIGHT0, GL_SHININESS, white_light);

    switch (fm_selected)
    {
    case 1:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2Airplane);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2Airplane);
        break;

    case 2:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2Ballon);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2Ballon);
        break;

    case 3:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2Helicopter);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2Helicopter);
        break;
    }

    glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
    glLightfv(GL_LIGHT1, GL_SHININESS, white_light);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);

    if (light0)
    {
        glEnable(GL_LIGHT0);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }

    if (light1)
    {
        glEnable(GL_LIGHT1);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    //glEnable(GL_LIGHT2);
    glDepthFunc(GL_LEQUAL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Mudança entre camera dentro e fora
    switch (fm_selected)
    {
    case 1:
        if (airplane.inside)
        {
            gluLookAt(
                airplane.position[0] - (airplane.facing[0] * 10),
                airplane.position[1] + 100,
                airplane.position[2] - (airplane.facing[2] * 10),
                airplane.position[0] - (airplane.facing[0] * 300),
                airplane.position[1],
                airplane.position[2] - (airplane.facing[2] * 300),
                0.0,
                1.0,
                0.0);
        }
        else
        {
            gluLookAt(
                airplane.position[0] + (airplane.facing[0] * 300),
                airplane.position[1] + 100,
                airplane.position[2] + (airplane.facing[2] * 300),
                airplane.position[0] - (airplane.facing[0] * 100),
                airplane.position[1] - (airplane.facing[1] * 100),
                airplane.position[2] - (airplane.facing[2] * 100),
                0.0,
                1.0,
                0.0);
        }
        break;

    case 2:
        if (air_balloon.inside)
        {
            gluLookAt(
                air_balloon.position[0] - (air_balloon.facing[0] * 10),
                air_balloon.position[1] + 100,
                air_balloon.position[2] - (air_balloon.facing[2] * 10),
                air_balloon.position[0] - (air_balloon.facing[0] * 300),
                air_balloon.position[1],
                air_balloon.position[2] - (air_balloon.facing[2] * 300),
                0.0,
                1.0,
                0.0);
        }
        else
        {
            gluLookAt(
                air_balloon.position[0] + (air_balloon.facing[0] * 300),
                air_balloon.position[1] + 100,
                air_balloon.position[2] + (air_balloon.facing[2] * 300),
                air_balloon.position[0] - (air_balloon.facing[0] * 100),
                air_balloon.position[1] - (air_balloon.facing[1] * 100),
                air_balloon.position[2] - (air_balloon.facing[2] * 100),
                0.0,
                1.0,
                0.0);
        }
        break;

    case 3:
        if (helicopter.inside)
        {
            gluLookAt(
                helicopter.position[0] - (helicopter.facing[0] * 10),
                helicopter.position[1] + 100,
                helicopter.position[2] - (helicopter.facing[2] * 10),
                helicopter.position[0] - (helicopter.facing[0] * 300),
                helicopter.position[1],
                helicopter.position[2] - (helicopter.facing[2] * 300),
                0.0,
                1.0,
                0.0);
        }
        else
        {
            gluLookAt(
                helicopter.position[0] + (helicopter.facing[0] * 300),
                helicopter.position[1] + 100,
                helicopter.position[2] + (helicopter.facing[2] * 300),
                helicopter.position[0] - (helicopter.facing[0] * 100),
                helicopter.position[1] - (helicopter.facing[1] * 100),
                helicopter.position[2] - (helicopter.facing[2] * 100),
                0.0,
                1.0,
                0.0);
        }
        break;
    }

    //Desenha chão
    glPushMatrix();
    glColor4f(0.5, 0.5, 0.5, 0.3);
    glRotatef(30, 0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-10000.0, 0.0, -10000.0);
    glVertex3f(-10000.0, 0.0, 10000.0);
    glVertex3f(10000.0, 0.0, 10000.0);
    glVertex3f(10000.0, 0.0, -10000.0);
    glEnd();
    glPopMatrix();

    //Desenha estrada
    for (int i = 0; i < 500; i += 50)
    {
        glPushMatrix();
        glColor4f(1.0, 0.0, 1.0, 0.8);
        glTranslatef(315 - i, 2, 20);
        glScalef(15.0, 15.0, 15.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        for (auto &face : road.faces)
        {
            for (auto &vertex : face.vertices)
            {
                glVertex4f(road.geometric_vertices[vertex.geometric_vertex].x,
                           road.geometric_vertices[vertex.geometric_vertex].y,
                           road.geometric_vertices[vertex.geometric_vertex].z,
                           road.geometric_vertices[vertex.geometric_vertex].w);

                glNormal3f(road.normal_vertices[vertex.normal_vertex].i,
                           road.normal_vertices[vertex.normal_vertex].j,
                           road.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    //Desenha prédio
    for (int i = 150; i < 2000; i += 150)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(100, 2, 0 + i);
        glScalef(5.0, 5.0, 5.0);
        glRotatef(45, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        for (auto &face : building.faces)
        {
            for (auto &vertex : face.vertices)
            {
                glVertex4f(building.geometric_vertices[vertex.geometric_vertex].x,
                           building.geometric_vertices[vertex.geometric_vertex].y,
                           building.geometric_vertices[vertex.geometric_vertex].z,
                           building.geometric_vertices[vertex.geometric_vertex].w);

                glNormal3f(building.normal_vertices[vertex.normal_vertex].i,
                           building.normal_vertices[vertex.normal_vertex].j,
                           building.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    //Desenha casa
    for (int i = 500; i < 5000; i += 500)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(100 + i, 2, 100 + i);
        glScalef(5.0, 5.0, 5.0);
        glRotatef(45, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        for (auto &face : farmhouse.faces)
        {
            for (auto &vertex : face.vertices)
            {
                glVertex4f(farmhouse.geometric_vertices[vertex.geometric_vertex].x,
                           farmhouse.geometric_vertices[vertex.geometric_vertex].y,
                           farmhouse.geometric_vertices[vertex.geometric_vertex].z,
                           farmhouse.geometric_vertices[vertex.geometric_vertex].w);

                glNormal3f(farmhouse.normal_vertices[vertex.normal_vertex].i,
                           farmhouse.normal_vertices[vertex.normal_vertex].j,
                           farmhouse.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    //Desenha torre
    glPushMatrix();
    glColor4f(0.0, 1.0, 0.0, 0.8);
    glTranslatef(-1000, 2, -1000);
    glScalef(50.0, 50.0, 50.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : tower.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(tower.geometric_vertices[vertex.geometric_vertex].x,
                       tower.geometric_vertices[vertex.geometric_vertex].y,
                       tower.geometric_vertices[vertex.geometric_vertex].z,
                       tower.geometric_vertices[vertex.geometric_vertex].w);

            glNormal3f(tower.normal_vertices[vertex.normal_vertex].i,
                       tower.normal_vertices[vertex.normal_vertex].j,
                       tower.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha montanha
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glTranslatef(-1500, 2, -1500);
    glScalef(5.0, 5.0, 5.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    for (auto &face : mountain.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(mountain.geometric_vertices[vertex.geometric_vertex].x,
                       mountain.geometric_vertices[vertex.geometric_vertex].y,
                       mountain.geometric_vertices[vertex.geometric_vertex].z,
                       mountain.geometric_vertices[vertex.geometric_vertex].w);
            glNormal3f(mountain.normal_vertices[vertex.normal_vertex].i,
                       mountain.normal_vertices[vertex.normal_vertex].j,
                       mountain.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha balão
    glPushMatrix();
    glTranslatef(air_balloon.position[0], air_balloon.position[1], air_balloon.position[2]);
    glScalef(1.0, 1.0, 1.0);
    glRotatef(air_balloon.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLE_STRIP);
    for (auto &face : air_balloon.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(air_balloon.model.geometric_vertices[vertex.geometric_vertex].x,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].y,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].z,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].w);

            glNormal3f(air_balloon.model.normal_vertices[vertex.normal_vertex].i,
                       air_balloon.model.normal_vertices[vertex.normal_vertex].j,
                       air_balloon.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha helicóptero
    glPushMatrix();
    glTranslatef(helicopter.position[0], helicopter.position[1], helicopter.position[2]);
    glRotatef(helicopter.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    for (auto &face : helicopter.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(helicopter.model.geometric_vertices[vertex.geometric_vertex].x,
                       helicopter.model.geometric_vertices[vertex.geometric_vertex].y,
                       helicopter.model.geometric_vertices[vertex.geometric_vertex].z,
                       helicopter.model.geometric_vertices[vertex.geometric_vertex].w);

            glNormal3f(helicopter.model.normal_vertices[vertex.normal_vertex].i,
                       helicopter.model.normal_vertices[vertex.normal_vertex].j,
                       helicopter.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();

    //Desenha avião
    glPushMatrix();
    glTranslatef(airplane.position[0], airplane.position[1], airplane.position[2]);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLE_STRIP);
    for (auto &face : airplane.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(airplane.model.geometric_vertices[vertex.geometric_vertex].x,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].y,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].z,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].w);

            glNormal3f(airplane.model.normal_vertices[vertex.normal_vertex].i,
                       airplane.model.normal_vertices[vertex.normal_vertex].j,
                       airplane.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }

    if (key == 'x' || key == 'X')
    {
        switch (fm_selected)
        {
        case 1:
            if (airplane.position[1] < 4000)
                airplane.position[1] += 5.0;
            break;
        case 2:
            if (air_balloon.position[1] < 4000)
                air_balloon.position[1] += 5.0;
            break;
        case 3:
            if (helicopter.position[1] < 4000)
                helicopter.position[1] += 5.0;
            break;
        }
    }

    if (key == 'z' || key == 'Z')
    {

        switch (fm_selected)
        {
        case 1:
            if (airplane.position[1] > 2.0)
                airplane.position[1] -= 5.0;
            break;
        case 2:
            if (air_balloon.position[1] > 2.0)
                air_balloon.position[1] -= 5.0;
            break;
        case 3:
            if (helicopter.position[1] > 2.0)
                helicopter.position[1] -= 5.0;
            break;
        }
    }

    if (key == 'a' || key == 'A')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.angle += 1.0;
            rotate_y(-1.0);
            break;
        case 2:
            air_balloon.angle += 1.0;
            rotate_y(-1.0);
            break;
        case 3:
            helicopter.angle += 1.0;
            rotate_y(-1.0);
            break;
        }
    }

    if (key == 'd' || key == 'D')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.angle -= 1.0;
            rotate_y(1.0);
            break;
        case 2:
            air_balloon.angle -= 1.0;
            rotate_y(1.0);
            break;
        case 3:
            helicopter.angle -= 1.0;
            rotate_y(1.0);
            break;
        }
    }

    if (key == 'w' || key == 'W')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.moving = true;
            break;
        case 2:
            air_balloon.moving = true;
            break;
        case 3:
            helicopter.moving = true;
            break;
        }
    }

    if (key == 'e' || key == 'E')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.moving = false;
            break;
        case 2:
            air_balloon.moving = false;
            break;
        case 3:
            helicopter.moving = false;
            break;
        }
    }

    if (key == 'f' || key == 'F')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.inside = false;
            break;
        case 2:
            air_balloon.inside = false;
            break;
        case 3:
            helicopter.inside = false;
            break;
        }
    }

    if (key == 'i' || key == 'I')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.inside = true;
            break;
        case 2:
            air_balloon.inside = true;
            break;
        case 3:
            helicopter.inside = true;
            break;
        }
    }

    if (key == 'l' || key == 'L')
    {
        if (!active_light)
        {
            glEnable(GL_LIGHTING);
            active_light = true;
        }
        else
        {
            glDisable(GL_LIGHTING);
            active_light = false;
        }
    }

    if (key == '4')
    {
        light0 = !light0;
    }

    if (key == '5')
    {
        light1 = !light1;
    }

    if (key == 's' || key == 'S')
    {
        glShadeModel(GL_FLAT);
    }

    if (key == 'g' || key == 'G')
    {
        glShadeModel(GL_SMOOTH);
    }

    if (key == '1')
    {
        fm_selected = 1;
        air_balloon.moving = false;
        helicopter.moving = false;
    }

    if (key == '2')
    {
        fm_selected = 2;
        airplane.moving = false;
        helicopter.moving = false;
    }

    if (key == '3')
    {
        fm_selected = 3;
        airplane.moving = false;
        air_balloon.moving = false;
    }
}

int main(int argc, char **argv)
{
    Model model, model2, model3;

    //Load dos modelos
    OBJ obj("Samples/airplane.obj");
    obj.load(model);
    airplane.model = model;

    OBJ obj2("Samples/tower.obj");
    obj2.load(tower);

    OBJ obj3("Samples/building.obj");
    obj3.load(building);

    OBJ obj4("Samples/road.obj");
    obj4.load(road);

    OBJ obj5("Samples/farmhouse.obj");
    obj5.load(farmhouse);

    OBJ obj6("Samples/mountain.obj");
    obj6.load(mountain);

    OBJ obj7("Samples/air_balloon.obj");
    obj7.load(model2);
    air_balloon.model = model2;
    air_balloon.angle = 90.0;
    air_balloon.position[0] = -500.0;
    air_balloon.position[1] = 5.0;
    air_balloon.position[2] = 500.0;

    OBJ obj8("Samples/helicopter.obj");
    obj8.load(model3);
    helicopter.model = model3;
    helicopter.angle = 90.0;
    helicopter.position[0] = 500.0;
    helicopter.position[2] = 500.0;

    //Main do openGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Trabalho de CG");
    init();
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    glutMainLoop();

    return 0;
}
