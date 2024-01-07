#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

const char *vertexShaderSource = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = 
  "#version 330 core\n"
  "out vec4 fColor;\n"
  "void main() {\n"
    "fColor = vec4(0.0f, 0.8f, 1.0f, 1.0f);\n"
  "}\0";


GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

GLuint VBO;
GLuint VAO;

void initRenderer() {

  //creeate vertex shader object
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  //create fragment shader object
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  //create shader program
  shaderProgram  = glCreateProgram();
  
  //attach shaders to the program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  //link shaders
  glLinkProgram(shaderProgram);

  //delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //generate vao & vbo
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  //bind vao
  glBindVertexArray(VAO);

  //bind array buffer to vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //add trianngle to vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //tell gl how to read vbo
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  //enable vbo
  glEnableVertexAttribArray(0);

  //bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

void updRenderer() {

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

}
