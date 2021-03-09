#include "PcHeader.h"

#include <DShow.h>

#include "Trace.h"
#include "TraceSys.h"
#include "TraceWin.h"
#include "SystemTrace.h"

#include "WrapperWin.h"

#include "WinWebcam.h"

#pragma comment(lib, "strmiids")

using namespace wrap_win;


namespace win_cam
{

//+-----------------------------------------------------------------------------+
//! DEVICE_INFO																	!
//+-----------------------------------------------------------------------------+
DEVICE_INFO::DEVICE_INFO()
{
	Clear();
}

DEVICE_INFO::DEVICE_INFO(const DEVICE_INFO& p_gr)
{
	(*this) = p_gr;
}

DEVICE_INFO::~DEVICE_INFO()
{
	Clear();
}

void DEVICE_INFO::Clear()
{
	m_stFriendlyName.clear();
	m_stDevicePath.clear();
}

DEVICE_INFO& DEVICE_INFO::operator=(const DEVICE_INFO& p_gr)
{
	m_stFriendlyName	= p_gr.m_stFriendlyName;
	m_stDevicePath		= p_gr.m_stDevicePath;

	return (*this);
}

//+-----------------------------------------------------------------------------+
//! CWinWebcam																	!
//+-----------------------------------------------------------------------------+
CWinWebcam::CWinWebcam()
{
	m_Webcam = NULL;

	Clear();
}

CWinWebcam::~CWinWebcam()
{
	Clear();
}

void CWinWebcam::Clear()
{
	DELETE_POINTER(m_Webcam);
}

uint32_t CWinWebcam::ForEachMoniker(std::function<uint32_t(IMoniker*)> p_fCall)
{
	uint32_t nError = 0;
	HRESULT hr = S_OK;
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pEnum = NULL;
	IMoniker *pMon = NULL;

	if(!nError) nError = ComCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,IID_ICreateDevEnum,(void**)&pDevEnum);

	if(!nError) hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));
	
	if (!nError && pEnum)
	{
		if(!nError) hr = pEnum->Reset();
		if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));

		while(!nError && pEnum->Next(1, &pMon, NULL) == S_OK)
		{
			nError = p_fCall(pMon);
			RELEASE_POINTER(pMon);
		}
	}

	RELEASE_POINTER(pEnum);
	RELEASE_POINTER(pDevEnum);

	return nError;
}

uint32_t CWinWebcam::GetDeviceInfo(IMoniker* p_pMoniker, DEVICE_INFO& p_grInfo)
{
	uint32_t nError = 0;
	HRESULT hr = S_OK;
	IPropertyBag *pBag = NULL;
	VARIANT var;

	var.vt = VT_BSTR;

	if(!nError) nError = ASSERTION_POINTER(p_pMoniker);
	if(!nError) hr = p_pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));

	if(!nError) hr = pBag->Read(L"FriendlyName", &var, NULL);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));
	if(!nError)
	{
		p_grInfo.m_stFriendlyName = var.bstrVal;
		SysFreeString(var.bstrVal);
	}

	if(!nError) hr = pBag->Read(L"DevicePath", &var, NULL);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));
	if(!nError)
	{
		p_grInfo.m_stDevicePath = var.bstrVal;
		SysFreeString(var.bstrVal);
	}

	RELEASE_POINTER(pBag);

	if(nError) p_grInfo.Clear();
	return nError;
}

uint32_t CWinWebcam::getDeviceInfoList(STL_DEVICE_INFO& p_stlInfo)
{	
	uint32_t nError = 0;
	p_stlInfo.clear();

	if(!nError) nError = ForEachMoniker([&p_stlInfo,this](IMoniker* p_pMon)->uint32_t
	{
		uint32_t nError = 0;

		if(!nError) nError = ASSERTION_POINTER(p_pMon);
		if(!nError)
		{
			p_stlInfo.emplace_back();
			nError = GetDeviceInfo(p_pMon,p_stlInfo.back());
		}

		return nError;
	});

	if(nError) p_stlInfo.clear();
	return nError;
}

uint32_t CWinWebcam::setDeviceIndex(uint32_t deviceIndex)
{
	uint32_t error = STID_NONE;
	DELETE_POINTER(m_Webcam);

	try
	{
		m_Webcam = new cv::VideoCapture(deviceIndex);

		if (!m_Webcam->isOpened())
			error = TRACE_SYS(STE_CAMERA, _T("Can't open Device with given index."));
	}
	catch (...)
	{
		error = TRACE_SYS(STE_CAMERA, _T("Device Index does not exist."));
	}

	return error;
}

uint32_t CWinWebcam::readMat(cv::Mat& mat)
{
	uint32_t error = STID_NONE;

	if (m_Webcam)
	{
		m_Webcam->read(mat);
	}
	else
	{
		error = TRACE_SYS(STE_INIT, _T("Device is missing."));
	}

	return error;
}

double CWinWebcam::getCamWindowProperty(int p_Property)
{
	return cv::getWindowProperty(CAM_WINDOW_NAME, p_Property);
}

void CWinWebcam::destroyCamWindow()
{
	cv::destroyAllWindows();
}

void CWinWebcam::camWinShowImage(cv::InputArray p_Mat)
{
	cv::imshow(CAM_WINDOW_NAME, p_Mat);
}

void CWinWebcam::createCamWindow()
{
	cv::namedWindow(CAM_WINDOW_NAME, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_NORMAL);
}

bool CWinWebcam::setCamProperty(int p_Property, double p_Value)
{
	bool ret = false;

	if (m_Webcam)
	{
		return m_Webcam->set(p_Property, p_Value);
	}

	return ret;
}

double CWinWebcam::getCamProperty(int p_Property)
{
	if (m_Webcam)
	{
		return m_Webcam->get(p_Property);
	}

	return 0.0;
}

}
