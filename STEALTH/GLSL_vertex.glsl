#version 330 core

layout(location = 0) in vec3 inPosition; // 정점 위치 attribute
layout(location = 1) in vec3 inNormal;   // 정점 법선 attribute

out vec3 fragPos; // 프래그먼트 셰이더로 전달할 정점 위치
out vec3 normal;  // 프래그먼트 셰이더로 전달할 법선 벡터

uniform mat4 model;      // 모델 행렬
uniform mat4 view;       // 뷰 행렬
uniform mat4 projection; // 투영 행렬

void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    fragPos = vec3(model * vec4(inPosition, 1.0));
    normal = vec3(transpose(inverse(model)) * vec4(inNormal, 1.0));
}
