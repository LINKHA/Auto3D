// For conditions of distribution and use, see copyright notice in License.txt

#include "../Base/String.h"
#include "../AutoConfig.h"

#pragma once

namespace Auto3D
{

	/// Low-resolution operating system timer.
	class AUTO_API Timer
	{
	public:
		/// Construct. Get the starting clock value.
		Timer();

		/// Return elapsed milliseconds.
		unsigned ElapsedMSec();
		/// Reset the timer.
		void Reset();

	private:
		/// Starting clock value in milliseconds.
		unsigned startTime;
	};

	/// High-resolution operating system timer used in profiling.
	class AUTO_API HiresTimer
	{
	public:
		/// Construct. Get the starting high-resolution clock value.
		HiresTimer();

		/// Return elapsed microseconds.
		long long ElapsedUSec();
		/// Reset the timer.
		void Reset();

		/// Perform one-time initialization to check support and frequency. Is called automatically at program start.
		static void Initialize();
		/// Return if high-resolution timer is supported.
		static bool IsSupported() { return supported; }
		/// Return high-resolution timer frequency if supported.
		static long long Frequency() { return frequency; }

	private:
		/// Starting clock value in CPU ticks.
		long long startTime;

		/// High-resolution timer support flag.
		static bool supported;
		/// High-resolution timer frequency.
		static long long frequency;
	};

	/// Return a date/time stamp as a string.
	AUTO_API String TimeStamp();
	/// Return current time as seconds since epoch.
	AUTO_API unsigned CurrentTime();

}
