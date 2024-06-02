#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
//�K�v�ȃw�b�_�t�@�C�����C���N���[�h����
#define PI 3.141592653//PI�̒l���`(��芸����10���܂�)

static int year = 0, day = 0;//�N�Ɠ��̏����l
int samplingTime = 50;//�^�C�}�[�̃T���v�����O�^�C���A50ms���ƂɍX�V����B

//�q���p�Ɋ֐���
void drawSatellite(double distance, double size, double  orbit, double rotation)//�q����`�悷��֐�
{
    glPushMatrix();//�����_�ł̍s���ۑ�
    glRotated((double)year / orbit, 0.0, 1.0, 0.0);//�q���̌��]
    glTranslated(distance, 0.0, 0.0);//������ݒ�
    glRotated((double)day / rotation, 0.0, 1.0, 0.0);//�q���̎��]
    glutWireSphere(size, 10, 8);//�q����`��
    glPopMatrix();//�s��𕜌�
}

void myInit(char* progname)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//�_�u���o�b�t�@�����O��RGBA���[�h
    glutInitWindowSize(1000, 1000);//�E�C���h�T�C�Y�ݒ�A�������������ԊO���Ȃ̂ő傫�߂�
    glutInitWindowPosition(0, 0);//��ʍ���ɕ\��
    glutCreateWindow(progname); // �E�B���h�E���J��
    glClearColor(0.0, 0.0, 0.0, 0.0);//�w�i�F������
}

//�������������̂��߂�ǂ���������֐���
void drawPlanet(double distance, double size, double yearRotation, double dayRotation, double satelliteData[2][4], int numSatellites)//�����̈Ӗ��͂��ꂼ��A�����A�T�C�Y�A���]�����A���]�����A�q���̃f�[�^�i[][4]�ł܂Ƃ߂ēn���Ă���B�j�A�q���̐�
{
    glPushMatrix();//�����_�ł̍s���ۑ�
    glRotated((double)year / yearRotation, 0.0, 1.0, 0.0);//�f���̌��]
    glTranslated(distance, 0.0, 0.0);//������ݒ�
    glRotated((double)day / dayRotation, 0.0, 1.0, 0.0);//�f���̎��]
    glutWireSphere(size, 10, 8);//�f����`��

    for (int i = 0; i < numSatellites; i++)//�q���̐������`��
    {
        drawSatellite(satelliteData[i][0], satelliteData[i][1], satelliteData[i][2], satelliteData[i][3]);//�����̈Ӗ��͂��ꂼ��A�����A�T�C�Y�A���]�����A���]����
    }
    glPopMatrix();//�s��𕜌�
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//�o�b�t�@���N���A
    glColor3d(1.0, 1.0, 1.0);//�F�𔒂ɐݒ�
    //���z�@���]����
    glPushMatrix();
    glRotated((double)day / 25.38, 0.0, 1.0, 0.0); // ���]���x��ǉ�
    glutWireSphere(2.1, 20, 16);
    glPopMatrix();

    //�q���̃f�[�^(�����A�T�C�Y�A���]�����A���]����)
    double earthSatellites[1][4] ={1.2, 0.175, 27.32, 27.32}; // ���̌��]�����Ǝ��]����
    double marsSatellites[2][4] = { {1.0, 0.1, 0.319, 0.319}, {0.5, 0.05, 1.2624, 1.2624} }; // �t�H�{�X�ƃ_�C���X
    double jupiterSatellites[2][4] = { {1.5, 0.1,7.155,7.155}, {2.4, 0.1, 16.689, 16.689} }; // �K�j���f�ƃJ���X�g
    double saturnSatellites[2][4] = { {1.3, 0.1, 4.12, 4.12}, {1.8, 0.1, 15.945, 15.945} }; //���A�ƃ^�C�^��

    //����
    drawPlanet(3.0, 0.6, 0.24, 58.65, NULL, 0);//�����̈Ӗ��͂��ꂼ��A�����A�T�C�Y�A���]�����A���]����

    //����
    drawPlanet(5.0, 0.7, 0.61, -243.01, NULL, 0);//���]�̌����t

    //�n��
    drawPlanet(7.0, 0.6, 1.0, 1.0, earthSatellites, 1);

    //�ΐ�
    drawPlanet(9.0, 0.6, 1.88, 1.03, marsSatellites, 2);

    //�ؐ�
    drawPlanet(13.0, 1.0, 11.86, 0.41, jupiterSatellites, 2);

    //�y��
    drawPlanet(16.0, 0.9, 29.45, 0.42, saturnSatellites, 2);

    //�V����
    drawPlanet(19.0, 0.8, 84.01, -0.71, NULL, 0);//���]�̌����t

    //�C����
    drawPlanet(22.0, 1.0, 164.79, 0.67, NULL, 0);

    //������(�ȉ~�O��)
    glPushMatrix();//�����_�ł̍s���ۑ�
    glRotated((double)year / 247.74, 0.0, 1.0, 0.0);//���]
    double X = 24.5 * cos(-year * 2 * PI / 247.74);//�ȉ~�̔}��ϐ��\�� x���W
    double Z = 24.5 * sin(-year * 2 * PI / 247.74) * 0.6; //�ȉ~�̔}��ϐ��\�� z���W
    glTranslated(X, 0.0, Z);//���������͑ȉ~�O����x���W��z���W�@Y�������Ȃ��̂�xz���ʏ�ɕ`�悷�邽��
    glRotated((double)day / 6.39, 0.0, 1.0, 0.0);//���](���]�����Ƌt����)
    glutWireSphere(0.1, 10, 8);//��������`��
    glPopMatrix();//�s��𕜌�
    glutSwapBuffers();//�_�u���o�b�t�@�����O
}

