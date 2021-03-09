#include "PcHeader.h"
#include "WebcamImageDataPipe.h"
#include <QtWidgets/QApplication>

#include "version.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	WebcamImageDataPipe w;
	w.setWindowTitle("Cam Image Pipe V" VERSION_STRING);
	w.show();
	return app.exec();
}
