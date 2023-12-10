#pragma once
#include "config.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords; // 추가된 부분
 };

GLuint loadObj(const char* filename, vector<Vertex>& vertices);

extern vector<Vertex> model_1;
extern vector<Vertex> model_2;
extern vector<Vertex> model_3;
extern vector<Vertex> model_4;
extern GLuint vertexCount_1;
extern GLuint vertexCount_2;
extern GLuint vertexCount_3;
extern GLuint vertexCount_4;