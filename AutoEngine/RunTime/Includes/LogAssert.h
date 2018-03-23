#ifndef LOG_ASSERT_H_
#define LOG_ASSERT_H_



#define DebugAssertIf(x)	do { (void)sizeof(x); } while(0)
#define DebugAssert(x) 		do { (void)sizeof(x); } while(0)
#define DebugAssertMsg(x, ...) {  }



//Assert
#define AssertIf(x) assert(!(x))

#endif // 