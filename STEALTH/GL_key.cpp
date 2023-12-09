// 키보드 조작
#include "gl_func.h"
#include "soundController.cpp"

extern bool rotateRight, rotateLeft, gameUpdate, overShake;
extern float shakeTime;
extern GLfloat camY;

bool bgmPlay, bgmStop;
bool enginePlay, engineStop;
bool neelingPlay, neelingStop;

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		glutDestroyWindow(1);
		break;
	case 32:
		if (!gameUpdate) {
			bgmPlay = true;  // 사운드 on
			enginePlay = true;
			gameUpdate = true;
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