#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QWidget>
#include <QtGui>
#include <fstream>
#include <iostream>


class startSim : public QWidget
{
  Q_OBJECT

 public:
  QGroupBox *loadBox;
  QGroupBox *scaleBox;
  QGroupBox *borderBox;
  QGroupBox *sizeOfSunBox;
  QGroupBox *stationarySunBox;
  QGroupBox *numberOfObjectsBox;
  QGroupBox *avgSizeOfObjectsBox;
  QGroupBox *objectPreviewSizeBox;
  QGroupBox *initialSpinBox;
  QGroupBox *distanceFromSunBox;
  QGroupBox *inertiaBox;
  QGroupBox *collisionBox;
  QGroupBox *fragmentBox;
  QGroupBox *traceBox;
  QLineEdit * text_saveName;
  QPushButton *button_save;
  QPushButton *button_simulate;

  private slots:
    void changeConfig()
    {
      QString loadConfig = loadBox->findChild<QComboBox *>()->currentText();
      std::string configFileName = loadConfig.toUtf8().constData();
      configFileName = "configs/" + configFileName;

      std::ifstream configFile(configFileName.c_str());
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

      scaleBox->findChild<QSpinBox *>()->setValue(scale);
      if (border) borderBox->findChild<QCheckBox *>()->setCheckState(Qt::Checked);
      else borderBox->findChild<QCheckBox *>()->setCheckState(Qt::Unchecked);
      sizeOfSunBox->findChild<QDoubleSpinBox *>()->setValue(sizeOfSun);
      if (stationarySun) stationarySunBox->findChild<QCheckBox *>()->setCheckState(Qt::Checked);
      else stationarySunBox->findChild<QCheckBox *>()->setCheckState(Qt::Unchecked);
      numberOfObjectsBox->findChild<QSpinBox *>()->setValue(numberOfObjects);
      avgSizeOfObjectsBox->findChild<QSpinBox *>()->setValue(avgSizeOfObjects);
      objectPreviewSizeBox->findChild<QSpinBox *>()->setValue(objectPreviewSize);
      initialSpinBox->findChild<QSpinBox *>()->setValue(initialSpin);
      distanceFromSunBox->findChild<QSpinBox *>()->setValue(distanceFromSun);
      if (inertia) inertiaBox->findChild<QCheckBox *>()->setCheckState(Qt::Checked);
      else inertiaBox->findChild<QCheckBox *>()->setCheckState(Qt::Unchecked);
      if (collision) collisionBox->findChild<QCheckBox *>()->setCheckState(Qt::Checked);
      else collisionBox->findChild<QCheckBox *>()->setCheckState(Qt::Unchecked);
      if (fragment) fragmentBox->findChild<QCheckBox *>()->setCheckState(Qt::Checked);
      else fragmentBox->findChild<QCheckBox *>()->setCheckState(Qt::Unchecked);
      traceBox->findChild<QSpinBox *>()->setValue(trace);
    }

