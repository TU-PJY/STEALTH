// 타이머
#include "gl_func.h"
#include "pillar.h"
#include "soundController.cpp"

extern GLfloat rot, sx, cz, fov, camRot, camMove, camMove2, gz, camY;
extern GLfloat shakeX, shakeY, shakeZ;
GLfloat speed = 2;
GLfloat acc = 0;
GLfloat deg, deg2;
bool rotateRight, rotateLeft; // 각각 오른쪽 회전, 왼쪽 회전
bool cliffEnable;  // true일 시 절벽이 나타난다
bool stealthNeeling;  // true 일 시 세로로 기울인다, 조작에 따른 각도 제어 무시
bool accSet;
bool gameUpdate = false;
bool playWindSound;

int genDelay = 150;  // 장애물 생성 딜레이
int delay = 80;
int num = 0;
int moveDistance = 0;
int level = 0;

GLfloat camAcc = 0.6;

random_device rd;  mt19937 gen(rd());
uniform_real_distribution <GLfloat> dis(-9.0f, 9.0f);
uniform_real_distribution <GLfloat> shake_range(-0.2f, 0.2f);
uniform_real_distribution <GLfloat> shake_range2(-0.9f, 0.9f);
uniform_real_distribution <GLfloat> shake_range3(-1.5f, 1.5f);

void init() {
	delay = 80;
	genDelay = 150;
	num = 0;
	camRot = 0;
	camMove = 0;
	camMove2 = 0;
	shakeX = 0;
	shakeY = 0;
	shakeZ = 0;
	fov = 35;
	gz = 0;
	cz = 0;
	sx = 0;
	rot = 0;
	moveDistance = 0;
	deg = 0;
	deg2 = 0;
	camAcc = 0.6;
	accSet = false;
	cliffEnable = false;
	stealthNeeling = false;
	gameUpdate = false;
}

void rotateStealth() {
	if (!stealthNeeling) {
		if (rotateRight) {  // 오른쪽 회전
			rot -= 6;
			camRot -= 1;
			if (rot < -45)
				rot = -45;
			if (camRot < -30)
				camRot = -30;
		
		}
		if (rotateLeft) {  // 왼쪽 회전
			rot += 6;
			camRot += 1;
			if (rot > 45)
				rot = 45;
			if (camRot > 30)
				camRot = 30;
		
		}

		if (rot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			rot -= 6;
			camRot -= 1;
			if (rot < 0)
				rot = 0;
			
		}
		if (rot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			rot += 6;
			camRot += 1;
			if (rot > 0)
				rot = 0;
		}
		if (camRot > 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			camRot -= 1;
			if (camRot < 0)
				camRot = 0;

		}
		if (camRot < 0 && ((!rotateRight && !rotateLeft) || (rotateRight && rotateLeft))) {
			camRot += 1;
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
			sx += 0.5;
			if (sx > 9)
				sx = 9;
		}
		if (rotateLeft) {  // 왼쪽 이동
			sx -= 0.5;
			if (sx < -9)
				sx = -9;
		}
	}
	else {
		if (sx > 0) {
			sx -= 0.5;
			if (sx < 0)
				sx = 0;
		}
		if (sx < 0) {
			sx += 0.5;
			if (sx > 0)
				sx = 0;
		}
	}
}

void generatePillar() {
	if (genDelay > 0)
		genDelay--;

	else {
		for (int i = 0; i < 2; i++) {
			if (num < 40) {
				p[num].x = dis(gen);
				p[num].z = -80;
				p[num].height = 0;
			}
			num++;
		}
		if (num < 40) {  // 벽에 붙어서 피하는 꼼수 방지
			p[num].x = -9.0;
			p[num].z = -80;
			p[num++].height = 0;
		}
		
		if (num < 40) {
			p[num].x = 9.0;
			p[num].z = -80;
			p[num++].height = 0;
		}
		genDelay = delay;
	}
}

void removePillar(int idx) {
	if (idx == num)
		num--;
	else {
		for (int i = idx; i < num; i++) {
			p[i].x = p[i + 1].x;
			p[i].z = p[i + 1].z;
			p[i].height = p[i + 1].height;
			p[i].type = p[i + 1].type;
		}
		num--;
	}
}

void movePillar() {
	for (int i = 0; i < num; i++) {
		p[i].z += speed;
		p[i].height += 1;

		if (p[i].z == -30)
			playWindSound = true;

		if (p[i].z > 40)
			removePillar(i);
	}
}

void updateCliff() {
	cz += speed;

	if (cz + 250 > 250) {
		stealthNeeling = true;
		if (!accSet) {
			neelingPlay = true;
			windSoundNum++;

			num = 0;
			acc = 1;
			deg = 0;
			deg2 = 0;
			camRot = 0;
			accSet = true;  // 한 번만 acc가 세팅되도록 한다
		}
	}

	if (cz - 250 > 300) {
		neelingStop = true;

		stealthNeeling = false;
		cliffEnable = false;
		acc = 1;
		accSet = false;
		genDelay = 150;
		if (delay > 10)
			delay -= 5;
	}
}

void shakeDisplay() {
	if (!stealthNeeling) {
		shakeX = 0;
		shakeY = 0;
		//shakeZ = 0;
		shakeX = shake_range(gen);
		shakeY = shake_range(gen);
		//shakeZ = shake_range(gen);
	}
	else {
		shakeX = 0;
		shakeY = 0;
		//shakeZ = 0;
		shakeX = shake_range2(gen);
		shakeY = shake_range2(gen);
		//shakeZ = shake_range2(gen);
	}
}

void checkCollision() {
	for (int i = 0; i < num; i++) {  // 충돌 시 게임이 초기화 된다
		if ((p[i].x - 1.5 <= sx && sx <= p[i].x + 1.5) && (8 <= p[i].z && p[i].z <= 12)) {
			camY = -5.0;
			bgmStop = true;
			engineStop = true;
			init();
		}
	}
}

void timerOperation(int value) {
	if (gameUpdate) {
		if (camY < 10.0) {
			camY += camAcc;
			camAcc -= 0.01;
			if (camY > 10.0)
				camY = 10.0;
		}

		rotateStealth();
		moveStealth();

		//checkCollision();
		movePillar();

		if (cliffEnable)
			updateCliff();

		if (!cliffEnable) {
			moveDistance++;
			generatePillar();
		}

		if (moveDistance > 1200 && !cliffEnable) {
			moveDistance = 0;
			cz = -80;
			cliffEnable = true;
		}

		if (stealthNeeling) {
			fov += acc;
			acc -= 0.01;
			if (acc < 0)
				acc = 0;
			if (fov > 120)
				fov = 120;
		}
		if (!stealthNeeling) {
			fov -= acc;
			acc -= 0.01;

			if (acc < 0)
				acc = 0;
			if (fov < 35)
				fov = 35;
		}
	}

	camMove = sin(deg);
	deg += 0.04;

	camMove2 = sin(deg2);
	deg2 += 0.02;

	shakeDisplay();

	if (playWindSound) {
		channelNum++;
		windSoundNum++;
		playWindSound = false;
	}

	glutTimerFunc(10, timerOperation, 1);
	glutPostRedisplay();
}
