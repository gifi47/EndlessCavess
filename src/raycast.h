#ifndef RAYCAST_H
#define RAYCAST_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include "Rendering/Shader.h"

#include "chunk.h"
#include "world.h"

struct RaycastResult {
    bool hit;
    glm::ivec3 blockPos;
    glm::ivec3 faceNormal;
};

RaycastResult Raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float maxDistance, const Chunk& chunk) {
    glm::vec3 localOrigin = rayOrigin - glm::vec3(chunk.x * CHUNK_SIZE_X, chunk.y * CHUNK_SIZE_Y, chunk.z * CHUNK_SIZE_Z);

    glm::ivec3 currentBlock = glm::ivec3(std::round(localOrigin.x), std::round(localOrigin.y), std::round(localOrigin.z));
    glm::vec3 rayStep = glm::sign(rayDirection);
    glm::vec3 deltaDist = glm::abs(1.0f / rayDirection);

    glm::vec3 sideDist;
    for (int i = 0; i < 3; ++i) {
        if (rayDirection[i] < 0) {
            sideDist[i] = (localOrigin[i] - (currentBlock[i] - 0.5f)) * deltaDist[i];
        } else {
            sideDist[i] = ((currentBlock[i] + 0.5f) - localOrigin[i]) * deltaDist[i];
        }
    }

    glm::ivec3 step;
    int axis;
    float traveledDistance = 0.0f;

    while (traveledDistance < maxDistance) {
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            sideDist.x += deltaDist.x;
            localOrigin.x += rayStep.x;
            axis = 0;
        } else if (sideDist.y < sideDist.z) {
            sideDist.y += deltaDist.y;
            localOrigin.y += rayStep.y;
            axis = 1;
        } else {
            sideDist.z += deltaDist.z;
            localOrigin.z += rayStep.z;
            axis = 2;
        }

        if (localOrigin.x < -0.5f || localOrigin.x >= CHUNK_SIZE_X -0.5f ||
            localOrigin.y < -0.5f || localOrigin.y >= CHUNK_SIZE_Y -0.5f ||
            localOrigin.z < -0.5f || localOrigin.z >= CHUNK_SIZE_Z -0.5f) {
            break;
        }
        currentBlock.x = std::round(localOrigin.x);
        currentBlock.y = std::round(localOrigin.y);
        currentBlock.z = std::round(localOrigin.z);
        if (chunk.blocks[currentBlock.x][currentBlock.y][currentBlock.z] != 0) {
            RaycastResult result;
            result.hit = true;
            result.blockPos = glm::ivec3(
                chunk.x * CHUNK_SIZE_X + currentBlock.x,
                chunk.y * CHUNK_SIZE_Y + currentBlock.y,
                chunk.z * CHUNK_SIZE_Z + currentBlock.z
            );

            result.faceNormal = glm::ivec3(0);
            if (axis == 0) result.faceNormal.x = -rayStep.x;
            else if (axis == 1) result.faceNormal.y = -rayStep.y;
            else result.faceNormal.z = -rayStep.z;

            return result;
        }

        traveledDistance += glm::abs(rayDirection[axis]);
    }

    return { false, glm::ivec3(0), glm::ivec3(0) };
}

RaycastResult Raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float maxDistance, World& world) {
    glm::vec3 localOrigin = rayOrigin;

    glm::ivec3 currentBlock = glm::ivec3(std::round(localOrigin.x), std::round(localOrigin.y), std::round(localOrigin.z));

    glm::vec3 rayStep = glm::sign(rayDirection);
    glm::vec3 deltaDist = glm::abs(1.0f / rayDirection);

    glm::vec3 sideDist;
    for (int i = 0; i < 3; ++i) {
        if (rayDirection[i] < 0) {
            sideDist[i] = (localOrigin[i] - (currentBlock[i] - 0.5f)) * deltaDist[i];
        } else {
            sideDist[i] = ((currentBlock[i] + 0.5f) - localOrigin[i]) * deltaDist[i];
        }
    }

    if (world.GetBlock(currentBlock.x, currentBlock.y, currentBlock.z) != 0){
        RaycastResult result;
        result.hit = true;
        result.blockPos = currentBlock;
        result.faceNormal = glm::ivec3(0);
        
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            result.faceNormal.x = -rayStep.x;
        } else if (sideDist.y < sideDist.z) {
            result.faceNormal.y = -rayStep.y;
        } else {
            result.faceNormal.z = -rayStep.z;
        }
        return result;
    }

    glm::ivec3 step;
    int axis;
    float traveledDistance = 0.0f;

    while (traveledDistance < maxDistance) {
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            sideDist.x += deltaDist.x;
            localOrigin.x += rayStep.x;
            axis = 0;
        } else if (sideDist.y < sideDist.z) {
            sideDist.y += deltaDist.y;
            localOrigin.y += rayStep.y;
            axis = 1;
        } else {
            sideDist.z += deltaDist.z;
            localOrigin.z += rayStep.z;
            axis = 2;
        }

        currentBlock.x = std::round(localOrigin.x);
        currentBlock.y = std::round(localOrigin.y);
        currentBlock.z = std::round(localOrigin.z);

        if (world.GetBlock(currentBlock.x, currentBlock.y, currentBlock.z) != 0) {
            RaycastResult result;
            result.hit = true;
            result.blockPos = currentBlock;

            result.faceNormal = glm::ivec3(0);
            if (axis == 0) result.faceNormal.x = -rayStep.x;
            else if (axis == 1) result.faceNormal.y = -rayStep.y;
            else result.faceNormal.z = -rayStep.z;

            return result;
        }

        traveledDistance += glm::abs(rayDirection[axis]);
    }

    return { false, glm::ivec3(0), glm::ivec3(0) };
}

class RayDebugDrawer {
private:
    GLuint VAO, VBO;
    std::vector<float> lineVertices;
    
public:
    RayDebugDrawer() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glm::vec3 s(0, 0, 0);
        glm::vec3 e(1, 0, 0);
        UpdateRay(s, e);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
    }
    
    void UpdateRay(const glm::vec3& start, const glm::vec3& end, float thickness = 0.2f) {
        /*lineVertices = {
            start.x, start.y, start.z,
            end.x, end.y, end.z
        };*/
        glm::vec3 direction = glm::normalize(end - start);
        glm::vec3 perpendicular = glm::cross(direction, glm::vec3(0, 1, 0)); // Перпендикулярный вектор
        perpendicular = glm::normalize(perpendicular) * thickness;
        std::vector<float> quadVertices = {
            start.x + perpendicular.x, start.y + perpendicular.y, start.z + perpendicular.z,
            start.x - perpendicular.x, start.y - perpendicular.y, start.z - perpendicular.z,
            end.x + perpendicular.x, end.y + perpendicular.y, end.z + perpendicular.z,
            end.x - perpendicular.x, end.y - perpendicular.y, end.z - perpendicular.z,
        };
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_DYNAMIC_DRAW);
    }
    
    void Draw(Rendering::Shader& shader, glm::vec3 color, const glm::mat4& MVP) {
        
        glDisable(GL_CULL_FACE);
        shader.Use();
        glUniform3fv(glGetUniformLocation(shader.programId, "color"), 1, &color[0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);
    }
    
    ~RayDebugDrawer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif //RAYCAST_H