#pragma once

///Safe delete for cpp
#define SafeDelete(_X)	 do { if(_X){ delete (_X); (_X) = nullptr; } } while(0)
///Safe delete array for cpp 
#define SafeDeleteArray(_X) do { if(_X) { delete[] (_X); (_X) = nullptr; } } while(0)
///Safe free for c
#define SafeFree(_X)  do { if(_X) { free(_X); (_X) = nullptr; } } while(0)
///Safe release for c
#define SafeRelease(_X)  do { if(_X) { (_X)->release(); } } while(0)
///Safe release for c and initialize
#define SafeReleaseNull(_X) do { if(_X) { (_X)->release(); (_X) = nullptr; } } while(0)
///Safe retain for c
#define SafeRetain(_X)  do { if(_X) { (_X)->retain(); } } while(0)
///Break if
#define BreakIf(_X) do { if(_X) break; } while(0)
