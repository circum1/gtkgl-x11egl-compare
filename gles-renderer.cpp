/*
 * gles-renderer.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: fery
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "gles-renderer.h"
#include "util.h"

namespace GLES_RENDERER {

//GLint attr_pos;
//GLint attr_color;
//GLint u_matrix;
GLint u_texture_unit_location;
//GLint u_scrollingTexture_unit_location;
//GLint u_horizontal_shift;
//GLint u_offsets;

GLuint vao;
GLuint mvp_location;
GLuint position_index;
float mvp[16];
GLuint program;

static void
init_buffers (GLuint  position_index,
              GLuint *vao_out)
{
  GLuint vao, buffer;
  const GLfloat verts[][2] = {
      {  1, -1 },
      { -1, -1 },
      {  1,  1 },
      { -1,  1 }
  };

  /* we need to create a VAO to store the other buffers */
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);

  /* this is the VBO that holds the vertex data */
  glGenBuffers (1, &buffer);
  glBindBuffer (GL_ARRAY_BUFFER, buffer);
  glBufferData (GL_ARRAY_BUFFER, sizeof (verts), verts, GL_STATIC_DRAW);

  /* enable and set the position attribute */
  glEnableVertexAttribArray (position_index);
  glVertexAttribPointer(position_index, 2, GL_FLOAT, GL_FALSE, 0, 0);

  /* reset the state; we will re-enable the VAO when needed */
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);

  /* the VBO is referenced by the VAO */
  glDeleteBuffers (1, &buffer);

  if (vao_out != NULL)
    *vao_out = vao;
}

static bool
init_shaders (GLuint   *program_out,
              GLuint   *mvp_location_out,
              GLuint   *position_location_out
              )
{
  GLuint& program = *program_out;
  GLuint& mvp_location = *mvp_location_out;
  GLuint& position_location = *position_location_out;

  /* get the location of the "mvp" uniform */
  mvp_location = glGetUniformLocation (program, "mvp");
  position_location = glGetAttribLocation (program, "position");

  return program != 0;
}

static void
gl_init ()
{
  /* initialize the shaders and retrieve the program data */
  if (!init_shaders (&program,
                     &mvp_location,
                     &position_index
                     ))
    {
      assert(false);
      return;
    }
}

void printCompileError(GLuint shader) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
    glDeleteShader(shader); // Don't leak the shader.
    fprintf(stderr, "Error: %s\n", &errorLog[0]);
}

GLuint createShader(const char *filename, GLenum type) {
    char *text = loadFile(filename);
    GLuint shaderId = glCreateShader(type);
    GLint stat;
    glShaderSource(shaderId, 1, &text, NULL);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &stat);
    if (!stat) {
       printf("Error: shader %s did not compile!\n", filename);
       printCompileError(shaderId);
       printf("Source:\n%s\n", text);
       exit(1);
    }
    free(text);
    return shaderId;
}

void
createShaders(void)
{
   GLuint fragShader, vertShader;
   GLint stat;


//   fragShader = createShader("opengl30.frag", GL_FRAGMENT_SHADER);
//   vertShader = createShader("opengl30.vert", GL_VERTEX_SHADER);
   fragShader = createShader("gles20.frag", GL_FRAGMENT_SHADER);
   vertShader = createShader("gles20.vert", GL_VERTEX_SHADER);

   program = glCreateProgram();
   glAttachShader(program, fragShader);
   glAttachShader(program, vertShader);
   glLinkProgram(program);

   glGetProgramiv(program, GL_LINK_STATUS, &stat);
   if (!stat) {
      char log[1000];
      GLsizei len;
      glGetProgramInfoLog(program, 1000, &len, log);
      printf("Error: linking:\n%s\n", log);
      exit(1);
   }

   glUseProgram(program);

//   if (0) {
//      /* test setting attrib locations */
//      glBindAttribLocation(program, attr_pos, "pos");
//      glBindAttribLocation(program, attr_color, "color");
//      glLinkProgram(program);  /* needed to put attribs into effect */
//   }
//   else {
//      /* test automatic attrib locations */
//      attr_pos = glGetAttribLocation(program, "pos");
//      attr_color = glGetAttribLocation(program, "color");
//   }

//   u_matrix = glGetUniformLocation(program, "modelviewProjection");
   u_texture_unit_location = glGetUniformLocation(program, "u_TextureUnit");
//   u_scrollingTexture_unit_location = glGetUniformLocation(program, "u_ScrollingTextureUnit");
//   u_horizontal_shift = glGetUniformLocation(program, "u_horizontalShift");
//   u_offsets = glGetUniformLocation(program, "u_offsets");

//   printf("Uniform modelviewProjection at %d\n", u_matrix);
//   printf("Attrib pos at %d\n", attr_pos);
//   printf("Attrib color at %d\n", attr_color);
}

