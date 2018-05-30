/*
qmake -project && qmake qt.pro && make
*/

#include <QtGui>
#include <dirent.h>
#include <cstdlib>
#include <fstream>

#include "simulation.h"


QString makeGroupLoad(QGroupBox *box, QString l)
{
  QLabel *label = new QLabel(l);
  QComboBox *loadbox = new QComboBox();
  DIR *dpdf;
  struct dirent *epdf;
  dpdf = opendir("configs");
  if (dpdf != NULL)
    {
      while ((epdf = readdir(dpdf)))
	{
	  QString s = epdf->d_name;
	  if (s != ".." && s != ".")
	    {
	      if (s == "config") loadbox->insertItem(0, s);
	      else loadbox->addItem(s);
	    }
	}
    }
  loadbox->setCurrentIndex(0);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(label);
  layout->addWidget(loadbox);
  layout->setSpacing(0);
  layout->setMargin(0);
  box->setLayout(layout);
  return loadbox->currentText();
}

void makeGroupSpin(QGroupBox *box, QString l, int min, int max, int val)
{
  QLabel *label = new QLabel(l);
  QSpinBox *spinBox = new QSpinBox();
  spinBox->setRange(min, max);
  spinBox->setValue(val);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(label);
  layout->addWidget(spinBox);
  layout->setSpacing(0);
  layout->setMargin(0);
  box->setLayout(layout);
}

void makeGroupSpinDouble(QGroupBox *box, QString l, double min, double max, double val)
{
  QLabel *label = new QLabel(l);
  QDoubleSpinBox *spinBox = new QDoubleSpinBox();
  spinBox->setRange(min, max);
  spinBox->setValue(val);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(label);
  layout->addWidget(spinBox);
  layout->setSpacing(0);
  layout->setMargin(0);
  box->setLayout(layout);
}

void makeGroupCheck(QGroupBox *box, QString l, int checked)
{
  QLabel *label = new QLabel(l);
  QCheckBox *checkbox = new QCheckBox();
  if (checked) checkbox->setCheckState(Qt::Checked);
  else checkbox->setCheckState(Qt::Unchecked);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(label);
  layout->addWidget(checkbox);
  layout->setSpacing(0);
  layout->setMargin(0);
  box->setLayout(layout);
}


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  startSim sim;

  sim.loadBox = new QGroupBox;
  QString configFileName1 = makeGroupLoad(sim.loadBox, "load config");
  std::string configFileName = configFileName1.toUtf8().constData();
  configFileName = "configs/" + configFileName;

  std::ifstream configFile;
  configFile.open(configFileName.c_str());

  int scale, border, stationarySun, numberOfObjects, avgSizeOfObjects, objectPreviewSize, initialSpin, distanceFromSun, inertia, collision, fragment, trace;
  double sizeOfSun;
  std::string input;
  do{ configFile >> input; }while(input != ":");
  configFile >> scale;
  do{ configFile >> input; }while(input != ":");
  configFile >> border;
  do{ configFile >> input; }while(input != ":");
  configFile >> sizeOfSun;
  do{ configFile >> input; }while(input != ":");
  configFile >> stationarySun;
  do{ configFile >> input; }while(input != ":");
  configFile >> numberOfObjects;
  do{ configFile >> input; }while(input != ":");
  configFile >> avgSizeOfObjects;
  do{ configFile >> input; }while(input != ":");
  configFile >> objectPreviewSize;
  do{ configFile >> input; }while(input != ":");
  configFile >> initialSpin;
  do{ configFile >> input; }while(input != ":");
  configFile >> distanceFromSun;
  do{ configFile >> input; }while(input != ":");
  configFile >> inertia;
  do{ configFile >> input; }while(input != ":");
  configFile >> collision;
  do{ configFile >> input; }while(input != ":");
  configFile >> fragment;
  do{ configFile >> input; }while(input != ":");
  configFile >> trace;

  sim.scaleBox = new QGroupBox;
  makeGroupSpin(sim.scaleBox, "scale", 1, 30, scale);
  sim.borderBox = new QGroupBox;
  makeGroupCheck(sim.borderBox, "border", border);
  sim.sizeOfSunBox = new QGroupBox;
  makeGroupSpinDouble(sim.sizeOfSunBox, "size of sun", 0.01, 30, sizeOfSun);
  sim.stationarySunBox = new QGroupBox;
  makeGroupCheck(sim.stationarySunBox, "stationary sun", stationarySun);
  sim.numberOfObjectsBox = new QGroupBox;
  makeGroupSpin(sim.numberOfObjectsBox, "number of objects", 1, 50000, numberOfObjects);
  sim.avgSizeOfObjectsBox = new QGroupBox;
  makeGroupSpin(sim.avgSizeOfObjectsBox, "avarage size of objects", 1, 30, avgSizeOfObjects);
  sim.objectPreviewSizeBox = new QGroupBox;
  makeGroupSpin(sim.objectPreviewSizeBox, "object preview size", 1, 30, objectPreviewSize);
  sim.initialSpinBox = new QGroupBox;
  makeGroupSpin(sim.initialSpinBox, "initial spin", 0, 30, initialSpin);
  sim.distanceFromSunBox = new QGroupBox;
  makeGroupSpin(sim.distanceFromSunBox, "distance from sun", 1, 30, distanceFromSun);
  sim.inertiaBox = new QGroupBox;
  makeGroupCheck(sim.inertiaBox, "inertia", inertia);
  sim.collisionBox = new QGroupBox;
  makeGroupCheck(sim.collisionBox, "collision", collision);
  sim.fragmentBox = new QGroupBox;
  makeGroupCheck(sim.fragmentBox, "fragment", fragment);
  sim.traceBox = new QGroupBox;
  makeGroupSpin(sim.traceBox, "trace", 0, 30, trace);

  sim.text_saveName = new QLineEdit();
  sim.button_save = new QPushButton("Save");
  sim.button_simulate = new QPushButton("Simulate");


  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(sim.loadBox);
  QObject::connect(sim.loadBox->findChild<QComboBox *>(), SIGNAL(currentIndexChanged(int)), &sim, SLOT(changeConfig()));

  mainLayout->addWidget(sim.scaleBox);
  mainLayout->addWidget(sim.borderBox);
  mainLayout->addWidget(sim.sizeOfSunBox);
  mainLayout->addWidget(sim.stationarySunBox);
  mainLayout->addWidget(sim.numberOfObjectsBox);
  mainLayout->addWidget(sim.avgSizeOfObjectsBox);
  mainLayout->addWidget(sim.objectPreviewSizeBox);
  mainLayout->addWidget(sim.initialSpinBox);
  mainLayout->addWidget(sim.distanceFromSunBox);
  mainLayout->addWidget(sim.inertiaBox);
  mainLayout->addWidget(sim.collisionBox);
  mainLayout->addWidget(sim.fragmentBox);
  mainLayout->addWidget(sim.traceBox);

  mainLayout->addSpacing(20);

  mainLayout->addWidget(sim.text_saveName);
  mainLayout->addWidget(sim.button_save);
  QObject::connect(sim.button_save, SIGNAL(clicked()), &sim, SLOT(save()));

  mainLayout->addWidget(sim.button_simulate);
  QObject::connect(sim.button_simulate, SIGNAL(clicked()), &sim, SLOT(start()));

  mainLayout->setSpacing(0);
  mainLayout->setMargin(0);


  QWidget window;
  window.setWindowTitle("Simulation config");
  window.resize(360, 420);
  window.setLayout(mainLayout);
  window.show();
  return app.exec();
}
