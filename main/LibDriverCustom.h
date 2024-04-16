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

#ifndef LIB_DRIVER_CUSTOM_H
#define LIB_DRIVER_CUSTOM_H

#include <pthread.h>

#include "Processing.h"

struct DriverCustomConfig
{
	size_t sizeStack;
};

struct DriverCustom
{
	pthread_t thread;
	DriverCustomConfig config;
	FuncInternalDrive pFctDrive;
	void *pProc;
};

/*
 * Literature
 * - https://man7.org/linux/man-pages/man3/pthread_create.3.html
 * - https://man7.org/linux/man-pages/man3/pthread_join.3.html
 * - https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
 * - https://man7.org/linux/man-pages/man3/pthread_attr_setstacksize.3.html
 */
#if defined(__linux__)
void internalDrive(void *pData);
void *internalDriverCreate(FuncInternalDrive pFctDrive, void *pProc, void *pConfigDriver);
void internalDriverCleanUp(void *pDriver);
#endif

#endif

