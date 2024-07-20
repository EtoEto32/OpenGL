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
GLfloat sphere_position_x = 0.0f;
GLfloat rotation_speed = 0.5f;
GLfloat sphere_scale = 1.0f;
int last_mouse_x, last_mouse_y;
int is_mouse_down = 0;
GLfloat background_color[3] = { 0.0f, 0.0f, 0.0f }; // 黒い背景
GLfloat sphere_color[3] = { 1.0f, 1.0f, 1.0f };
GLfloat color_change_rate = 0.01f;
#define STAR_COUNT 100
GLfloat stars[STAR_COUNT][2];

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    // テクスチャの読み込みは省略

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

    // 初期背景色を設定
    glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);

    // 乱数の初期化
    srand((unsigned int)time(NULL));

    // 星の位置をランダムに生成
    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i][0] = (float)rand() / RAND_MAX * 2.0f - 1.0f; // x座標 (-1.0 〜 1.0)
        stars[i][1] = (float)rand() / RAND_MAX * 2.0f - 1.0f; // y座標 (-1.0 〜 1.0)
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
        sphere_color[i] = 1.0f - background_color[i];
    }
}

void randomizeSphereColor() {
    for (int i = 0; i < 3; i++) {
        sphere_color[i] = (float)rand() / RAND_MAX;
    }
}

void drawStars() {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // 白い星
    for (int i = 0; i < STAR_COUNT; i++) {
        glVertex2f(stars[i][0], stars[i][1]);
    }
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawSphere() {
    glBindTexture(GL_TEXTURE_2D, texture);

    GLfloat mat_diffuse[] = { sphere_color[0], sphere_color[1], sphere_color[2], 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glutSolidSphere(1.0, 50, 50);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 背景を描画
    drawStars();

    gluLookAt(camera_distance * sin(camera_angle_y) * cos(camera_angle_x),
        camera_distance * sin(camera_angle_x),
        camera_distance * cos(camera_angle_y) * cos(camera_angle_x),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // ライトの位置を更新
    GLfloat light_position[] = { 5.0f * cosf(light_angle), 5.0f, 5.0f * sinf(light_angle), 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 回転する球を描画
    glTranslatef(sphere_position_x, 0.0f, -10.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);
    glScalef(sphere_scale, sphere_scale, sphere_scale);
    drawSphere();

    // 別のオブジェクトを追加
    glLoadIdentity();
    gluLookAt(camera_distance * sin(camera_angle_y) * cos(camera_angle_x),
        camera_distance * sin(camera_angle_x),
        camera_distance * cos(camera_angle_y) * cos(camera_angle_x),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glTranslatef(2.0f, 0.0f, -10.0f);
    glRotatef(-angle, 1.0f, 0.0f, 0.0f);
    glScalef(sphere_scale, sphere_scale, sphere_scale);
    drawSphere();

    glutSwapBuffers();
    angle += rotation_speed;
    light_angle += 0.01f;
    updateColors(); // 色を更新
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
    case 27: // ESCキーで終了
        exit(0);
        break;
    case 'w': // カメラを前進
        camera_distance -= 0.5f;
        break;
    case 's': // カメラを後退
        camera_distance += 0.5f;
        break;
    case 'a': // カメラを左に回転
        camera_angle_y -= 0.1f;
        break;
    case 'd': // カメラを右に回転
        camera_angle_y += 0.1f;
        break;
    case 'r': // 球の回転速度を速くする
        rotation_speed += 0.1f;
        break;
    case 'f': // 球の回転速度を遅くする
        rotation_speed -= 0.1f;
        break;
    case 'q': // 球を左に移動
        sphere_position_x -= 0.5f;
        break;
    case 'e': // 球を右に移動
        sphere_position_x += 0.5f;
        break;
    case 'c': // 球の色をランダムに変更
        randomizeSphereColor();
        break;
    case 'z': // 球を縮小
        sphere_scale -= 0.1f;
        break;
    case 'x': // 球を拡大
        sphere_scale += 0.1f;
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
