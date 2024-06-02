#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
//必要なヘッダファイルをインクルードした
#define PI 3.141592653//PIの値を定義(取り敢えず10桁まで)

static int year = 0, day = 0;//年と日の初期値
int samplingTime = 50;//タイマーのサンプリングタイム、50msごとに更新する。

//衛星用に関数化
void drawSatellite(double distance, double size, double  orbit, double rotation)//衛星を描画する関数
{
    glPushMatrix();//現時点での行列を保存
    glRotated((double)year / orbit, 0.0, 1.0, 0.0);//衛星の公転
    glTranslated(distance, 0.0, 0.0);//距離を設定
    glRotated((double)day / rotation, 0.0, 1.0, 0.0);//衛星の自転
    glutWireSphere(size, 10, 8);//衛星を描画
    glPopMatrix();//行列を復元
}

void myInit(char* progname)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//ダブルバッファリングとRGBAモード
    glutInitWindowSize(1000, 1000);//ウインドサイズ設定、冥王星がだいぶ外回りなので大きめに
    glutInitWindowPosition(0, 0);//画面左上に表示
    glutCreateWindow(progname); // ウィンドウを開く
    glClearColor(0.0, 0.0, 0.0, 0.0);//背景色を黒に
}

//いちいち書くのもめんどくさいから関数化
void drawPlanet(double distance, double size, double yearRotation, double dayRotation, double satelliteData[2][4], int numSatellites)//引数の意味はそれぞれ、距離、サイズ、公転周期、自転周期、衛星のデータ（[][4]でまとめて渡している。）、衛星の数
{
    glPushMatrix();//現時点での行列を保存
    glRotated((double)year / yearRotation, 0.0, 1.0, 0.0);//惑星の公転
    glTranslated(distance, 0.0, 0.0);//距離を設定
    glRotated((double)day / dayRotation, 0.0, 1.0, 0.0);//惑星の自転
    glutWireSphere(size, 10, 8);//惑星を描画

    for (int i = 0; i < numSatellites; i++)//衛星の数だけ描画
    {
        drawSatellite(satelliteData[i][0], satelliteData[i][1], satelliteData[i][2], satelliteData[i][3]);//引数の意味はそれぞれ、距離、サイズ、公転周期、自転周期
    }
    glPopMatrix();//行列を復元
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//バッファをクリア
    glColor3d(1.0, 1.0, 1.0);//色を白に設定
    //太陽　自転だけ
    glPushMatrix();
    glRotated((double)day / 25.38, 0.0, 1.0, 0.0); // 自転速度を追加
    glutWireSphere(2.1, 20, 16);
    glPopMatrix();

    //衛星のデータ(距離、サイズ、公転周期、自転周期)
    double earthSatellites[1][4] ={1.2, 0.175, 27.32, 27.32}; // 月の公転周期と自転周期
    double marsSatellites[2][4] = { {1.0, 0.1, 0.319, 0.319}, {0.5, 0.05, 1.2624, 1.2624} }; // フォボスとダイモス
    double jupiterSatellites[2][4] = { {1.5, 0.1,7.155,7.155}, {2.4, 0.1, 16.689, 16.689} }; // ガニメデとカリスト
    double saturnSatellites[2][4] = { {1.3, 0.1, 4.12, 4.12}, {1.8, 0.1, 15.945, 15.945} }; //レアとタイタン

    //水星
    drawPlanet(3.0, 0.6, 0.24, 58.65, NULL, 0);//引数の意味はそれぞれ、距離、サイズ、公転周期、自転周期

    //金星
    drawPlanet(5.0, 0.7, 0.61, -243.01, NULL, 0);//自転の向き逆

    //地球
    drawPlanet(7.0, 0.6, 1.0, 1.0, earthSatellites, 1);

    //火星
    drawPlanet(9.0, 0.6, 1.88, 1.03, marsSatellites, 2);

    //木星
    drawPlanet(13.0, 1.0, 11.86, 0.41, jupiterSatellites, 2);

    //土星
    drawPlanet(16.0, 0.9, 29.45, 0.42, saturnSatellites, 2);

    //天王星
    drawPlanet(19.0, 0.8, 84.01, -0.71, NULL, 0);//自転の向き逆

    //海王星
    drawPlanet(22.0, 1.0, 164.79, 0.67, NULL, 0);

    //冥王星(楕円軌道)
    glPushMatrix();//現時点での行列を保存
    glRotated((double)year / 247.74, 0.0, 1.0, 0.0);//公転
    double X = 24.5 * cos(-year * 2 * PI / 247.74);//楕円の媒介変数表示 x座標
    double Z = 24.5 * sin(-year * 2 * PI / 247.74) * 0.6; //楕円の媒介変数表示 z座標
    glTranslated(X, 0.0, Z);//距離引数は楕円軌道のx座標とz座標　Yが動かないのはxz平面上に描画するため
    glRotated((double)day / 6.39, 0.0, 1.0, 0.0);//自転(公転方向と逆向き)
    glutWireSphere(0.1, 10, 8);//冥王星を描画
    glPopMatrix();//行列を復元
    glutSwapBuffers();//ダブルバッファリング
}

void myReshape(int width, int height)//ウィンドウサイズ変更時のコールバック関数
{
    glViewport(0, 0, width, height);//ビューポートの設定
    glMatrixMode(GL_PROJECTION);//投影変換行列の設定
    glLoadIdentity();//行列の初期化
    gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);//透視投影(視野角、アスペクト比、前方クリップ面、後方クリップ面)
    glMatrixMode(GL_MODELVIEW);//モデルビュー変換行列の設定
    glLoadIdentity();//行列の初期化
    gluLookAt(0.0, 15.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//視点の設定(視点の位置、注視点の位置、視界の上方向) x-z平面上から見る
}

void myKeyboard(unsigned char key, int x, int y)//キーボード入力のコールバック関数
{
    switch (key) {
    case 'd'://日の早送り
        day = (day + 10) % 360;
        glutPostRedisplay();
        break;
    case 'D'://日の巻き戻し
        day = (day - 10) % 360;
        glutPostRedisplay();
        break;
    case 'y'://年の早送り
        year = (year + 5) % 360;
        glutPostRedisplay();
        break;
    case 'Y'://年の巻き戻し
        year = (year - 5) % 360;
        glutPostRedisplay();
        break;
    case 27://ESCキーで終了
        exit(0);
        break;
    default:
        break;
    }
}

void myTimer(int value)//タイマー関数
{
    if (value == 1)//valueが1の時だけ更新する
    {
        glutTimerFunc(samplingTime, myTimer, 1);//50msごとに更新
        year = (year + 1);//年を更新
        day = (day + 5);//日を更新
        glutPostRedisplay();//再描画を始める
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);//GLUTの初期化
    myInit(argv[0]);//ウィンドウの初期化
    glutKeyboardFunc(myKeyboard);//キーボード入力のコールバック関数
    glutTimerFunc(samplingTime, myTimer, 1);//タイマー関数(50msごと)
    glutReshapeFunc(myReshape);//ウィンドウサイズ変更時のコールバック関数
    glutDisplayFunc(myDisplay);//描画関数
    glutMainLoop();//メインループ
    return 0;
}
