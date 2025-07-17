#include "orbit.h"

void tst::Orbit::Create(Rendering::Batch& centerObj, Rendering::Batch& rockObj, unsigned int amount)
{
    center = centerObj;
    rock = rockObj;

    centerModelMatrix = glm::mat4(1.0f);
    centerModelMatrix = glm::translate(centerModelMatrix, glm::vec3(0.0f, -70.0f, 0.0f));
    centerModelMatrix = glm::scale(centerModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));

    this->amount = amount;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 250.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }
}

void tst::Orbit::Draw(glm::mat4& projView)
{
    glm::mat4 MVP = projView * centerModelMatrix;
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    texture.Use();
    center.Use();
    // draw meteorites
    glm::mat4 MVP0 = projView * modelMatrices[0];
    rockShader.Use();
    glUniformMatrix4fv(uniformMVPLocation, 1, GL_FALSE, &MVP0[0][0]);
    glUniform1f(uniformModifireLocation, sinf(glfwGetTime()));
    texture.Use();
    rock.Use();
    for (unsigned int i = 1; i < amount; i++)
    {
        glm::mat4 MVP = projView * modelMatrices[i];
        rockShader.Use();
        glUniformMatrix4fv(uniformMVPLocation, 1, GL_FALSE, &MVP[0][0]);
        //glUniform1f(glGetUniformLocation(rockShader.programId, "modifire"), sinf(glfwGetTime()));
        texture.Use();
        rock.Use();
    }
}
