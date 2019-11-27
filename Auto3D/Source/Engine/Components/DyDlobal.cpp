#include "DyDlobal.h"

namespace Auto3D
{

	GLUSfloat glusVector3Dotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3])
	{
		return vector0[0] * vector1[0] + vector0[1] * vector1[1] + vector0[2] * vector1[2];
	}

	GLUSfloat glusVector2Dotf(const GLUSfloat vector0[2], const GLUSfloat vector1[2])
	{
		return vector0[0] * vector1[0] + vector0[1] * vector1[1];
	}

	GLUSvoid glusVector3Crossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3])
	{
		GLUSint i;

		GLUSfloat temp[3];

		temp[0] = vector0[1] * vector1[2] - vector0[2] * vector1[1];
		temp[1] = vector0[2] * vector1[0] - vector0[0] * vector1[2];
		temp[2] = vector0[0] * vector1[1] - vector0[1] * vector1[0];

		for (i = 0; i < 3; i++)
		{
			result[i] = temp[i];
		}
	}

	FILE* glusFileOpen(const char* filename, const char* mode)
	{
		char buffer[GLUS_MAX_FILENAME];

		if (!filename)
		{
			// Note: Automatic errno setting.
			return fopen(filename, mode);
		}

		if (strlen(filename) + strlen(GLUS_BASE_DIRECTORY) >= GLUS_MAX_FILENAME)
		{
			// Note: Automatic errno setting.
			return fopen(filename, mode);
		}

		strcpy(buffer, GLUS_BASE_DIRECTORY);
		strcat(buffer, filename);

		return fopen(buffer, mode);
	}

	void glusMemoryFree(void* pointer)
	{
		free(pointer);
	}

	int glusFileClose(FILE* stream)
	{
		return fclose(stream);
	}

	void* glusMemoryMalloc(size_t size)
	{
		return malloc(size);
	}


	GLUSboolean _glusFileCheckRead(FILE* f, size_t actualRead, size_t expectedRead)
	{
		if (!f)
		{
			return GLUS_FALSE;
		}

		if (actualRead < expectedRead)
		{
			glusFileClose(f);

			return GLUS_FALSE;
		}

		return GLUS_TRUE;
	}

	GLUSvoid glusFileDestroyText(GLUStextfile* textfile)
	{
		if (!textfile)
		{
			return;
		}

		if (textfile->text)
		{
			glusMemoryFree(textfile->text);

			textfile->text = 0;
		}

		textfile->length = 0;
	}

	GLUSboolean glusFileLoadText(const GLUSchar* filename, GLUStextfile* textfile)
	{
		FILE* f;
		size_t elementsRead;

		if (!filename || !textfile)
		{
			return GLUS_FALSE;
		}

		textfile->text = 0;

		textfile->length = 0;

		f = glusFileOpen(filename, "rb");

		if (!f)
		{
			return GLUS_FALSE;
		}

		if (fseek(f, 0, SEEK_END))
		{
			glusFileClose(f);

			return GLUS_FALSE;
		}

		textfile->length = ftell(f);

		if (textfile->length < 0 || textfile->length == GLUS_MAX_TEXTFILE_LENGTH)
		{
			glusFileClose(f);

			textfile->length = 0;

			return GLUS_FALSE;
		}

		textfile->text = (GLUSchar*)glusMemoryMalloc((size_t)textfile->length + 1);

		if (!textfile->text)
		{
			glusFileClose(f);

			textfile->length = 0;

			return GLUS_FALSE;
		}

		memset(textfile->text, 0, (size_t)textfile->length + 1);

		rewind(f);

		elementsRead = fread(textfile->text, 1, (size_t)textfile->length, f);

		if (!_glusFileCheckRead(f, elementsRead, (size_t)textfile->length))
		{
			glusFileDestroyText(textfile);

			return GLUS_FALSE;
		}

		glusFileClose(f);

		return GLUS_TRUE;
	}



	static GLUSuint g_verbosity = GLUS_LOG_INFO;


	GLUSvoid glusLogPrint(GLUSuint verbosity, const char* format, ...)
	{
		if (g_verbosity == GLUS_LOG_NOTHING || verbosity == GLUS_LOG_NOTHING)
		{
			return;
		}

		if (g_verbosity >= verbosity)
		{
			const char* logString = "UNKNOWN";
			char buffer[GLUS_MAX_CHARS_LOGGING + 1];
			va_list argList;

			if (verbosity > GLUS_LOG_NOTHING&& verbosity <= GLUS_LOG_SEVERE)
			{
				logString = GLUS_LOG_STRINGS[verbosity];
			}

			buffer[GLUS_MAX_CHARS_LOGGING] = '\0';

			va_start(argList, format);

			vsnprintf(buffer, GLUS_MAX_CHARS_LOGGING, format, argList);

			printf("LOG [%s]: %s\n", logString, buffer);

			va_end(argList);
		}
	}


	GLUSvoid glusProgramDestroy(GLUSprogram* shaderprogram)
	{
		if (!shaderprogram)
		{
			return;
		}

		if (shaderprogram->program)
		{
			glDeleteProgram(shaderprogram->program);

			shaderprogram->program = 0;
		}

		if (shaderprogram->fragment)
		{
			glDeleteShader(shaderprogram->fragment);

			shaderprogram->fragment = 0;
		}

		if (shaderprogram->geometry)
		{
			glDeleteShader(shaderprogram->geometry);

			shaderprogram->geometry = 0;
		}

		if (shaderprogram->evaluation)
		{
			glDeleteShader(shaderprogram->evaluation);

			shaderprogram->evaluation = 0;
		}

		if (shaderprogram->control)
		{
			glDeleteShader(shaderprogram->control);

			shaderprogram->control = 0;
		}

		if (shaderprogram->vertex)
		{
			glDeleteShader(shaderprogram->vertex);

			shaderprogram->vertex = 0;
		}

		if (shaderprogram->compute)
		{
			glDeleteShader(shaderprogram->compute);

			shaderprogram->compute = 0;
		}
	}

	GLUSboolean glusProgramCreateFromSource(GLUSprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
	{
		GLUSint compiled;

		GLUSint logLength, charsWritten;

		char* log;

		if (!shaderProgram || !vertexSource || !fragmentSource)
		{
			return GLUS_FALSE;
		}

		shaderProgram->program = 0;
		shaderProgram->compute = 0;
		shaderProgram->vertex = 0;
		shaderProgram->control = 0;
		shaderProgram->evaluation = 0;
		shaderProgram->geometry = 0;
		shaderProgram->fragment = 0;

		shaderProgram->vertex = glCreateShader(GLUS_VERTEX_SHADER);

		glShaderSource(shaderProgram->vertex, 1, (const char**)vertexSource, 0);

		glCompileShader(shaderProgram->vertex);

		glGetShaderiv(shaderProgram->vertex, GLUS_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			glGetShaderiv(shaderProgram->vertex, GLUS_INFO_LOG_LENGTH, &logLength);

			log = (char*)glusMemoryMalloc((size_t)logLength);

			if (!log)
			{
				return GLUS_FALSE;
			}

			glGetShaderInfoLog(shaderProgram->vertex, logLength, &charsWritten, log);

			glusLogPrint(GLUS_LOG_ERROR, "Vertex shader compile error:");
			glusLogPrint(GLUS_LOG_ERROR, "%s", log);

			glusMemoryFree(log);

			shaderProgram->vertex = 0;

			return GLUS_FALSE;
		}

		if (controlSource)
		{
			shaderProgram->control = glCreateShader(GLUS_TESS_CONTROL_SHADER);

			glShaderSource(shaderProgram->control, 1, (const char**)controlSource, 0);

			glCompileShader(shaderProgram->control);

			glGetShaderiv(shaderProgram->control, GLUS_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				glGetShaderiv(shaderProgram->control, GLUS_INFO_LOG_LENGTH, &logLength);

				log = (char*)glusMemoryMalloc((size_t)logLength);

				if (!log)
				{
					glusProgramDestroy(shaderProgram);

					return GLUS_FALSE;
				}

				glGetShaderInfoLog(shaderProgram->control, logLength, &charsWritten, log);

				glusLogPrint(GLUS_LOG_ERROR, "Control shader compile error:");
				glusLogPrint(GLUS_LOG_ERROR, "%s", log);

				glusMemoryFree(log);

				shaderProgram->control = 0;

				glusProgramDestroy(shaderProgram);

				return GLUS_FALSE;
			}
		}

		if (evaluationSource)
		{
			shaderProgram->evaluation = glCreateShader(GLUS_TESS_EVALUATION_SHADER);

			glShaderSource(shaderProgram->evaluation, 1, (const char**)evaluationSource, 0);

			glCompileShader(shaderProgram->evaluation);

			glGetShaderiv(shaderProgram->evaluation, GLUS_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				glGetShaderiv(shaderProgram->evaluation, GLUS_INFO_LOG_LENGTH, &logLength);

				log = (char*)glusMemoryMalloc((size_t)logLength);

				if (!log)
				{
					glusProgramDestroy(shaderProgram);

					return GLUS_FALSE;
				}

				glGetShaderInfoLog(shaderProgram->evaluation, logLength, &charsWritten, log);

				glusLogPrint(GLUS_LOG_ERROR, "Evaluation shader compile error:");
				glusLogPrint(GLUS_LOG_ERROR, "%s", log);

				glusMemoryFree(log);

				shaderProgram->evaluation = 0;

				glusProgramDestroy(shaderProgram);

				return GLUS_FALSE;
			}
		}

		if (geometrySource)
		{
			shaderProgram->geometry = glCreateShader(GLUS_GEOMETRY_SHADER);

			glShaderSource(shaderProgram->geometry, 1, (const char**)geometrySource, 0);

			glCompileShader(shaderProgram->geometry);

			glGetShaderiv(shaderProgram->geometry, GLUS_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				glGetShaderiv(shaderProgram->geometry, GLUS_INFO_LOG_LENGTH, &logLength);

				log = (char*)glusMemoryMalloc((size_t)logLength);

				if (!log)
				{
					glusProgramDestroy(shaderProgram);

					return GLUS_FALSE;
				}

				glGetShaderInfoLog(shaderProgram->geometry, logLength, &charsWritten, log);

				glusLogPrint(GLUS_LOG_ERROR, "Geometry shader compile error:");
				glusLogPrint(GLUS_LOG_ERROR, "%s", log);

				glusMemoryFree(log);

				shaderProgram->geometry = 0;

				glusProgramDestroy(shaderProgram);

				return GLUS_FALSE;
			}
		}

		shaderProgram->fragment = glCreateShader(GLUS_FRAGMENT_SHADER);

		glShaderSource(shaderProgram->fragment, 1, (const char**)fragmentSource, 0);

		glCompileShader(shaderProgram->fragment);

		glGetShaderiv(shaderProgram->fragment, GLUS_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			glGetShaderiv(shaderProgram->fragment, GLUS_INFO_LOG_LENGTH, &logLength);

			log = (char*)glusMemoryMalloc((size_t)logLength);

			if (!log)
			{
				glusProgramDestroy(shaderProgram);

				return GLUS_FALSE;
			}

			glGetShaderInfoLog(shaderProgram->fragment, logLength, &charsWritten, log);

			glusLogPrint(GLUS_LOG_ERROR, "Fragment shader compile error:");
			glusLogPrint(GLUS_LOG_ERROR, "%s", log);

			glusMemoryFree(log);

			shaderProgram->fragment = 0;

			glusProgramDestroy(shaderProgram);

			return GLUS_FALSE;
		}

		shaderProgram->program = glCreateProgram();

		glAttachShader(shaderProgram->program, shaderProgram->vertex);

		if (shaderProgram->control)
		{
			glAttachShader(shaderProgram->program, shaderProgram->control);
		}

		if (shaderProgram->evaluation)
		{
			glAttachShader(shaderProgram->program, shaderProgram->evaluation);
		}

		if (shaderProgram->geometry)
		{
			glAttachShader(shaderProgram->program, shaderProgram->geometry);
		}

		glAttachShader(shaderProgram->program, shaderProgram->fragment);

		return GLUS_TRUE;
	}


	GLUSboolean glusProgramLink(GLUSprogram* shaderProgram)
	{
		GLUSint linked;

		GLUSint logLength, charsWritten;

		char* log;

		if (!shaderProgram)
		{
			return GLUS_FALSE;
		}

		glLinkProgram(shaderProgram->program);

		glGetProgramiv(shaderProgram->program, GLUS_LINK_STATUS, &linked);

		if (!linked)
		{
			glGetProgramiv(shaderProgram->program, GLUS_INFO_LOG_LENGTH, &logLength);

			log = (char*)glusMemoryMalloc((size_t)logLength);

			if (!log)
			{
				glusProgramDestroy(shaderProgram);

				return GLUS_FALSE;
			}

			glGetProgramInfoLog(shaderProgram->program, logLength, &charsWritten, log);

			glusLogPrint(GLUS_LOG_ERROR, "Shader program link error:");
			glusLogPrint(GLUS_LOG_ERROR, "%s", log);

			glusMemoryFree(log);

			shaderProgram->program = 0;

			glusProgramDestroy(shaderProgram);

			return GLUS_FALSE;
		}

		return GLUS_TRUE;
	}

	GLUSboolean glusProgramBuildFromSource(GLUSprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
	{
		if (!glusProgramCreateFromSource(shaderProgram, vertexSource, controlSource, evaluationSource, geometrySource, fragmentSource))
		{
			return GLUS_FALSE;
		}

		return glusProgramLink(shaderProgram);
	}

	GLUSvoid glusImageDestroyTga(GLUStgaimage* tgaimage)
	{
		if (!tgaimage)
		{
			return;
		}

		if (tgaimage->data)
		{
			glusMemoryFree(tgaimage->data);

			tgaimage->data = 0;
		}

		tgaimage->width = 0;

		tgaimage->height = 0;

		tgaimage->depth = 0;

		tgaimage->format = 0;
	}

	static char* resourcePath = NULL;

	char lastFileName[_MAX_PATH];

	static GLUSvoid glusImageSwapColorChannel(GLUSint width, GLUSint height, GLUSenum format, GLUSubyte* data)
	{
		GLUSint i;
		GLUSubyte temp;
		GLUSint bytesPerPixel = 3;

		if (!data)
		{
			return;
		}

		if (format == GLUS_RGBA)
		{
			bytesPerPixel = 4;
		}

		// swap the R and B values to get RGB since the bitmap color format is in BGR
		for (i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel)
		{
			temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = temp;
		}
	}

	char* file_name(const char* filename)
	{
		memset(lastFileName, 0, sizeof(lastFileName));

		strcpy(lastFileName, resourcePath);
		strcat(lastFileName, filename);

		return lastFileName;
	}

	GLUSboolean glusImageLoadTga(const GLUSchar* filename, GLUStgaimage* tgaimage)
	{
		//filename = file_name(filename);
		FILE* file;

		GLUSboolean hasColorMap = GLUS_FALSE;

		GLUSubyte imageType;
		GLUSubyte bitsPerPixel;

		GLUSushort firstEntryIndex;
		GLUSushort colorMapLength;
		GLUSubyte colorMapEntrySize;
		GLUSubyte* colorMap = 0;

		GLUSuint i, k;

		size_t elementsRead;

		// check, if we have a valid pointer
		if (!filename || !tgaimage)
		{
			return GLUS_FALSE;
		}

		tgaimage->width = 0;
		tgaimage->height = 0;
		tgaimage->depth = 0;
		tgaimage->data = 0;
		tgaimage->format = 0;

		// open filename in "read binary" mode
		file = glusFileOpen(filename, "rb");

		if (!file)
		{
			return GLUS_FALSE;
		}

		// seek through the tga header, up to the type:
		if (fseek(file, 2, SEEK_CUR))
		{
			glusFileClose(file);

			return GLUS_FALSE;
		}

		// read the image type
		elementsRead = fread(&imageType, 1, 1, file);

		if (!_glusFileCheckRead(file, elementsRead, 1))
		{
			return GLUS_FALSE;
		}

		// check the type
		if (imageType != 1 && imageType != 2 && imageType != 3 && imageType != 9 && imageType != 10 && imageType != 11)
		{
			glusFileClose(file);

			return GLUS_FALSE;
		}

		if (imageType == 1 || imageType == 9)
		{
			hasColorMap = GLUS_TRUE;
		}

		if (!hasColorMap)
		{
			// seek through the tga header, up to the width/height:
			if (fseek(file, 9, SEEK_CUR))
			{
				glusFileClose(file);

				return GLUS_FALSE;
			}
		}
		else
		{
			elementsRead = fread(&firstEntryIndex, 2, 1, file);

			if (!_glusFileCheckRead(file, elementsRead, 1))
			{
				glusImageDestroyTga(tgaimage);

				return GLUS_FALSE;
			}

			elementsRead = fread(&colorMapLength, 2, 1, file);

			if (!_glusFileCheckRead(file, elementsRead, 1))
			{
				glusImageDestroyTga(tgaimage);

				return GLUS_FALSE;
			}

			elementsRead = fread(&colorMapEntrySize, 1, 1, file);

			if (!_glusFileCheckRead(file, elementsRead, 1))
			{
				glusImageDestroyTga(tgaimage);

				return GLUS_FALSE;
			}

			// seek through the tga header, up to the width/height:
			if (fseek(file, 4, SEEK_CUR))
			{
				glusFileClose(file);

				return GLUS_FALSE;
			}
		}

		// read the width
		elementsRead = fread(&tgaimage->width, 2, 1, file);

		if (!_glusFileCheckRead(file, elementsRead, 1))
		{
			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		if (tgaimage->width > GLUS_MAX_DIMENSION)
		{
			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		// read the height
		elementsRead = fread(&tgaimage->height, 2, 1, file);

		if (!_glusFileCheckRead(file, elementsRead, 1))
		{
			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		if (tgaimage->height > GLUS_MAX_DIMENSION)
		{
			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		tgaimage->depth = 1;

		// read the bits per pixel
		elementsRead = fread(&bitsPerPixel, 1, 1, file);

		if (!_glusFileCheckRead(file, elementsRead, 1))
		{
			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		// check the pixel depth
		if (bitsPerPixel != 8 && bitsPerPixel != 24 && bitsPerPixel != 32)
		{
			glusFileClose(file);

			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}
		else
		{
			tgaimage->format = GLUS_SINGLE_CHANNEL;
			if (bitsPerPixel == 24)
			{
				tgaimage->format = GLUS_RGB;
			}
			else if (bitsPerPixel == 32)
			{
				tgaimage->format = GLUS_RGBA;
			}
		}

		// move file pointer to beginning of targa data
		if (fseek(file, 1, SEEK_CUR))
		{
			glusFileClose(file);

			glusImageDestroyTga(tgaimage);

			return GLUS_FALSE;
		}

		if (hasColorMap)
		{
			// Create color map space.

			GLUSint bytesPerPixel = colorMapEntrySize / 8;

			colorMap = (GLUSubyte*)glusMemoryMalloc((size_t)colorMapLength * bytesPerPixel * sizeof(GLUSubyte));

			if (!colorMap)
			{
				glusFileClose(file);

				glusImageDestroyTga(tgaimage);

				return GLUS_FALSE;
			}

			// Read in the color map.

			elementsRead = fread(colorMap, 1, (size_t)colorMapLength * bytesPerPixel * sizeof(GLUSubyte), file);

			if (!_glusFileCheckRead(file, elementsRead, (size_t)colorMapLength * bytesPerPixel * sizeof(GLUSubyte)))
			{
				glusImageDestroyTga(tgaimage);

				glusMemoryFree(colorMap);
				colorMap = 0;

				return GLUS_FALSE;
			}

			// swap the color if necessary
			if (colorMapEntrySize == 24 || colorMapEntrySize == 32)
			{
				glusImageSwapColorChannel(colorMapLength, 1, colorMapEntrySize == 24 ? GLUS_RGB : GLUS_RGBA, colorMap);
			}
		}

		// allocate enough memory for the targa  data
		tgaimage->data = (GLUSubyte*)glusMemoryMalloc((size_t)tgaimage->width * tgaimage->height * bitsPerPixel / 8);

		// verify memory allocation
		if (!tgaimage->data)
		{
			glusFileClose(file);

			glusImageDestroyTga(tgaimage);

			if (hasColorMap)
			{
				glusMemoryFree(colorMap);
				colorMap = 0;
			}

			return GLUS_FALSE;
		}

		if (imageType == 1 || imageType == 2 || imageType == 3)
		{
			// read in the raw data
			elementsRead = fread(tgaimage->data, 1, (size_t)tgaimage->width * tgaimage->height * bitsPerPixel / 8, file);

			if (!_glusFileCheckRead(file, elementsRead, (size_t)tgaimage->width * tgaimage->height * bitsPerPixel / 8))
			{
				glusImageDestroyTga(tgaimage);

				if (hasColorMap)
				{
					glusMemoryFree(colorMap);
					colorMap = 0;
				}

				return GLUS_FALSE;
			}
		}
		else
		{
			// RLE encoded
			GLUSint pixelsRead = 0;

			while (pixelsRead < tgaimage->width * tgaimage->height)
			{
				GLUSubyte amount;

				elementsRead = fread(&amount, 1, 1, file);

				if (!_glusFileCheckRead(file, elementsRead, 1))
				{
					glusImageDestroyTga(tgaimage);

					if (hasColorMap)
					{
						glusMemoryFree(colorMap);
						colorMap = 0;
					}

					return GLUS_FALSE;
				}

				if (amount & 0x80)
				{
					GLUSint i;
					GLUSint k;

					amount &= 0x7F;

					amount++;

					// read in the rle data
					elementsRead = fread(&tgaimage->data[pixelsRead * bitsPerPixel / 8], 1, bitsPerPixel / 8, file);

					if (!_glusFileCheckRead(file, elementsRead, bitsPerPixel / 8))
					{
						glusImageDestroyTga(tgaimage);

						if (hasColorMap)
						{
							glusMemoryFree(colorMap);
							colorMap = 0;
						}

						return GLUS_FALSE;
					}

					for (i = 1; i < amount; i++)
					{
						for (k = 0; k < bitsPerPixel / 8; k++)
						{
							tgaimage->data[(pixelsRead + i) * bitsPerPixel / 8 + k] = tgaimage->data[pixelsRead * bitsPerPixel / 8 + k];
						}
					}
				}
				else
				{
					amount &= 0x7F;

					amount++;

					// read in the raw data
					elementsRead = fread(&tgaimage->data[pixelsRead * bitsPerPixel / 8], 1, (size_t)amount * bitsPerPixel / 8, file);

					if (!_glusFileCheckRead(file, elementsRead, (size_t)amount * bitsPerPixel / 8))
					{
						glusImageDestroyTga(tgaimage);

						if (hasColorMap)
						{
							glusMemoryFree(colorMap);
							colorMap = 0;
						}

						return GLUS_FALSE;
					}
				}

				pixelsRead += amount;
			}
		}

		// swap the color if necessary
		if (bitsPerPixel == 24 || bitsPerPixel == 32)
		{
			glusImageSwapColorChannel(tgaimage->width, tgaimage->height, tgaimage->format, tgaimage->data);
		}

		// close the file
		glusFileClose(file);

		if (hasColorMap)
		{
			GLUSubyte* data = tgaimage->data;

			GLUSint bytesPerPixel;

			// Allocating new memory, as current memory is a look up table index and not a color.

			bytesPerPixel = colorMapEntrySize / 8;

			tgaimage->data = (GLUSubyte*)glusMemoryMalloc((size_t)tgaimage->width * tgaimage->height * bytesPerPixel);

			if (!tgaimage->data)
			{
				glusImageDestroyTga(tgaimage);

				glusMemoryFree(data);
				data = 0;

				glusMemoryFree(colorMap);
				colorMap = 0;

				return GLUS_FALSE;
			}

			tgaimage->format = GLUS_SINGLE_CHANNEL;
			if (colorMapEntrySize == 24)
			{
				tgaimage->format = GLUS_RGB;
			}
			else if (colorMapEntrySize == 32)
			{
				tgaimage->format = GLUS_RGBA;
			}

			// Copy color values from the color map into the image data.

			for (i = 0; i < (GLUSuint)tgaimage->width * (GLUSuint)tgaimage->height; i++)
			{
				for (k = 0; k < (GLUSuint)bytesPerPixel; k++)
				{
					tgaimage->data[i * bytesPerPixel + k] = colorMap[(firstEntryIndex + data[i]) * bytesPerPixel + k];
				}
			}

			// Freeing data.

			glusMemoryFree(data);
			data = 0;

			glusMemoryFree(colorMap);
			colorMap = 0;
		}

		return GLUS_TRUE;
	}


	static GLUSvoid glusShapeInitf(GLUSshape* shape)
	{
		if (!shape)
		{
			return;
		}

		memset(shape, 0, sizeof(GLUSshape));

		shape->mode = GLUS_TRIANGLES;
	}

	static GLUSboolean glusShapeCheckf(GLUSshape* shape)
	{
		if (!shape)
		{
			return GLUS_FALSE;
		}

		return shape->vertices && shape->normals && shape->tangents && shape->texCoords && shape->indices;
	}

	GLUSvoid glusShapeDestroyf(GLUSshape* shape)
	{
		if (!shape)
		{
			return;
		}

		if (shape->vertices)
		{
			glusMemoryFree(shape->vertices);

			shape->vertices = 0;
		}

		if (shape->normals)
		{
			glusMemoryFree(shape->normals);

			shape->normals = 0;
		}

		if (shape->tangents)
		{
			glusMemoryFree(shape->tangents);

			shape->tangents = 0;
		}

		if (shape->bitangents)
		{
			glusMemoryFree(shape->bitangents);

			shape->bitangents = 0;
		}

		if (shape->texCoords)
		{
			glusMemoryFree(shape->texCoords);

			shape->texCoords = 0;
		}

		if (shape->allAttributes)
		{
			glusMemoryFree(shape->allAttributes);

			shape->allAttributes = 0;
		}

		if (shape->indices)
		{
			glusMemoryFree(shape->indices);

			shape->indices = 0;
		}

		shape->numberVertices = 0;
		shape->numberIndices = 0;
		shape->mode = 0;
	}

	static GLUSboolean glusShapeFinalizef(GLUSshape* shape)
	{
		GLUSuint i;

		// vertex, normal, tangent, bitangent, texCoords
		GLUSuint stride = 4 + 3 + 3 + 3 + 2;

		if (!shape)
		{
			return GLUS_FALSE;
		}

		// Add bitangents
		shape->bitangents = (GLUSfloat*)glusMemoryMalloc(3 * shape->numberVertices * sizeof(GLUSfloat));

		if (!shape->bitangents)
		{
			return GLUS_FALSE;
		}

		for (i = 0; i < shape->numberVertices; i++)
		{
			glusVector3Crossf(&(shape->bitangents[i * 3]), &(shape->normals[i * 3]), &(shape->tangents[i * 3]));
		}

		//

		shape->allAttributes = (GLUSfloat*)glusMemoryMalloc(stride * shape->numberVertices * sizeof(GLUSfloat));

		if (!shape->allAttributes)
		{
			return GLUS_FALSE;
		}

		for (i = 0; i < shape->numberVertices; i++)
		{
			shape->allAttributes[i * stride + 0] = shape->vertices[i * 4 + 0];
			shape->allAttributes[i * stride + 1] = shape->vertices[i * 4 + 1];
			shape->allAttributes[i * stride + 2] = shape->vertices[i * 4 + 2];
			shape->allAttributes[i * stride + 3] = shape->vertices[i * 4 + 3];

			shape->allAttributes[i * stride + 4] = shape->normals[i * 3 + 0];
			shape->allAttributes[i * stride + 5] = shape->normals[i * 3 + 1];
			shape->allAttributes[i * stride + 6] = shape->normals[i * 3 + 2];

			shape->allAttributes[i * stride + 7] = shape->tangents[i * 3 + 0];
			shape->allAttributes[i * stride + 8] = shape->tangents[i * 3 + 1];
			shape->allAttributes[i * stride + 9] = shape->tangents[i * 3 + 2];

			shape->allAttributes[i * stride + 10] = shape->bitangents[i * 3 + 0];
			shape->allAttributes[i * stride + 11] = shape->bitangents[i * 3 + 1];
			shape->allAttributes[i * stride + 12] = shape->bitangents[i * 3 + 2];

			shape->allAttributes[i * stride + 13] = shape->texCoords[i * 2 + 0];
			shape->allAttributes[i * stride + 14] = shape->texCoords[i * 2 + 1];
		}

		return GLUS_TRUE;
	}

	GLUSboolean glusShapeCreatePlanef(GLUSshape* shape, const GLUSfloat halfExtend)
	{
		GLUSuint i;

		GLUSuint numberVertices = 4;
		GLUSuint numberIndices = 6;

		GLUSfloat xy_vertices[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };

		GLUSfloat xy_normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

		GLUSfloat xy_tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

		GLUSfloat xy_texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

		GLUSindex xy_indices[] = { 0, 1, 2, 1, 3, 2 };

		if (!shape)
		{
			return GLUS_FALSE;
		}
		glusShapeInitf(shape);

		shape->numberVertices = numberVertices;
		shape->numberIndices = numberIndices;

		shape->vertices = (GLUSfloat*)glusMemoryMalloc(4 * numberVertices * sizeof(GLUSfloat));
		shape->normals = (GLUSfloat*)glusMemoryMalloc(3 * numberVertices * sizeof(GLUSfloat));
		shape->tangents = (GLUSfloat*)glusMemoryMalloc(3 * numberVertices * sizeof(GLUSfloat));
		shape->texCoords = (GLUSfloat*)glusMemoryMalloc(2 * numberVertices * sizeof(GLUSfloat));
		shape->indices = (GLUSindex*)glusMemoryMalloc(numberIndices * sizeof(GLUSindex));

		if (!glusShapeCheckf(shape))
		{
			glusShapeDestroyf(shape);

			return GLUS_FALSE;
		}

		memcpy(shape->vertices, xy_vertices, sizeof(xy_vertices));
		for (i = 0; i < numberVertices; i++)
		{
			shape->vertices[i * 4 + 0] *= halfExtend;
			shape->vertices[i * 4 + 1] *= halfExtend;
		}

		memcpy(shape->normals, xy_normals, sizeof(xy_normals));

		memcpy(shape->tangents, xy_tangents, sizeof(xy_tangents));

		memcpy(shape->texCoords, xy_texCoords, sizeof(xy_texCoords));

		memcpy(shape->indices, xy_indices, sizeof(xy_indices));

		if (!glusShapeFinalizef(shape))
		{
			glusShapeDestroyf(shape);

			return GLUS_FALSE;
		}

		return GLUS_TRUE;
	}

	//////////////////////////////////
	GLUSfloat glusVector3Lengthf(const GLUSfloat vector[3])
	{
		return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	}


	GLUSboolean glusVector3Normalizef(GLUSfloat vector[3])
	{
		GLUSint i;

		GLUSfloat length = glusVector3Lengthf(vector);

		if (length == 0.0f)
		{
			return GLUS_FALSE;
		}

		for (i = 0; i < 3; i++)
		{
			vector[i] /= length;
		}

		return GLUS_TRUE;
	}

	GLUSboolean glusMatrix4x4Orthof(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal)
	{
		if ((right - left) == 0.0f || (top - bottom) == 0.0f || (farVal - nearVal) == 0.0f)
		{
			return GLUS_FALSE;
		}

		result[0] = 2.0f / (right - left);
		result[1] = 0.0f;
		result[2] = 0.0f;
		result[3] = 0.0f;
		result[4] = 0.0f;
		result[5] = 2.0f / (top - bottom);
		result[6] = 0.0f;
		result[7] = 0.0f;
		result[8] = 0.0f;
		result[9] = 0.0f;
		result[10] = -2.0f / (farVal - nearVal);
		result[11] = 0.0f;
		result[12] = -(right + left) / (right - left);
		result[13] = -(top + bottom) / (top - bottom);
		result[14] = -(farVal + nearVal) / (farVal - nearVal);
		result[15] = 1.0f;

		return GLUS_TRUE;
	}

	GLUSboolean glusMatrix4x4Frustumf(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal)
	{
		if ((right - left) == 0.0f || (top - bottom) == 0.0f || (farVal - nearVal) == 0.0f)
		{
			return GLUS_FALSE;
		}

		result[0] = 2.0f * nearVal / (right - left);
		result[1] = 0.0f;
		result[2] = 0.0f;
		result[3] = 0.0f;
		result[4] = 0.0f;
		result[5] = 2.0f * nearVal / (top - bottom);
		result[6] = 0.0f;
		result[7] = 0.0f;
		result[8] = (right + left) / (right - left);
		result[9] = (top + bottom) / (top - bottom);
		result[10] = -(farVal + nearVal) / (farVal - nearVal);
		result[11] = -1.0f;
		result[12] = 0.0f;
		result[13] = 0.0f;
		result[14] = -(2.0f * farVal * nearVal) / (farVal - nearVal);
		result[15] = 0.0f;

		return GLUS_TRUE;
	}


	GLUSvoid glusMatrix4x4Identityf(GLUSfloat matrix[16])
	{
		matrix[0] = 1.0f;
		matrix[1] = 0.0f;
		matrix[2] = 0.0f;
		matrix[3] = 0.0f;

		matrix[4] = 0.0f;
		matrix[5] = 1.0f;
		matrix[6] = 0.0f;
		matrix[7] = 0.0f;

		matrix[8] = 0.0f;
		matrix[9] = 0.0f;
		matrix[10] = 1.0f;
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
	}

	GLUSboolean glusMatrix4x4Perspectivef(GLUSfloat result[16], const GLUSfloat fovy, const GLUSfloat aspect, const GLUSfloat zNear, const GLUSfloat zFar)
	{
		GLUSfloat xmin, xmax, ymin, ymax;

		if (fovy <= 0.0f || fovy >= 180.0f)
		{
			return GLUS_FALSE;
		}

		ymax = zNear * tanf(fovy * GLUS_PI / 360.0f);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;

		return glusMatrix4x4Frustumf(result, xmin, xmax, ymin, ymax, zNear, zFar);
	}
	GLUSvoid glusMatrix4x4Multiplyf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16])
	{
		GLUSint i;

		GLUSfloat temp[16];

		GLUSint row;
		GLUSint column;
		for (column = 0; column < 4; column++)
		{
			for (row = 0; row < 4; row++)
			{
				temp[column * 4 + row] = 0.0f;

				for (i = 0; i < 4; i++)
				{
					temp[column * 4 + row] += matrix0[i * 4 + row] * matrix1[column * 4 + i];
				}
			}
		}

		for (i = 0; i < 16; i++)
		{
			matrix[i] = temp[i];
		}
	}

	GLUSvoid glusMatrix4x4Translatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
	{
		GLUSfloat temp[16];

		glusMatrix4x4Identityf(temp);

		temp[12] = x;
		temp[13] = y;
		temp[14] = z;

		glusMatrix4x4Multiplyf(matrix, matrix, temp);
	}

	GLUSvoid glusMatrix4x4LookAtf(GLUSfloat result[16], const GLUSfloat eyeX, const GLUSfloat eyeY, const GLUSfloat eyeZ, const GLUSfloat centerX, const GLUSfloat centerY, const GLUSfloat centerZ, const GLUSfloat upX, const GLUSfloat upY, const GLUSfloat upZ)
	{
		GLUSfloat forward[3], side[3], up[3];

		forward[0] = centerX - eyeX;
		forward[1] = centerY - eyeY;
		forward[2] = centerZ - eyeZ;

		glusVector3Normalizef(forward);

		up[0] = upX;
		up[1] = upY;
		up[2] = upZ;

		glusVector3Crossf(side, forward, up);
		glusVector3Normalizef(side);

		glusVector3Crossf(up, side, forward);

		result[0] = side[0];
		result[1] = up[0];
		result[2] = -forward[0];
		result[3] = 0.0f;
		result[4] = side[1];
		result[5] = up[1];
		result[6] = -forward[1];
		result[7] = 0.0f;
		result[8] = side[2];
		result[9] = up[2];
		result[10] = -forward[2];
		result[11] = 0.0f;
		result[12] = 0.0f;
		result[13] = 0.0f;
		result[14] = 0.0f;
		result[15] = 1.0f;

		glusMatrix4x4Translatef(result, -eyeX, -eyeY, -eyeZ);
	}

	///////////////////////////////////////////////////////////////////////////

	GLUSfloat glusMathDegToRadf(const GLUSfloat degrees)
	{
		return degrees * 2.0f * GLUS_PI / 360.0f;
	}

	GLUSvoid glusQuaternionRotateRyf(GLUSfloat quaternion[4], const GLUSfloat angle)
	{
		GLUSfloat halfAngleRadian = glusMathDegToRadf(angle) * 0.5f;

		quaternion[0] = 0.0f;
		quaternion[1] = sinf(halfAngleRadian);
		quaternion[2] = 0.0f;
		quaternion[3] = cosf(halfAngleRadian);
	}

	GLUSvoid glusQuaternionGetMatrix4x4f(GLUSfloat matrix[16], const GLUSfloat quaternion[4])
	{
		GLUSfloat x = quaternion[0];
		GLUSfloat y = quaternion[1];
		GLUSfloat z = quaternion[2];
		GLUSfloat w = quaternion[3];

		matrix[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
		matrix[1] = 2.0f * x * y + 2.0f * w * z;
		matrix[2] = 2.0f * x * z - 2.0f * w * y;
		matrix[3] = 0.0f;

		matrix[4] = 2.0f * x * y - 2.0f * w * z;
		matrix[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
		matrix[6] = 2.0f * y * z + 2.0f * w * x;
		matrix[7] = 0.0f;

		matrix[8] = 2.0f * x * z + 2.0f * w * y;
		matrix[9] = 2.0f * y * z - 2.0f * w * x;
		matrix[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
	}

	GLUSvoid glusMatrix4x4MultiplyVector3f(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3])
	{
		GLUSint i;

		GLUSfloat temp[3];

		for (i = 0; i < 3; i++)
		{
			temp[i] = matrix[i] * vector[0] + matrix[4 + i] * vector[1] + matrix[8 + i] * vector[2];
		}

		for (i = 0; i < 3; i++)
		{
			result[i] = temp[i];
		}
	}

	GLUSboolean glusShapeCreateSpheref(GLUSshape* shape, const GLUSfloat radius, const GLUSuint numberSlices)
	{
		GLUSuint i, j;

		GLUSuint numberParallels = numberSlices / 2;
		GLUSuint numberVertices = (numberParallels + 1) * (numberSlices + 1);
		GLUSuint numberIndices = numberParallels * numberSlices * 6;

		GLUSfloat angleStep = (2.0f * GLUS_PI) / ((GLUSfloat)numberSlices);

		GLUSuint indexIndices;

		// used later to help us calculating tangents vectors
		GLUSfloat helpVector[3] = { 1.0f, 0.0f, 0.0f };
		GLUSfloat helpQuaternion[4];
		GLUSfloat helpMatrix[16];

		if (numberSlices < 3 || numberVertices > GLUS_MAX_VERTICES || numberIndices > GLUS_MAX_INDICES)
		{
			return GLUS_FALSE;
		}

		if (!shape)
		{
			return GLUS_FALSE;
		}
		glusShapeInitf(shape);

		shape->numberVertices = numberVertices;
		shape->numberIndices = numberIndices;

		shape->vertices = (GLUSfloat*)glusMemoryMalloc(4 * numberVertices * sizeof(GLUSfloat));
		shape->normals = (GLUSfloat*)glusMemoryMalloc(3 * numberVertices * sizeof(GLUSfloat));
		shape->tangents = (GLUSfloat*)glusMemoryMalloc(3 * numberVertices * sizeof(GLUSfloat));
		shape->texCoords = (GLUSfloat*)glusMemoryMalloc(2 * numberVertices * sizeof(GLUSfloat));
		shape->indices = (GLUSindex*)glusMemoryMalloc(numberIndices * sizeof(GLUSindex));

		if (!glusShapeCheckf(shape))
		{
			glusShapeDestroyf(shape);

			return GLUS_FALSE;
		}

		for (i = 0; i < numberParallels + 1; i++)
		{
			for (j = 0; j < numberSlices + 1; j++)
			{
				GLUSuint vertexIndex = (i * (numberSlices + 1) + j) * 4;
				GLUSuint normalIndex = (i * (numberSlices + 1) + j) * 3;
				GLUSuint tangentIndex = (i * (numberSlices + 1) + j) * 3;
				GLUSuint texCoordsIndex = (i * (numberSlices + 1) + j) * 2;

				shape->vertices[vertexIndex + 0] = radius * sinf(angleStep * (GLUSfloat)i) * sinf(angleStep * (GLUSfloat)j);
				shape->vertices[vertexIndex + 1] = radius * cosf(angleStep * (GLUSfloat)i);
				shape->vertices[vertexIndex + 2] = radius * sinf(angleStep * (GLUSfloat)i) * cosf(angleStep * (GLUSfloat)j);
				shape->vertices[vertexIndex + 3] = 1.0f;

				shape->normals[normalIndex + 0] = shape->vertices[vertexIndex + 0] / radius;
				shape->normals[normalIndex + 1] = shape->vertices[vertexIndex + 1] / radius;
				shape->normals[normalIndex + 2] = shape->vertices[vertexIndex + 2] / radius;

				shape->texCoords[texCoordsIndex + 0] = (GLUSfloat)j / (GLUSfloat)numberSlices;
				shape->texCoords[texCoordsIndex + 1] = 1.0f - (GLUSfloat)i / (GLUSfloat)numberParallels;

				// use quaternion to get the tangent vector
				glusQuaternionRotateRyf(helpQuaternion, 360.0f * shape->texCoords[texCoordsIndex + 0]);
				glusQuaternionGetMatrix4x4f(helpMatrix, helpQuaternion);

				glusMatrix4x4MultiplyVector3f(&shape->tangents[tangentIndex], helpMatrix, helpVector);
			}
		}

		indexIndices = 0;
		for (i = 0; i < numberParallels; i++)
		{
			for (j = 0; j < numberSlices; j++)
			{
				shape->indices[indexIndices++] = i * (numberSlices + 1) + j;
				shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + j;
				shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + (j + 1);

				shape->indices[indexIndices++] = i * (numberSlices + 1) + j;
				shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + (j + 1);
				shape->indices[indexIndices++] = i * (numberSlices + 1) + (j + 1);
			}
		}

		if (!glusShapeFinalizef(shape))
		{
			glusShapeDestroyf(shape);

			return GLUS_FALSE;
		}

		return GLUS_TRUE;
	}


	GLUSvoid glusMatrix4x4GetScalef(GLUSfloat scales[3], const GLUSfloat matrix[16])
	{
		GLUSint i;

		for (i = 0; i < 3; i++)
		{
			scales[i] = sqrtf(matrix[4 * i] * matrix[4 * i] + matrix[4 * i + 1] * matrix[4 * i + 1] + matrix[4 * i + 2] * matrix[4 * i + 2]);
		}
	}

	GLUSvoid glusMatrix4x4Transposef(GLUSfloat matrix[16])
	{
		GLUSint column;
		GLUSint row;

		GLUSfloat temp[16];

		glusMatrix4x4Copyf(temp, matrix, GLUS_FALSE);

		for (column = 0; column < 4; column++)
		{
			for (row = 0; row < 4; row++)
			{
				matrix[row * 4 + column] = temp[column * 4 + row];
			}
		}
	}
	GLUSvoid glusMatrix4x4InverseRigidBodyf(GLUSfloat matrix[16])
	{
		GLUSfloat scales[3];
		GLUSfloat inverseScale[16];
		GLUSfloat inverseRotation[16];
		GLUSfloat inverseTranslation[16];

		glusMatrix4x4GetScalef(scales, matrix);
		glusMatrix4x4Identityf(inverseScale);
		// Square of it, as rotation still contains the scale
		inverseScale[0] = 1.0f / (scales[0] * scales[0]);
		inverseScale[5] = 1.0f / (scales[1] * scales[1]);
		inverseScale[10] = 1.0f / (scales[2] * scales[2]);

		glusMatrix4x4Copyf(inverseRotation, matrix, GLUS_TRUE);
		glusMatrix4x4Transposef(inverseRotation);

		glusMatrix4x4Identityf(inverseTranslation);
		inverseTranslation[12] = -matrix[12];
		inverseTranslation[13] = -matrix[13];
		inverseTranslation[14] = -matrix[14];

		glusMatrix4x4Multiplyf(matrix, inverseRotation, inverseTranslation);
		glusMatrix4x4Multiplyf(matrix, inverseScale, matrix);
	}

	GLUSvoid glusMatrix4x4ExtractMatrix3x3f(GLUSfloat matrix[9], const GLUSfloat source[16])
	{
		matrix[0] = source[0];
		matrix[1] = source[1];
		matrix[2] = source[2];

		matrix[3] = source[4];
		matrix[4] = source[5];
		matrix[5] = source[6];

		matrix[6] = source[8];
		matrix[7] = source[9];
		matrix[8] = source[10];
	}


	GLUSvoid glusMatrix4x4Copyf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly)
	{
		GLUSint i;

		if (rotationOnly)
		{
			for (i = 0; i < 12; i++)
			{
				if ((i + 1) % 4 == 0)
				{
					continue;
				}

				matrix[i] = source[i];
			}

			matrix[3] = 0.0f;
			matrix[7] = 0.0f;
			matrix[11] = 0.0f;

			matrix[12] = 0.0f;
			matrix[13] = 0.0f;
			matrix[14] = 0.0f;
			matrix[15] = 1.0f;
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
				matrix[i] = source[i];
			}
		}
	}
}