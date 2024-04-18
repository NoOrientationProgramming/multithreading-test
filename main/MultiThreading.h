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

#ifndef MULTI_THREADING_H
#define MULTI_THREADING_H

#include <list>

#include "Processing.h"
#include "SizeStackPrinting.h"

class MultiThreading : public Processing
{

public:

	static MultiThreading *create()
	{
		return new (std::nothrow) MultiThreading;
	}

protected:

	MultiThreading();
	virtual ~MultiThreading() {}

private:

	MultiThreading(const MultiThreading &) : Processing("") {}
	MultiThreading &operator=(const MultiThreading &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	Success childrenStart();
	void childrenRepel();

	/* member variables */
	//uint32_t mStartMs;
	bool mCheckEnabled;
	std::list<SizeStackPrinting *> mLstPrint;

	/* static functions */
#if CONFIG_PROC_HAVE_DRIVERS
	static void customInternalDrive(void *pData);
#endif
	/* static variables */

	/* constants */

};

#endif

