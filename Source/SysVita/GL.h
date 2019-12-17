#ifndef SYSGL_GL_H_
#define SYSGL_GL_H_

#ifdef DAEDALUS_VITA
#include <vitaGL.h>
#include <vita2d.h>
#include <stdlib.h>
#endif

#include "Utility/DaedalusTypes.h"

#define GL_UNPACK_ALIGNMENT               0x0CF5

#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368


//extern GLFWwindow * gWindow;

// FIXME: burn all of this with fire.

void sceGuFog(float mn, float mx, u32 col);

enum EGuTextureWrapMode
{
	GU_CLAMP			= GL_CLAMP_TO_EDGE,
	GU_REPEAT			= GL_REPEAT,
};

enum EGuMatrixType
{
	GU_PROJECTION		= GL_PROJECTION,
};

struct ScePspFMatrix4
{
	float m[16];
};

void glPixelStorei( 	GLenum pname,
  	GLint param);

void sceGuSetMatrix(EGuMatrixType type, const ScePspFMatrix4 * mtx);


#endif // SYSGL_GL_H_
