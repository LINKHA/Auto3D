#include "GLDebug.h"
#include <iostream>
#include <glad/glad.h>
void WINAPI glDebugOutput(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
		case 0x8246:				std::cout << "Source: API"; break;
		case 0x8247:				std::cout << "Source: Window System"; break;
		case 0x8248:				std::cout << "Source: Shader Compiler"; break;
		case 0x8249:				std::cout << "Source: Third Party"; break;
		case 0x824A:				std::cout << "Source: Application"; break;
		case 0x824B:				std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case 0x824C:				std::cout << "Type: Error"; break;
		case 0x824D:				std::cout << "Type: Deprecated Behaviour"; break;
		case 0x824E:				std::cout << "Type: Undefined Behaviour"; break;
		case 0x824F:				std::cout << "Type: Portability"; break;
		case 0x8250:				std::cout << "Type: Performance"; break;
		case 0x8268:				std::cout << "Type: Marker"; break;
		case 0x8269:				std::cout << "Type: Push Group"; break;
		case 0x826A:				std::cout << "Type: Pop Group"; break;
		case 0x8251:				std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
		case 0x9146:				std::cout << "Severity: high"; break;
		case 0x9147:				std::cout << "Severity: medium"; break;
		case 0x9148:				std::cout << "Severity: low"; break;
		case 0x826B:				std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}