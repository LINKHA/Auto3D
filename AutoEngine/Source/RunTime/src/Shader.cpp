#include "Shader.h"
#include "Math/Math.h"
#include "Graphics.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "File.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "NewDef.h"

namespace Auto3D {

void CommentOutFunction(STRING& code, const STRING& signature)
{
	unsigned startPos = code.Find(signature);
	unsigned braceLevel = 0;
	if (startPos == STRING::NO_POS)
		return;

	code.Insert(startPos, "/*");

	for (unsigned i = startPos + 2 + signature.Length(); i < code.Length(); ++i)
	{
		if (code[i] == '{')
			++braceLevel;
		else if (code[i] == '}')
		{
			--braceLevel;
			if (braceLevel == 0)
			{
				code.Insert(i + 1, "*/");
				return;
			}
		}
	}
}

Shader::Shader(Ambient* ambient)
	: Super(ambient)
	, _timeStamp(0)
	, _numVariations(0)
{}

Shader::~Shader()
{

}

void Shader::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Shader>();
}

bool Shader::BeginLoad(Deserializer& source)
{
	auto* graphics = GetSubSystem<Graphics>();
	if (!graphics)
		return false;

	// Load the shader source code and resolve any includes
	_timeStamp = 0;
	STRING shaderCode;
	if (!processSource(shaderCode, source))
		return false;
	// Get geometry shader return 
	if (shaderCode.Find("void GS(") != STRING::NO_POS)
	{
		_gsSourceCode = shaderCode;
		_gsSourceCode.Replace("void GS(", "void main(");
		return true;
	}
	if (shaderCode.Find("void VS(") != STRING::NO_POS && shaderCode.Find("void FS(") == STRING::NO_POS)
	{
		_vsSourceCode = shaderCode;
		_vsSourceCode.Replace("void VS(", "void main(");
		return true;
	}
	if (shaderCode.Find("void FS(") != STRING::NO_POS && shaderCode.Find("void VS(") == STRING::NO_POS)
	{
		_fsSourceCode = shaderCode;
		_fsSourceCode.Replace("void FS(", "void main(");
		return true;
	}
	// Comment out the unneeded shader function
	_vsSourceCode = shaderCode;
	_fsSourceCode = shaderCode;

	CommentOutFunction(_vsSourceCode, "void FS(");
	CommentOutFunction(_fsSourceCode, "void VS(");
	// OpenGL: rename either VS() FS() to main()
#ifdef AUTO_OPENGL
	_vsSourceCode.Replace("void VS(", "void main(");
	_fsSourceCode.Replace("void FS(", "void main(");
#endif
	
	return true;
}

bool Shader::processSource(STRING& code, Deserializer& source)
{
	auto* cache = GetSubSystem<ResourceSystem>();
	// If the source if a non-packaged file, store the timestamp
	auto* file = dynamic_cast<File*>(&source);
	if (file)
	{
		auto* fileSystem = GetSubSystem<FileSystem>();
		STRING fullName = cache->GetResourceFileName(file->GetName());
		unsigned fileTimeStamp = fileSystem->GetLastModifiedTime(fullName);
		if (fileTimeStamp > _timeStamp)
			_timeStamp = fileTimeStamp;
	}

	while (!source.IsEof())
	{
		STRING line = source.ReadLine();

		if (line.StartsWith("#include"))
		{
			STRING includeFileName = GetPath(source.GetName()) + line.SubString(9).Replaced("\"", "").Trimmed();

			SharedPtr<File> includeFile(cache->GetFile(includeFileName));
			if (!includeFile)
				return false;

			// Add the include file into the current code recursively
			if (!processSource(code, *includeFile))
				return false;
		}
		else
		{
			code += line;
			code += "\n";
		}
	}

	// Finally insert an empty line to mark the space between files
	code += "\n";

	return true;
}

}