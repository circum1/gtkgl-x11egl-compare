/*
 * gles-renderer.h
 *
 *  Created on: Jan 11, 2017
 *      Author: fery
 */

#ifndef FERY_LINUX_GTK_GLES_DEMO_GLES_RENDERER_H_
#define FERY_LINUX_GTK_GLES_DEMO_GLES_RENDERER_H_

#include <GLES3/gl3.h>

namespace GLES_RENDERER {

extern GLint attr_pos;
extern GLint attr_color;
extern GLint u_matrix;
extern GLint u_texture_unit_location;
extern GLint u_scrollingTexture_unit_location;
//extern GLint u_horizontal_shift;
extern GLint u_offsets;

void createShaders(void);
GLuint loadTexture( const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels);

void renderFrame();
void initGlRenderer();
} // namespace

#endif /* FERY_LINUX_GTK_GLES_DEMO_GLES_RENDERER_H_ */