    void save()
    {
      int scale = scaleBox->findChild<QSpinBox *>()->value();
      bool borderB = borderBox->findChild<QCheckBox *>()->isChecked();
      int border;
      if (borderB) border = 1;
      else border = 0;
      double sizeOfSun = sizeOfSunBox->findChild<QDoubleSpinBox *>()->value();
      bool stationarySunB = stationarySunBox->findChild<QCheckBox *>()->isChecked();
      int stationarySun;
      if (stationarySunB) stationarySun = 1;
      else stationarySun = 0;
      int numberOfObjects = numberOfObjectsBox->findChild<QSpinBox *>()->value();
      int avgSizeOfObjects = avgSizeOfObjectsBox->findChild<QSpinBox *>()->value();
      int objectPreviewSize = objectPreviewSizeBox->findChild<QSpinBox *>()->value();
      int initialSpin = initialSpinBox->findChild<QSpinBox *>()->value();
      int distanceFromSun = distanceFromSunBox->findChild<QSpinBox *>()->value();
      bool inertiaB = inertiaBox->findChild<QCheckBox *>()->isChecked();
      int inertia;
      if (inertiaB) inertia = 1;
      else inertia = 0;
      bool collisionB = collisionBox->findChild<QCheckBox *>()->isChecked();
      int collision;
      if (collisionB) collision = 1;
      else collision = 0;
      bool fragmentB = fragmentBox->findChild<QCheckBox *>()->isChecked();
      int fragment;
      if (fragmentB) fragment = 1;
      else fragment = 0;
   	  int trace = traceBox->findChild<QSpinBox *>()->value();


      QString loadConfig = text_saveName->text();
      std::string configFileName1 = loadConfig.toUtf8().constData();
      std::string configFileName = "configs/" + configFileName1;

      std::ofstream configFile;
      configFile.open(configFileName.c_str());

      configFile << "scale :                     " << scale << std::endl;
      configFile << "border? (1/0) :             " << border << std::endl;
      configFile << "size of sun :               " << sizeOfSun << std::endl;
      configFile << "stationary sun? (1/0) :     " << stationarySun << std::endl;
      configFile << "number of objects :         " << numberOfObjects << std::endl;
      configFile << "avarage size of objects :   " << avgSizeOfObjects << std::endl;
      configFile << "object preview size :       " << objectPreviewSize << std::endl;
      configFile << "initial spin :              " << initialSpin << std::endl;
      configFile << "distance from sun :         " << distanceFromSun << std::endl;
      configFile << "inertia? (1/0) :            " << inertia << std::endl;
      configFile << "collision? (1/0) :          " << collision << std::endl;
      configFile << "fragment? (1/0) :           " << fragment << std::endl;
      configFile << "trace :                     " << trace;

      configFile.close();

      QString s = QString::fromStdString(configFileName1);
      loadBox->findChild<QComboBox *>()->addItem(s);
      loadBox->findChild<QComboBox *>()->setCurrentIndex(loadBox->findChild<QComboBox *>()->findText(s));
    }

    void start()
    {
      int scale = scaleBox->findChild<QSpinBox *>()->value();
      bool borderB = borderBox->findChild<QCheckBox *>()->isChecked();
      int border;
      if (borderB) border = 1;
      else border = 0;
      double sizeOfSun = sizeOfSunBox->findChild<QDoubleSpinBox *>()->value();
      bool stationarySunB = stationarySunBox->findChild<QCheckBox *>()->isChecked();
      int stationarySun;
      if (stationarySunB) stationarySun = 1;
      else stationarySun = 0;
      int numberOfObjects = numberOfObjectsBox->findChild<QSpinBox *>()->value();
      int avgSizeOfObjects = avgSizeOfObjectsBox->findChild<QSpinBox *>()->value();
      int objectPreviewSize = objectPreviewSizeBox->findChild<QSpinBox *>()->value();
      int initialSpin = initialSpinBox->findChild<QSpinBox *>()->value();
      int distanceFromSun = distanceFromSunBox->findChild<QSpinBox *>()->value();
      bool inertiaB = inertiaBox->findChild<QCheckBox *>()->isChecked();
      int inertia;
      if (inertiaB) inertia = 1;
      else inertia = 0;
      bool collisionB = collisionBox->findChild<QCheckBox *>()->isChecked();
      int collision;
      if (collisionB) collision = 1;
      else collision = 0;
      bool fragmentB = fragmentBox->findChild<QCheckBox *>()->isChecked();
      int fragment;
      if (fragmentB) fragment = 1;
      else fragment = 0;
      int trace = traceBox->findChild<QSpinBox *>()->value();

      std::ofstream configFile;
      configFile.open("configs/config");

      configFile << "scale :                     " << scale << std::endl;
      configFile << "border? (1/0) :             " << border << std::endl;
      configFile << "size of sun :               " << sizeOfSun << std::endl;
      configFile << "stationary sun? (1/0) :     " << stationarySun << std::endl;
      configFile << "number of objects :         " << numberOfObjects << std::endl;
      configFile << "avarage size of objects :   " << avgSizeOfObjects << std::endl;
      configFile << "object preview size :       " << objectPreviewSize << std::endl;
      configFile << "initial spin :              " << initialSpin << std::endl;
      configFile << "distance from sun :         " << distanceFromSun << std::endl;
      configFile << "inertia? (1/0) :            " << inertia << std::endl;
      configFile << "collision? (1/0) :          " << collision << std::endl;
      configFile << "fragment? (1/0) :           " << fragment;
      configFile << "trace :           " << trace;

      configFile.close();

      std::system("./sol &");
    }
};
#endif