GLuint loadTexture(
                    const GLsizei width, const GLsizei height,
                    const GLenum type, const GLvoid* pixels) {
    GLuint texture_object_id;
    glGenTextures(1, &texture_object_id);
    assert(texture_object_id != 0);

    glBindTexture(GL_TEXTURE_2D, texture_object_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
//    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture_object_id;
}

GLuint texture;

void init_mvp(float *res) {
    /* initialize a matrix as an identity matrix */
    res[0] = 1.f; res[4] = 0.f; res[8] = 0.f; res[12] = 0.f; res[1] = 0.f; res[5] = 1.f; res[9] = 0.f; res[13] = 0.f;
    res[2] = 0.f; res[6] = 0.f; res[10] = 1.f; res[14] = 0.f; res[3] = 0.f; res[7] = 0.f; res[11] = 0.f; res[15] = 1.f;
}

void renderFrame(void) {
//    static const GLfloat verts[][2] = { { 1, -1 }, { -1, -1 }, { 1, 1 },
//            { -1, 1 } };
//    static const GLfloat colors[][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
//            { 1, 0, 0 }, };

    static int counter = 0;
    static int initialized;
    static Timer fullTimer;
    static int fullcounter = 0;
    static double fulltime = 0.0;
    static double avgtime = 0.0;
    static double maxtime = 0.0;
    static double mintime = 100000000000000.0;
    static usec_time firstFrame = 0;
    static usec_time prevFrame = getCurrentTimeInMicroSec();
    static usec_time lastFrame = getCurrentTimeInMicroSec();
    static usec_time minFrame = 1000000000;
    static usec_time maxFrame = 0;

    static char *staticLayer;

    if (!initialized) {
        staticLayer = loadFile("testpic.raw");
        fullTimer.start();
        initialized = 1;
    }

    for (int i = 0; i < 480; i++) {
        ((GLuint*) staticLayer)[230 * 480 + i] = 0xff000000 + (counter << 2);
    }

    Timer t;
    glUseProgram (program);

    /* Set modelview/projection matrix */
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0]);
    glBindVertexArray (vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    t.start();
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 480, 234, GL_RGBA,
                GL_UNSIGNED_BYTE, staticLayer);
        glUniform1i(u_texture_unit_location, 0);

//        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
//        glVertexAttribPointer(attr_color, 3, GL_FLOAT, GL_FALSE, 0, colors);
//        glEnableVertexAttribArray(attr_pos);
//        glEnableVertexAttribArray(attr_color);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//        glDisableVertexAttribArray(attr_pos);
//        glDisableVertexAttribArray(attr_color);
        glBindVertexArray (0);
    }
    glFinish();
    t.stop();

    avgtime += t.getElapsedTimeInMicroSec();
    fulltime += t.getElapsedTimeInMicroSec();
    fullcounter++;

    if (fullcounter == 100) {
        fulltime = 0.0;
        fullTimer.start();
        firstFrame = getCurrentTimeInMicroSec();
    }

    usec_time now = getCurrentTimeInMicroSec();

    maxtime = std::max(t.getElapsedTimeInMicroSec(), maxtime);
    mintime = std::min(t.getElapsedTimeInMicroSec(), mintime);
    maxFrame = std::max(now-prevFrame, maxFrame);
    minFrame = std::min(now-prevFrame, minFrame);

    prevFrame = now;

    if (++counter % 120 == 0) {
       printf("Full avg compute time: %.0f\n", fulltime / (fullcounter-100));
        printf("1 sec avg compute time: %.0f\n", avgtime / counter);
        printf("Min compute time: %d\n", (int)mintime);
        printf("Max compute time: %d\n", (int)maxtime);

        if (fullcounter>100) {
            printf("Full avg FPS: %2.2f\n", 1.0*(fullcounter-100)/(now-firstFrame)*1000000);
            printf("1 sec FPS: %2.2f\n", 1.0*(counter)/(now-lastFrame)*1000000);
            printf("Min frame time: %d\n", (int)minFrame);
            printf("Max frame time: %d\n\n", (int)maxFrame);
        }
        counter = 0;
        avgtime = 0;
        lastFrame = now;
        avgtime = maxtime = 0.0;
        mintime = 100000000000000.0;
        minFrame = 100000000;
        maxFrame = 0;

    }
}

void initGlRenderer() {
//    glClearColor(0.4, 0.4, 0.4, 0.0);
    int i;
    printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
    printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
    printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
    printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &i);
    printf("Uniform components = %d\n", i);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &i);
    printf("Combined components = %d\n", i);

    createShaders();

    init_mvp(mvp);
    gl_init();

    /* initialize the vertex buffers */
    init_buffers (position_index, &vao);

    char *img = (char*) calloc(1, 480 * 234 * 4);
    texture = loadTexture(480, 234, GL_RGBA, img);
    free(img);
}


} // namespace
