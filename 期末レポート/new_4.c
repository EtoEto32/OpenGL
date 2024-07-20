#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

GLuint texture; // テクスチャオブジェクト
GLfloat angle = 0.0; // 球の回転角度
GLfloat light_angle = 0.0; // ライトの回転角度
GLfloat camera_angle_x = 0.0f; // カメラのx軸角度
GLfloat camera_angle_y = 0.0f; // カメラのy軸角度
GLfloat camera_distance = 10.0f; // カメラの距離
GLfloat rotation_speed = 0.5f; // 球の回転速度
GLfloat sphere_scale = 1.0f; // 球のスケール
int last_mouse_x, last_mouse_y; // マウスの最後の位置
int is_mouse_down = 0; // マウスが押されているかのフラグ
int move_spheres = 0; // 球が動くかどうかのフラグ
GLfloat background_color[3] = { 0.0f, 0.0f, 0.0f }; // 背景色（黒）
GLfloat sphere_color[3] = { 1.0f, 1.0f, 1.0f }; // 球の色（白）
GLfloat color_change_rate = 0.01f; // 色の変化率
#define STAR_COUNT 100 // 星の数
#define SPHERE_COUNT 4 // 球の数
GLfloat stars[STAR_COUNT][2]; // 星の位置を保持する配列
GLfloat sphere_positions[SPHERE_COUNT][3]; // 球の位置を保持する配列
GLfloat sphere_velocities[SPHERE_COUNT][2]; // 球の速度を保持する配列
GLfloat initial_positions[SPHERE_COUNT][3]; // 球の初期位置を保持する配列

// 初期化関数
void init() {
    glEnable(GL_DEPTH_TEST); // 深度テストを有効にする
    glEnable(GL_TEXTURE_2D); // テクスチャマッピングを有効にする

    // ライティングの設定
    glEnable(GL_LIGHTING); // ライティングを有効にする
    glEnable(GL_LIGHT0); // ライト0を有効にする
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // ライトの位置を設定
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 拡散光の設定
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 鏡面反射光の設定
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // 環境光の設定
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

    // 球の初期位置と速度を設定
    initial_positions[0][0] = -2.0f; initial_positions[0][1] = 0.0f; initial_positions[0][2] = -10.0f;
    initial_positions[1][0] = 2.0f; initial_positions[1][1] = 0.0f; initial_positions[1][2] = -10.0f;
    initial_positions[2][0] = -2.0f; initial_positions[2][1] = 2.0f; initial_positions[2][2] = -10.0f;
    initial_positions[3][0] = 2.0f; initial_positions[3][1] = 2.0f; initial_positions[3][2] = -10.0f;

    for (int i = 0; i < SPHERE_COUNT; i++) {
        sphere_positions[i][0] = initial_positions[i][0];
        sphere_positions[i][1] = initial_positions[i][1];
        sphere_positions[i][2] = initial_positions[i][2];
        sphere_velocities[i][0] = ((float)rand() / RAND_MAX - 0.5f) * 0.01f; // x方向の速度
        sphere_velocities[i][1] = ((float)rand() / RAND_MAX - 0.5f) * 0.01f; // y方向の速度
    }
}

// 背景色と球の色を更新する関数
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

// 球の色をランダムに変更する関数
void randomizeSphereColor() {
    for (int i = 0; i < 3; i++) {
        sphere_color[i] = (float)rand() / RAND_MAX;
    }
}

// 球の速度をランダムに変更する関数
void randomizeSphereVelocities() {
    for (int i = 0; i < SPHERE_COUNT; i++) {
        sphere_velocities[i][0] = ((float)rand() / RAND_MAX - 0.5f) * 0.01f;
        sphere_velocities[i][1] = ((float)rand() / RAND_MAX - 0.5f) * 0.01f;
    }
}

