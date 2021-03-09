#include "PcHeader.h"

#include "FilterDefinition.h"

namespace xfp
{
//--------------------------------------------------------------+
// default                                                      !
//--------------------------------------------------------------+
tstring getFilterId()
{
	return QUuid::createUuid().toString().toStdWString();
}

FILTER getDefaultFilterUnknown()
{
	xfp::FILTER filter;
	filter.m_Type = FILTER_NAME_UNKNOWN;
	filter.m_Flag = 0;
	filter.m_Id = getFilterId();

	return filter;
}

FILTER getDefaultFilterGaussianBlur()
{
	xfp::FILTER filter;
	filter.m_Type = FILTER_NAME_GAUSSIAN_BLUR;
	filter.m_Flag = FILTER_FLAG_ENABLE;
	filter.m_Id = getFilterId();

	xfp::setOption(filter.m_Options, FILTER_KEY_KERNAL_SIZE_X, _T("5"));
	xfp::setOption(filter.m_Options, FILTER_KEY_KERNAL_SIZE_Y, _T("5"));
	xfp::setOption(filter.m_Options, FILTER_KEY_SIGMA_X, _T("1.5"));
	xfp::setOption(filter.m_Options, FILTER_KEY_SIGMA_Y, _T("1.5"));
	xfp::setOption(filter.m_Options, FILTER_KEY_BORDER, _T("1"));

	return filter;
}

FILTER getDefaultFilterSobel()
{
	xfp::FILTER filter;
	filter.m_Type = FILTER_NAME_SOBEL;
	filter.m_Flag = FILTER_FLAG_ENABLE;
	filter.m_Id = getFilterId();

	xfp::setOption(filter.m_Options, FILTER_KEY_KERNAL_SIZE, _T("3"));
	xfp::setOption(filter.m_Options, FILTER_KEY_DELTA_X, _T("1"));
	xfp::setOption(filter.m_Options, FILTER_KEY_DELTA_Y, _T("1"));
	xfp::setOption(filter.m_Options, FILTER_KEY_SCALE, _T("1.0"));
	xfp::setOption(filter.m_Options, FILTER_KEY_DELTA, _T("0.0"));
	xfp::setOption(filter.m_Options, FILTER_KEY_BORDER, _T("1"));

	return filter;
}

}

