#pragma once
enum RunMode
{
	DefaultMode = -1,
	AwakeMode,
	StartMode,
	UpdateMode,
	FixUpdateMode,
	FinishMode,
	DrawMode,
};
enum DepthMode
{
	DEPTH_ALWAYS = 0x0207,	//Always pass the depth test 
	DEPTH_NEVER = 0x0200,	//Never pass the depth test 
	DEPTH_LESS = 0x0201,	//Pass test when the segment depth value is less than the depth value of the buffer
	DEPTH_EQUAL = 0x0202,	//Pass the test when the fragment depth value is equal to the buffer depth value
	DEPTH_LEQUAL = 0x0203,	//Pass the test when the fragment depth value is less than or equal to the depth value of the buffer.
	DEPTH_GREATER = 0x0204,	//Pass test when the segment depth value is greater than the depth value of the buffer
	DEPTH_NOTEQUAL = 0x0205,	//When the fragment depth value is not equal to the depth value of the buffer, it passes the test.
	DEPTH_GEQUAL = 0x0206,	//Pass the test when the fragment depth value is greater than the depth value of the buffer
};
