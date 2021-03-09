#include "PcHeader.h"
#include "WebcamImageDataPipe.h"

#include "Version.h"
#include "Trace.h"
#include "TraceSys.h"
#include "TraceCStd.h"
#include "SystemTrace.h"
#include "StringCore.h"
#include "WrapperCStd.h"

#include "DlgFilterSetup.h"

#define DEFAULT_TIMER_TACT		500
#define MAX_DEBUG_ITEM_COUNT	50

using namespace std;

WebcamImageDataPipe::WebcamImageDataPipe(QWidget *parent)
	: QDialog(parent)
{	
	ui.setupUi(this);

	uint32_t err = STID_NONE;
	m_TimerId = startTimer(1000);
	m_DockedCam = true;
	m_ImagePipe = false;

	m_Settings = new QSettings(qApp->applicationDirPath() + "/ImageDataPipe.ini", QSettings::IniFormat);
	m_TimerTact = m_Settings->value("pipe/timer-tact", DEFAULT_TIMER_TACT).toUInt();

	this->ui.labelFooter->setText(QString("(c) Andreas Steinert"));
	this->ui.lineEditTimerTactMs->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
	this->ui.lineEditTimerTactMs->setText(m_Settings->value("pipe/timer-tact", DEFAULT_TIMER_TACT).toString());
	this->ui.lineEditImageDir->setText(m_Settings->value("pipe/image-dir", "").toString());
	QString filePipeXml = m_Settings->value("filter/path", qApp->applicationDirPath() + "/ImageDataPipe.xml").toString();
	this->ui.lineEditFilterSaveAs->setText(filePipeXml);

	this->ui.comboBoxFilter->addItem(QString::fromWCharArray(FILTER_NAME_GAUSSIAN_BLUR), FILTER_TYPE_GAUSSIAN_BLUR);
	this->ui.comboBoxFilter->addItem(QString::fromWCharArray(FILTER_NAME_SOBEL), FILTER_TYPE_SOBEL);

	this->ui.horizontalSliderBrightness->setMinimum(0);
	this->ui.horizontalSliderBrightness->setMaximum(1000);
	this->ui.horizontalSliderBrightness->setValue(10 * m_Settings->value("webcam/brightness", 50).toDouble());
	this->ui.horizontalSliderContrast->setMinimum(0);
	this->ui.horizontalSliderContrast->setMaximum(1000);
	this->ui.horizontalSliderContrast->setValue(10 * m_Settings->value("webcam/contrast", 50).toDouble());
	this->ui.horizontalSliderSaturation->setMinimum(0);
	this->ui.horizontalSliderSaturation->setMaximum(1000);
	this->ui.horizontalSliderSaturation->setValue(10 * m_Settings->value("webcam/saturation", 50).toDouble());
	this->ui.horizontalSliderGain->setMinimum(0);
	this->ui.horizontalSliderGain->setMaximum(1000);
	this->ui.horizontalSliderGain->setValue(10 * m_Settings->value("webcam/gain", 50).toDouble());
	this->ui.horizontalSliderExposure->setMinimum(0);
	this->ui.horizontalSliderExposure->setMaximum(1000);
	this->ui.horizontalSliderExposure->setValue(10 * m_Settings->value("webcam/exposure", 50).toDouble());
	this->ui.comboBoxResolution->addItem(QString("162x120"), RESOLUTION::RESOLUTION_160_120);
	this->ui.comboBoxResolution->addItem(QString("176x144"), RESOLUTION::RESOLUTION_176_144);
	this->ui.comboBoxResolution->addItem(QString("192x144"), RESOLUTION::RESOLUTION_192_144);
	this->ui.comboBoxResolution->addItem(QString("240x160"), RESOLUTION::RESOLUTION_240_160);
	this->ui.comboBoxResolution->addItem(QString("320x240"), RESOLUTION::RESOLUTION_320_240);
	this->ui.comboBoxResolution->addItem(QString("352x240"), RESOLUTION::RESOLUTION_352_240);
	this->ui.comboBoxResolution->addItem(QString("352x288"), RESOLUTION::RESOLUTION_352_288);
	this->ui.comboBoxResolution->addItem(QString("384x288"), RESOLUTION::RESOLUTION_384_288);
	this->ui.comboBoxResolution->addItem(QString("480x360"), RESOLUTION::RESOLUTION_480_360);
	this->ui.comboBoxResolution->addItem(QString("640x360"), RESOLUTION::RESOLUTION_640_360);
	this->ui.comboBoxResolution->addItem(QString("640x480"), RESOLUTION::RESOLUTION_640_480);
	this->ui.comboBoxResolution->addItem(QString("704x480"), RESOLUTION::RESOLUTION_704_480);
	this->ui.comboBoxResolution->addItem(QString("720x480"), RESOLUTION::RESOLUTION_720_480);
	this->ui.comboBoxResolution->addItem(QString("800x480"), RESOLUTION::RESOLUTION_800_480);
	this->ui.comboBoxResolution->addItem(QString("800x600"), RESOLUTION::RESOLUTION_800_600);
	this->ui.comboBoxResolution->addItem(QString("960x720"), RESOLUTION::RESOLUTION_960_720);
	this->ui.comboBoxResolution->addItem(QString("1024x768"), RESOLUTION::RESOLUTION_1024_768);
	this->ui.comboBoxResolution->addItem(QString("1280x720"), RESOLUTION::RESOLUTION_1280_720);
	this->ui.comboBoxResolution->addItem(QString("1280x800"), RESOLUTION::RESOLUTION_1280_800);
	this->ui.comboBoxResolution->addItem(QString("1280x960"), RESOLUTION::RESOLUTION_1280_960);
	this->ui.comboBoxResolution->addItem(QString("1280x1024"), RESOLUTION::RESOLUTION_1280_1024);
	this->ui.comboBoxResolution->addItem(QString("1600x1200"), RESOLUTION::RESOLUTION_1600_1200);
	this->ui.comboBoxResolution->addItem(QString("1920x1080"), RESOLUTION::RESOLUTION_1920_1080);
	this->ui.comboBoxResolution->addItem(QString("2048x1536"), RESOLUTION::RESOLUTION_2048_1536);
	this->ui.comboBoxResolution->setCurrentIndex(m_Settings->value("webcam/resolution", 9).toInt());

	this->ui.comboBoxColor->addItem(QString("RGB"), COLOR::COLOR_RGB);
	this->ui.comboBoxColor->addItem(QString("GRAYSCALE"), COLOR::COLOR_GRAYSCALE);
	this->ui.comboBoxColor->setCurrentIndex(m_Settings->value("webcam/color", 0).toInt());

	connect(this->ui.pushImageDirectory, SIGNAL(clicked()), this, SLOT(onImageDirectory()));
	connect(this->ui.pushButtonRefreshWebcam, SIGNAL(clicked()), this, SLOT(onRefreshWebcam()));
	connect(this->ui.pushButtonReconnectWebcam, SIGNAL(clicked()), this, SLOT(onReconnectWebcam()));
	connect(this->ui.lineEditTimerTactMs, SIGNAL(editingFinished()), this, SLOT(onTimerTactChanged()));
	connect(this->ui.pushButtonCamDocker, SIGNAL(clicked()), this, SLOT(onDockCam()));
	connect(this->ui.pushButtonFilterSetPath, SIGNAL(clicked()), this, SLOT(onFilterSetPath()));
	connect(this->ui.pushButtonFilterLoad, SIGNAL(clicked()), this, SLOT(onFilterLoad()));
	connect(this->ui.pushButtonFilterSave, SIGNAL(clicked()), this, SLOT(onFilterSave()));
	connect(this->ui.pushButtonFilterEdit, SIGNAL(clicked()), this, SLOT(onFilterEdit()));
	connect(this->ui.pushButtonFilterClear, SIGNAL(clicked()), this, SLOT(onFilterClear()));
	connect(this->ui.pushButtonFilterRemove, SIGNAL(clicked()), this, SLOT(onFilterRemove()));
	connect(this->ui.pushButtonFilterUp, SIGNAL(clicked()), this, SLOT(onFilterMoveUp()));
	connect(this->ui.pushButtonFilterDown, SIGNAL(clicked()), this, SLOT(onFilterMoveDown()));
	connect(this->ui.pushButtonFilterCreate, SIGNAL(clicked()), this, SLOT(onCreateFilter()));
	connect(this->ui.pushButtonClearDebug, SIGNAL(clicked()), this, SLOT(onClearDebug()));
	connect(this->ui.horizontalSliderBrightness, SIGNAL(valueChanged(int)), this, SLOT(onValueChangedBrightness(int)));
	connect(this->ui.horizontalSliderContrast, SIGNAL(valueChanged(int)), this, SLOT(onValueChangedContrast(int)));
	connect(this->ui.horizontalSliderSaturation, SIGNAL(valueChanged(int)), this, SLOT(onValueChangedSaturation(int)));
	connect(this->ui.horizontalSliderGain, SIGNAL(valueChanged(int)), this, SLOT(onValueChangedGain(int)));
	connect(this->ui.horizontalSliderExposure, SIGNAL(valueChanged(int)), this, SLOT(onValueChangedExposure(int)));
	connect(this->ui.comboBoxResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(onSetResolution(int)));
	connect(this->ui.comboBoxColor, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeColor(int)));
	connect(this->ui.pushStartStopCap, SIGNAL(clicked()), this, SLOT(onStartStopCap()));

	onRefreshWebcam();
	onReconnectWebcam();

	loadFilterFromFile();
	this->adjustSize();
}

