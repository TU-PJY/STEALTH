#version 330 core

in vec3 fragPos; // 정점 셰이더로부터 전달받은 정점 위치
in vec3 normal;  // 정점 셰이더로부터 전달받은 법선 벡터

out vec4 fragColor; // 최종 출력 색상

uniform vec3 lightPos;    // 빛의 위치
uniform vec3 viewPos;     // 카메라 위치
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // 앰비언트
    float ambientLight = 0.3;
    vec3 ambient = ambientLight * lightColor;
    
    // 빛의 방향 계산
    vec3 lightDir = normalize(lightPos - fragPos);

    // 법선 벡터 정규화
    vec3 normalVector = normalize(normal);

    // 디퓨즈 조명 계산
    float diff = max(dot(normalVector, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;

    int shininess = 128;
    // 스페큘러 조명 계산
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalVector);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular =  spec * lightColor;

    // 최종 색상 계산
    vec3 result = (ambient + diffuse + specular) * objectColor;

    // 최종 출력 색상 설정
    fragColor = vec4(result, 1.0);
}
