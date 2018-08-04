#include "Auto.h"
#ifdef _OPENGL_4_PLUS_
#include "Graphics.h"
#include "GLGather.h"
#include "GLDebug.h"
#include "GraphicsDefines.h"

#include "../../EngineSetting/Optimize.h"
namespace Auto3D {


static const unsigned glFillType[] =
{
	GL_FILL,
	GL_LINE,
	GL_POINT
};

static const unsigned glChangeMode[] =
{
	GL_STATIC_DRAW,	
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW	
};

static const unsigned glBufferMode[] =
{
	GL_ARRAY_BUFFER ,
	GL_ELEMENT_ARRAY_BUFFER 
};

static const unsigned glDepthMode[] =
{
	GL_ALWAYS,		
	GL_NEVER,		
	GL_LESS,		
	GL_EQUAL,		
	GL_LEQUAL,		
	GL_GREATER,		
	GL_NOTEQUAL,	
	GL_GEQUAL	
};

static const unsigned glStencilOps[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

static const unsigned glBlendSrcFu[] =
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA
};

static const unsigned glBlendDestFu[] =
{
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

static const unsigned glBlendOp[] =
{
	GL_FUNC_ADD,
	GL_FUNC_REVERSE_SUBTRACT
};

static const unsigned glElementTypes[] =
{
	GL_INT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE
};

static void GetGLPrimitiveType(unsigned elementCount, PrimitiveTypes type, unsigned& primitiveCount, GLenum& glPrimitiveType)
{
	switch (type)
	{
	case PRIM_TYPE_TRIANGLE_LIST:
		primitiveCount = elementCount / 3;
		glPrimitiveType = GL_TRIANGLES;
		break;

	case PRIM_TYPE_LINE_LIST:
		primitiveCount = elementCount / 2;
		glPrimitiveType = GL_LINES;
		break;

	case PRIM_TYPE_POINT_LIST:
		primitiveCount = elementCount;
		glPrimitiveType = GL_POINTS;
		break;

	case PRIM_TYPE_TRIANGLE_STRIP:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_STRIP;
		break;

	case PRIM_TYPE_LINE_STRIP:
		primitiveCount = elementCount - 1;
		glPrimitiveType = GL_LINE_STRIP;
		break;

	case PRIM_TYPE_TRIANGLE_FAN:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_FAN;
		break;
	}
}

void Graphics::RegisterDebug()
{
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
bool Graphics::BeginFrame()
{
	if (!IsInitialized() || IsDeviceLost())
		return false;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SetColorWrite(true);
	SetDepthWrite(true);
	Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL);
	return true;
}
void Graphics::EndFrame()
{
	if (!IsInitialized())
		return;
	SDL_GL_SwapWindow(_window);
}

void Graphics::Clear(unsigned flags, const Color & color, float depth, unsigned stencil)
{
	bool oldColorWrite = _colorWrite;
	bool oldDepthWrite = _depthWrite;

	if (flags & CLEAR_COLOR && !oldColorWrite)
		SetColorWrite(true);
	if (flags & CLEAR_DEPTH && !oldDepthWrite)
		SetDepthWrite(true);
	if (flags & CLEAR_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(MATH_MAX_UNSIGNED);

	unsigned glFlags = 0;
	if (flags & CLEAR_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	if (flags & CLEAR_DEPTH)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & CLEAR_STENCIL)
	{
		glFlags |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(glFlags);
}

void Graphics::SetColorWrite(bool enable)
{
	if (enable != _colorWrite)
	{
		if (enable)
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		else
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		_colorWrite = enable;
	}
}
void Graphics::SetDepthWrite(bool enable)
{
	if (enable != _depthWrite)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
		_depthWrite = enable;
	}
}
void Graphics::Draw(PrimitiveTypes type,unsigned vertexStart,unsigned vertexCount)
{
	if (!vertexCount)
		return;
	unsigned primitiveCount;
	GLenum glPrimitiveType;

	GetGLPrimitiveType(vertexCount, type, primitiveCount, glPrimitiveType);
	glDrawArrays(glPrimitiveType, vertexStart, vertexCount);

	_numPrimitives += primitiveCount;
	_numBatches++;
}

void Graphics::Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount)
{
	//if (!indexCount || !indexBuffer_ || !indexBuffer_->GetGPUObjectName())
	//	return;


	//unsigned indexSize;// = _indexBuffer->GetIndexSize();
	//unsigned primitiveCount;
	//GLenum glPrimitiveType;

	//GetGLPrimitiveType(indexCount, type, primitiveCount, glPrimitiveType);
	//GLenum indexType = indexSize == sizeof(unsigned short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	//glDrawElements(glPrimitiveType, indexCount, indexType, reinterpret_cast<const GLvoid*>(indexStart * indexSize));

	//_numPrimitives += primitiveCount;
	//_numBatches++;
}

void Graphics::DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,
	unsigned instanceCount)
{
	//glDrawElementsInstanced();
	//_numPrimitives += primitiveCount;
	//_numBatches++;
}
}
#endif //_OPENGL_4_PLUS_