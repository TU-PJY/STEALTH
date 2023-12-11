// 타이머
#include "gl_func.h"
#include "pillar.h"
#include "soundController.cpp"

extern GLfloat rot, sx, cz, cliffHeight, stealthPos;  // 모델 변환 변수
extern GLfloat shakeX, shakeY, shakeZ, fov, camRot, camMove, camMove2, camY, camZ, camRot;  // 화면 효과 변수

GLfloat speed = 3;  // 전투기 속도(장애물 다가오는 속도)
GLfloat acc = 0;  // fov 가속도
GLfloat deg, deg2;  // 카메라 흔들림
GLfloat camAcc = 0.6;  // 카메라 가속도

bool rotateRight, rotateLeft; // 각각 오른쪽 회전, 왼쪽 회전
bool cliffEnable;  // true일 시 절벽이 나타난다
bool stealthNeeling;  // true 일 시 세로로 기울인다, 조작에 따른 각도 제어 무시
bool accSet;  // 절벽을 지날 때의 애니메이션 
bool gameUpdate = false;  // true 일 시 게임 업데이트 진행
bool playWindSound;  // 바람 소리 재생 여부

int genDelay = 150;  // 장애물 생성 딜레이
int delay = 100;  // 다음 장애물 생성 딜레이
int moveDistance = 0;  // 이동 거리

int num = 0;  // 장애물 개수

random_device rd;  mt19937 gen(rd());
uniform_real_distribution <GLfloat> dis(-7.0f, 7.0f);  // 장애물 위치
uniform_real_distribution <GLfloat> shake(-0.1f, 0.1f);  // 홈 화면 카메라 흔들림
uniform_real_distribution <GLfloat> shake_range(-0.2f, 0.2f);  // 인 게임 카메라 흔들림
uniform_real_distribution <GLfloat> shake_range2(-0.9f, 0.9f);  // 인 게임 절벽 지날 때의 흔들림

// 게임 오버 시 게임 상태 초기화
void init() {
	delay = 100;
	genDelay = 150;
	num = 0;
	moveDistance = 0;

	camAcc = 0.6;
	camRot = 0;
	camMove = 0;
	camMove2 = 0;

	deg = 0;
	deg2 = 0;

	fov = 35;

	shakeX = 0;
	shakeY = 0;

	cz = 0;
	sx = 0;
	rot = 0;
	
	accSet = false;
	cliffEnable = false;
	stealthNeeling = false;
	gameUpdate = false;
}  