WebcamImageDataPipe::~WebcamImageDataPipe()
{}

void WebcamImageDataPipe::loadFilterFromFile()
{
	uint32_t err = STID_NONE;
	xfp::XFILTERPIPE filterPipe;
	this->ui.listWidgetFilter->clear();

	QString filePath = this->ui.lineEditFilterSaveAs->text();
	err = xfp::LoadXFilterPipe(filePath.toStdWString(), filterPipe);

	if (!err)
	{
		FOR_EACH(it, filterPipe.m_Filters)
		{
			QListWidgetItem* item = new QListWidgetItem;
			toListItem(*it, item);
			this->ui.listWidgetFilter->insertItem(this->ui.listWidgetFilter->count(), item);
		}		
	}
	else
	{
		outputDebug(err);
	}

	updateExecutor();
}

void WebcamImageDataPipe::timerEvent(QTimerEvent *event)
{
	killTimer(m_TimerId);
	uint32_t err = STID_NONE;

	QString fileName = QDateTime::currentDateTimeUtc().toString("yyyy-dd-MM-HH-mm-ss-zzz-'Z'");
	this->ui.labelTimer->setText(fileName);

	auto size = this->ui.labelWebcam->size();
	QImage::Format colorFormat = QImage::Format_RGB888;

	if (!err)
	{
		err = m_WebcamAddOn.readMat(m_Source);
		if (err == STID_NONE && m_Source.empty())
		{
			err = TRACE_SYS(STE_ARRAY, _T("Unexpected empty image."));
		}
	}

	if (!err)
	{
		err = m_FilterPipeExecutor.execute(m_Source, m_Destination);
	}

	if (!err)
	{
		try
		{
			switch (this->ui.comboBoxColor->currentData().toInt())
			{
			case COLOR_GRAYSCALE:
				cv::cvtColor(m_Destination, m_MatOutQImage, cv::COLOR_BGR2GRAY);
				cv::cvtColor(m_Destination, m_MatOutCamWin, cv::COLOR_BGR2GRAY);
				colorFormat = QImage::Format_Grayscale8;
				break;
			case COLOR_RGB:
			default:
				cv::cvtColor(m_Destination, m_MatOutQImage, cv::COLOR_BGR2RGB);
				m_Destination.copyTo(m_MatOutCamWin);
				colorFormat = QImage::Format_RGB888;
			}
		}
		catch (cv::Exception& p_Err)
		{
			std::string str(p_Err.what());
			err = TRACE_SYS(STE_EXCEPTION, core::A2W(str).c_str());
		}
	}

	if (!err)
	{
		QImage image((uchar*)m_MatOutQImage.data, m_MatOutQImage.cols,
			m_MatOutQImage.rows, m_MatOutQImage.step, colorFormat);
		QImage qPreviewImage = image.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
		this->ui.labelWebcam->setPixmap(QPixmap::fromImage(qPreviewImage));

		if (m_ImagePipe)
		{
			QString filePath = this->ui.lineEditImageDir->text();
			filePath += "/" + fileName + ".png";
			// qPreviewImage.save(filePath, "PNG");
			cv::imwrite(filePath.toStdString().c_str(), m_MatOutCamWin);
		}

		if (!m_DockedCam)
		{
			if (m_WebcamAddOn.getCamWindowProperty(cv::WND_PROP_VISIBLE))
			{
				m_WebcamAddOn.camWinShowImage(m_MatOutCamWin);
			}
			else
			{
				m_DockedCam = true;
				m_WebcamAddOn.destroyCamWindow();
				this->ui.pushButtonCamDocker->setText("Undock Cam");
			}
		}
	}

	if (err) outputDebug(err);

	m_TimerId = startTimer(m_TimerTact);
}

void WebcamImageDataPipe::outputDebug(uint32_t error)
{
	tstring errorText = _T("");

	if (SYS_TRACE->DetailedErrorId(error, errorText))
	{
		QListWidgetItem *newItem = new QListWidgetItem;
		newItem->setText(QString::fromStdWString(errorText));
		this->ui.listWidgetDebug->insertItem(0, newItem);
	}

	if (this->ui.listWidgetDebug->count() > MAX_DEBUG_ITEM_COUNT)
	{
		delete this->ui.listWidgetDebug->takeItem(MAX_DEBUG_ITEM_COUNT);
	}
}

void WebcamImageDataPipe::onImageDirectory()
{
	QString imageDir = QFileDialog::getExistingDirectory(this, "Select Image Destination");

	if (!imageDir.isEmpty())
	{
		m_Settings->setValue("pipe/image-dir", imageDir);
		this->ui.lineEditImageDir->setText(imageDir);
	}
}

void WebcamImageDataPipe::onRefreshWebcam()
{
	win_cam::STL_DEVICE_INFO deviceList;
	m_WebcamAddOn.getDeviceInfoList(deviceList);

	this->ui.comboBoxWebcamSelection->clear();
	
	uint32_t idx = 0;
	FOR_EACH(it, deviceList) 
	{
		this->ui.comboBoxWebcamSelection->addItem(QString::fromStdWString(it->m_stFriendlyName), QVariant(idx));
		idx++;
	}

	uint32_t idxCurrent = m_Settings->value("webcam/source", 0).toUInt();
	this->ui.comboBoxWebcamSelection->setCurrentIndex(idxCurrent);
}

