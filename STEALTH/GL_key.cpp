// 키보드 조작
#include "gl_func.h"
#include "soundController.cpp"

extern bool rotateRight, rotateLeft, gameUpdate;

bool bgmPlay, bgmStop;  // 사운트 컨트롤러 변수는 여기에 먼저 선언한 뒤에 사용
bool enginePlay, engineStop;
bool neelingPlay, neelingStop;
bool bgmHomePlay, bgmHomeStop;;
bool crashPlay;
bool cliffPlay;
int windSoundNum = 0;
int windSoundNum2 = 0;

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		glutDestroyWindow(1);
		break;

	case 32:  // 스페이스바 눌러 게임 시작
		if (!gameUpdate) {
			bgmPlay = true;  // 인게임 브금 on
			bgmHomeStop = true;  // 홈 사운드 off

			enginePlay = true;  // 엔진사운드 on
			gameUpdate = true;  // 게임 업데이트를 시작한다
		}
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void specialKeyDown(int KEY, int x, int y) {
	switch (KEY) {
	case GLUT_KEY_LEFT:
		rotateLeft = true;
		break;
	case GLUT_KEY_RIGHT:
		rotateRight = true;
		break;
	}

	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void specialKeyUp(int KEY, int x, int y) {
	switch (KEY) {
	case GLUT_KEY_LEFT:
		rotateLeft = false;
		break;
	case GLUT_KEY_RIGHT:
		rotateRight = false;
		break;
	}

	if (glutGetWindow() != 0)
		glutPostRedisplay();
}