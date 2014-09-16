#include "bundlerinwin.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

string StringRepalceSub(string &str, const string oldSub, const string newSub)
{
	int oldSubLen = oldSub.size();//替换的长度
	unsigned int  p = string::npos;
	p = str.find(oldSub, 0);//查找替换的位置
	if (p == string::npos)	{
		return str;
	}
	else	{
		string restr = str;
		return restr.replace(p, oldSubLen, newSub);
	}

}


bundlerInWin::bundlerInWin(QWidget *parent)
	:ui(new Ui::bundlerInWin), QWidget(parent)
{
	ui->setupUi(this);

	initwidgets();
	initconnects();

	focal_length = 5;
	
	QHBoxLayout *dirlayout = new QHBoxLayout();
	dirlayout->addWidget(dirlineedit);
	dirlayout->addWidget(dirbtn);
	QHBoxLayout *paramlayout = new QHBoxLayout();
	paramlayout->addWidget(paramedit);
	paramlayout->addWidget(slider);
	QVBoxLayout *btnlayout = new QVBoxLayout();
	btnlayout->addWidget(genlistbtn);
	btnlayout->addWidget(changeimgbtn);
	btnlayout->addWidget(runbtn);

	QVBoxLayout *mainlayout = new QVBoxLayout();
	mainlayout->addLayout(dirlayout);
	mainlayout->addLayout(paramlayout);
	mainlayout->addLayout(btnlayout);
	setLayout(mainlayout);
}

bundlerInWin::~bundlerInWin()
{
	delete ui;
}

void bundlerInWin::initwidgets()
{
	dirbtn = new QPushButton("choose dir");
	dirlineedit = new QLineEdit();

	genlistbtn = new QPushButton("genlistfile");
	changeimgbtn = new QPushButton("changeImage");
	runbtn = new QPushButton("run");

	slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum(10000);
	slider->setValue(0);

	paramedit = new QSpinBox();
	paramedit->setRange(0, 10000);
	paramedit->setValue(5);
}

void bundlerInWin::initconnects()
{
	connect(dirbtn, SIGNAL(clicked()), this, SLOT(choosedir()));

	connect(genlistbtn, SIGNAL(clicked()), this, SLOT(GenListFile()));
	connect(changeimgbtn, SIGNAL(clicked()), this, SLOT(changeImage()));
	connect(runbtn, SIGNAL(clicked()), this, SLOT(runbundler()));

	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLcdvalue(int)));
	connect(paramedit, SIGNAL(valueChanged(int)), this, SLOT(setSlidervalue(int)));
}

bool bundlerInWin::choosedir()
{
	dirname = QFileDialog::getExistingDirectory(this);
	if (dirname.isEmpty()) {
		QMessageBox::warning(NULL, "warning:", "No such directory!");
		return false;
	}
	dirlineedit->setText(dirname);
	return true;
}

void bundlerInWin::GenListFile()
{
	QDir dir(dirname);
	dir.setFilter(QDir::Files);
	QFileInfoList list = dir.entryInfoList();

	ofstream listfile("result/list.txt");

	int i = 0;
	do {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
			i++;
			continue;
		}
		listfile << fileInfo.absoluteFilePath().toStdString() << " 0 " << focal_length << endl;
		i++;
	} while (i < list.size());

	listfile.close();
}

void bundlerInWin::changeImage()
{
	ifstream listfile("result/list.txt");
	ofstream keyfile("result/list_keys.txt");
	string str, new_imagename, keyname, comm;
	int len = 0, i = 0;
	QDir dir;
	string tmpstr;
	vector<string> str_list;
	while (getline(listfile, tmpstr)) {
		int pos = tmpstr.find_first_of(" ");
		str = tmpstr.substr(0, pos);
		i++;
		Mat image = imread(str);
		Mat img_gray;
		cvtColor(image, img_gray, CV_RGB2GRAY);
		new_imagename = StringRepalceSub(str, "testImage", "pgmImage");
		keyname = StringRepalceSub(str, "jpg", "key");
		new_imagename = StringRepalceSub(new_imagename, "jpg", "pgm");
//		keyname = StringRepalceSub(str, "png", "key");
//		new_imagename = StringRepalceSub(new_imagename, "png", "pgm");	

		imwrite(new_imagename, img_gray);

		keyfile << keyname << endl;
		comm = dir.currentPath().toStdString() + "/bin/siftWin32.exe <" + new_imagename + " >" + keyname;
		system(comm.c_str());
	}

	listfile.close();
	keyfile.close();
}


void bundlerInWin::runbundler()
{
	system("cls");
	QDir dir;
	string comm;
	comm = dir.currentPath().toStdString() + "/bin/KeyMatchFull.exe result/list_keys.txt result/matches.init.txt";
	system(comm.c_str());
	comm = dir.currentPath().toStdString() + "/bin/bundler.exe result/list.txt --options_file result/options.txt";
	system(comm.c_str());
}

void bundlerInWin::setLcdvalue(int value)
{
	focal_length = slider->value();
	paramedit->setValue(focal_length);
}

void bundlerInWin::setSlidervalue(int value)
{
	focal_length = paramedit->value();
	slider->setValue(focal_length);
}