void WebcamImageDataPipe::onReconnectWebcam()
{
	uint32_t err = STID_NONE;
	QVariant idx = this->ui.comboBoxWebcamSelection->currentIndex();

	if (idx.isValid())
	{
		m_Settings->setValue("webcam/source", idx.toUInt());
		m_WebcamAddOn.setDeviceIndex(idx.toUInt());
		if (err != STID_NONE) outputDebug(err);
		else setupCam();
	}

}

void WebcamImageDataPipe::onTimerTactChanged()
{
	killTimer(m_TimerId);
	m_TimerTact = this->ui.lineEditTimerTactMs->text().toUInt();
	m_Settings->setValue("pipe/timer-tact", this->ui.lineEditTimerTactMs->text());
	m_TimerId = startTimer(m_TimerTact);
}

void WebcamImageDataPipe::onDockCam()
{	
	m_DockedCam = !m_DockedCam;

	if (m_DockedCam)
	{
		m_WebcamAddOn.destroyCamWindow();
		this->ui.pushButtonCamDocker->setText("Undock Cam");
	}
	else
	{
		m_WebcamAddOn.createCamWindow();
		this->ui.pushButtonCamDocker->setText("Dock Cam");
	}
}

void WebcamImageDataPipe::onFilterSetPath()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Filter Pipe"), "", tr("XML Files (*.xml)"));

	if (fileName.length())
	{
		TRACER(fileName.toStdWString().c_str());
		this->ui.lineEditFilterSaveAs->setText(fileName);
		m_Settings->setValue("filter/path", fileName);
	}
}

