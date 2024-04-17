
## Status

- In progress

## Reason

For certain systems, we may wish to modify the stack size of the worker threads (internal drivers).
However, with the default implementation of threads in the ```Processing()``` class, this is not feasible.
The rationale behind this limitation is that the Processing() class aims to be as **platform-agnostic** as possible.
Therefore, it utilizes the ```std::thread``` class from C++11 for its internal drivers, which cannot set the stack size.

To overcome this limitation, it is possible to use **platform-specific** drivers, which offer greater flexibility compared to the std::thread class.
The file ```LibDriverPlatform``` implements the creation of drivers for popular systems such as Linux or Windows.

## Usage

Compile LibDriverPlatform.cpp from ```ProcessingCommon``` and include its header file.
At application startup, set the functions from LibDriverPlatform as the default method for creating an internal driver
```cpp
Processing::driverInternalCreateAndCleanUpSet(driverPlatformCreate, driverPlatformCleanUp);
```

If desired, the default stack size of new threads can be changed using
```cpp
ConfigDriver::sizeStackDefaultSet(0x1000000);
```

Additionally, the stack size for each individual process can be adjusted as follows
```cpp
pPrint = SizeStackPrinting::create();
if (!pPrint)
    return procErrLog(-1, "could not create process");

ConfigDriver config;
config.mSizeStack <<= 1;
pPrint->configDriverSet(&config);

start(pPrint, DrivenByNewInternalDriver);
```

## Example output

```
0x6466c3e58390 SizeStackPrinting           Stack size: 0x00800000 (8388608)
0x6466c3e585e0 SizeStackPrinting           Stack size: 0x00800000 (8388608)
0x6466c3e58830 SizeStackPrinting           Stack size: 0x00800000 (8388608)
0x6466c3e582d0 SizeStackPrinting           Stack size: 0x007ff000 (8384512)
0x6466c3e56bf0 MultiThreading              using platform driver
0x6466c3e582d0 SizeStackPrinting           Stack size: 0x00004000 (16384)
0x7bd56c001490 SizeStackPrinting           Stack size: 0x00004000 (16384)
0x7bd564000fb0 SizeStackPrinting           Stack size: 0x00008000 (32768)
0x7bd564001050 SizeStackPrinting           Stack size: 0x007ff000 (8384512)
0x6466c3e56bf0 MultiThreading              using custom default stack size
0x7bd568000fb0 SizeStackPrinting           Stack size: 0x01000000 (16777216)
0x7bd56c001530 SizeStackPrinting           Stack size: 0x01000000 (16777216)
0x7bd568001050 SizeStackPrinting           Stack size: 0x02000000 (33554432)
0x6466c3e58390 SizeStackPrinting           Stack size: 0x007ff000 (8384512)
```
