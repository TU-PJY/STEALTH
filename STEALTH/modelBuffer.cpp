// 모델 버퍼 
#include "buffer.h"
#include "model.h"
#include "texture.h"

vector<Vertex> model_1;  // 모델 여러 종류 추가 시 model_2, model_3... 식으로 벡터 추가
vector<Vertex> model_2;
vector<Vertex> model_3;

GLuint vertexCount_1 = loadObj("..//res//model//stealth.obj", model_1);  // 모델 여러 종류 추가 시 vertexCount_2, vertexCount_3... 식으로 추가
GLuint vertexCount_2 = loadObj("..//res//model//front.obj", model_2);
GLuint vertexCount_3 = loadObj("..//res//model//front.obj", model_3);

GLfloat vertexData[][48] = {
	// 앞
	{
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0, 0.0
	},
	// 뒤
	{
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0, 0.0,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0, 0.0,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0, 1.0,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0, 1.0,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0, 1.0,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0, 0.0
	},
	// 오
	{
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0, 0.0,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0, 0.0,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0, 1.0,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0, 1.0,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0, 1.0,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0, 0.0
	},
	// 왼
	{
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0, 0.0,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0, 0.0,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0, 1.0,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0, 1.0,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0, 1.0,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0, 0.0
	},
	// 아래
	{
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0, 0.0,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0, 0.0,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0, 1.0,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0, 1.0,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0, 1.0,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0, 0.0
	},
	// 위
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0, 0.0,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0, 0.0,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0, 1.0,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0, 1.0,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0, 1.0,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0, 0.0
	}
};

GLuint VAO[MODEL_COUNT], VBO;  // MODEL_COUNT는 config.h에 정의되어있음
BITMAPINFO* bmp;
unsigned int texture[5];
unsigned char* texture_data;


void vertexInput(int idx) {  // vertex
	switch (idx) {
	case 0:
		glBufferData(GL_ARRAY_BUFFER, model_1.size() * sizeof(Vertex), model_1.data(), GL_STATIC_DRAW);
		break;
	case 1:
		glBufferData(GL_ARRAY_BUFFER, model_2.size() * sizeof(Vertex), model_2.data(), GL_STATIC_DRAW);
		break;
	case 2:
		glBufferData(GL_ARRAY_BUFFER, model_2.size() * sizeof(Vertex), model_3.data(), GL_STATIC_DRAW);
		break;
	}

	if(3 <= idx && idx <= 46)  // 장애물 및 절벽
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vertexData), vertexData, GL_STATIC_DRAW);
}

void setBuffer(int idx) {
	glGenVertexArrays(1, &VAO[idx]);
	glBindVertexArray(VAO[idx]);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vertexInput(idx);

	if (idx < 3) {  // 전투기, 배경, 땅
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}
	else {  // 장애물
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // 노말 속성
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}
}

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;

	//--- 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	//--- 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}

	//--- 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}

	//--- BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	//--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}

	//--- 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}

	//--- 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * (*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);

	//--- 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}

	//--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}

	fclose(fp);
	return bits;
}

void setTexture() {
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = LoadDIBitmap("..//res//texture//texture_stealth.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = LoadDIBitmap("..//res//texture//texture_sky.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = LoadDIBitmap("..//res//texture//texture_land.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 380, 380, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = LoadDIBitmap("..//res//texture//texture_wall.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = LoadDIBitmap("..//res//texture//texture_pillar.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);
}

