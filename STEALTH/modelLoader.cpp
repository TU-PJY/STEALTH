#include "model.h"
#include "config.h"

GLuint loadObj(const char* filename, vector<Vertex>& vertices) {
    using namespace glm;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open obj file: " << filename << endl;
        return 0;
    }

    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> texCoords; // 추가된 부분

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "v") {
            vec3 position;
            iss >> position.x >> position.y >> position.z;

            positions.push_back(position);
        }

        else if (type == "vn") {
            vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;

            normals.push_back(normal);
        }

        else if (type == "vt") { // 추가된 부분
            vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;

            texCoords.push_back(texCoord);
        }

        else if (type == "f") {
            for (int i = 0; i < 3; ++i) {
                string vertex;
                iss >> vertex;
                istringstream vertexStream(vertex);
                string indexStr;

                getline(vertexStream, indexStr, '/');
                int index = stoi(indexStr) - 1;
                Vertex v;
                v.position = positions[index];

                getline(vertexStream, indexStr, '/');
                if (!indexStr.empty()) {
                    int texCoordIndex = stoi(indexStr) - 1;
                    v.texCoords = texCoords[texCoordIndex];
                }

                getline(vertexStream, indexStr, '/');
                if (!indexStr.empty()) {
                    int normalIndex = stoi(indexStr) - 1;
                    v.normal = normals[normalIndex];
                }
                vertices.push_back(v);
            }
        }
    }

    return vertices.size();
}