// 전투기 회전
void rotateStealth() {
	if (!stealthNeeling) {
		if (rotateRight) {  // 오른쪽 회전
			rot -= 6;
			camRot -= 1;
			if (rot < -45)
				rot = -45;
			if (camRot < -30)  // 카메라도 같이 회전한다
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

		// 조작키를 떼었을 때 각도가 다시 복구된다
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

	// 절벽을 지날 때 자동으로 수직으로 세운다
	if (stealthNeeling) {
		if (rot < 90) {
			rot += 2;
			if (rot > 90)
				rot = 90;
		}
	}
}

// 전투기 좌우 이동
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
	else {  // 절벽에 진입할 때는 플레이어의 입력을 무시하고 정가운데로 스스로 이동한다
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

// 장애물 생성
void generatePillar() {
	if (genDelay > 0)  // 딜레이가 0이 되면 장애물을 생성한다
		genDelay--;

	else {
		if (num < 40) {
			p[num].x = dis(gen);  // 랜덤 위치 장애물
			p[num].z = -180;
			p[num].height = 0;
			p[num++].width = 5;

			p[num].x = -9.5;  // 벽에 뭍어 피하는 꼼수를 방지
			p[num].z = -180;
			p[num].height = 0;
			p[num++].width = 2;

			p[num].x = 9.5;  // ''
			p[num].z = -180;
			p[num].height = 0;
			p[num++].width = 2;

			genDelay = delay;  // 다시 장애물 생성 딜레이를 설정한다
		}
	}
}

// 화면 뒤로 넘어가면 장애물 삭제
void removePillar(int idx) {
	if (idx == num)
		num--;

	else if (idx < num) {
		for (int i = idx; i < num; i++) {
			p[i].x = p[i + 1].x;
			p[i].z = p[i + 1].z;
			p[i].height = p[i + 1].height;
			p[i].width = p[i + 1].width;
		}
		num--;
	}
}

// 장애물이 앞으로 다가온다(전투기가 앞으로 날아간다)
void movePillar() {
	for (int i = 0; i < num; i++) {
		if (p[i].z > 40)  // 화면 뒤로 넘어가면 삭제한다
			removePillar(i);

		if (p[i].z == -30)  // 전투기가 장애물을 지나치면 바람 소리를 재생한다
			playWindSound = true;

		p[i].z += speed;  // 전투기 속도로 장애물이 다가온다(화면 상에서)
		p[i].height += 0.5;  // 장애물 높이가 점차 높아진다
	}
}

// 다음 레벨로 넘어가기
void updateCliff() {
	cz += speed;  // 절벽이 전투기 속도로 다가온다

	// 절벽에 진입하면
	if (cz + 400 > 400) {
		stealthNeeling = true;  // 전투기는 옆으로 90도 돌린다
		if (!accSet) {
			neelingPlay = true;  // 강력한 엔진 소리를 재생한다
			windSoundNum++;  // 절벽에 진입할 때 바람 소리를 출력한다

			camAcc = 0.6;  // 카메라 가속도 설정
			num = 0;  // 장애물을 0개로 초기화
			acc = 1;  // fov 가속도 설정
			camRot = 0;
			accSet = true;  // 한 번만 acc가 세팅되도록 한다
		}
	}

	// 절벽을 벗어나면
	if (cz - 400 > 450) {
		neelingStop = true;  // 강력한 엔진 소리를 멈춘다

		accSet = false;  // 가속도 설정 상태 해제
		stealthNeeling = false;  // 전투기는 다시 원래 각도로 복귀한다
		cliffEnable = false;  // 더 이상 절벽을 보이지 않는다
		cliffPlay = true;  // 절벽을 벗어날 때 강력한 바람 소리를 재생한다

		acc = 1;  // fov 가속도 설정
		camAcc = 0.6;  // 카메라 가속도 설정

		genDelay = 150;  // 곧바로 장애물이 나오지 않도록 장애물 생성 딜레이를 조금 길게 설정
		if (delay > 10)  // 딜레이가 10인 상태에서는 더 이상 딜레이가 줄어들지 않는다.
			delay -= 10;  // 다음 장애물 생성 딜레이를 설정한다
	}
}

// 화면 흔들림 효과
void shakeDisplay() {
	// 절벽을 지날때와 지나지 않을 때의 흔들리는 정도가 다르다
	if (!stealthNeeling) {
		shakeX = 0;
		shakeY = 0;
		shakeX = shake_range(gen);
		shakeY = shake_range(gen);
	}
	else {
		shakeX = 0;
		shakeY = 0;
		shakeX = shake_range2(gen);
		shakeY = shake_range2(gen);
	}
}

// 홈 화면 흔들립 효과
void shakeDisplayHome() {
	shakeX = 0;
	shakeY = 0;
	shakeX = shake(gen);
	shakeY = shake(gen);
}

// 충돌 처리
void checkCollision() {
	for (int i = 0; i < num; i++) {  // 충돌 시 게임이 초기화 된다
		if ((p[i].x - p[i].width / 2 - 0.5 <= sx && sx <= p[i].x + p[i].width / 2 + 0.5) && (8 <= p[i].z && p[i].z <= 12)) {
			camY = -5.0;  // 충돌 시 홈 화면으로 돌아간다
			crashPlay = true;
			bgmStop = true;
			bgmHomePlay = true;
			engineStop = true;
			camZ = 15.0;
			stealthPos = 100.0;
			init();
		}
	}
}

// 타이머
void timerOperation(int value) {
	if (gameUpdate) {  // 게임 업데이트 활성화 시 아래의 코드들이 동작
		rotateStealth();
		moveStealth();
		movePillar();
		shakeDisplay();
		checkCollision();

		if (cliffEnable) {
			cliffHeight += 0.5;  // 절벽의 높이가 점차 높아진다
			updateCliff();
		}

		if (!cliffEnable) { 
			if(delay > 10)
				moveDistance++;  // 거리를 측정한다
			// 절벽 근처에 가면 더 이상 장애물이 생성되지 않는다. (절벽 바로 앞이나 내부에 생성 방지)
			// 딜레이가 10일때는 무한으로 장애물이 생성된다
			if (moveDistance < 1400)  
				generatePillar();
		}

		// 절벽 위치에 다다르면 절벽을 지날 준비를 한다
		// 딜레이가 10일때는 더 이상 절벽이 나오지 않고 무한으로 장애물이 생성된다
		if (moveDistance > 1500 && !cliffEnable && delay > 10) {
			moveDistance = 0;  
			cz = -150;
			cliffHeight = 0;
			cliffEnable = true;
		}

		// 전투기가 절벽을 지날 때 아래의 코드 동작
		if (stealthNeeling) {
			fov += acc;  // fov 애니메이션
			acc -= 0.01;
			if (acc < 0)
				acc = 0;
			if (fov > 120)
				fov = 120;

			if (camY > -5.0) {  // 카메라 애니메이션
				camY -= camAcc;
				camAcc -= 0.01;
				if (camY < -5.0)
					camY = -5.0;
			}
		}

		// 카메라, fov 원래 상태로 복귀 애니메이션
		if (!stealthNeeling) {
			fov -= acc;
			acc -= 0.01;

			if (acc < 0)
				acc = 0;
			if (fov < 35)
				fov = 35;


			if (camY < 10.0) {
				camY += camAcc;
				camAcc -= 0.01;
				if (camY > 10.0)
					camY = 10.0;
			}
		}

		// 카메라 흔들림 효과 2
		if (!stealthNeeling) {
			camMove = sin(deg) * 1.5;  // x축 흔들림
			deg += 0.04;

			camMove2 = sin(deg2) * 1.5;  // y축 흔들림
			deg2 += 0.02;
		}
		if (stealthNeeling) {
			camMove = sin(deg);  // x축 흔들림
			deg += 0.04;

			camMove2 = sin(deg2);  // y축 흔들림
			deg2 += 0.02;
		}
	}

	// 홈 화면일 때 아래의 코드 동작
	if (!gameUpdate) {
		shakeDisplayHome();

		camMove = sin(deg);  // 인게임보다 조금 느리게 카메라가 흔들린다
		deg += 0.02;

		camMove2 = sin(deg2);
		deg2 += 0.01;
	}

	// 홈 화면 진입 시 카메라가 뒤로 움직이면서 전투기가 뒤에서 앞으로 나오는 장면 연출
	if (camZ < 40) {
		camZ += 0.2;
		if (camZ > 40)
			camZ = 40;
	}
	if (stealthPos > 10) {
		stealthPos -= 0.5;
		if (stealthPos < 10)
			stealthPos = 10;
	}

	if (playWindSound) {  // 바람소리 중복 재생 방지를 위해 여기서 바람 소리 재생 여부 설정
		windSoundNum++;
		playWindSound = false;  // 중복 재생되지 않도록 한다.
	}

	glutTimerFunc(10, timerOperation, 1);
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
