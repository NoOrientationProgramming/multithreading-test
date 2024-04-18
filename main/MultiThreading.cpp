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

#include "MultiThreading.h"
#include "LibDriverPlatform.h"

#define dForEach_ProcState(gen) \
		gen(StStart) \
		gen(StTest1Start) \
		gen(StTest1DoneWait) \
		gen(StDriveSwitch) \
		gen(StTest2Start) \
		gen(StTest2DoneWait) \
		gen(StDriverSwitch) \
		gen(StTest3Start) \
		gen(StTest3DoneWait) \
		gen(StDefaultConfigSwitch) \
		gen(StTest4Start) \
		gen(StTest4DoneWait) \

#define dGenProcStateEnum(s) s,
dProcessStateEnum(ProcState);

#if 1
#define dGenProcStateString(s) #s,
dProcessStateStr(ProcState);
#endif

using namespace std;

#define LOG_LVL	0

MultiThreading::MultiThreading()
	: Processing("MultiThreading")
	//, mStartMs(0)
	, mCheckEnabled(false)
{
	mState = StStart;
}

/* member functions */

Success MultiThreading::process()
{
	//uint32_t curTimeMs = millis();
	//uint32_t diffMs = curTimeMs - mStartMs;
	Success success;
#if 0
	dStateTrace;
#endif
	switch (mState)
	{
	case StStart:

		mState = StTest1Start;

		break;
	case StTest1Start:

		success = childrenStart();
		if (success != Positive)
			return procErrLog(-1, "could not start children");

		mState = StTest1DoneWait;

		break;
	case StTest1DoneWait:

		success = childrenSuccess();
		if (success == Pending)
			break;

		if (success != Positive)
			return procErrLog(-1, "tests failed");

		childrenRepel();

		mState = StDriveSwitch;

		break;
	case StDriveSwitch:

		procWrnLog("using custom drive function");
#if CONFIG_PROC_HAVE_DRIVERS
		Processing::internalDriveSet(customInternalDrive);
#endif
		mState = StTest2Start;

		break;
	case StTest2Start:

		success = childrenStart();
		if (success != Positive)
			return procErrLog(-1, "could not start children");

		mState = StTest2DoneWait;

		break;
	case StTest2DoneWait:

		success = childrenSuccess();
		if (success == Pending)
			break;

		if (success != Positive)
			return procErrLog(-1, "tests failed");

		childrenRepel();

		mState = StDriverSwitch;

		break;
	case StDriverSwitch:

		procWrnLog("using platform driver");
#if CONFIG_PROC_HAVE_DRIVERS
		Processing::driverInternalCreateAndCleanUpSet(
				driverPlatformCreate,
				driverPlatformCleanUp);
#if defined(__linux__)
		mCheckEnabled = true;
#endif
#endif
		mState = StTest3Start;

		break;
	case StTest3Start:

		success = childrenStart();
		if (success != Positive)
			return procErrLog(-1, "could not start children");

		mState = StTest3DoneWait;

		break;
	case StTest3DoneWait:

		success = childrenSuccess();
		if (success == Pending)
			break;

		if (success != Positive)
			return procErrLog(-1, "tests failed");

		childrenRepel();

		mState = StDefaultConfigSwitch;

		break;
	case StDefaultConfigSwitch:

		procWrnLog("using custom default stack size");

		ConfigDriver::sizeStackDefaultSet(0x1000000);

		mState = StTest4Start;

		break;
	case StTest4Start:

		success = childrenStart();
		if (success != Positive)
			return procErrLog(-1, "could not start children");

		mState = StTest4DoneWait;

		break;
	case StTest4DoneWait:

		success = childrenSuccess();
		if (success == Pending)
			break;

		if (success != Positive)
			return procErrLog(-1, "tests failed");

		childrenRepel();

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

Success MultiThreading::childrenStart()
{
	SizeStackPrinting *pPrint;

	// parent
	pPrint = SizeStackPrinting::create();
	if (!pPrint)
		return procErrLog(-1, "could not create process");

	start(pPrint);
	mLstPrint.push_back(pPrint);

	// new internal, config set by driver
	pPrint = SizeStackPrinting::create();
	if (!pPrint)
		return procErrLog(-1, "could not create process");

	start(pPrint, DrivenByNewInternalDriver);
	mLstPrint.push_back(pPrint);

	// new internal, config set by user, default config used
	pPrint = SizeStackPrinting::create();
	if (!pPrint)
		return procErrLog(-1, "could not create process");

	{
		ConfigDriver config;
#if CONFIG_PROC_HAVE_DRIVERS
		pPrint->configDriverSet(&config);
#endif
		if (mCheckEnabled)
			pPrint->mSizeStackCheck = config.mSizeStack;
	}

	start(pPrint, DrivenByNewInternalDriver);
	mLstPrint.push_back(pPrint);

	// new internal, config set by user, config changed
	pPrint = SizeStackPrinting::create();
	if (!pPrint)
		return procErrLog(-1, "could not create process");

	{
		ConfigDriver config;
		config.mSizeStack <<= 1;
#if CONFIG_PROC_HAVE_DRIVERS
		pPrint->configDriverSet(&config);
#endif
		if (mCheckEnabled)
			pPrint->mSizeStackCheck = config.mSizeStack;
	}

	start(pPrint, DrivenByNewInternalDriver);
	mLstPrint.push_back(pPrint);

	return Positive;
}

void MultiThreading::childrenRepel()
{
	list<SizeStackPrinting *>::iterator iter;

	iter = mLstPrint.begin();
	for (; iter != mLstPrint.end(); ++iter)
		repel(*iter);

	mLstPrint.clear();
}

void MultiThreading::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", ProcStateString[mState]);
#endif
}

/* static functions */
#if CONFIG_PROC_HAVE_DRIVERS
void MultiThreading::customInternalDrive(void *pData)
{
	Processing *pChild = (Processing *)pData;

	//wrnLog("started custom drive function");

	while (1)
	{
		pChild->treeTick();
		this_thread::sleep_for(chrono::milliseconds(2));

		if (pChild->progress())
			continue;

		Processing::undrivenSet(pChild);
		break;
	}
}
#endif

