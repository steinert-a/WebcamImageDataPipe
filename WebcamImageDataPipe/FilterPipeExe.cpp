#include "PcHeader.h"

#include "Trace.h"
#include "TraceSys.h"
#include "TraceCStd.h"
#include "SystemTrace.h"
#include "WrapperCStd.h"
#include "StringCore.h"

#include "FilterDefinition.h"
#include "FilterPipe.h"
#include "FilterPipeExe.h"

using namespace wrap_cstd;

namespace fpe
{
//--------------------------------------------------------------+
// Filter                                                       !
//--------------------------------------------------------------+
Filter::Filter(): m_FilterType(FILTER_TYPE_UNKNOWN)
{}

Filter::Filter(const uint32_t& p_FilterType): m_FilterType(p_FilterType)
{}

Filter::~Filter()
{}


//--------------------------------------------------------------+
// FilterGaussianBlur                                           !
//--------------------------------------------------------------+
FilterGaussianBlur::FilterGaussianBlur(): Filter(FILTER_TYPE_GAUSSIAN_BLUR)
{	
	m_KernalSizeX = 3;
	m_KernalSizeY = 3;
	m_SigmaX = 1.5;
	m_SigmaY = 1.5;
	m_Border = cv::BORDER_REPLICATE;
}

FilterGaussianBlur::~FilterGaussianBlur()
{}

//--------------------------------------------------------------+
// FilterSobel                                                  !
//--------------------------------------------------------------+
FilterSobel::FilterSobel() : Filter(FILTER_TYPE_SOBEL)
{
	m_KernalSize = 3;
	m_DeltaX = 1;
	m_DeltaY = 1;
	m_Scale = 1.0;
	m_Delta = 0.0;
	m_Border = cv::BORDER_REPLICATE;
}

FilterSobel::~FilterSobel()
{}

//--------------------------------------------------------------+
// FilterPipeExecutor                                           !
//--------------------------------------------------------------+
FilterPipeExecutor::FilterPipeExecutor()
{
	m_FrontBuffer = new cv::Mat();
	m_BackBuffer = new cv::Mat();
}


FilterPipeExecutor::~FilterPipeExecutor()
{
	DELETE_POINTER(m_FrontBuffer);
	DELETE_POINTER(m_BackBuffer);
	clear();
}

uint32_t FilterPipeExecutor::execute(const cv::Mat& p_Source, cv::Mat& p_Destination)
{
	uint32_t err = STE_NONE;
	p_Source.copyTo(*m_FrontBuffer);

	FOR_EACH(filter, m_FilterPipe)
	{
		if (err) break;

		switch ((*filter)->m_FilterType)
		{
		case FILTER_TYPE_GAUSSIAN_BLUR:
			try
			{
				FilterGaussianBlur* pFilter = (FilterGaussianBlur*)(*filter);
				cv::GaussianBlur(*m_FrontBuffer, *m_BackBuffer,
					cv::Size(pFilter->m_KernalSizeX, pFilter->m_KernalSizeY),
					pFilter->m_SigmaX, pFilter->m_SigmaY, pFilter->m_Border);
				swapBuffer();
			}
			catch (cv::Exception& p_Err)
			{
				std::string str(p_Err.what());
				err = TRACE_SYS(STE_EXCEPTION, core::A2W(str).c_str());
			}
			break;
		case FILTER_TYPE_SOBEL:
			try
			{
				FilterSobel* pFilter = (FilterSobel*)(*filter);
				cv::Sobel(*m_FrontBuffer, *m_BackBuffer, -1,
					pFilter->m_DeltaX, pFilter->m_DeltaY,
					pFilter->m_KernalSize, pFilter->m_Scale, pFilter->m_Delta,
					pFilter->m_Border);
				swapBuffer();
			}
			catch (cv::Exception& p_Err)
			{
				std::string str(p_Err.what());
				err = TRACE_SYS(STE_EXCEPTION, core::A2W(str).c_str());
			}
			break;
		case FILTER_TYPE_UNKNOWN:
		default:
			err = TRACE_SYS(STE_TYPE, _T("Unknown filter type"));
		}	
	}

	m_FrontBuffer->copyTo(p_Destination);
	return err;
}

uint32_t FilterPipeExecutor::reinit(const xfp::XFILTERPIPE& p_FilterPipe)
{
	uint32_t err = STE_NONE;
	m_FilterPipe.clear();

	FOR_EACH(filter, p_FilterPipe.m_Filters)
	{	
		if (filter->m_Flag & FILTER_FLAG_ENABLE)
		{
			if (!err && !filter->m_Type.compare(FILTER_NAME_GAUSSIAN_BLUR))
			{
					FilterGaussianBlur* pFilter = new FilterGaussianBlur();
					m_FilterPipe.push_back(pFilter);

					tstring ksizex, ksizey, sigmax, sigmay, border;
					ksizex = xfp::getOption(filter->m_Options, FILTER_KEY_KERNAL_SIZE_X, _T("3"));
					ksizey = xfp::getOption(filter->m_Options, FILTER_KEY_KERNAL_SIZE_Y, _T("3"));
					sigmax = xfp::getOption(filter->m_Options, FILTER_KEY_SIGMA_X, _T("1.5"));
					sigmay = xfp::getOption(filter->m_Options, FILTER_KEY_SIGMA_Y, _T("1.5"));
					border = xfp::getOption(filter->m_Options, FILTER_KEY_BORDER, _T("1"));
				
					if (!err) err = StrToUInt32(ksizex.c_str(), pFilter->m_KernalSizeX);
					if (!err) err = StrToUInt32(ksizey.c_str(), pFilter->m_KernalSizeY);
					if (!err) err = StrToDouble(sigmax.c_str(), pFilter->m_SigmaX);
					if (!err) err = StrToDouble(sigmay.c_str(), pFilter->m_SigmaY);
					if (!err) err = StrToUInt32(border.c_str(), pFilter->m_Border);
			}
			else if (!err && !filter->m_Type.compare(FILTER_NAME_SOBEL))
			{
				FilterSobel* pFilter = new FilterSobel();
				m_FilterPipe.push_back(pFilter);

				tstring ksize, depth, deltax, deltay, scale, delta, border;
				ksize = xfp::getOption(filter->m_Options, FILTER_KEY_KERNAL_SIZE, _T("3"));
				deltax = xfp::getOption(filter->m_Options, FILTER_KEY_DELTA_X, _T("1"));
				deltay = xfp::getOption(filter->m_Options, FILTER_KEY_DELTA_Y, _T("1"));
				scale = xfp::getOption(filter->m_Options, FILTER_KEY_SCALE, _T("1.0"));
				delta = xfp::getOption(filter->m_Options, FILTER_KEY_DELTA, _T("0.0"));
				border = xfp::getOption(filter->m_Options, FILTER_KEY_BORDER, _T("1"));

				if (!err) err = StrToUInt32(ksize.c_str(), pFilter->m_KernalSize);
				if (!err) err = StrToUInt32(deltax.c_str(), pFilter->m_DeltaX);
				if (!err) err = StrToUInt32(deltay.c_str(), pFilter->m_DeltaY);
				if (!err) err = StrToDouble(scale.c_str(), pFilter->m_Scale);
				if (!err) err = StrToDouble(delta.c_str(), pFilter->m_Delta);
				if (!err) err = StrToUInt32(border.c_str(), pFilter->m_Border);
			}
			else
			{
				err = TRACE_SYS(STE_TYPE, _T("Unknown filter type"));
			}
		}

		if (err) break;
	}

	if (err) m_FilterPipe.clear();
	return err;
}

void FilterPipeExecutor::clear()
{
	FOR_EACH(it, m_FilterPipe)
	{
		DELETE_POINTER(*it);
	}
	m_FilterPipe.clear();
}

void FilterPipeExecutor::swapBuffer()
{
	std::swap(m_FrontBuffer, m_BackBuffer);
}

}