void myReshape(int width, int height)//�E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N�֐�
{
    glViewport(0, 0, width, height);//�r���[�|�[�g�̐ݒ�
    glMatrixMode(GL_PROJECTION);//���e�ϊ��s��̐ݒ�
    glLoadIdentity();//�s��̏�����
    gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);//�������e(����p�A�A�X�y�N�g��A�O���N���b�v�ʁA����N���b�v��)
    glMatrixMode(GL_MODELVIEW);//���f���r���[�ϊ��s��̐ݒ�
    glLoadIdentity();//�s��̏�����
    gluLookAt(0.0, 15.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//���_�̐ݒ�(���_�̈ʒu�A�����_�̈ʒu�A���E�̏����) x-z���ʏォ�猩��
}

void myKeyboard(unsigned char key, int x, int y)//�L�[�{�[�h���͂̃R�[���o�b�N�֐�
{
    switch (key) {
    case 'd'://���̑�����
        day = (day + 10) % 360;
        glutPostRedisplay();
        break;
    case 'D'://���̊����߂�
        day = (day - 10) % 360;
        glutPostRedisplay();
        break;
    case 'y'://�N�̑�����
        year = (year + 5) % 360;
        glutPostRedisplay();
        break;
    case 'Y'://�N�̊����߂�
        year = (year - 5) % 360;
        glutPostRedisplay();
        break;
    case 27://ESC�L�[�ŏI��
        exit(0);
        break;
    default:
        break;
    }
}

void myTimer(int value)//�^�C�}�[�֐�
{
    if (value == 1)//value��1�̎������X�V����
    {
        glutTimerFunc(samplingTime, myTimer, 1);//50ms���ƂɍX�V
        year = (year + 1);//�N���X�V
        day = (day + 5);//�����X�V
        glutPostRedisplay();//�ĕ`����n�߂�
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);//GLUT�̏�����
    myInit(argv[0]);//�E�B���h�E�̏�����
    glutKeyboardFunc(myKeyboard);//�L�[�{�[�h���͂̃R�[���o�b�N�֐�
    glutTimerFunc(samplingTime, myTimer, 1);//�^�C�}�[�֐�(50ms����)
    glutReshapeFunc(myReshape);//�E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N�֐�
    glutDisplayFunc(myDisplay);//�`��֐�
    glutMainLoop();//���C�����[�v
    return 0;
}
