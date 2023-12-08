// 모델 버퍼 
#include "buffer.h"
#include "model.h"

vector<Vertex> model_1;  // 모델 여러 종류 추가 시 model_2, model_3... 식으로 벡터 추가
GLuint vertexCount_1 = loadObj("여기에 파일 경로 작성", model_1);  // 모델 여러 종류 추가 시 vertexCount_2, vertexCount_3... 식으로 추가
// model.h에도 동일하게 추가해야함

GLuint VAO[MODEL_COUNT], VBO;  // MODEL_COUNT는 config.h에 정의되어있음

void vertexInput(int idx) {  // vertex
	switch (idx) {
	case 0:
		glBufferData(GL_ARRAY_BUFFER, model_1.size() * sizeof(Vertex), model_1.data(), GL_STATIC_DRAW);
		break;
	}
}

void setBuffer(int idx) {
	glGenVertexArrays(1, &VAO[idx]);
	glBindVertexArray(VAO[idx]);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vertexInput(idx);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
}