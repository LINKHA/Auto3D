#pragma once
#include "../Graphics/Shader.h"
#include "../Engine/ModuleManager.h"
#include "../Resource/ResourceCache.h"
#include "../Graphics/Graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glad.h>

namespace Auto3D
{
/**
 * The number of waves. Also has to be changed in the shader.
 */
#define NUMBERWAVES 4

 /**
  * Length (width and height) of the water surface,
  */
#define WATER_PLANE_LENGTH 128

  /**
   * Size of the normal map texture.
   */
#define TEXTURE_SIZE 1024

typedef unsigned char GLUSboolean;
typedef signed char GLUSbyte;
typedef unsigned char GLUSubyte;
typedef char GLUSchar;
typedef short GLUSshort;
typedef unsigned short GLUSushort;
typedef int GLUSint;
typedef unsigned int GLUSuint;
typedef int GLUSfixed;
typedef long long int  GLUSint64;
typedef unsigned long long int GLUSuint64;
typedef int GLUSsizei;
typedef unsigned int GLUSenum;
// GLUSintptr, GLUSsizeiptr and GLUSsync not implemented.
typedef unsigned int GLUSbitfield;
typedef unsigned short GLUShalf;
typedef float GLUSfloat;
typedef float GLUSclampf;
typedef double GLUSdouble;
typedef double GLUSclampd;

#define GLUS_NO_ERROR 0
#define GLUS_OK 	  0
#define GLUS_TRUE     1
#define GLUS_FALSE    0

#define GLUS_POINTS 									   0x0000

#define GLUS_LINES										   0x0001
#define GLUS_LINE_LOOP									   0x0002
#define GLUS_LINE_STRIP 								   0x0003
#define GLUS_LINES_ADJACENCY							   0x000A
#define GLUS_LINE_STRIP_ADJACENCY						   0x000B

#define GLUS_TRIANGLES									   0x0004
#define GLUS_TRIANGLE_STRIP 							   0x0005
#define GLUS_TRIANGLE_FAN								   0x0006
#define GLUS_TRIANGLES_ADJACENCY						   0x000C
#define GLUS_TRIANGLE_STRIP_ADJACENCY					   0x000D

#define GLUS_UNPACK_ALIGNMENT 							   0x0CF5
#define GLUS_PACK_ALIGNMENT								   0x0D05

#define GLUS_BYTE										   0x1400
#define GLUS_UNSIGNED_BYTE								   0x1401
#define GLUS_SHORT										   0x1402
#define GLUS_UNSIGNED_SHORT								   0x1403
#define GLUS_INT										   0x1404
#define GLUS_UNSIGNED_INT								   0x1405
#define GLUS_FLOAT										   0x1406
#define GLUS_DOUBLE										   0x140A

#define GLUS_VERSION									   0x1F02
#define GLUS_EXTENSIONS									   0x1F03

#define GLUS_COMPILE_STATUS								   0x8B81
#define GLUS_LINK_STATUS								   0x8B82
#define GLUS_VALIDATE_STATUS							   0x8B83
#define GLUS_INFO_LOG_LENGTH							   0x8B84

#define GLUS_FRAMEBUFFER								   0x8D40

#define GLUS_COMPRESSED_R11_EAC                            0x9270
#define GLUS_COMPRESSED_SIGNED_R11_EAC                     0x9271
#define GLUS_COMPRESSED_RG11_EAC                           0x9272
#define GLUS_COMPRESSED_SIGNED_RG11_EAC                    0x9273
#define GLUS_COMPRESSED_RGB8_ETC2                          0x9274
#define GLUS_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2      0x9276
#define GLUS_COMPRESSED_RGBA8_ETC2_EAC                     0x9278

#define GLUS_PI		3.1415926535897932384626433832795f

#define GLUS_LOG_NOTHING	0
#define GLUS_LOG_ERROR   	1
#define GLUS_LOG_WARNING 	2
#define GLUS_LOG_INFO    	3
#define GLUS_LOG_DEBUG   	4
#define GLUS_LOG_SEVERE		5

#define GLUS_VERTICES_FACTOR 4
#define GLUS_VERTICES_DIVISOR 4

#define GLUS_MAX_STRING 	256

#define GLUS_MAX_FILENAME	2048

#ifndef GLUS_BASE_DIRECTORY
#define GLUS_BASE_DIRECTORY ""
#endif

#define GLUSindex GLUSuint

#define GLUS_BASE_DIRECTORY ""
#define GLUS_MAX_STRING 	256

#define GLUS_MAX_FILENAME	2048
#define GLUS_NO_ERROR 0
#define GLUS_OK 	  0
#define GLUS_TRUE     1
#define GLUS_FALSE    0

#define GLUS_MAX_TEXTFILE_LENGTH 2147483646

#define GLUSvoid void


#define GLUS_VERTEX_SHADER              0x00008B31
#define GLUS_FRAGMENT_SHADER            0x00008B30
#define GLUS_TESS_EVALUATION_SHADER     0x00008E87
#define GLUS_TESS_CONTROL_SHADER        0x00008E88
#define GLUS_GEOMETRY_SHADER            0x00008DD9
#define GLUS_COMPUTE_SHADER 			0x000091B9

#define GLUS_VERTEX_SHADER_BIT 			0x00000001
#define GLUS_FRAGMENT_SHADER_BIT 		0x00000002
#define GLUS_GEOMETRY_SHADER_BIT 		0x00000004
#define GLUS_TESS_CONTROL_SHADER_BIT 	0x00000008
#define GLUS_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GLUS_COMPUTE_SHADER_BIT 		0x00000020
#define GLUS_ALL_SHADER_BITS 			0xFFFFFFFF

#define GLUS_COMPILE_STATUS								   0x8B81
#define GLUS_LINK_STATUS								   0x8B82
#define GLUS_VALIDATE_STATUS							   0x8B83
#define GLUS_INFO_LOG_LENGTH							   0x8B84

#define GLUS_LOG_NOTHING	0
#define GLUS_LOG_ERROR   	1
#define GLUS_LOG_WARNING 	2
#define GLUS_LOG_INFO    	3
#define GLUS_LOG_DEBUG   	4
#define GLUS_LOG_SEVERE		5

#define GLUS_MAX_CHARS_LOGGING 2047

#define GLUS_MAX_DIMENSION 16384

#define GLUS_RED  						0x00001903
#define GLUS_ALPHA  					0x00001906
#define GLUS_RGB    					0x00001907
#define GLUS_RGBA   					0x00001908
#define GLUS_LUMINANCE  				0x00001909


#define GLUS_SINGLE_CHANNEL GLUS_RED

#define GLUS_MAX_VERTICES 1048576
#define GLUS_MAX_INDICES  (GLUS_MAX_VERTICES*GLUS_VERTICES_FACTOR)

static const char* GLUS_LOG_STRINGS[] = {
	"",
	"ERROR",
	"WARNING",
	"INFO",
	"DEBUG",
	"SEVERE"
};


typedef struct _GLUStgaimage
{
	/**
		* Width of the TGA image.
		*/
	GLUSushort width;

	/**
		* Height of the TGA image.
		*/
	GLUSushort height;

	/**
		* Depth of the image.
		*/
	GLUSushort depth;

	/**
		* Pixel data.
		*/
	GLUSubyte* data;

	/**
		* Format of the TGA image. Can be:
		*
		* GLUS_RGB
		* GLUS_RGBA
		* GLUS_LUMINANCE
		*
		* Last entry can also be interpreted as GLUS_ALPHA.
		*/
	GLUSenum format;

} GLUStgaimage;

typedef struct _GLUStextfile
{
	/**
		* Contains the data of the text file.
		*/
	GLUSchar* text;

	/**
		* The length of the text file without the null terminator.
		*/
	GLUSint length;

} GLUStextfile;

typedef struct _GLUSprogram
{
	/**
		* The created program.
		*/
	GLUSuint program;

	/**
		* Compute shader.
		*/
	GLUSuint compute;

	/**
		* Vertex shader.
		*/
	GLUSuint vertex;

	/**
		* Tessellation control shader.
		*/
	GLUSuint control;

	/**
		* Tessellation evaluation shader.
		*/
	GLUSuint evaluation;

	/**
		* Geometry shader.
		*/
	GLUSuint geometry;

	/**
		* Fragment shader.
		*/
	GLUSuint fragment;

} GLUSprogram;

typedef struct _GLUSshape
{
	/**
		* Vertices in homogeneous coordinates.
		*/
	GLUSfloat* vertices;

	/**
		* Normals.
		*/
	GLUSfloat* normals;

	/**
		* Tangents.
		*/
	GLUSfloat* tangents;

	/**
		* Bitangents.
		*/
	GLUSfloat* bitangents;

	/**
		* Texture coordinates.
		*/
	GLUSfloat* texCoords;

	/**
		* All above values in one array. Not created by the model loader.
		*/
	GLUSfloat* allAttributes;

	/**
		* Indices.
		*/
	GLUSindex* indices;

	/**
		* Number of vertices.
		*/
	GLUSuint numberVertices;

	/**
		* Number of indices.
		*/
	GLUSuint numberIndices;

	/**
		* Triangle render mode - could be either:
		*
		* GLUS_TRIANGLES
		* GLUS_TRIANGLE_STRIP
		*/
	GLUSenum mode;

} GLUSshape;



GLUSfloat glusVector3Dotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

GLUSfloat glusVector2Dotf(const GLUSfloat vector0[2], const GLUSfloat vector1[2]);

GLUSvoid glusVector3Crossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

FILE* glusFileOpen(const char* filename, const char* mode);

void glusMemoryFree(void* pointer);

int glusFileClose(FILE* stream);

void* glusMemoryMalloc(size_t size);

GLUSboolean _glusFileCheckRead(FILE* f, size_t actualRead, size_t expectedRead);
GLUSvoid glusFileDestroyText(GLUStextfile* textfile);
GLUSboolean glusFileLoadText(const GLUSchar* filename, GLUStextfile* textfile);

GLUSvoid glusLogPrint(GLUSuint verbosity, const char* format, ...);


GLUSvoid glusProgramDestroy(GLUSprogram* shaderprogram);

GLUSboolean glusProgramCreateFromSource(GLUSprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);


GLUSboolean glusProgramLink(GLUSprogram* shaderProgram);

GLUSboolean glusProgramBuildFromSource(GLUSprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);

GLUSvoid glusImageDestroyTga(GLUStgaimage* tgaimage);

//static GLUSvoid glusImageSwapColorChannel(GLUSint width, GLUSint height, GLUSenum format, GLUSubyte* data);
char* file_name(const char* filename);
GLUSboolean glusImageLoadTga(const GLUSchar* filename, GLUStgaimage* tgaimage);


//static GLUSvoid glusShapeInitf(GLUSshape* shape);
//static GLUSboolean glusShapeCheckf(GLUSshape* shape);

GLUSvoid glusShapeDestroyf(GLUSshape* shape);

//static GLUSboolean glusShapeFinalizef(GLUSshape* shape);

GLUSboolean glusShapeCreatePlanef(GLUSshape* shape, const GLUSfloat halfExtend);

//////////////////////////////////
GLUSfloat glusVector3Lengthf(const GLUSfloat vector[3]);

GLUSboolean glusVector3Normalizef(GLUSfloat vector[3]);

GLUSboolean glusMatrix4x4Orthof(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);

GLUSboolean glusMatrix4x4Frustumf(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);

GLUSvoid glusMatrix4x4Identityf(GLUSfloat matrix[16]);

GLUSboolean glusMatrix4x4Perspectivef(GLUSfloat result[16], const GLUSfloat fovy, const GLUSfloat aspect, const GLUSfloat zNear, const GLUSfloat zFar);
GLUSvoid glusMatrix4x4Multiplyf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16]);

