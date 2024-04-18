
project(
	'multithreading',
	'cpp',
)

cxx = meson.get_compiler('cpp')

# Sources

srcs = [
	'components/ProcessingCore/Processing.cpp',
	'components/ProcessingCore/Log.cpp',
	'components/ProcessingCore/SystemDebugging.cpp',
	'components/ProcessingCore/SystemCommanding.cpp',
	'components/ProcessingCore/TcpListening.cpp',
	'components/ProcessingCore/TcpTransfering.cpp',
	'components/ProcessingCommon/ThreadPooling.cpp',
	'components/ProcessingCommon/LibDriverPlatform.cpp',
	'main/main.cpp',
	'main/AppSupervising.cpp',
	'main/MultiThreading.cpp',
	'main/SizeStackPrinting.cpp',
]

if host_machine.system() == 'linux'
	srcs += 'components/ProcessingCommon/LibFilesys.cpp'
endif

# Arguments

args = [
	'-DCONFIG_PROC_HAVE_LOG=1',
	'-DCONFIG_PROC_HAVE_DRIVERS=1',
	'-DCONFIG_PROC_INFO_BUFFER_SIZE=1024',
]

# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
warningsLinux = [
	'-Wall',
	'-Wextra',
	'-Wpedantic',
	'-Werror',
	'-Wfatal-errors',
	'-Wreorder',
	'-Wswitch-enum',
	'-Wuseless-cast',
	'-Wparentheses',
	'-Wshift-overflow',
	'-Wsign-compare',
	'-Wzero-as-null-pointer-constant',
	'-Wcast-align',
	'-Wcast-qual',
	'-Wcatch-value',
	'-Wchar-subscripts',
	'-Wswitch-default',
	'-Wctor-dtor-privacy',
	'-Wduplicated-branches',
	'-Wduplicated-cond',
	'-Wempty-body',
	'-Wextra-semi',
	'-Wfloat-equal',
	'-Wformat',
	'-Wformat-extra-args',
	'-Wimplicit-fallthrough',
	'-Wmissing-field-initializers',
	'-Wnull-dereference',
	'-Wshadow',
]

if host_machine.system() == 'windows'
	args += '-D_WIN32_WINNT=_WIN32_WINNT_WIN10'
	args += '-DWINVER=_WIN32_WINNT_WIN10'
endif

if cxx.get_id() == 'msvc'
	args += '/std:c11'
elif cxx.get_id() == 'gcc'
	args += warningsLinux
	args += '-std=gnu++11'
endif

# Dependencies

deps = [
	dependency('threads'),
]

if host_machine.system() == 'windows'
	deps += cxx.find_library('ws2_32')
endif

# Application

myApp = executable(
	'app',
	[
		srcs,
	],
	include_directories : include_directories([
		'components/ProcessingCore',
		'components/ProcessingCommon',
	]),
	dependencies : [
		deps,
	],
	cpp_args : [
		args,
	],
)

