#pragma once

#ifdef ENUM
#	undef ENUM
#endif // ENUM
#ifdef ENUM_END
#	undef ENUM_END
#endif // ENUM_END

#define ENUM(T)struct ENUM_CLASS_##T { enum ENUM_##T {
#define ENUM_END(T) };}; using T = ENUM_CLASS_##T::ENUM_##T