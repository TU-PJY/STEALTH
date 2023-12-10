#include "config.h"  // 중요 헤더 파일
#include "shader.h"  // 셰이더 생성
#include "buffer.h"  // 버퍼
#include "translate.h"  // 변환
#include "gl_func.h"  // GL 기능 함수
#include "screen.h"
#include "soundController.cpp"


FMOD::System *ssystem;
FMOD::Sound *bgm, *engine, *neeling, *wind, *cliffEnd, *bgmHome, *crash;
FMOD::Channel* bgmChannel = 0, *bgmHomeChannel = 0, * engineChannel = 0, * neelingChannel = 0, * windChannel = 0, *cliffChannel = 0, *crashChannel = 0;
void* extradriverdata = 0;
FMOD_RESULT result;

extern GLuint ID;
int projectionMode = modePers;  // 직각투영/원근투영, 기본 원근투영 모드, modeOrtho로 변경 시 알아서 바뀜

int WIDTH = GetSystemMetrics(SM_CXSCREEN);
int HEIGHT = GetSystemMetrics(SM_CYSCREEN);  // 화면 사이즈에 맞추어 창을 출력한다

void controlSound() {  // 사운드 컨트롤
	if (bgmPlay) {
		bgmChannel->stop();
		ssystem->playSound(bgm, 0, false, &bgmChannel);
		bgmChannel->setVolume(0.7);
		bgmPlay = false;
	}
	if (bgmStop) {
		bgmChannel->stop();
		bgmStop = false;
	}

	if (bgmHomePlay) {
		bgmHomeChannel->stop();
		ssystem->playSound(bgmHome, 0, false, &bgmHomeChannel);
		bgmHomeChannel->setVolume(0.6);
		bgmHomePlay = false;
	}
	if (bgmHomeStop) {
		bgmHomeChannel->stop();
		bgmHomeStop = false;
	}

	if (enginePlay) {
		engineChannel->stop();
		ssystem->playSound(engine, 0, false, &engineChannel);
		engineChannel->setVolume(0.6);
		enginePlay = false;
	}
	if (engineStop) {
		engineChannel->stop();
		engineStop = false;
	}

	if (neelingPlay) {
		neelingChannel->stop();
		ssystem->playSound(neeling, 0, false, &neelingChannel);
		neelingChannel->setVolume(0.6);
		neelingPlay = false;
	}
	if (neelingStop) {
		neelingChannel->stop();
		neelingStop = false;
	}

	if (windSoundNum != windSoundNum2) {  // 앞 번호와 뒤 번호가 다를때만 재생한다
		windChannel->stop();
		ssystem->playSound(wind, 0, false, &windChannel);
		windChannel->setVolume(0.5);

		windSoundNum2++;  // 뒤 번호 갱신
	}

	if (cliffPlay) {
		cliffChannel->stop();
		ssystem->playSound(cliffEnd, 0, false, &cliffChannel);
		cliffChannel->setVolume(1.0);
		cliffPlay = false;
	}

	if (crashPlay) {
		crashChannel->stop();
		ssystem->playSound(crash, 0, false, &crashChannel);
		crashPlay = false;
	}
}

GLvoid displayOutput() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glUseProgram(ID);
	
	setCamera(); // 이 부분은 여기서 건들 필요 없음
	setProjection(projectionMode); // 이 부분은 여기서 건들 필요 없음
	setLight();

	// MODEL_COUNT는 config.h에 정의되어있음
	for (int i = 0; i < MODEL_COUNT; i++) {  // MODEL_COUNT 만큼 반복문을 돌며 변환과 출력 반복
		setTransform(i);  // 변환 세팅
		finishTransform(i); // 변환을 glsl로 전달
		modelOutput(i);  // 최종 출력, 3개 함수 모두 modelOutput.cpp에 있음
	}
	
	controlSound();

	glutSwapBuffers();
}

GLvoid displayReshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void main(int argc, char** argv) {
	{  // fold here
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GL_MULTISAMPLE);
		glutInitWindowPosition(X_POS, Y_POS);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("OpenGL");
		glutFullScreen();  // 전체화면으로 전환한다

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cerr << "Unable to initialize GLEW" << endl;  exit(EXIT_FAILURE);
		}
		else cout << "GLEW Initialized" << endl;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		makeShaderProgram();

		result = FMOD::System_Create(&ssystem);
		if (result != FMOD_OK)
			exit(0);

		ssystem->init(64, FMOD_INIT_NORMAL, extradriverdata);
		ssystem->createSound("..//res//sounds//bgm.ogg", FMOD_LOOP_NORMAL, 0, &bgm);
		ssystem->createSound("..//res//sounds//engine.ogg", FMOD_LOOP_NORMAL, 0, &engine);
		ssystem->createSound("..//res//sounds//neeling.ogg", FMOD_LOOP_NORMAL, 0, &neeling);
		ssystem->createSound("..//res//sounds//wind.ogg", FMOD_DEFAULT, 0, &wind);
		ssystem->createSound("..//res//sounds//end_of_cliff.ogg", FMOD_DEFAULT, 0, &cliffEnd);
		ssystem->createSound("..//res//sounds//home_bgm.ogg", FMOD_DEFAULT, 0, &bgmHome);
		ssystem->createSound("..//res//sounds//crash.ogg", FMOD_DEFAULT, 0, &crash);

		ssystem->playSound(bgmHome, 0, false, &bgmHomeChannel);
		bgmHomeChannel->setVolume(0.7);
	}

	// MODEL_COUNT는 config.h에 정의되어있음
	for(int i = 0; i < MODEL_COUNT; i ++)  // MODEL_COUNT 만큼 버퍼 초기화
		setBuffer(i);  // modelBuffer.cpp에 있음
	setTexture();
	
	glutDisplayFunc(displayOutput);
	glutReshapeFunc(displayReshape);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutTimerFunc(10, timerOperation, 1);
	glutMainLoop();
}
