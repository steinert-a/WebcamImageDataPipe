#pragma once

namespace fpe
{
//--------------------------------------------------------------+
// Filter                                                       !
//--------------------------------------------------------------+
class Filter
{
public:
	Filter();
	Filter(const uint32_t&);
	~Filter();

public:
	uint32_t m_FilterType;
};

typedef std::list<Filter*> FILTER_PIPE;

//--------------------------------------------------------------+
// FilterGaussianBlur                                           !
//--------------------------------------------------------------+
class FilterGaussianBlur: public Filter
{
public:
	FilterGaussianBlur();
	~FilterGaussianBlur();

public:
	uint32_t m_KernalSizeX;
	uint32_t m_KernalSizeY;
	double m_SigmaX;
	double m_SigmaY;
	uint32_t m_Border;
};

//--------------------------------------------------------------+
// FilterSobel                                                  !
//--------------------------------------------------------------+
class FilterSobel: public Filter
{
public:
	FilterSobel();
	~FilterSobel();

public:
	uint32_t m_KernalSize;
	uint32_t m_DeltaX;
	uint32_t m_DeltaY;
	double m_Scale;
	double m_Delta;
	uint32_t m_Border;
};

//--------------------------------------------------------------+
// FilterPipeExecutor                                           !
//--------------------------------------------------------------+
class FilterPipeExecutor
{
public:
	FilterPipeExecutor();
	~FilterPipeExecutor();

private:
	FILTER_PIPE m_FilterPipe;
	cv::Mat* m_FrontBuffer;
	cv::Mat* m_BackBuffer;

public:
	uint32_t execute(const cv::Mat&, cv::Mat&);
	uint32_t reinit(const xfp::XFILTERPIPE&);
	void clear();
	void swapBuffer();
};

}