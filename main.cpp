#include "xviewer.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QDir>
#include "xcamera_config.h"
#define TEST_CAM_PATH "test.db"
#include "xcamera_record.h"
int main(int argc, char* argv[])
{
	const char* save_path = "./video/0/";
	QDir dir;
	dir.mkpath(save_path);

	QApplication a(argc, argv);
	XViewer w;
	w.show();
	auto re = a.exec();
	return re;
}
