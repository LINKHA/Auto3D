#pragma once

#define LogString(_message)
#define InfoString(_message)
#define WarningString(_message)
#define ErrorString(_message)
#define LogRawString(_message)
#define LogStringF(_format, ...)
#define InfoStringF(_format, ...)
#define WarningStringF(_format, ...)
#define ErrorStringF(_format, ...)
#define LogRawStringF(_format, ...)
#define Print(message) LogString(message)
#define print(message) LogString(message)