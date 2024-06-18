#include <cglm/vec3.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include "Audio.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

Shader shader;
Camera camera;
vec3 camPos = {4, 4, 4};
vec3 lightPos;

Texture tile;
Object plane;

Texture tableTex;
Object table;

Texture skyTexture;
Object sky;

Texture marioTexture;
Object mario;

Sound testSound;

void initRenderer() {

    createSound(&testSound, "test.wav", true, 20);
    playSound(&testSound);

    createShader(&shader);
    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);


    createTexture(&tile, "tile.png");
    createObject(&plane, &tile, "plane.obj", 0, 0, 0,    3, 3, 3,    0, 0, 0);

    createTexture(&tableTex, "table.png");
    createObject(&table, &tableTex, "table.obj", 0, 0, 0,    1, 1, 1,    0, 0, 0);

    createTexture(&skyTexture, "sky2.png");
    createObject(&sky, &skyTexture, "sky.obj", 0, 0, 0,    2, 2, 2,    0, 0, 0);
    sky.model.lit = false;

    createTexture(&marioTexture, "mario.png");
    createObject(&mario, &marioTexture, "mario.obj", -8, 1.2, 8,    1, 1, 1,    0, 0, 0);


    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    lightPos[0] = 10;
    lightPos[1] = 10;
    lightPos[2] = 10;

}

void renderObject(Object* object) {
    
    glBindVertexArray(object->model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->model.VBO);
    glBufferData(GL_ARRAY_BUFFER, (object->model.postnCount + object->model.indexCount + object->model.normlCount) * sizeof(Vertex), object->model.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->model.indexCount * 3, object->model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    mat4 transformationMatrix;
    createTransformationMatrix(&transformationMatrix, object);
    setMatrix(&shader, "transMatrix", transformationMatrix);

    useShader(&shader);
    setBool(&shader, "lightEnabled", object->model.lit);

    setVec3(&shader, "viewPos", camera.pos);
    setVec3(&shader, "lightPos", lightPos);

    glBindTexture(GL_TEXTURE_2D, object->model.texture.id);
    
    glDrawElements(GL_TRIANGLES, object->model.indexCount, GL_UNSIGNED_INT, 0);

}

void render() {
    cameraMatrix(&camera, 67.0f, 0.1f, 200.0f, &shader, "camMatrix");
    cameraMove(&camera);
    printf("\r%f  %f  %f", camera.pos[0], camera.pos[1], camera.pos[2]);
    fflush(stdout);
    glm_vec3_copy(camera.pos, sky.position);

    renderObject(&table);
    renderObject(&plane);
    //renderObject(&sky);
    renderObject(&mario);
}
