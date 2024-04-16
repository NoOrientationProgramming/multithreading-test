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
	, mStartMs(0)
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

		sizeStackPrint();

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

void SizeStackPrinting::sizeStackPrint()
{
	pthread_attr_t attrThread;
	bool initConfigDone = false;
	int res;
	size_t sizeStack;

	res = pthread_attr_init(&attrThread);
	if (res)
	{
		errLog(-1, "could not initialize thread attributes: %s (%d)", strerror(errno), errno);
		goto drvExit;
	}

	initConfigDone = true;

	res = pthread_attr_getstacksize(&attrThread, &sizeStack);
	if (res)
	{
		errLog(-1, "could not get stack size: %s (%d)", strerror(errno), errno);
		goto drvExit;
	}

	procWrnLog("Stack size: 0x%08x, %zu", sizeStack, sizeStack);

drvExit:

	if (initConfigDone)
	{
		res = pthread_attr_destroy(&attrThread);
		if (res)
			errLog(-1, "could not destroy thread attributes: %s (%d)", strerror(errno), errno);
	}
}

void SizeStackPrinting::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */

