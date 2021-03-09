#pragma once

#include <QtWidgets/QDialog>
#include "ui_WebcamImageDataPipe.h"

#include "WinWebcam.h"

#include "FilterDefinition.h"
#include "FilterPipe.h"
#include "FilterPipeExe.h"

enum RESOLUTION {
	RESOLUTION_160_120 = 1,
	RESOLUTION_176_144,
	RESOLUTION_192_144,
	RESOLUTION_240_160,
	RESOLUTION_320_240,
	RESOLUTION_352_240,
	RESOLUTION_352_288,
	RESOLUTION_384_288,
	RESOLUTION_480_360,
	RESOLUTION_640_360,
	RESOLUTION_640_480,
	RESOLUTION_704_480,
	RESOLUTION_720_480,	
	RESOLUTION_800_480,
	RESOLUTION_800_600,
	RESOLUTION_960_720,
	RESOLUTION_1024_768,
	RESOLUTION_1280_720,
	RESOLUTION_1280_800,
	RESOLUTION_1280_960,
	RESOLUTION_1280_1024,
	RESOLUTION_1600_1200,
	RESOLUTION_1920_1080,
	RESOLUTION_2048_1536
};

enum COLOR {
	COLOR_RGB = 1,
	COLOR_GRAYSCALE
};

class WebcamImageDataPipe : public QDialog
{
	Q_OBJECT

public:
	WebcamImageDataPipe(QWidget *parent = Q_NULLPTR);
	~WebcamImageDataPipe();

private:
	Ui::WebcamImageDataPipeClass ui;
	win_cam::CWinWebcam m_WebcamAddOn;
	fpe::FilterPipeExecutor m_FilterPipeExecutor;

	int m_TimerId;
	uint32_t m_TimerTact;
	QSettings* m_Settings;
	bool m_DockedCam;
	bool m_ImagePipe;
	cv::Mat m_Source;
	cv::Mat m_Destination;

	cv::Mat m_MatOutQImage;
	cv::Mat m_MatOutCamWin;

private:
	void timerEvent(QTimerEvent *event);
	void outputDebug(uint32_t);
	void toListItem(const xfp::FILTER&, QListWidgetItem*);
	xfp::XFILTERPIPE getFilterPipe();
	void loadFilterFromFile();
	void updateExecutor();
	void setupCam();
	void setResolutionText();

private slots:
	void onTimerTactChanged();
	void onImageDirectory();
	void onReconnectWebcam();
	void onRefreshWebcam();
	void onCreateFilter();
	void onDockCam();
	void onFilterSetPath();
	void onFilterLoad();
	void onFilterSave();
	void onFilterEdit();
	void onFilterClear();
	void onFilterRemove();
	void onFilterMoveUp();
	void onFilterMoveDown();
	void onClearDebug();
	void onValueChangedBrightness(int);
	void onValueChangedContrast(int);
	void onValueChangedSaturation(int);
	void onValueChangedGain(int);
	void onValueChangedExposure(int);
	void onChangeResolution(int);
	void onChangeColor(int);
	void onStartStopCap();
};
