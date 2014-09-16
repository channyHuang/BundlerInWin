#include "bundlerinwin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	bundlerInWin w;
	w.show();
	return a.exec();
}