// 球の位置を更新する関数
void updateSpherePositions() {
    for (int i = 0; i < SPHERE_COUNT; i++) {
        sphere_positions[i][0] += sphere_velocities[i][0];
        sphere_positions[i][1] += sphere_velocities[i][1];

        // 球が画面外に出た場合の処理
        if (sphere_positions[i][0] < -2.0f || sphere_positions[i][0] > 2.0f) {
            sphere_velocities[i][0] = -sphere_velocities[i][0];
        }
        if (sphere_positions[i][1] < -2.0f || sphere_positions[i][1] > 2.0f) {
            sphere_velocities[i][1] = -sphere_velocities[i][1];
        }
    }
}

// 球を移動させるためのタイマーコールバック関数
void moveSpheres(int value) {
    if (move_spheres) {
        updateSpherePositions();
        glutPostRedisplay();
        glutTimerFunc(16, moveSpheres, 0); // 約60FPSで再度呼び出す
    }
}

// 星を描画する関数
void drawStars() {
    glDisable(GL_LIGHTING); // ライティングを無効にする

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

    glEnable(GL_LIGHTING); // ライティングを再度有効にする
}

// 球を描画する関数
void drawSphere(GLfloat x, GLfloat y, GLfloat z) {
    glBindTexture(GL_TEXTURE_2D, texture); // テクスチャをバインドする

    // 球の材質の設定
    GLfloat mat_diffuse[] = { sphere_color[0], sphere_color[1], sphere_color[2], 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sphere_scale, sphere_scale, sphere_scale); // 球のスケールを設定
    glutSolidSphere(1.0, 50, 50); // 球を描画
    glPopMatrix();
}

// 描画関数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // バッファをクリア
    glLoadIdentity();

    // 背景を描画
    drawStars();

    // カメラの位置を設定
    gluLookAt(camera_distance * sin(camera_angle_y) * cos(camera_angle_x),
        camera_distance * sin(camera_angle_x),
        camera_distance * cos(camera_angle_y) * cos(camera_angle_x),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // ライトの位置を更新
    GLfloat light_position[] = { 5.0f * cosf(light_angle), 5.0f, 5.0f * sinf(light_angle), 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 球を描画
    for (int i = 0; i < SPHERE_COUNT; i++) {
        drawSphere(sphere_positions[i][0], sphere_positions[i][1], sphere_positions[i][2]);
    }

    glutSwapBuffers(); // バッファを交換
    angle += rotation_speed;
    light_angle += 0.01f;
    updateColors(); // 色を更新
}

// ウィンドウのリサイズに対応する関数
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); // ビューポートを設定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0); // 投影行列を設定
    glMatrixMode(GL_MODELVIEW);
}

// キーボード入力に対応する関数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESCキーで終了
        exit(0);
        break;
    case 13: // エンターキーで球の位置をランダムに移動または初期化
        move_spheres = !move_spheres;
        if (move_spheres) {
            glutTimerFunc(0, moveSpheres, 0);
        }
        else {
            for (int i = 0; i < SPHERE_COUNT; i++) {
                sphere_positions[i][0] = initial_positions[i][0];
                sphere_positions[i][1] = initial_positions[i][1];
                sphere_positions[i][2] = initial_positions[i][2];
            }
        }
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

// マウス入力に対応する関数
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

// マウスのドラッグに対応する関数
void motion(int x, int y) {
    if (is_mouse_down) {
        camera_angle_y += (x - last_mouse_x) * 0.005f;
        camera_angle_x += (y - last_mouse_y) * 0.005f;
        last_mouse_x = x;
        last_mouse_y = y;
        glutPostRedisplay();
    }
}

// メイン関数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ダブルバッファリング、RGBモード、深度バッファ
    glutInitWindowSize(800, 600); // ウィンドウサイズ
    glutInitWindowPosition(100, 100); // ウィンドウの初期位置
    glutCreateWindow("Advanced OpenGL Texture Mapping"); // ウィンドウのタイトル

    init(); // 初期化関数の呼び出し
    glutDisplayFunc(display); // 描画関数の設定
    glutReshapeFunc(reshape); // リサイズ関数の設定
    glutKeyboardFunc(keyboard); // キーボード入力関数の設定
    glutMouseFunc(mouse); // マウス入力関数の設定
    glutMotionFunc(motion); // マウスドラッグ関数の設定
    glutMainLoop(); // メインループ

    return 0;
}
