// 타이머
#include "gl_func.h"

extern GLfloat rot;
bool rotateRight, rotateLeft; // 각각 오른쪽 회전, 왼쪽 회전

void rotateStealth() {
	if (rotateRight) {  // 오른쪽 회전
		rot -= 2;
		if (rot < -45)
			rot = -45;
	}
	if (rotateLeft) {  // 왼쪽 회전
		rot += 2;
		if (rot > 45)
			rot = 45;
	}
	if (rot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft)))
		rot -= 2;
	if (rot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft)))
		rot += 2;
}

void timerOperation(int value) {
	rotateStealth();

	glutTimerFunc(10, timerOperation, 1);
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
