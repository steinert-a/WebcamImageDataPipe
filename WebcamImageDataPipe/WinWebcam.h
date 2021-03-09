#pragma once

#define CAM_WINDOW_NAME		"Undocked Cam"

namespace win_cam
{

//+-----------------------------------------------------------------------------+
//! DEVICE_INFO																	!
//+-----------------------------------------------------------------------------+
struct DEVICE_INFO
{
public:
	DEVICE_INFO();
	DEVICE_INFO(const DEVICE_INFO&);
	~DEVICE_INFO();

public:
	void Clear();
	DEVICE_INFO& operator=(const DEVICE_INFO&);

public:
	tstring		m_stFriendlyName;
	tstring		m_stDevicePath;
};

typedef std::list<DEVICE_INFO>		STL_DEVICE_INFO;

//+-----------------------------------------------------------------------------+
//! CWinWebcam																	!
//+-----------------------------------------------------------------------------+
class CWinWebcam
{
public:
	CWinWebcam();
	~CWinWebcam();

public:
	cv::VideoCapture* m_Webcam;

private:
	uint32_t ForEachMoniker(std::function<uint32_t(IMoniker*)>);
	uint32_t GetDeviceInfo(IMoniker*, DEVICE_INFO&);

public:
	void Clear();

	uint32_t setDeviceIndex(uint32_t deviceIndex);
	uint32_t readMat(cv::Mat&);

	uint32_t getDeviceInfoList(STL_DEVICE_INFO&);

	double getCamWindowProperty(int);
	void destroyCamWindow();
	void camWinShowImage(cv::InputArray);
	void createCamWindow();
	bool setCamProperty(int, double);
	double getCamProperty(int);
};


}