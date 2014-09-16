#ifndef BUNDLERINWIN_H
#define BUNDLERINWIN_H

#include <QtWidgets/QWidget>
#include "ui_bundlerinwin.h"

#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QSlider>
#include <QSpinBox>

namespace Ui
{
	class bundlerInWin;
};

class bundlerInWin : public QWidget
{
	Q_OBJECT

public:
	bundlerInWin(QWidget *parent = 0);
	~bundlerInWin();

	void initwidgets();
	void initconnects();

public slots :
	bool choosedir();

	void GenListFile();
	void changeImage();
	void runbundler();

	void setLcdvalue(int);
	void setSlidervalue(int);
private:
	Ui::bundlerInWin *ui;

	QLineEdit *dirlineedit;
	QPushButton *dirbtn;
	QString dirname;

	QPushButton *genlistbtn, *changeimgbtn, *runbtn;
	int focal_length;
	QSpinBox *paramedit;
	QSlider *slider;
};

#endif // BUNDLERINWIN_H
