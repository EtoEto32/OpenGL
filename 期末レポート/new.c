#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

GLuint texture;
GLfloat angle = 0.0;
GLfloat light_angle = 0.0;
GLfloat camera_angle_x = 0.0f;
GLfloat camera_angle_y = 0.0f;
GLfloat camera_distance = 10.0f;
GLfloat cube_position_x = 0.0f;
GLfloat rotation_speed = 0.5f;
GLfloat cube_scale = 1.0f;
int last_mouse_x, last_mouse_y;
int is_mouse_down = 0;
GLfloat background_color[3] = { 0.0f, 0.0f, 0.0f }; // �����w�i
GLfloat cube_color[3] = { 1.0f, 1.0f, 1.0f };
GLfloat color_change_rate = 0.01f;
#define STAR_COUNT 100
GLfloat stars[STAR_COUNT][2];

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    // �e�N�X�`���̓ǂݍ��݂͏ȗ�

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // �����w�i�F��ݒ�
    glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);

    // �����̏�����
    srand((unsigned int)time(NULL));

    // ���̈ʒu�������_���ɐ���
    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i][0] = (float)rand() / RAND_MAX; // x���W
        stars[i][1] = (float)rand() / RAND_MAX; // y���W
    }
}

void updateColors() {
    for (int i = 0; i < 3; i++) {
        background_color[i] += color_change_rate;
        if (background_color[i] > 1.0f || background_color[i] < 0.0f) {
            color_change_rate = -color_change_rate;
        }
    }
    glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);

    for (int i = 0; i < 3; i++) {
        cube_color[i] = 1.0f - background_color[i];
    }
}

void randomizeCubeColor() {
    for (int i = 0; i < 3; i++) {
        cube_color[i] = (float)rand() / RAND_MAX;
    }
}

void drawStars() {
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // ������
    for (int i = 0; i < STAR_COUNT; i++) {
        glVertex2f(stars[i][0], stars[i][1]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawCube() {
    glBindTexture(GL_TEXTURE_2D, texture);

    GLfloat mat_diffuse[] = { cube_color[0], cube_color[1], cube_color[2], 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    // �O��
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // ���
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // ����
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // �E��
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // ���
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // ����
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // �w�i��`��
    drawStars();

    gluLookAt(camera_distance * sin(camera_angle_y) * cos(camera_angle_x),
        camera_distance * sin(camera_angle_x),
        camera_distance * cos(camera_angle_y) * cos(camera_angle_x),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // ���C�g�̈ʒu���X�V
    GLfloat light_position[] = { 5.0f * cosf(light_angle), 5.0f, 5.0f * sinf(light_angle), 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // ��]���闧���̂�`��
    glTranslatef(cube_position_x, 0.0f, -10.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);
    glScalef(cube_scale, cube_scale, cube_scale);
    drawCube();

    // �ʂ̃I�u�W�F�N�g��ǉ�
    glLoadIdentity();
    gluLookAt(camera_distance * sin(camera_angle_y) * cos(camera_angle_x),
        camera_distance * sin(camera_angle_x),
        camera_distance * cos(camera_angle_y) * cos(camera_angle_x),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glTranslatef(2.0f, 0.0f, -10.0f);
    glRotatef(-angle, 1.0f, 0.0f, 0.0f);
    glScalef(cube_scale, cube_scale, cube_scale);
    drawCube();

    glutSwapBuffers();
    angle += rotation_speed;
    light_angle += 0.01f;
    updateColors(); // �F���X�V
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC�L�[�ŏI��
        exit(0);
        break;
    case 'w': // �J������O�i
        camera_distance -= 0.5f;
        break;
    case 's': // �J���������
        camera_distance += 0.5f;
        break;
    case 'a': // �J���������ɉ�]
        camera_angle_y -= 0.1f;
        break;
    case 'd': // �J�������E�ɉ�]
        camera_angle_y += 0.1f;
        break;
    case 'r': // �����̂̉�]���x�𑬂�����
        rotation_speed += 0.1f;
        break;
    case 'f': // �����̂̉�]���x��x������
        rotation_speed -= 0.1f;
        break;
    case 'q': // �����̂����Ɉړ�
        cube_position_x -= 0.5f;
        break;
    case 'e': // �����̂��E�Ɉړ�
        cube_position_x += 0.5f;
        break;
    case 'c': // �����̂̐F�������_���ɕύX
        randomizeCubeColor();
        break;
    case 'z': // �����̂��k��
        cube_scale -= 0.1f;
        break;
    case 'x': // �����̂��g��
        cube_scale += 0.1f;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        is_mouse_down = 1;
        last_mouse_x = x;
        last_mouse_y = y;
    }
    else {
        is_mouse_down = 0;
    }
}

void motion(int x, int y) {
    if (is_mouse_down) {
        camera_angle_y += (x - last_mouse_x) * 0.005f;
        camera_angle_x += (y - last_mouse_y) * 0.005f;
        last_mouse_x = x;
        last_mouse_y = y;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Advanced OpenGL Texture Mapping");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
