// 모델 변환 
#include "translate.h"
#include "model.h"
#include "screen.h"
#include "texture.h"
#include "pillar.h"

extern GLuint ID;
extern GLuint VAO[MODEL_COUNT];  // MODEL_COUNT는 config.h에 정의되어있음
extern bool cliffEnable;
extern bool stealthNeeling;
extern bool gameUpdate;

glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix;

unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
unsigned int lightPosLocation, lightColorLocation, objColorLocation;

GLfloat rot;  // 전투기 회전 각도
GLfloat sx;  // 전투기 좌우 위치
GLfloat width;
GLfloat cz; // 절벽 위치
GLfloat fov = 35;
GLfloat gz;

GLfloat shakeX, shakeY, shakeZ;
GLfloat camRot;

GLfloat camMove, camMove2;
GLfloat camZ = 15.0f;
GLfloat stealthPos = 100.0f;  // to 10.0

GLfloat camY = -5.0;

GLfloat cliffHeight;

extern int num;

Pillar p[50];

void finishTransform(int idx) {  // 변환 전달 
	projectionLocation = glGetUniformLocation(ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	viewLocation = glGetUniformLocation(ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	lightPosLocation = glGetUniformLocation(ID, "lightPos"); // lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	lightColorLocation = glGetUniformLocation(ID, "lightColor"); // lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	objColorLocation = glGetUniformLocation(ID, "objectColor"); // object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation,1.0, 1.0, 1.0);

	viewPosLocation = glGetUniformLocation(ID, "viewPos"); // viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	modelLocation = glGetUniformLocation(ID, "model"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(transformMatrix)); // modelTransform 변수에 변환 값 적용하기

	glBindVertexArray(VAO[idx]);  // 각 모델마다 지정된 VAO만 사용
}

void setCamera() {  // 카메라 세팅
	using namespace glm;
	view = mat4(1.0f);
	cameraPos = vec3(0.0f, camY, camZ);
	cameraDirection = vec3(0.0f, 0.0f, 0.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	view = lookAt(cameraPos, cameraDirection, cameraUp);
	view = translate(view, vec3(camMove, camMove2, 0.0));
	view = rotate(view, radians(-camRot), vec3(0.0, 0.0, 1.0));
	if(!gameUpdate)
		view = rotate(view, radians(8.0f), vec3(0.0, 1.0, 0.0));
}

void setProjection(int projectionMode) {  // 투영 세팅
	using namespace glm;
	projection = mat4(1.0f);
	GLfloat ratio = 1.0 * WIDTH / HEIGHT;  // 화면 비율을 구하여 모델이 제대로 나오도록 함
	switch (projectionMode) {
	case modePers:
		projection = perspective(radians(fov), ratio, 0.1f, 300.0f);
		break;
	case modeOrtho:
		projection = ortho(-3.0f, 3.0f, -3.0f, 3.0f, 1.0f, 100.0f);
		break;
	}
}

void setLight() {  // 조명 세팅
	using namespace glm;
	lightMatrix = mat4(1.0f);
	lightPos = vec3(0.0f, 50.0f, 20.0f);  // 조명 위치
	// 여기에 빛 변환 추가

	vec3 initialLightPos = vec3(lightPos.x, lightPos.y, lightPos.z);
	lightPos = lightMatrix * vec4(initialLightPos, 1.0f);
}

void setTransform(int idx) {  // 변환 세팅
	using namespace glm;
	transformMatrix = mat4(1.0f);  // 최종 행렬

	scaleMatrix = mat4(1.0f);  // 신축 행렬
	rotateMatrix = mat4(1.0f);  // 회전 행렬
	translateMatrix = mat4(1.0f);  // 이동 행렬

	translateMatrix = translate(translateMatrix, vec3(shakeX, shakeY, shakeZ));

	switch (idx) {  // 변환 추가 
	case 0:  // 전투기 변환
		translateMatrix = translate(translateMatrix, vec3(sx, -5.0, stealthPos));  // 화면 하단에 위치해야 하므로 y축으로 -5.0만큼 이동
		translateMatrix = scale(translateMatrix, vec3(0.5, 0.5, 0.5));
		translateMatrix = rotate(translateMatrix, radians(rot), vec3(0.0, 0.0, 1.0));  // 제자리 회전
		break;

	case 1:  // 배경
		translateMatrix = translate(translateMatrix, vec3(0.0, 0.0, -200.0));
		translateMatrix = scale(translateMatrix, vec3(100.0, 300.0, 0.1));
		break;

	case 2:  // 땅
		translateMatrix = translate(translateMatrix, vec3(0.0, -10.0, gz));
		translateMatrix = scale(translateMatrix, vec3(10.0, 1.0, 10.0));
		translateMatrix = rotate(translateMatrix, radians(GLfloat(-90)), vec3(1.0, 0.0, 0.0));
		break;

	case 3: // 왼쪽 절벽
		translateMatrix = translate(translateMatrix, vec3(-17.0, -5.0, cz - 400));
		translateMatrix = scale(translateMatrix, vec3(30, cliffHeight, 800));
		break;

	case 4: // 오른쪽 절벽
		translateMatrix = translate(translateMatrix, vec3(17.0, -5.0, cz - 400));
		translateMatrix = scale(translateMatrix, vec3(30, cliffHeight, 800));
		break;

	case 5:  // 왼쪽벽
		translateMatrix = translate(translateMatrix, vec3(-25.0, 0.0, 0.0));
		translateMatrix = scale(translateMatrix, vec3(30, 100, 500));
		break;

	case 6:  // 오른쪽벽
		translateMatrix = translate(translateMatrix, vec3(25.0, 0.0, 0.0));
		translateMatrix = scale(translateMatrix, vec3(30, 100, 500));
		break;
	}

	if (6 < idx && idx < num + 7) {  // 장애물
		translateMatrix = translate(translateMatrix, vec3(p[idx - 7].x, -10.0, p[idx - 7].z));
		translateMatrix = scale(translateMatrix, vec3(p[idx - 7].width, p[idx - 7].height, 2));
	}

	transformMatrix = scaleMatrix * rotateMatrix * translateMatrix;  // 최종 변환
}

void modelOutput(int idx) {  // 모델 출력 
	switch (idx) {
	case 0:  // 전투기 출력
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glDrawArrays(GL_TRIANGLES, 0, model_1.size());
		break;

	case 1:  // 배경 출력
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 0, model_2.size());
		break;

	case 2:  // 땅 출력
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glDrawArrays(GL_TRIANGLES, 0, model_3.size());
		break;

	case 3: case 4:
		if (cliffEnable) {
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		break;

	case 5: case 6:
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		break;
	}

	if (6 < idx && idx < num + 7) {
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
