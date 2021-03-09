#pragma once
// [MAJOR].[MINOR].[REVISION].[BUILD]

// support macros
#define VERSION_NAME(arg) #arg								// return string macro name
#define VERSION_VALUE(arg) VERSION_NAME(arg)				// return string macro value

// version
#define VERSION_MAJOR		3								// compatibility
#define VERSION_MINOR		0								// feature / functional add on
#define VERSION_REVISION	0								// bugfix
#define VERSION_BUILD		0								// build / clean up / process / ...


#define VERSION_STRING		VERSION_VALUE(VERSION_MAJOR) "." VERSION_VALUE(VERSION_MINOR) "." \
							VERSION_VALUE(VERSION_REVISION) "." VERSION_VALUE(VERSION_BUILD)

#define COMPANY_NAME		"Andreas Steinert"
#define APP_DESCRIPTION		"Image data acquisition with OpenCV"
#define PRODUCT_NAME		"CamImagePipe"
#define ORIGINAL_FILENAME	"WebcamImageDataPipe.exe"
#define INTERNAL_NAME		"Webcam Image Data Pipe"
#define COPYRIGHT			"Copyright (C) Andreas Steinert 2019"

