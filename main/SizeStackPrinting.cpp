/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 16.04.2024

  Copyright (C) 2024-now Authors and www.dsp-crowd.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "SizeStackPrinting.h"
#include "LibDriverPlatform.h"

#define dForEach_ProcState(gen) \
		gen(StStart) \
		gen(StMain) \

#define dGenProcStateEnum(s) s,
dProcessStateEnum(ProcState);

#if 1
#define dGenProcStateString(s) #s,
dProcessStateStr(ProcState);
#endif

using namespace std;

#define LOG_LVL	0

SizeStackPrinting::SizeStackPrinting()
	: Processing("SizeStackPrinting")
	, mSizeStackCheck(0)
	, mSizeStack(0)
	//, mStartMs(0)
{
	mState = StStart;
}

/* member functions */

Success SizeStackPrinting::process()
{
	//uint32_t curTimeMs = millis();
	//uint32_t diffMs = curTimeMs - mStartMs;
	//Success success;
#if 0
	dStateTrace;
#endif
	switch (mState)
	{
	case StStart:

		mState = StMain;

		break;
	case StMain:

		mSizeStack = sizeStackGet();
		if (!mSizeStack)
			return procErrLog(-1, "could not get stack size");

		procInfLog("Stack size: 0x%08x (%zu)", mSizeStack, mSizeStack);

		if (mSizeStackCheck && mSizeStackCheck != mSizeStack)
			return procErrLog(-1, "wrong stack size. Should be 0x%08x (%zu), but is 0x%08x (%zu)",
					mSizeStackCheck, mSizeStackCheck,
					mSizeStack, mSizeStack);

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

void SizeStackPrinting::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */

