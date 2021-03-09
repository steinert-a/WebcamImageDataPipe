#pragma once

#include "FilterPipe.h"

#define FILTER_NAME_UNKNOWN			_T("UNKNOWN")
#define FILTER_NAME_GAUSSIAN_BLUR	_T("GAUSSIAN_BLUR")
#define FILTER_NAME_SOBEL			_T("SOBEL")

#define FILTER_TYPE_UNKNOWN			0
#define FILTER_TYPE_GAUSSIAN_BLUR	1
#define FILTER_TYPE_SOBEL			2

#define FILTER_FLAG_ENABLE			1

#define FILTER_KEY_KERNAL_SIZE		_T("kernal-size")
#define FILTER_KEY_KERNAL_SIZE_X	_T("kernal-size-x")
#define FILTER_KEY_KERNAL_SIZE_Y	_T("kernal-size-y")
#define FILTER_KEY_SIGMA			_T("sigma")
#define FILTER_KEY_SIGMA_X			_T("sigma-x")
#define FILTER_KEY_SIGMA_Y			_T("sigma-y")
#define FILTER_KEY_DEPTH			_T("depth")
#define FILTER_KEY_DELTA			_T("delta")
#define FILTER_KEY_DELTA_X			_T("delta-x")
#define FILTER_KEY_DELTA_Y			_T("delta-y")
#define FILTER_KEY_BORDER			_T("border")
#define FILTER_KEY_SCALE			_T("scale")
#define FILTER_KEY_SCALE_X			_T("scale-x")
#define FILTER_KEY_SCALE_Y			_T("scale-y")


namespace xfp
{
//--------------------------------------------------------------+
// default                                                      !
//--------------------------------------------------------------+
tstring getFilterId();
FILTER getDefaultFilterUnknown();
FILTER getDefaultFilterGaussianBlur();
FILTER getDefaultFilterSobel();

}
