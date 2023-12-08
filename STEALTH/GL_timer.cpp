// 타이머
#include "gl_func.h"
#include "pillar.h"

extern GLfloat rot, sx, cz, fov, camRot, camMove, camMove2, gz;
extern GLfloat shakeX, shakeY, shakeZ;
GLfloat speed = 1.5;
GLfloat acc = 0;
GLfloat deg, deg2;
bool rotateRight, rotateLeft; // 각각 오른쪽 회전, 왼쪽 회전
bool cliffEnable;  // true일 시 절벽이 나타난다
bool stealthNeeling;  // true 일 시 세로로 기울인다, 조작에 따른 각도 제어 무시
bool camRotate;
bool accSet;
bool timerOperate = true;

int genDelay;  // 장애물 생성 딜레이
int delay = 80;
int num = 0;
int moveDistance = 0;

random_device rd;  mt19937 gen(rd());
uniform_real_distribution <GLfloat> dis(-15.0f, 15.0f);
uniform_real_distribution <GLfloat> shake_range(-0.2f, 0.2f);
uniform_real_distribution <GLfloat> shake_range2(-0.8f, 0.8f);

void init() {
	delay = 80;
	num = 0;
	accSet = false;
	cliffEnable = false;
	stealthNeeling = false;
	camRot = 0;
	camMove = 0;
	camMove2 = 0;
	shakeX = 0;
	shakeY = 0;
	shakeZ = 0;
	fov = 45;
	gz = 0;
	cz = 0;
	sx = 0;
	rot = 0;
	moveDistance = 0;
	deg = 0;
	deg2 = 0;
}

void rotateStealth() {
	if (!stealthNeeling) {
		if (rotateRight) {  // 오른쪽 회전
			rot -= 2;
			camRot -= 0.5;
			if (rot < -45)
				rot = -45;
			if (camRot < -30)
				camRot = -30;
		
		}
		if (rotateLeft) {  // 왼쪽 회전
			rot += 2;
			camRot += 0.5;
			if (rot > 45)
				rot = 45;
			if (camRot > 30)
				camRot = 30;
		
		}

		if (rot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			rot -= 2;
			camRot -= 0.5;
			if (rot < 0)
				rot = 0;
			
		}
		if (rot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			rot += 2;
			camRot += 0.5;
			if (rot > 0)
				rot = 0;
		}
		if (camRot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			camRot -= 0.5;
			if (camRot < 0)
				camRot = 0;

		}
		if (camRot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			camRot += 0.5;
			if (camRot > 0)
				camRot = 0;
		}
	}

	if (stealthNeeling) {
		if (rot < 90) {
			rot += 2;
			if (rot > 90)
				rot = 90;
		}
	}
}

void moveStealth() {
	if (!stealthNeeling) {
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
	else {
		if (sx > 0) {
			sx -= 0.2 * 2;
			if (sx < 0)
				sx = 0;
		}
		if (sx < 0) {
			sx += 0.2 * 2;
			if (sx > 0)
				sx = 0;
		}
	}
}

void generatePillar() {
	if (genDelay > 0)
		genDelay--;
	else {
		if (num < 20) {
			p[num].x = dis(gen);
			p[num].z = -50;
			cout << p[num].z << endl;
			p[num].height = 0;
			genDelay = delay;
			num++;
		}
	}
}

void removePillar(int idx) {
	if (idx == num)
		num--;
	else if (idx < num) {
		for (int i = idx; i < num; i++) {
			p[i].x = p[i + 1].x;
			p[i].z = p[i + 1].z;
			p[i].height = p[i + 1].height;
		}
		num--;
	}
}

void movePillar() {
	for (int i = 0; i < num; i++) {
		p[i].z += speed;
		p[i].height += 1;
		if (p[i].z > 30)
			removePillar(i);
	}
}

void updateCliff() {
	cz += speed;

	if (cz + 250 > 250) {
		stealthNeeling = true;
		if (!accSet) {
			acc = 1;
			deg = 0;
			deg2 = 0;
			camRot = 0;
			accSet = true;  // 한 번만 acc가 세팅되도록 한다
		}
	}

	if (cz - 250 > 300) {
		stealthNeeling = false;
		cliffEnable = false;
		acc = 1;
		accSet = false;
		delay -= 10;
		if (delay < 10) {
			init();
			timerOperate = false;
		}
	}
}

void shakeDisplay() {
	if (!stealthNeeling) {
		shakeX = 0;
		shakeY = 0;
		shakeZ = 0;
		shakeX = shake_range(gen);
		shakeY = shake_range(gen);
		shakeZ = shake_range(gen);
	}
	else {
		shakeX = 0;
		shakeY = 0;
		shakeZ = 0;
		shakeX = shake_range2(gen);
		shakeY = shake_range2(gen);
		shakeZ = shake_range2(gen);
	}
}

void timerOperation(int value) {
	rotateStealth();
	moveStealth();
	if (!cliffEnable)
		generatePillar();
	movePillar();

	if(cliffEnable)
		updateCliff();

	if(!cliffEnable)
		moveDistance++;

	if (moveDistance > 1000 && !cliffEnable) {
		moveDistance = 0;
		cz = -80;
		cliffEnable = true;
	}

	if (stealthNeeling) {
		fov += acc;
		acc -= 0.01;
		if (acc < 0)
			acc = 0;
		if (fov > 80)
			fov = 80;
	}
	if (!stealthNeeling) {
		fov -= acc;
		acc -= 0.01;

		if (acc < 0)
			acc = 0;
		if (fov < 45)
			fov = 45;

		camMove = sin(deg);
		deg += 0.04;

		camMove2 = sin(deg2);
		deg2 += 0.02;
	}

	gz += speed;
	if (gz > 5)
		gz = 0;

	shakeDisplay();

	if (timerOperate)
		glutTimerFunc(10, timerOperation, 1);
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
