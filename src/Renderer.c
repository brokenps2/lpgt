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
vec3 soundPos = {0, 0, 0};
vec3 lightPos;

float frameVertices[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

unsigned int fbo;
unsigned int rbo;
unsigned int textureColorbuffer;
unsigned int pvao;
unsigned int pvbo;

Texture tile;
Object plane;

Texture tableTex;
Object table;

Texture skyTexture;
Object sky;

Texture marioTexture;
Object mario;

Texture radioTexture;
Object radio;

Texture baseColor;
Object disco;

Texture spectTex;
Object spect;

Sound testSound;

time_t t;

void initRenderer() {

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_TRUE);

    createSound(&testSound, "audio/test2.wav", true, 2, soundPos);
    //playSoundFrom(&testSound, 5);

    createShader(&shader);
    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);

    createTexture(&tile, "images/tile.png");
    createObject(&plane, &tile, "models/plane.obj", 0, 0, 0,    4, 4, 4,    0, 0, 0);

    createTexture(&tableTex, "images/table.png");
    createObject(&table, &tableTex, "models/table.obj", 0, 0, 0,    1, 1, 1,    0, 0, 0);

    createTexture(&skyTexture, "images/sky2.png");
    createObject(&sky, &skyTexture, "models/sky.obj", 0, 0, 0,    2.9, 2.9, 2.9,    0, 0, 0);
    sky.model.lit = false;

    createTexture(&marioTexture, "images/mario.png");
    createObject(&mario, &marioTexture, "models/mario.obj", 7, 1.2, -3,    1, 1, 1,    0, 0, 0);

    createTexture(&radioTexture, "images/radio.png");
    createObject(&radio, &radioTexture, "models/radio.obj", 0, 2.5, 0,    1, 1, 1,    0, 70, 0);

    createTexture(&baseColor, "images/basicColors.png");
    createObject(&disco, &baseColor, "models/disco.obj", 0, 5, 0,    2, 2, 2,    0, 0, 0);

    createTexture(&spectTex, "images/spect.png");
    createObject(&spect, &spectTex, "models/spect.obj", -5, 3, 5,    1, 1, 1,    0, 0, 0);


    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    lightPos[0] = 20;
    lightPos[1] = 20;
    lightPos[2] = 20;

}

void renderObject(Object* object) {

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(object->model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->model.VBO);
    glBufferData(GL_ARRAY_BUFFER, (object->model.indexCount) * sizeof(Vertex), object->model.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->model.indexCount, object->model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    mat4 transformationMatrix;
    loadTransformationMatrix(&transformationMatrix, object);
    setMatrix(&shader, "transMatrix", transformationMatrix);

    useShader(&shader);
    setBool(&shader, "lightEnabled", object->model.lit);
    setBool(&shader, "frame", false);

    setVec3(&shader, "viewPos", camera.pos);
    setVec3(&shader, "lightPos", lightPos);

    glBindTexture(GL_TEXTURE_2D, object->model.texture.id);

    glDrawElements(GL_TRIANGLES, object->model.indexCount, GL_UNSIGNED_INT, 0);

}

void render() {


    cameraMatrix(&camera, 67.0f, 0.1f, 200.0f, &shader, "camMatrix");
    cameraMove(&camera);
    printf("\r%f  %f  %f     %f", camera.pos[0], camera.pos[1], camera.pos[2], radio.rotation[1]);
    fflush(stdout);
    glm_vec3_copy(camera.pos, sky.position);
    sky.rotation[1] += 0.02;

    disco.position[0] += (sin(glfwGetTime()) / 10) * 2;
    disco.position[2] += (cos(glfwGetTime()) / 10) * 2;
    disco.rotation[1] += 1.5;

    if(isKeyDown(GLFW_KEY_0)) {
        radio.rotation[1] += 2;
    }
    if(isKeyDown(GLFW_KEY_9)) {
        radio.rotation[1] -= 2;
    }

    updateAudio(camera.pos, camera.direction);

    renderObject(&table);
    renderObject(&disco);
    //renderObject(&mario);
    renderObject(&radio);
    //renderObject(&spect);
    renderObject(&plane);

}
