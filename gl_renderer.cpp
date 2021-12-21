#include "gl_renderer.h"
#include <GLES2/gl2.h>

/*
 * Return the loaded and compiled shader
 */
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
  GLuint shader = glCreateShader(type);
  if (!shader)
  {
    return 0;
  }

  glShaderSource(shader, 1, &shaderSrc, nullptr);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    return 0;
  }

  return shader;
}

/*
 * Initialize the shaders and return the program object
 */
GLuint initProgramObject()
{
  char vShaderStr[] = "#version 300 es                          \n"
                      "layout(location = 0) in vec4 vPosition;  \n"
                      "void main()                              \n"
                      "{                                        \n"
                      "   gl_Position = vPosition;              \n"
                      "}                                        \n";

  char fShaderStr[] = "#version 300 es                              \n"
                      "precision mediump float;                     \n"
                      "out vec4 fragColor;                          \n"
                      "void main()                                  \n"
                      "{                                            \n"
                      "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
                      "}                                            \n";

  GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
  GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

  GLuint programObject = glCreateProgram();
  if (!programObject)
  {
    return 0;
  }

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);

  glLinkProgram(programObject);

  GLint linked;
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    return 0;
  }

  return programObject;
}

/*
 * Draw a triangle
 */
void draw(GLuint programObject, GLint width, GLint height)
{
  GLfloat vVertices[] = {0.0f, 1.0f, 0.0f,
                         -1.0f, -1.0f, 0.0f,
                         1.0f, -1.0f, 0.0f};

  glViewport(0, 0, width, height);
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(programObject);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

class GLRendererImpl
{
  GLuint _program = 0;

public:
  ~GLRendererImpl()
  {
    glDeleteProgram(_program);
  }

  bool initialize()
  {
    _program = initProgramObject();
    if (!_program)
    {
      return false;
    }
    return true;
  }

  void draw(int width, int height)
  {
    ::draw(_program, width, height);
  }
};

GLRenderer::GLRenderer()
    : _impl(new GLRendererImpl)
{
}

GLRenderer::~GLRenderer()
{
  delete _impl;
}

bool GLRenderer::initialize()
{
  return _impl->initialize();
}

void GLRenderer::draw(int width, int height)
{
  _impl->draw(width, height);
}