void WebcamImageDataPipe::onFilterLoad()
{
	int ret = QMessageBox::question(this,
		"Load XML file", "Load filter pipe XML file and discard all changes?",
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

	if (ret == QMessageBox::Ok)
	{
		loadFilterFromFile();
	}
}

void WebcamImageDataPipe::onFilterSave()
{
	int ret = QMessageBox::question(this,
		"Save XML file", "Save or overwrite filter pipe XML file?",
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

	if (ret == QMessageBox::Ok)
	{
		xfp::XFILTERPIPE filterPipe = getFilterPipe();
		QString filePath = this->ui.lineEditFilterSaveAs->text();
		uint32_t err = xfp::SaveXFilterPipe(filterPipe, filePath.toStdWString());
		if (err) outputDebug(err);
	}
}

void WebcamImageDataPipe::onCreateFilter()
{
	uint32_t err = STE_NONE;
	xfp::FILTER filter;

	switch (this->ui.comboBoxFilter->currentData().toUInt())
	{
	case FILTER_TYPE_GAUSSIAN_BLUR:
		filter = xfp::getDefaultFilterGaussianBlur();
		break;
	case FILTER_TYPE_SOBEL:
		filter = xfp::getDefaultFilterSobel();
		break;
	case FILTER_TYPE_UNKNOWN:
	default:
		filter = xfp::getDefaultFilterUnknown();
		err = TRACE_SYS(STE_TYPE, _T("Unknown filter type."));
		outputDebug(err);
		break;
	}

	if (!err)
	{
		DlgFilterSetup dlg(this, filter);
		if (dlg.exec())
		{
			filter = dlg.getFilter();
			QListWidgetItem* item = new QListWidgetItem;
			toListItem(filter, item);
			this->ui.listWidgetFilter->insertItem(this->ui.listWidgetFilter->count(), item);
			updateExecutor();
		}
	}
}

void WebcamImageDataPipe::toListItem(const xfp::FILTER& p_Filter, QListWidgetItem* p_Item)
{
	if(!p_Item) p_Item = new QListWidgetItem;

	QString text = QString::fromStdWString(p_Filter.m_Type);
	if (p_Filter.m_Options.size())
	{
		QString subText;
		uint32_t count = 0;
		FOR_EACH(it, p_Filter.m_Options)
		{
			if (count % 4 == 0) subText += "\n";
			if (count % 4 != 0) subText += " / ";
			subText += QString("%1: %2").arg(QString::fromStdWString(it->m_Name), QString::fromStdWString(it->m_Value));
			count++;
		}
		text += subText;
	}
	
	tstring xml;
	xfp::FilterToString(p_Filter, xml);

	p_Item->setData(Qt::UserRole, QString::fromStdWString(xml));
	p_Item->setData(Qt::DisplayRole, text);

	if (p_Filter.m_Flag & FILTER_FLAG_ENABLE)
	{
		p_Item->setForeground(QBrush(QColor(55,55,155)));
	}
	else
	{
		p_Item->setForeground(Qt::gray);
	}
}

xfp::XFILTERPIPE WebcamImageDataPipe::getFilterPipe()
{
	xfp::XFILTERPIPE pipe;

	for (int i = 0; i < this->ui.listWidgetFilter->count(); i++)
	{
		QListWidgetItem* item = this->ui.listWidgetFilter->item(i);
		xfp::FILTER filter;
		QString xml = item->data(Qt::UserRole).toString();
		xfp::FilterFromString(xml.toStdWString(), filter);
		pipe.m_Filters.push_back(filter);
	}

	xfp::setOption(pipe.m_Config.m_Options, _T("version"), QString::fromLatin1(VERSION_STRING).toStdWString());

	return pipe;
}

void WebcamImageDataPipe::onFilterEdit()
{
	QListWidgetItem* item = this->ui.listWidgetFilter->currentItem();

	if (item)
	{
		QString xml = item->data(Qt::UserRole).toString();
		xfp::FILTER filter;
		xfp::FilterFromString(xml.toStdWString(), filter);

		DlgFilterSetup dlg(this, filter);
		if (dlg.exec())
		{
			filter = dlg.getFilter();
			toListItem(filter, item);
			updateExecutor();
		}
	}
}

void WebcamImageDataPipe::onFilterClear()
{
	int ret = QMessageBox::question(this,
		"Clear Pipe", "Remove all filters from pipe?",
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

	if (ret == QMessageBox::Ok)
	{
		this->ui.listWidgetFilter->clear();
		updateExecutor();
	}
}

void WebcamImageDataPipe::onFilterRemove()
{
	QListWidgetItem* item = this->ui.listWidgetFilter->currentItem();

	if (item)
	{
		QString xml = item->data(Qt::UserRole).toString();
		xfp::FILTER filter;
		xfp::FilterFromString(xml.toStdWString(), filter);

		int ret = QMessageBox::question(this,
			"Remove Filter", "Remove selected filter?",
			QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

		if (ret == QMessageBox::Ok)
		{
			this->ui.listWidgetFilter->takeItem(this->ui.listWidgetFilter->row(item));
			updateExecutor();
		}
	}
}

void WebcamImageDataPipe::onFilterMoveUp()
{
	QListWidgetItem* item = this->ui.listWidgetFilter->currentItem();
	int row = this->ui.listWidgetFilter->row(item);

	if (item && row != 0)
	{
		this->ui.listWidgetFilter->takeItem(row);
		this->ui.listWidgetFilter->insertItem(row - 1, item);
		this->ui.listWidgetFilter->setCurrentItem(item);
		updateExecutor();
	}
}

void WebcamImageDataPipe::onFilterMoveDown()
{
	QListWidgetItem* item = this->ui.listWidgetFilter->currentItem();
	int row = this->ui.listWidgetFilter->row(item);

	if (item && row < this->ui.listWidgetFilter->count()-1)
	{
		this->ui.listWidgetFilter->takeItem(row);
		this->ui.listWidgetFilter->insertItem(row + 1, item);
		this->ui.listWidgetFilter->setCurrentItem(item);
		updateExecutor();
	}
}

void WebcamImageDataPipe::updateExecutor()
{
	xfp::XFILTERPIPE pipe = getFilterPipe();
	uint32_t err = STE_NONE;
	err = m_FilterPipeExecutor.reinit(pipe);
	if (err)
	{
		outputDebug(err);
	}
}

void WebcamImageDataPipe::onClearDebug()
{
	this->ui.listWidgetDebug->clear();
}

void WebcamImageDataPipe::setupCam()
{
	onValueChangedBrightness(this->ui.horizontalSliderBrightness->value());
	onValueChangedContrast(this->ui.horizontalSliderContrast->value());
	onValueChangedSaturation(this->ui.horizontalSliderSaturation->value());
	onValueChangedGain(this->ui.horizontalSliderGain->value());
	onValueChangedExposure(this->ui.horizontalSliderExposure->value());

	onChangeResolution(this->ui.comboBoxResolution->currentIndex());

	setResolutionText();
}

void WebcamImageDataPipe::onValueChangedBrightness(int newValue)
{
	double percent = newValue / 10.0;
	double minVal = m_Settings->value("webcam/brightness-min", 0).toDouble();
	double maxVal = m_Settings->value("webcam/brightness-max", 250).toDouble();
	double val = (percent / 100.0) * (maxVal - minVal) + minVal;

	m_Settings->setValue("webcam/brightness", percent);
	m_WebcamAddOn.setCamProperty(cv::CAP_PROP_BRIGHTNESS, val);
}

void WebcamImageDataPipe::onValueChangedContrast(int newValue)
{
	double percent = newValue / 10.0;
	double minVal = m_Settings->value("webcam/contrast-min", 0).toDouble();
	double maxVal = m_Settings->value("webcam/contrast-max", 100).toDouble();
	double val = (percent / 100.0) * (maxVal - minVal) + minVal;

	m_Settings->setValue("webcam/contrast", percent);
	m_WebcamAddOn.setCamProperty(cv::CAP_PROP_CONTRAST, val);
}

void WebcamImageDataPipe::onValueChangedSaturation(int newValue)
{
	double percent = newValue / 10.0;
	double minVal = m_Settings->value("webcam/saturation-min", 0).toDouble();
	double maxVal = m_Settings->value("webcam/saturation-max", 250).toDouble();
	double val = (percent / 100.0) * (maxVal - minVal) + minVal;

	m_Settings->setValue("webcam/saturation", percent);
	m_WebcamAddOn.setCamProperty(cv::CAP_PROP_SATURATION, val);
}

void WebcamImageDataPipe::onValueChangedGain(int newValue)
{
	double percent = newValue / 10.0;
	double minVal = m_Settings->value("webcam/gain-min", 0).toDouble();
	double maxVal = m_Settings->value("webcam/gain-max", 250).toDouble();
	double val = (percent / 100.0) * (maxVal - minVal) + minVal;

	m_Settings->setValue("webcam/gain", percent);
	m_WebcamAddOn.setCamProperty(cv::CAP_PROP_GAIN, val);
}

void WebcamImageDataPipe::onValueChangedExposure(int newValue)
{
	double percent = newValue / 10.0;
	double minVal = m_Settings->value("webcam/exposure-min", -20).toDouble();
	double maxVal = m_Settings->value("webcam/exposure-max", 10).toDouble();
	double val = (percent / 100.0) * (maxVal - minVal) + minVal;

	m_Settings->setValue("webcam/exposure", percent);
	m_WebcamAddOn.setCamProperty(cv::CAP_PROP_EXPOSURE, val);
}

void WebcamImageDataPipe::onChangeResolution(int p_Idx)
{
	m_Settings->setValue("webcam/resolution", p_Idx);
	switch (this->ui.comboBoxResolution->currentData().toInt())
	{
	case RESOLUTION_160_120:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 160);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 120);
		break;
	case RESOLUTION_176_144:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 176);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 144);
		break;
	case RESOLUTION_192_144:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 192);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 144);
		break;
	case RESOLUTION_240_160:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 240);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 160);
		break;
	case RESOLUTION_320_240:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 320);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 240);
		break;
	case RESOLUTION_352_240:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 352);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 240);
		break;
	case RESOLUTION_352_288:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 352);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 288);
		break;
	case RESOLUTION_384_288:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 384);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 288);
		break;
	case RESOLUTION_480_360:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 480);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 360);
		break;
	case RESOLUTION_640_360:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 640);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 360);
		break;
	case RESOLUTION_640_480:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 640);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 480);
		break;
	case RESOLUTION_704_480:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 704);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 480);
		break;
	case RESOLUTION_720_480:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 720);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 480);
		break;
	case RESOLUTION_800_480:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 800);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 480);
		break;
	case RESOLUTION_800_600:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 800);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 600);
		break;
	case RESOLUTION_960_720:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 960);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 720);
		break;
	case RESOLUTION_1024_768:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1024);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 768);
		break;
	case RESOLUTION_1280_720:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1280);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 720);
		break;
	case RESOLUTION_1280_800:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1280);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 800);
		break;
	case RESOLUTION_1280_960:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1280);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 960);
		break;
	case RESOLUTION_1280_1024:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1280);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 1024);
		break;
	case RESOLUTION_1600_1200:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1600);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 1200);
		break;
	case RESOLUTION_1920_1080:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 1920);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 1080);
		break;
	case RESOLUTION_2048_1536:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 2048);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 1536);
		break;
	default:
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_WIDTH, 480);
		m_WebcamAddOn.setCamProperty(cv::CAP_PROP_FRAME_HEIGHT, 360);
	}

	setResolutionText();
}

void WebcamImageDataPipe::onChangeColor(int p_Idx)
{
	m_Settings->setValue("webcam/color", p_Idx);
}

void WebcamImageDataPipe::setResolutionText()
{
	int32_t resWidth = (uint32_t)m_WebcamAddOn.getCamProperty(cv::CAP_PROP_FRAME_WIDTH);
	int32_t resHeight = (uint32_t)m_WebcamAddOn.getCamProperty(cv::CAP_PROP_FRAME_HEIGHT);

	QString resText = QString("CAM Resolution: %1 x %2").arg(QString::number(resWidth), QString::number(resHeight));
	this->ui.labelResolution->setText(resText);
}

void WebcamImageDataPipe::onStartStopCap()
{
	m_ImagePipe = !m_ImagePipe;

	if (m_ImagePipe)
	{
		this->ui.pushStartStopCap->setText("Stop Capure");
	}
	else
	{
		this->ui.pushStartStopCap->setText("Start Capure");
	}
}
