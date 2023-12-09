#include "config.h"  // 중요 헤더 파일
#include "shader.h"  // 셰이더 생성
#include "buffer.h"  // 버퍼
#include "translate.h"  // 변환
#include "gl_func.h"  // GL 기능 함수
#include "screen.h"
#include "soundController.cpp"


FMOD::System *ssystem;
FMOD::Sound *bgm, *engine, *neeling, *wind;
FMOD::Channel* bgmChannel = 0, * engineChannel = 0, * neelingChannel = 0, * windChannel = 0;
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
		bgmPlay = false;
	}
	if (bgmStop) {
		bgmChannel->stop();
		bgmStop = false;
	}

	if (enginePlay) {
		engineChannel->stop();
		ssystem->playSound(engine, 0, false, &engineChannel);
		enginePlay = false;
	}
	if (engineStop) {
		engineChannel->stop();
		engineStop = false;
	}

	if (neelingPlay) {
		neelingChannel->stop();
		ssystem->playSound(neeling, 0, false, &neelingChannel);
		neelingPlay = false;
	}
	if (neelingStop) {
		neelingChannel->stop();
		neelingStop = false;
	}

	if (windSoundNum != windSoundNum2) {  // 빈번하게 플레이 되므로 두 채널이 번갈아 가면서 플레이 한다
		windChannel->stop();
		ssystem->playSound(wind, 0, false, &windChannel);
		windSoundNum2++;
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