GLUSvoid glusMatrix4x4Translatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);

GLUSvoid glusMatrix4x4LookAtf(GLUSfloat result[16], const GLUSfloat eyeX, const GLUSfloat eyeY, const GLUSfloat eyeZ, const GLUSfloat centerX, const GLUSfloat centerY, const GLUSfloat centerZ, const GLUSfloat upX, const GLUSfloat upY, const GLUSfloat upZ);



///////////////////////////////////////////////////////////////////////////

GLUSfloat glusMathDegToRadf(const GLUSfloat degrees);

GLUSvoid glusQuaternionRotateRyf(GLUSfloat quaternion[4], const GLUSfloat angle);

GLUSvoid glusQuaternionGetMatrix4x4f(GLUSfloat matrix[16], const GLUSfloat quaternion[4]);
GLUSvoid glusMatrix4x4MultiplyVector3f(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3]);

GLUSboolean glusShapeCreateSpheref(GLUSshape* shape, const GLUSfloat radius, const GLUSuint numberSlices);


GLUSvoid glusMatrix4x4GetScalef(GLUSfloat scales[3], const GLUSfloat matrix[16]);

GLUSvoid glusMatrix4x4Transposef(GLUSfloat matrix[16]);
GLUSvoid glusMatrix4x4InverseRigidBodyf(GLUSfloat matrix[16]);

GLUSvoid glusMatrix4x4ExtractMatrix3x3f(GLUSfloat matrix[9], const GLUSfloat source[16]);

GLUSvoid glusMatrix4x4Copyf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly);
}