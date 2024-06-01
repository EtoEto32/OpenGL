#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
//�K�v�ȃw�b�_�t�@�C�����C���N���[�h����
#define PI 3.141592653//PI�̒l���`(��芸����10���܂�)


static int year = 0, day = 0;//�N�Ɠ��̏����l
int samplingTime = 50;//�^�C�}�[�̃T���v�����O�^�C���A50ms���ƂɍX�V����B

void myInit(char *progname)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//�_�u���o�b�t�@�����O��RGBA���[�h
	glutInitWindowSize(1000, 1000);//�E�C���h�T�C�Y�ݒ�A�������������ԊO���Ȃ̂ő傫�߂�
	glutInitWindowPosition(0, 0);//��ʍ���ɕ\��
	glutCreateWindow(progname); // �E�B���h�E���J��
	glClearColor(0.0, 0.0, 0.0, 0.0);//�w�i�F������
}
//�q���p�Ɋ֐���
void drawSatellite(double distance, double size, double orbitPeriod, double rotationPeriod)//�q����`�悷��֐�
{
    glPushMatrix();//�����_�ł̍s���ۑ�
	glRotated((double)year / orbitPeriod, 0.0, 1.0, 0.0);//�q���̌��]
	glTranslated(distance, 0.0, 0.0);//������ݒ�
    glRotated((double)day / rotationPeriod, 0.0, 1.0, 0.0);//�q���̎��]
	glutWireSphere(size, 10, 8);//�q����`��
	glPopMatrix();//�s��𕜌�
}
//�������������̂��߂�ǂ���������֐���
void drawPlanet(double distance, double size, double yearRotation, double dayRotation, double satelliteData[][4], int numSatellites)//�����̈Ӗ��͂��ꂼ��A�����A�T�C�Y�A���]�����A���]�����A�q���̃f�[�^�A�q���̐�
{
	glPushMatrix();////�����_�ł̍s���ۑ�
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

    //���z
    glutWireSphere(2.5, 20, 16);

	//�q���̃f�[�^(�����A�T�C�Y�A���]�����A���]����)
	double earthSatellites[][4] = { {0.7, 0.04, 27.32, 27.32} }; // ���̌��]�����Ǝ��]����
    double marsSatellites[][4] = { {0.5, 0.05, 0.319, 0.319}, {0.5, 0.05, 1.2624, 1.2624} }; // �t�H�{�X�ƃ_�C���X
    double jupiterSatellites[][4] = { {1.2, 0.1,7.155,7.155}, {1.5, 0.1, 16.689, 16.689} }; // �K�j���f�ƃJ���X�g
    double saturnSatellites[][4] = { {0.8, 0.05, 4.12, 4.12}, {0.8, 0.05, 15.945, 15.945} }; //���A�ƃ^�C�^��

	//����
	drawPlanet(4.0, 0.2, 0.24, 58.65, NULL, 0);//�����̈Ӗ��͂��ꂼ��A�����A�T�C�Y�A���]�����A���]����

	//����
    drawPlanet(6.0, 0.3, 0.61, 243.01, NULL, 0);

    //�n���ƌ�
    drawPlanet(8.0, 0.15, 1.0, 1.0, earthSatellites, 1);

    //�ΐ�
    drawPlanet(10.0, 0.2, 1.88, 1.03, marsSatellites, 2);

    //�ؐ�
    drawPlanet(14.0, 0.6, 11.86, 0.41, jupiterSatellites, 2);

    //�y��
    drawPlanet(18.0, 0.5, 29.45, 0.42, saturnSatellites, 2);

    //�V����
    drawPlanet(20.0, 0.4, 84.01, 0.71, NULL, 0);

    //�C����
    drawPlanet(22.0, 0.4, 164.79, 0.67, NULL, 0);

    //������(�ȉ~�O��)
	glPushMatrix();//�����_�ł̍s���ۑ�
	glRotated((double)year / 247.74, 0.0, 1.0, 0.0);//���]
	double X = 24.0 * cos(year * PI / 247.74);//�ȉ~�̔}��ϐ��\���@x���W
	double Z = 24.0 * sin(year * PI / 247.74) * 0.8; //�ȉ~�̔}��ϐ��\���@z���W
	glTranslated(X, 0.0, Z);//���������͑ȉ~�O����x���W��z���W�@Y�������Ȃ��̂�xz���ʏ�ɕ`�悷�邽��
	glRotated((double)day / 6.39, 0.0, 1.0, 0.0);//���]
	glutWireSphere(0.1, 10, 8);//��������`��
	glPopMatrix();//�s��𕜌�
	glutSwapBuffers();//�_�u���o�b�t�@�����O ���ꂪ�Ȃ��Ƃ�������ۂ�...
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

