// 타이머
#include "gl_func.h"

extern GLfloat rot, sx;
bool rotateRight, rotateLeft; // 각각 오른쪽 회전, 왼쪽 회전

void rotateStealth() {
	if (rotateRight) {  // 오른쪽 회전
		rot -= 1 * 2;
		if (rot < -45)
			rot = -45;
	}
	if (rotateLeft) {  // 왼쪽 회전
		rot += 1 * 2;
		if (rot > 45)
			rot = 45;
	}
	if (rot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
		rot -= 1 * 2;
		if (rot < 0)
			rot = 0;
	}
	if (rot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
		rot += 1 * 2;
		if (rot > 0)
			rot = 0;
	}
}

void moveStealth() {
	if (rotateRight) {  // 오른쪽 이동
		sx += 0.2 * 2;
		if (sx > 15)
			sx = 15;
	}
	if (rotateLeft) {  // 왼쪽 이동
		sx -= 0.2 * 2;
		if (sx < -15)
			sx = -15;
	}
	if (sx > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
		sx -= 0.2 * 2;
		if (sx < 0)
			sx = 0;
	}
	if (sx < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
		sx += 0.2 * 2;
		if (sx > 0)
			sx = 0;
	}
}

void timerOperation(int value) {
	rotateStealth();
	moveStealth();

	glutTimerFunc(10, timerOperation, 1);
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
