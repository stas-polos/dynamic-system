#include "dynamic_system.h"
#include "src/widget/gwidget.h"
#include "src/matrix/matrix.h"
#include <QtGui>
#include <QDialog>
#include <QDateTime>
#include <QPixmap>
#include <QGLWidget>
#include <math.h>
#include <stdio.h>

DynamicSystem::DynamicSystem(QWidget * parent): QDialog(parent) {
  systemType = true;
  tableRowCount = 0;
  isChangable = true;
  isSave = false;
  isUsedFeedback = false;
  isProgControl = false;
  isFirstStart = true;
  isFirstStartSave = true;
  canSave = false;
  stepMultipler = 1;
  stepIndex = 1;
  systemSize = 3;
  tempM.setSize(1, (2 * systemSize) + 1);
  tempM_old.setSize(1, (2 * systemSize) + 1);
  graphArea = new GLWidget;
  graphArea -> setMaximumSize(278, 147);
  graphArea -> setMinimumSize(278, 147);
  // Size of graphics
  graphArea -> systemSize = 3;
  graphArea -> stepIndex = stepIndex;
  tableArea = new QTableWidget(this);
  tableArea -> verticalHeader() -> hide();
  QFont headerF("Tahoma", 7);
  tableRowCount = 0;
  // UI
  QVBoxLayout * mainInfo = new QVBoxLayout(this);
  QHBoxLayout * topSize = new QHBoxLayout;
  QGridLayout * topSystem = new QGridLayout;
  QGridLayout * topInfoM = new QGridLayout;
  QGridLayout * topInfoC = new QGridLayout;
  QGridLayout * bottomInfoS = new QGridLayout;
  QHBoxLayout * bottomCalc = new QHBoxLayout;
  QHBoxLayout * bottomInfoL = new QHBoxLayout;
  QHBoxLayout * bottomInfoA = new QHBoxLayout;
  QHBoxLayout * bottomInfoAi = new QHBoxLayout;
  QHBoxLayout * bottomInfoB = new QHBoxLayout;
  QHBoxLayout * bottomInfoBi = new QHBoxLayout;
  QVBoxLayout * bottomAll = new QVBoxLayout;
  QGridLayout * graphAll = new QGridLayout;
  QGroupBox * matrixInfo = new QGroupBox("Matrices", this);
  QGroupBox * graphInfo = new QGroupBox("Graphic", this);
  QGroupBox * changeInfo = new QGroupBox("Options", this);
  QGroupBox * radioSystem = new QGroupBox("System type", this);
  QLabel * labelA = new QLabel("Matrix A:", this);
  QLabel * labelB = new QLabel("Matrix B:", this);
  QLabel * labelC0 = new QLabel("Vector C0:", this);
  QLabel * labelX0 = new QLabel("Vector X0:", this);
  QLabel * labelStep = new QLabel("Step:", this);
  QLabel * labelCalc = new QLabel("Calculate values ​​at point at t =", this);
  radioDiscrete = new QRadioButton("Discrete system");
  radioContinuity = new QRadioButton("Continuous system");
  radioDiscrete -> setChecked(true);

  matrixA = new QTableWidget(systemSize, systemSize, this);
  matrixB = new QTableWidget(systemSize, systemSize, this);
  matrixC0 = new QTableWidget(systemSize, 1, this);
  matrixX0 = new QTableWidget(systemSize, 1, this);

  matrixA -> setMinimumSize(122, 122);
  matrixB -> setMinimumSize(122, 122);
  matrixC0 -> setMinimumSize(42, 122);
  matrixX0 -> setMinimumSize(42, 122);
  matrixA -> setMaximumSize(122, 122);
  matrixB -> setMaximumSize(122, 122);
  matrixC0 -> setMaximumSize(42, 122);
  matrixX0 -> setMaximumSize(42, 122);

  for (int i = 0; i < systemSize; i++) {
    matrixA -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
    matrixA -> verticalHeader() -> resizeSection(i, 120 / systemSize);

    matrixB -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
    matrixB -> verticalHeader() -> resizeSection(i, 120 / systemSize);

    matrixC0 -> horizontalHeader() -> resizeSection(i, 40);
    matrixC0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);

    matrixX0 -> horizontalHeader() -> resizeSection(i, 40);
    matrixX0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
  }

  // initialization of program control
  progDialogInit();
  resizeSystem(true);

  matrixA -> verticalHeader() -> hide();
  matrixA -> horizontalHeader() -> hide();

  matrixB -> verticalHeader() -> hide();
  matrixB -> horizontalHeader() -> hide();

  matrixC0 -> verticalHeader() -> hide();
  matrixC0 -> horizontalHeader() -> hide();

  matrixX0 -> verticalHeader() -> hide();
  matrixX0 -> horizontalHeader() -> hide();

  checkChangeA = new QCheckBox("Matrix A cannot be changed", this);
  checkPrevIter = new QCheckBox("Save previous iteration", this);
  buttonProgControl = new QPushButton("Program control", this);

  stepList = new QComboBox;
  stepList -> setEnabled(false);
  stepList -> addItem("0.1");
  stepList -> addItem("0.01");
  stepList -> addItem("0.001");

  labelStep -> setAlignment(Qt::AlignRight);
  topSystem -> addWidget(radioDiscrete, 0, 0);
  topSystem -> addWidget(radioContinuity, 0, 1);
  topSystem -> addWidget(labelStep, 0, 2);
  topSystem -> addWidget(stepList, 0, 3);
  radioSystem -> setLayout(topSystem);

  topInfoM -> addWidget(labelA, 0, 0);
  topInfoM -> addWidget(labelB, 0, 1);
  topInfoM -> addWidget(labelC0, 0, 2);
  topInfoM -> addWidget(labelX0, 0, 3);
  topInfoM -> addWidget(matrixA, 1, 0);
  topInfoM -> addWidget(matrixB, 1, 1);
  topInfoM -> addWidget(matrixC0, 1, 2);
  topInfoM -> addWidget(matrixX0, 1, 3);

  topInfoC -> addWidget(checkChangeA, 0, 0);
  topInfoC -> setHorizontalSpacing(250);
  topInfoC -> addWidget(buttonProgControl, 0, 1);
  bottomInfoS -> addWidget(checkPrevIter, 0, 0);
  matrixInfo -> setLayout(topInfoM);

  buttonCalc = new QPushButton("Calculate", this);
  editCalc = new QLineEdit(this);
  editCalc -> setText("1");
  editCalc -> setAlignment(Qt::AlignCenter);
  editCalc -> setFixedWidth(50);
  editCalc -> setMaxLength(10);

  bottomCalc -> addSpacing(400);
  bottomCalc -> addWidget(labelCalc);
  bottomCalc -> addWidget(editCalc);
  bottomCalc -> addWidget(buttonCalc);

  tableArea -> setColumnCount((2 * systemSize) + 1);
  tableArea -> setRowCount(tableRowCount);

  for (int tableInc = 0; tableInc < ((2 * systemSize) + 1); tableInc++) {
    tableArea -> horizontalHeader() -> resizeSection(tableInc, 36);
    tableArea -> horizontalHeader() -> setFont(headerF);
  }

  QStringList tableLabels;
  tableLabels += "t";
  tableLabels += "X0";
  tableLabels += "X1";
  tableLabels += "X2";
  tableLabels += "X*0";
  tableLabels += "X*1";
  tableLabels += "X*2";
  tableArea -> setHorizontalHeaderLabels(tableLabels);
  editX0 = new QLineEdit(this);
  editX1 = new QLineEdit(this);
  editY0 = new QLineEdit(this);
  editY1 = new QLineEdit(this);

  graphAll -> setHorizontalSpacing(5);
  graphAll -> setVerticalSpacing(5);

  graphAll -> addWidget(graphArea, 0, 0, 5, 20);
  graphAll -> addWidget(editX0, 5, 0, 1, 1);
  graphAll -> addWidget(editX1, 5, 19, 1, 1);
  graphAll -> addWidget(editY1, 0, 20, 1, 1);
  graphAll -> addWidget(editY0, 4, 20, 1, 1);
  graphAll -> addWidget(tableArea, 0, 21, 6, 2);

  editX0 -> setMaximumSize(25, 20);
  editX1 -> setMaximumSize(25, 20);
  editY0 -> setMaximumSize(25, 20);
  editY1 -> setMaximumSize(25, 20);

  graphArea -> setMaximumWidth(280);
  tableArea -> setMaximumWidth(200);

  editX0 -> setMaxLength(4);
  editX0 -> setAlignment(Qt::AlignCenter);
  editX1 -> setMaxLength(4);
  editX1 -> setAlignment(Qt::AlignCenter);
  editY0 -> setMaxLength(4);
  editY0 -> setAlignment(Qt::AlignCenter);
  editY1 -> setMaxLength(4);
  editY1 -> setAlignment(Qt::AlignCenter);

  editX0 -> setText("0");
  editX1 -> setText("5");
  editY0 -> setText("0");
  editY1 -> setText("5");

  graphInfo -> setLayout(graphAll);
  resize = new QPushButton("Change", this);

  QLabel * labelSize = new QLabel("Number of subsystems:", this);
  editSize = new QLineEdit(this);
  editSize -> setText("3");
  editSize -> setAlignment(Qt::AlignCenter);
  editSize -> setFixedWidth(20);
  editSize -> setMaxLength(2);

  topSize -> addWidget(labelSize);
  topSize -> addWidget(editSize);
  topSize -> addWidget(resize);
  topSize -> addSpacing(400);

  QLabel * labelChangeA = new QLabel("Control using matrix A:", this);
  decA = new QPushButton("Decrease", this);
  incA = new QPushButton("Increase", this);
  QLabel * labelStepA = new QLabel("Values in increments:", this);
  editA = new QLineEdit(this);

  QLabel * labelChangeB = new QLabel("Control using matrix B:", this);
  decB = new QPushButton("Decrease", this);
  incB = new QPushButton("Increase", this);
  QLabel * labelStepB = new QLabel("Values in increments:", this);
  editB = new QLineEdit(this);

  launch = new QPushButton("Execute", this);
  save = new QPushButton("Save", this);

  editA -> setText("0.01");
  editA -> setMaxLength(6);
  editA -> setAlignment(Qt::AlignCenter);
  editA -> setFixedWidth(80);

  editB -> setText("0.01");
  editB -> setMaxLength(6);
  editB -> setAlignment(Qt::AlignCenter);
  editB -> setFixedWidth(80);

  bottomInfoA -> addWidget(labelChangeA);
  bottomInfoAi -> addWidget(decA);
  bottomInfoAi -> addWidget(incA);
  bottomInfoAi -> addWidget(labelStepA);
  bottomInfoAi -> addWidget(editA);

  bottomInfoB -> addWidget(labelChangeB);
  bottomInfoBi -> addWidget(decB);
  bottomInfoBi -> addWidget(incB);
  bottomInfoBi -> addWidget(labelStepB);
  bottomInfoBi -> addWidget(editB);

  bottomAll -> addLayout(bottomInfoA);
  bottomAll -> addLayout(bottomInfoAi);
  bottomAll -> addLayout(bottomInfoB);
  bottomAll -> addLayout(bottomInfoBi);
  changeInfo -> setLayout(bottomAll);

  bottomInfoL -> addSpacing(200);
  bottomInfoL -> addWidget(launch);
  bottomInfoL -> addWidget(save);
  bottomInfoL -> addSpacing(200);

  mainInfo -> addLayout(topSize);
  mainInfo -> addWidget(radioSystem);
  mainInfo -> addWidget(matrixInfo);
  mainInfo -> addLayout(topInfoC);
  mainInfo -> addWidget(graphInfo);
  mainInfo -> addWidget(changeInfo);
  mainInfo -> addLayout(bottomInfoS);
  mainInfo -> addLayout(bottomCalc);
  mainInfo -> addLayout(bottomInfoL);
  setLayout(mainInfo);

  // signals
  connect(resize, SIGNAL(clicked()), SLOT(resizeData()));
  connect(radioDiscrete, SIGNAL(clicked()), SLOT(toggleStepOff()));
  connect(radioContinuity, SIGNAL(clicked()), SLOT(toggleStepOn()));
  connect(checkChangeA, SIGNAL(clicked()), SLOT(toggleEditA()));
  connect(buttonProgControl, SIGNAL(clicked()), SLOT(progControlOpen()));
  connect(editX0, SIGNAL(editingFinished()), SLOT(resizeX()));
  connect(editX1, SIGNAL(editingFinished()), SLOT(resizeX()));
  connect(editY0, SIGNAL(editingFinished()), SLOT(resizeY()));
  connect(editY1, SIGNAL(editingFinished()), SLOT(resizeY()));
  connect(incA, SIGNAL(clicked()), SLOT(incMatrixA()));
  connect(incB, SIGNAL(clicked()), SLOT(incMatrixB()));
  connect(decA, SIGNAL(clicked()), SLOT(decMatrixA()));
  connect(decB, SIGNAL(clicked()), SLOT(decMatrixB()));
  connect(checkPrevIter, SIGNAL(clicked()), SLOT(toggleSave()));
  connect(buttonCalc, SIGNAL(clicked()), SLOT(calculateInPoint()));
  connect(launch, SIGNAL(clicked()), SLOT(takeNewData()));
  connect(save, SIGNAL(clicked()), SLOT(saveData()));
  connect(progDialogProceed, SIGNAL(clicked()), SLOT(progProceed()));
  connect(progDialogCancel, SIGNAL(clicked()), SLOT(progCancel()));
  connect(progDialogSelect, SIGNAL(currentIndexChanged(int)), SLOT(progSelect(int)));

}

DynamicSystem::~DynamicSystem() {}

int DynamicSystem::pharseMatrix(int n, int m, QTableWidget * matrix) {
  QString sNum;
  for (int exI = 0; exI < n; exI++) {
    for (int exJ = 0; exJ < m; exJ++) {
      if (matrix -> item(exI, exJ) == NULL) {
        QMessageBox::warning(this, "Error", "Not all matrix values have been entered. Fill in all cells.", "Continue");
        return 1;
      }

      sNum = matrix -> item(exI, exJ) -> text();
      for (int strI = 0; strI < sNum.length(); strI++) {
        bool validNumber = sNum[strI] <= QChar('9') && sNum[strI] >= QChar('0');
        bool validChar = sNum[strI] <= QChar('.') && sNum[strI] >= QChar(',');
        if (!validNumber && !validChar) {
          QMessageBox::critical(this, "Error", "Invalid characters were found in matrices.", "Continue");
          return 1;
        }
      }
    }
  }
  return 0;
}

void DynamicSystem::addNewLine(matrix tmp, matrix tmp_old, int t) {
  QString sTmp;
  QString sTmp_old;
  for (int i = 0; i < ((2 * systemSize) + 1); i++) {
    sTmp.setNum(tmp.getElement(0, i));
    if (!isFirstStartSave) {
      sTmp_old.setNum(tmp_old.getElement(0, i));
    }

    if (isFirstStartSave) {
      tableArea -> setItem(t, i, new QTableWidgetItem(sTmp));
    } else {
      if (i == 0) {
        tableArea -> setItem(t, i, new QTableWidgetItem(sTmp));
      } else {
        tableArea -> setItem(t, i, new QTableWidgetItem(sTmp + ", " + sTmp_old));
      }
    }
  }
}

// function of X*
void DynamicSystem::getX_st(float t) {
  matrix newM(systemSize, 1);
  matrix E;
  E.setSingle(systemSize);
  matrix tmp1(A);
  matrix tmp2(C0);

  tmp1 = E - tmp1;
  tmp1.getInverse();
  newM = tmp1 * tmp2;
  for (int i = 0; i < systemSize; i++) {
    Xst.setElement(i, 0, tmp1.getElement(i, 0));
  }
}

void DynamicSystem::getX_st_old(float t) {
  matrix newM(systemSize, 1);
  matrix E;
  E.setSingle(systemSize);
  matrix tmp1(A_old);
  matrix tmp2(C0_old);

  tmp1 = E - tmp1;
  tmp1.getInverse();
  newM = tmp1 * tmp2;
  for (int i = 0; i < systemSize; i++) {
    Xst_old.setElement(i, 0, tmp1.getElement(i, 0));
  }
}

void DynamicSystem::getX(float t) {
  if (systemType == true) {
    calculateDiscreteX(t);
    return;
  }
  calculateContinuosX(t);
}

void DynamicSystem::calculateContinuosX(float t) {
  bool isExpBug = false;
  matrix expBug(systemSize, systemSize);
  expBug.init(0);
  matrix expBugInc(systemSize, systemSize);
  expBugInc.init(0);
  matrix tmp1(systemSize, systemSize);
  matrix tmp3(systemSize, systemSize);
  tmp1.setSingle(systemSize);
  tmp3.setSingle(systemSize);
  matrix tmp2(A);
  tmp1 = tmp1 - B;
  tmp1.getInverse();
  tmp2 = tmp2 - tmp3;
  tmp1 = tmp1 * tmp2; // (I-B)^-1 * (A-I)
  matrix tmp4(tmp1);
  tmp4.getExp(t); // first summator
  matrix tmp5(X0);
  matrix integral(systemSize, 1); // systemSize, 1
  integral.init(0);
  matrix tmpI1(tmp1);
  matrix tmpI3(B);
  matrix tmpI2(systemSize, systemSize);
  tmpI2.setSingle(systemSize);
  tmpI2 = tmpI2 - tmpI3;
  tmpI2.getInverse(); // Bt

  float a = 0;
  float b = t;
  float n = 200;
  float h = (b - a) / n;
  for (int k = 1; k <= n; k++) {
    getC(a + h * (k - 0.5));
    tmpI1.initMatrix(tmp1, systemSize, systemSize);
    tmpI1 = tmpI1 * (-1);
    tmpI1.getExp(a + h * (k - 0.5));
    tmpI1 = tmpI1 * tmpI2;
    tmpI1 = tmpI1 * C0;
    for (int p = 0; p < systemSize; p++) {
      integral.setElement(p, 0, integral.getElement(p, 0) + tmpI1.getElement(p, 0));
    }
  }
  integral = integral * h;

  tmp5 = tmp5 + integral;
  tmp4 = tmp4 * tmp5;
  for (int j = 0; j < systemSize; j++) {
    X.setElement(j, 0, tmp4.getElement(j, 0));
  }
}

void DynamicSystem::calculateDiscreteX(float t) {
  matrix tmp1(systemSize, systemSize);
  tmp1.setSingle(systemSize);
  matrix tmp2(A);
  tmp1 = tmp1 - B;
  tmp1.getInverse();
  tmp2 = tmp2 - B;
  tmp1 = tmp1 * tmp2; // (I-B)^-1 * (A-B)
  matrix tmp3(tmp1);
  matrix tmp4(tmp1);
  tmp3.getPow(t);
  tmp3 = tmp3 * X0;
  matrix tmp5(systemSize, systemSize);
  tmp5.setSingle(systemSize);
  tmp5 = tmp5 - B;
  tmp5.getInverse();
  matrix sumTmp(systemSize, systemSize);
  sumTmp.init(0);
  matrix tmpSum(tmp4);
  for (int step = 1; step <= t; step++) {
    getC(step - 1);
    tmp4.initMatrix(tmpSum, systemSize, systemSize);
    tmp4.getPow(t - step);
    tmp4 = tmp4 * tmp5;
    tmp4 = tmp4 * C0;
    sumTmp = sumTmp + tmp4;
  }

  for (int i = 0; i < systemSize; i++) {
    X.setElement(i, 0, tmp3.getElement(i, 0) + sumTmp.getElement(i, 0));
  }
}

void DynamicSystem::getC(float t) {
  // discreate system
  if (systemType == true) {
    if (isProgControl == true) {
      matrix firstSum(systemSize, systemSize);
      firstSum.setSingle(systemSize);
      matrix secondSum(A);
      firstSum = firstSum - B;
      secondSum = secondSum - B;

      matrix fi(systemSize, 1); // fi(t)
      matrix fi1(systemSize, 1); // fi(t+1)
      matrix tmpLin1(Ct);
      matrix tmpLin2(Ct);
      matrix tmpStep(Ct);
      matrix tmpStep1(d0);
      matrix tmpStep2(Ct);
      matrix tmpStep3(d0);
      matrix tmpHarm(Ct);
      matrix tmpHarm1(d0);
      matrix tmpHarm2(Ct);
      matrix tmpHarm3(d0);

      switch (progDialogSelect -> currentIndex()) {
      case 0:
        fi = tmpLin1 * pow(t, powK);
        tmpLin2 = tmpLin2 + a0;
        fi1 = tmpLin2 * pow(t + 1, powK);
        break;
      case 1:
        tmpStep1 = tmpStep1 * pow(t, powK);
        tmpStep = tmpStep + tmpStep1;
        fi = tmpStep;
        tmpStep3 = tmpStep3 * pow(t + 1, powK);
        tmpStep2 = tmpStep2 + a0;
        tmpStep2 = tmpStep2 + tmpStep3;
        fi1 = tmpStep2;
        break;
      case 2:
        switch (progSelectFunc -> currentIndex()) {
        case 0:
          tmpHarm1 = tmpHarm1 * sin(omega * t);
          tmpHarm3 = tmpHarm3 * sin(omega * (t + 1));
          break;
        case 1:
          tmpHarm1 = tmpHarm1 * cos(omega * t);
          tmpHarm3 = tmpHarm3 * cos(omega * (t + 1));
          break;
        case 2:
          tmpHarm1 = tmpHarm1 * tan(omega * t);
          tmpHarm3 = tmpHarm3 * tan(omega * (t + 1));
          break;
        }
        tmpHarm = tmpHarm + tmpHarm1;
        fi = tmpHarm;
        tmpHarm2 = tmpHarm2 + a0;
        tmpHarm2 = tmpHarm2 + tmpHarm3;
        fi1 = tmpHarm2;
        break;
      }

      firstSum = firstSum * fi1;
      secondSum = secondSum * fi;

      firstSum = firstSum - secondSum;

      for (int i = 0; i < systemSize; i++) {
        C0.setElement(i, 0, firstSum.getElement(i, 0));
      }
    }
  }
  // continuos system
  else {
    if (isProgControl == true) {}
  }
}

void DynamicSystem::getX_old(float t) {
  // Вычисление Х для дискретного случая
  if (systemType == true) {
    matrix pM1(systemSize, systemSize);
    matrix pM2(systemSize, systemSize);

    matrix tmp1;
    matrix tmp2(A_old);
    matrix tmp3;
    matrix tmp4(A_old);
    matrix tmp5(systemSize, systemSize);
    matrix tmp6;

    tmp1.setSingle(systemSize);
    tmp3.setSingle(systemSize);
    tmp6.setSingle(systemSize);

    tmp1 = tmp1 - B_old;
    tmp1.getInverse();
    tmp2 = tmp2 - B_old;
    tmp1 = tmp1 * tmp2;
    tmp5 = tmp1;
    tmp1.getPow(t);
    pM1 = tmp1 * X0_old;
    tmp3 = tmp3 - A_old;
    tmp3.getInverse();
    tmp4 = tmp4 - B_old;
    tmp3 = tmp3 * tmp4;
    tmp5.getPow(t - 1);
    tmp6 = tmp6 - B_old;
    tmp6.getInverse();
    tmp6 = tmp6 * C0_old;
    pM2 = tmp3 * tmp5;
    pM2 = pM2 * tmp6;

    for (int i = 0; i < systemSize; i++) {
      X_old.setElement(i, 0, pM1.getElement(i, 0) + Xst_old.getElement(i, 0) - pM2.getElement(i, 0));
    }

  }
  else {
    matrix X1(systemSize, systemSize);
    matrix X2(X0_old);
    matrix X3(systemSize, systemSize);

    matrix tmp1;
    matrix tmp2(A_old);
    matrix tmp3;
    tmp1.setSingle(systemSize);
    tmp3.setSingle(systemSize);

    tmp1 = tmp1 - B_old;
    tmp1.getInverse();
    tmp2 = tmp2 - tmp3;
    tmp1 = tmp1 * tmp2;
    tmp1.getExp(t);

    X1 = tmp1;
    X2 = X2 - Xst_old;
    X3 = X1 * X2;

    for (int j = 0; j < systemSize; j++) {
      X_old.setElement(j, 0, X3.getElement(j, 0) + Xst_old.getElement(j, 0));
    }
  }
}

// check productive matrices
bool DynamicSystem::isAllProductive() {
  matrix tmpA(A);
  matrix tmpB(B);
  matrix tmpAB(systemSize, systemSize);
  matrix E;
  E.setSingle(systemSize);

  matrix tmp1(systemSize, systemSize);
  matrix tmp2(systemSize, systemSize);
  matrix tmp3(systemSize, systemSize);

  if (tmpA.isProductive() == true) {
    if (tmpB.isProductive() == true) {
      tmpAB = tmpA - tmpB;
      if (tmpAB.isProductive() == true) {
        tmp1 = E - B;
        tmp1.getInverse(); /*\*/
        tmp2 = tmpAB;
        tmp2 = tmp1 * tmpAB;
        if (tmp2.isProductive() == true) return true;
        else return false;
      } else return false;
    } else return false;
  } else return false;

}

void DynamicSystem::takeIncK() {
  float tmpSum = 0.0;
  k.init(-0.01);
  for (int i = 0; i < systemSize; i++) {
    for (int j = 0; j < systemSize; j++) tmpSum += A.getElement(i, j);
    if ((tmpSum + 0.03) > 1.0) {
      for (int p = 0; p < systemSize; p++) k.setElement(i, p, 0.0);
    }
    tmpSum = 0.0;
  }
  A = A - k;
  QString sTmp;
  for (int x = 0; x < systemSize; x++)
    for (int y = 0; y < systemSize; y++) {
      sTmp.setNum(A.getElement(x, y));
      matrixA -> setItem(x, y, new QTableWidgetItem(sTmp));
    }
  isUsedFeedback = true;
}

void DynamicSystem::takeDecK() {
  k.init(0.0);
  for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
          if ((A.getElement(i, j) - B.getElement(i, j)) > 0.01) k.setElement(i, j, 0.01);
      }
  }

  A = A - k;
  QString sTmp;
  for (int x = 0; x < systemSize; x++) {
    for (int y = 0; y < systemSize; y++) {
      sTmp.setNum(A.getElement(x, y));
      matrixA -> setItem(x, y, new QTableWidgetItem(sTmp));
    }
  }

  isUsedFeedback = true;
}

void DynamicSystem::takeEqvK() {
  matrix tmpA(A);
  matrix tmpB(B);
  matrix tmpK(systemSize, systemSize);
  tmpK.init(0.01);
  k = tmpA - tmpB - tmpK;
  A = A - k;
  QString sTmp;
  for (int x = 0; x < systemSize; x++) {
    for (int y = 0; y < systemSize; y++) {
      sTmp.setNum(A.getElement(x, y));
      matrixA -> setItem(x, y, new QTableWidgetItem(sTmp));
    }
  }
  isUsedFeedback = true;
}

void DynamicSystem::saveOldData() {
  for (int i = 0; i < systemSize; i++) {
    for (int j = 0; j < systemSize; j++) {
      A_old.setElement(i, j, A.getElement(i, j) + k.getElement(i, j)); // Probable BUG
      B_old.setElement(i, j, B.getElement(i, j));
      k_old.setElement(i, j, k.getElement(i, j));
    }
    C0_old.setElement(i, 0, C0.getElement(i, 0));
    X0_old.setElement(i, 0, X0.getElement(i, 0));
  }
  canSave = true;
}

void DynamicSystem::resizeSystem(bool clear) {
  if (clear == true) {
    A.setSize(systemSize, systemSize);
    B.setSize(systemSize, systemSize);
    k.setSize(systemSize, systemSize);
    k.init(0);
    C0.setSize(systemSize, 1);
    X0.setSize(systemSize, 1);
    X.setSize(systemSize, 1);
    Xst.setSize(systemSize, 1);
    
    A_old.setSize(systemSize, systemSize);
    B_old.setSize(systemSize, systemSize);
    k_old.setSize(systemSize, systemSize);
    k_old.init(0);
    C0_old.setSize(systemSize, 1);
    X0_old.setSize(systemSize, 1);
    X_old.setSize(systemSize, 1);
    Xst_old.setSize(systemSize, 1);
    
    tempM.setSize(1, (2 * systemSize) + 1);
    tempM_old.setSize(1, (2 * systemSize) + 1);
    
    matrixA -> setColumnCount(systemSize);
    matrixA -> setRowCount(systemSize);
    matrixB -> setColumnCount(systemSize);
    matrixB -> setRowCount(systemSize);
    matrixC0 -> setRowCount(systemSize);
    matrixX0 -> setRowCount(systemSize);
    
    int fSize = 8;
    if (systemSize > 4) {
      fSize = 7;
      if (systemSize > 6) {
        fSize = 5;
        if (systemSize > 8)
          fSize = 4;
      }
    } else {
        fSize = 8;
    }

    QFont matrixFont("Tahoma", fSize);
    matrixA -> setFont(matrixFont);
    matrixB -> setFont(matrixFont);
    matrixC0 -> setFont(matrixFont);
    matrixX0 -> setFont(matrixFont);
    for (int i = 0; i < systemSize; i++) {
      matrixA -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
      matrixA -> verticalHeader() -> resizeSection(i, 120 / systemSize);
      matrixB -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
      matrixB -> verticalHeader() -> resizeSection(i, 120 / systemSize);
      matrixC0 -> horizontalHeader() -> resizeSection(i, 40);
      matrixC0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
      matrixX0 -> horizontalHeader() -> resizeSection(i, 40);
      matrixX0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    }
    
    matrixA -> clear();
    matrixB -> clear();
    matrixC0 -> clear();
    matrixX0 -> clear();
    
    progDialogResize();
    
    graphArea -> isGraphBuild = true;
    graphArea -> updateGL();
    
    tableRowCount = 0;
    tableArea -> setRowCount(tableRowCount);
    tableArea -> setColumnCount((2 * systemSize) + 1);
    
    for (int j = 0; j < ((2 * systemSize) + 1); j++) {
        tableArea -> horizontalHeader() -> resizeSection(j, 36);
    }

    QStringList tableLabels;
    QString sTmp;
    for (int k = 0; k < 3; k++) {
      if (k == 0) {
          tableLabels += "t";
      }

      if (k == 1) {
          for (int xi = 0; xi < systemSize; xi++) {
            sTmp.setNum(xi);
            tableLabels += "X" + sTmp;
          }
      }

      if (k == 2) {
          for (int xj = 0; xj < systemSize; xj++) {
            sTmp.setNum(xj);
            tableLabels += "X*" + sTmp;
          }
      }
    }
    tableArea -> setHorizontalHeaderLabels(tableLabels);
  }
  isChangable = true;
  isSave = false;
  isUsedFeedback = false;
  isFirstStart = true;
  isFirstStartSave = true;
  canSave = false;
}

void DynamicSystem::toggleStepOff() {
  stepList -> setEnabled(false);
  systemType = true;
  stepMultipler = 1;
  stepIndex = 1;
  graphArea -> stepIndex = 1;
  resizeSystem(false);
}

void DynamicSystem::toggleStepOn() {
  stepList -> setEnabled(true);
  systemType = false;
}

void DynamicSystem::progControlOpen() {
  if ((pharseMatrix(systemSize, 1, matrixC0) == 0)) {
    progDialog -> show();

    progOmega -> setText("1.00");
    progK -> setText("0.1");
    // Copy and initialize matrixes with numbers
    QString sTmp;
    double dTmp;

    for (int i = 0; i < systemSize; i++) {
      dTmp = matrixC0 -> item(i, 0) -> text().toDouble();
      sTmp.setNum(dTmp);
      progMatrixC0 -> setItem(i, 0, new QTableWidgetItem(sTmp));
      dTmp /= 100;
      sTmp.setNum(dTmp);
      progMatrixa0 -> setItem(i, 0, new QTableWidgetItem(sTmp));
      progMatrixa0_1 -> setItem(i, 0, new QTableWidgetItem(sTmp));
      dTmp = matrixC0 -> item(i, 0) -> text().toDouble();
      sTmp.setNum(dTmp);
      progMatrixd0 -> setItem(i, 0, new QTableWidgetItem(sTmp));
    }
  } else {
      QMessageBox::critical(this, "Error", "Matrix C0 is specified incorrectly\nIt is impossible to go to program controls", "Continue");
  }
}

void DynamicSystem::toggleEditA() {
  if (incA -> isEnabled() == true) {
    incA -> setEnabled(false);
    decA -> setEnabled(false);
    editA -> setEnabled(false);
    isChangable = false;
  } else {
    incA -> setEnabled(true);
    decA -> setEnabled(true);
    editA -> setEnabled(true);
    isChangable = true;
  }
}

void DynamicSystem::toggleSave() {
  if (isSave == true) {
    isSave = false;
    graphArea -> isSavedOld = false;
    isFirstStartSave = true;
  } else {
    isSave = true;
    graphArea -> isSavedOld = false;
    isFirstStartSave = true;
  }
}

void DynamicSystem::incMatrixA() {
  double dTmp;
  QString sTmp;
  if (pharseMatrix(systemSize, systemSize, matrixA) == 0) {
    for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
        dTmp = matrixA -> item(i, j) -> text().toDouble() + editA -> text().toDouble();
        sTmp.setNum(dTmp);
        matrixA -> setItem(i, j, new QTableWidgetItem(sTmp));
      }
    }
  }
}

void DynamicSystem::decMatrixA() {
  double dTmp;
  QString sTmp;
  if (pharseMatrix(systemSize, systemSize, matrixA) == 0) {
    for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
        dTmp = matrixA -> item(i, j) -> text().toDouble() - editA -> text().toDouble();
        sTmp.setNum(dTmp);
        matrixA -> setItem(i, j, new QTableWidgetItem(sTmp));
      }
    }
  }
}

void DynamicSystem::incMatrixB() {
  double dTmp;
  QString sTmp;
  if (pharseMatrix(systemSize, systemSize, matrixB) == 0) {
    for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
        dTmp = matrixB -> item(i, j) -> text().toDouble() + editB -> text().toDouble();
        sTmp.setNum(dTmp);
        matrixB -> setItem(i, j, new QTableWidgetItem(sTmp));
      }
    }
  }
}

void DynamicSystem::decMatrixB() {
  double dTmp;
  QString sTmp;
  if (pharseMatrix(systemSize, systemSize, matrixB) == 0) {
    for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
        dTmp = matrixB -> item(i, j) -> text().toDouble() - editB -> text().toDouble();
        sTmp.setNum(dTmp);
        matrixB -> setItem(i, j, new QTableWidgetItem(sTmp));
      }
    }
  }
}

void DynamicSystem::resizeX() {
  QString tmp;
  int X0, X1;
  bool ok = TRUE;
  X0 = editX0 -> text().toInt( & ok, 10);
  if (ok == FALSE) {
    QMessageBox::warning(this, "Error", "The starting coordinate is not an integer", "Continue");
    return;
  }
  X1 = editX1 -> text().toInt( & ok, 10);
  if (ok == FALSE) {
    QMessageBox::warning(this, "Error", "End coordinate is not an integer", "Continue");
    return;
  }
  if ((X0 < 0) || (X1 < 0)) {
    QMessageBox::warning(this, "Error", "Due to the non-negativity of solutions, it is impossible to specify a negative coordinate\n(must be greater than 0)", "Continue");
    return;
  }
  if (X0 == X1) {
    QMessageBox::critical(this, "Error", "The start and end coordinates along the X axis are the same. Unable to plot", "Continue");
    return;
  }
  if (X0 > X1) {
    QMessageBox::warning(this, "Warning", "The coordinates will be inverted (Start coordinate is greater than end coordinate)", "Continue");
    tmp = editX0 -> text();
    editX0 -> setText(editX1 -> text());
    editX1 -> setText(tmp);
  }
}

void DynamicSystem::resizeY() {
  QString tmp;
  int Y0, Y1;
  bool ok = TRUE;
  Y0 = editY0 -> text().toInt( & ok, 10);
  if (ok == FALSE) {
    QMessageBox::warning(this, "Error", "The starting coordinate is not an integer", "Continue");
    return;
  }

  Y1 = editY1 -> text().toInt( & ok, 10);
  if (ok == FALSE) {
    QMessageBox::warning(this, "Error", "End coordinate is not an integer", "Continue");
    return;
  }

  if (Y0 == Y1) {
    QMessageBox::critical(this, "Error", "The start and end coordinates along the Y axis are the same. Unable to plot", "Continue");
    return;
  }

  if (Y0 > Y1) {
    QMessageBox::warning(this, "Warning", "The coordinates will be inverted (Start coordinate is greater than end coordinate)", "Continue");
    tmp = editY0 -> text();
    editY0 -> setText(editY1 -> text());
    editY1 -> setText(tmp);
  }
}

void DynamicSystem::takeNewData() {
  bool ok0 = TRUE;
  bool ok1 = TRUE;
  bool ok2 = TRUE;
  bool ok3 = TRUE;
  
  editX0 -> text().toInt( & ok0, 10);
  editX1 -> text().toInt( & ok1, 10);
  editY0 -> text().toInt( & ok2, 10);
  editY1 -> text().toInt( & ok3, 10);
  
  tableRowCount = 0;
  tableArea -> clear();
  QStringList tableLabels;
  QString sTmp;
  for (int sC = 0; sC < 3; sC++) {
    if (sC == 0) {
      tableLabels += "t";
    }

    if (sC == 1) {
      for (int sCi = 0; sCi < systemSize; sCi++) {
        sTmp.setNum(sCi);
        tableLabels += "X" + sTmp;
      }
    }

    if (sC == 2) {
      for (int sCj = 0; sCj < systemSize; sCj++) {
        sTmp.setNum(sCj);
        tableLabels += "X*" + sTmp;
      }
    }
  }

  tableArea -> setHorizontalHeaderLabels(tableLabels);
  auto isValidMatrixA = pharseMatrix(systemSize, systemSize, matrixA) == 0;
  auto isValidMatrixB = pharseMatrix(systemSize, systemSize, matrixB) == 0;
  auto isValidMatrixC0 = pharseMatrix(systemSize, 1, matrixC0) == 0;
  auto isValidMatrixX0 = pharseMatrix(systemSize, 1, matrixX0) == 0;

  if (isValidMatrixA && isValidMatrixB && isValidMatrixC0 && isValidMatrixX0) {
    if (!((ok0 == FALSE) || (ok1 == FALSE) || (ok2 == FALSE) || (ok3 == FALSE))) {
      if (progFiNegative()) {
        for (int i = 0; i < systemSize; i++) {
          C0.setElement(i, 0, matrixC0 -> item(i, 0) -> text().toDouble());
          X0.setElement(i, 0, matrixX0 -> item(i, 0) -> text().toDouble());

          for (int j = 0; j < systemSize; j++) {
            A.setElement(i, j, matrixA -> item(i, j) -> text().toDouble());
            B.setElement(i, j, matrixB -> item(i, j) -> text().toDouble());
          }
        }
        
        bool ok = TRUE;
        x0 = editX0 -> text().toInt( & ok, 10);
        x1 = editX1 -> text().toInt( & ok, 10);
        if ((x0 < 0) || (x1 < 0)) {
          QMessageBox::critical(this, "Error", "Due to the non-negativity of solutions, it is impossible to specify a negative coordinate\n(must be greater than 0)", "Continue");
          return;
        }

        y0 = editY0 -> text().toInt( & ok, 10);
        y1 = editY1 -> text().toInt( & ok, 10);
        if ((x0 == x1) || (y0 == y1)) {
          QMessageBox::critical(this, "Error", "The start and end coordinates are the same. Unable to plot", "Continue");
          return;
        }

        if (systemType == false) {
          switch (stepList -> currentIndex()) {
          case 0:
            stepMultipler = 10;
            stepIndex = 0.1;
            graphArea -> stepIndex = 0.1;
            break;
          case 1:
            stepMultipler = 100;
            stepIndex = 0.01;
            graphArea -> stepIndex = 0.01;
            break;
          case 2:
            stepMultipler = 1000;
            stepIndex = 0.001;
            graphArea -> stepIndex = 0.001;
            break;
          }
        }
        feedbackLabel:          
          tableRowCount = stepMultipler * (x1 - x0) + 1;

        tableArea -> setRowCount(tableRowCount);
        graphArea -> pArray = new float * [tableRowCount];
        for (int count = 0; count < tableRowCount; count++) {
             graphArea -> pArray[count] = new float[2 * systemSize];
        }

        if (isFirstStartSave == false) {
          graphArea -> pArray_old = new float * [tableRowCount];
          for (int count_old = 0; count_old < tableRowCount; count_old++) {
               graphArea -> pArray_old[count_old] = new float[2 * systemSize];
          }
        }
        
        graphArea -> isGraphBuild = true;
        graphArea -> setLines(x0, x1, y0, y1);
        if (isAllProductive() == true) {
          int line = 0;
          bool isBug = false;
          Ct.initMatrix(C0, systemSize, 1);
          getC(0);
          getX_st(x0);
          if (isFirstStartSave == false) {
              getX_st_old(x0);
          }

          for (float t = x0; t <= x1; t += stepIndex) {
            tempM.setElement(0, 0, t);
            if (isBug == false) {
              getX(t);
              if (isFirstStartSave == false) {
                  getX_old(t);
              }
            }

            if (isBug == false && systemType == false) {
              for (int b = 0; b < systemSize; b++) {
                  if (((X.getElement(b, 0) - Xst.getElement(b, 0)) < 0.001) && systemType == false) {
                      isBug = true;
                  }
              }

            }

            if (isBug == true && systemType == false) {
              for (int bf = 0; bf < systemSize; bf++) {
                X.setElement(bf, 0, Xst.getElement(bf, 0));
                if (isFirstStartSave == false) {
                    X_old.setElement(bf, 0, Xst_old.getElement(bf, 0));
                }
              }
            }

            for (int xe = 0; xe < systemSize; xe++) {
              tempM.setElement(0, xe + systemSize + 1, Xst.getElement(xe, 0));
              tempM.setElement(0, xe + 1, X.getElement(xe, 0));
            }

            if (isFirstStartSave == false) {
              for (int xe_old = 0; xe_old < systemSize; xe_old++) {
                tempM_old.setElement(0, xe_old + systemSize + 1, Xst_old.getElement(xe_old, 0));
                tempM_old.setElement(0, xe_old + 1, X_old.getElement(xe_old, 0));
              }
            }

            addNewLine(tempM, tempM_old, line);
            for (int pa = 0; pa < systemSize; pa++) {
              graphArea -> pArray[line][pa] = X.getElement(pa, 0);
              graphArea -> pArray[line][pa + systemSize] = Xst.getElement(pa, 0);
            }

            if (isFirstStartSave == false) {
              for (int pa_old = 0; pa_old < systemSize; pa_old++) {
                graphArea -> pArray_old[line][pa_old] = X_old.getElement(pa_old, 0);
                graphArea -> pArray_old[line][pa_old + systemSize] = Xst_old.getElement(pa_old, 0);
              }
            }
            line++;
          }
          graphArea -> isGraphBuild = false;
          graphArea -> updateGL();

          if (isSave == true) {
            saveOldData();
            if (isFirstStartSave == false) {
                graphArea -> isSavedOld = true;
            }
            isFirstStartSave = false;
          }

        } else {
          matrix tmpAB(systemSize, systemSize);
          matrix tmpA(A);
          matrix tmpB(B);
          if (tmpA.isProductive() == true) {
            if (tmpB.isProductive() == true) {
              matrix tmp1(systemSize, systemSize);
              matrix tmp2(systemSize, systemSize);
              matrix tmp3(systemSize, systemSize);
              matrix E;
              E.setSingle(systemSize);
              tmpAB = tmpA - tmpB;
              if (tmpAB.isProductive() == true) {

                tmp1 = E - B;
                tmp1.getInverse();
                tmp2 = tmpAB;
                tmp2 = tmp1 * tmpAB;
                if (!(tmp2.isProductive() == true)) {
                    QMessageBox::critical(this, "Error", "The matrix (E-B)^(-1)*(A-B) is not productive!\nSystem construction is not available.", "Continue");
                }

              } else {
                  QMessageBox::critical(this, "Error", "The A-B matrix is ​​not productive!\nSystem building is not available.", "Continue");
              }
            } else {
                QMessageBox::critical(this, "Error", "Matrix B is not productive!\nSystem construction is not available.", "Continue");
            }
          } else {
              QMessageBox::critical(this, "Error", "Matrix A is not productive!\nSystem construction is not available.", "Continue");
          }

          if (tmpB.isProductive() == true) {
            for (int prodI = 0; prodI < systemSize; prodI++) {
                for (int prodJ = 0; prodJ < systemSize; prodJ++) {
                    if (A.getElement(prodI, prodJ) < B.getElement(prodI, prodJ)) {
                        QMessageBox::warning(this, "Warning", "The system is asymptotically unstable!\nWe build a system with a feedback gain matrix K.", "Continue");

                      k.init(0.0);
                      for (int i = 0; i < systemSize; i++) {
                          for (int j = 0; j < systemSize; j++) {
                              if (A.getElement(i, j) < B.getElement(i, j)) {
                                  if (A.getElement(i, j) < 0) {
                                      k.setElement(i, j, A.getElement(i, j) - B.getElement(i, j) - 0.01);
                                  } else {
                                      k.setElement(i, j, -(B.getElement(i, j) - A.getElement(i, j)) - 0.01);
                                  }
                              }
                          }
                      }

                      A = A - k;
                      QString sTmp;
                      for (int x = 0; x < systemSize; x++) {
                          for (int y = 0; y < systemSize; y++) {
                            sTmp.setNum(A.getElement(x, y));
                            matrixA -> setItem(x, y, new QTableWidgetItem(sTmp));
                          }
                      }
                      break;
                    }
                }
            }
          }
        }

        if ((isUsedFeedback == false) && (isAllProductive() == true)) {
          QMessageBox msgProd;
          QPushButton * incButton = msgProd.addButton("Increase", QMessageBox::ActionRole);
          QPushButton * decButton = msgProd.addButton("Decrease", QMessageBox::ActionRole);
          QPushButton * eqvButton = msgProd.addButton("Equality", QMessageBox::ActionRole);
          QPushButton * notButton = msgProd.addButton("Do not use", QMessageBox::ActionRole);
          msgProd.setIcon(QMessageBox::Question);
          msgProd.setWindowTitle("Selection of matrix k");
          msgProd.setInformativeText("Management:\n\n1. To increase the values ​​of the vector elements Xt and X*\n2. To reduce the values ​​of the vector elements Xt and X*\n3. To ensure equality of vector elements Xt and X*");
          msgProd.exec();
          if (msgProd.clickedButton() == incButton) {
            takeIncK();
            takeAttr = 1;
            goto feedbackLabel;
          }
          if (msgProd.clickedButton() == decButton) {
            takeDecK();
            takeAttr = 2;
            goto feedbackLabel;
          }
          if (msgProd.clickedButton() == eqvButton) {
            takeEqvK();
            takeAttr = 3;
            goto feedbackLabel;
          }
        }
        if ((isUsedFeedback == true) && (isAllProductive() == true)) {
          QMessageBox msgContinue;
          QPushButton * yesButton = msgContinue.addButton("Yes", QMessageBox::ActionRole);
          QPushButton * noButton = msgContinue.addButton("No", QMessageBox::ActionRole);
          msgContinue.setIcon(QMessageBox::Question);
          msgContinue.setWindowTitle("Feedback gain matrix k");
          msgContinue.setInformativeText("Continue with the choice of matrix k according to the given condition?");
          msgContinue.exec();
          if (msgContinue.clickedButton() == yesButton) {
            switch (takeAttr) {
            case 1:
              takeIncK();
              break;
            case 2:
              takeDecK();
              break;
            case 3:
              takeEqvK();
              break;
            }
            goto feedbackLabel;
          }
        }
        isUsedFeedback = false;
      } else
        QMessageBox::warning(this, "Error", "It is impossible to build a system. Matrix C(t) < 0.\nTry using other parameters in the matrix.", "Continue");
    } else
      QMessageBox::critical(this, "Error", "The coordinates are set incorrectly", "Continue");
  }
  isFirstStart = false;
  isProgControl = false;
}

void DynamicSystem::calculateInPoint() {
  if ((pharseMatrix(systemSize, systemSize, matrixA) == 0) && (pharseMatrix(systemSize, systemSize, matrixB) == 0) && (pharseMatrix(systemSize, 1, matrixC0) == 0) && (pharseMatrix(systemSize, 1, matrixX0) == 0)) {
    for (int i = 0; i < systemSize; i++) {
      C0.setElement(i, 0, matrixC0 -> item(i, 0) -> text().toDouble());
      X0.setElement(i, 0, matrixX0 -> item(i, 0) -> text().toDouble());
      for (int j = 0; j < systemSize; j++) {
        A.setElement(i, j, matrixA -> item(i, j) -> text().toDouble());
        B.setElement(i, j, matrixB -> item(i, j) -> text().toDouble());
      }
    }
    double point;
    if (systemType == true) {
      bool ok = TRUE;
      point = editCalc -> text().toInt( & ok, 10);
      if (ok == FALSE) {
        QMessageBox::critical(this, "Error", "The specified time t is not an integer.", "Continue");
        return;
      }
    } else {
      QString sTmp;
      sTmp = editCalc -> text();
      for (int stI = 0; stI < sTmp.length(); stI++) {
        if (!(sTmp[stI] <= QChar('9') && sTmp[stI] >= QChar('0')) && !(sTmp[stI] <= QChar('.') && sTmp[stI] >= QChar(','))) {
          QMessageBox::critical(this, "Error", "Invalid characters detected at specified time t", "Continue");
          return;
        }
      }
      point = editCalc -> text().toDouble();
    }

    if (point < 0) {
      QMessageBox::critical(this, "Error", "Due to the non-negativity of solutions, it is impossible to specify a negative moment in time t", "Continue");
      return;
    } else {
      if (isAllProductive() == true) {
        QString str("Function value at point t = ");
        QString sRes, sNum, sPoint;
        str += sPoint.setNum(point) + "\n";
        getX_st(point);
        getX(point);
        for (int sI = 0; sI < systemSize; sI++) {
          sRes.setNum(X.getElement(sI, 0));
          sNum.setNum(sI);
          str += "X" + sNum + " = " + sRes + "; ";
        }
        str += "\n Equilibrium state: \n";
        for (int stI = 0; stI < systemSize; stI++) {
          sRes.setNum(Xst.getElement(stI, 0));
          sNum.setNum(stI);
          str += "X*" + sNum + " = " + sRes + "; ";
        }
        QMessageBox::information(this, "Function value", str, "Continue");
      }
    }
  }

}

void DynamicSystem::resizeData() {
  bool ok = true;
  editSize -> text().toInt( & ok, 10);

  if (!(ok == FALSE)) {
    QMessageBox msgClear;
    QPushButton * yesClear = msgClear.addButton("Yes", QMessageBox::ActionRole);
    QPushButton * noClear = msgClear.addButton("No", QMessageBox::ActionRole);
    msgClear.setIcon(QMessageBox::Question);
    msgClear.setWindowTitle("Warning");
    msgClear.setInformativeText("When changing the number of subsystems, all previously entered values ​​will be lost.\nContinue?");
    msgClear.exec();
    
    if (msgClear.clickedButton() == yesClear) {
      systemSize = editSize -> text().toInt( & ok, 10);
      graphArea -> systemSize = systemSize;
      graphArea -> stepIndex = stepIndex;
      resizeSystem(true);
    }
  } else {
    QMessageBox::critical(this, "Error", "Invalid characters when specifying the number of subsystems", "Continue");
    return;
  }

}

void DynamicSystem::saveData() {
  FILE * fTxt, * fImage;

  if (isFirstStart == true) {
    QMessageBox::critical(this, "Error", "No information to save", "Continue");
    return;
  }

  QDateTime dateTime = QDateTime::currentDateTime();
  QString dateTimeT = dateTime.toString("dd-MM-yyyy-hh-mm-ss") + ".txt";
  QString dateTimeI = dateTime.toString("dd-MM-yyyy-hh-mm-ss") + ".bmp";
  dateTimeT.toLocal8Bit();
  dateTimeI.toLocal8Bit();

  char * fileTextName = dateTimeT.toAscii().data();
  char * fileImgName = dateTimeI.toAscii().data();

  if ((isSave == true) && (canSave == true)) {
      QMessageBox::critical(this, "Error", "Storing two states", "Continue");
  } else {
      QMessageBox::critical(this, "Error", "Saving one state", "Continue");
  }

  QImage scr(278, 147, QImage::Format_RGB16);
  scr = graphArea -> grabFrameBuffer();
  scr.save(dateTimeI, "BMP");
}

void DynamicSystem::progDialogInit() {
  progDialog = new QDialog(this);
  progDialog -> setFixedSize(500, 300);
  progDialog -> setWindowTitle("Program control");
  progDialog -> setModal(true);

  progDialogProceed = new QPushButton("Continue", this);
  progDialogCancel = new QPushButton("Cancel", this);

  progDialogSelect = new QComboBox(this);
  progDialogSelect -> addItem("Power");
  progDialogSelect -> addItem("Power (extended)");
  progDialogSelect -> addItem("Trigonometric");

  progSelectFunc = new QComboBox(this);
  progSelectFunc -> addItem("sin");
  progSelectFunc -> addItem("cos");
  progSelectFunc -> addItem("tg");

  progOmega = new QLineEdit(this);
  progK = new QLineEdit(this);
  progOmega -> setMaxLength(4);
  progOmega -> setAlignment(Qt::AlignCenter);
  progK -> setMaxLength(4);
  progK -> setAlignment(Qt::AlignCenter);
  progOmega -> setMaximumSize(35, 20);
  progK -> setMaximumSize(35, 20);
  progMatrixC0 = new QTableWidget(systemSize, 1, this);
  progMatrixa0 = new QTableWidget(systemSize, 1, this);
  progMatrixd0 = new QTableWidget(systemSize, 1, this);
  progMatrixa0_1 = new QTableWidget(systemSize, 1, this);
  progMatrixC0 -> setMinimumSize(42, 122);
  progMatrixa0 -> setMinimumSize(42, 122);
  progMatrixd0 -> setMinimumSize(42, 122);
  progMatrixa0_1 -> setMinimumSize(42, 122);
  progMatrixC0 -> setMaximumSize(42, 122);
  progMatrixa0 -> setMaximumSize(42, 122);
  progMatrixd0 -> setMaximumSize(42, 122);
  progMatrixa0_1 -> setMaximumSize(42, 122);
  for (int i = 0; i < systemSize; i++) {
    progMatrixC0 -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixC0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0 -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixd0 -> horizontalHeader() -> resizeSection(i, 40);
    progMatrixd0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0_1 -> horizontalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0_1 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
  }
  progMatrixC0 -> verticalHeader() -> hide();
  progMatrixC0 -> horizontalHeader() -> hide();
  progMatrixa0 -> verticalHeader() -> hide();
  progMatrixa0 -> horizontalHeader() -> hide();
  progMatrixd0 -> verticalHeader() -> hide();
  progMatrixd0 -> horizontalHeader() -> hide();
  progMatrixa0_1 -> verticalHeader() -> hide();
  progMatrixa0_1 -> horizontalHeader() -> hide();

  QVBoxLayout * mainProgDialog = new QVBoxLayout;
  QGridLayout * progDialogGrid = new QGridLayout;
  QHBoxLayout * progDialogUpperLayout = new QHBoxLayout;
  QHBoxLayout * progDialogLowerLayout = new QHBoxLayout;
  QGroupBox * progDialogGroup = new QGroupBox("Program control", this);
  QLabel * progDialogSelectLabel = new QLabel("Type of program control:");
  progDialogSelectLabel -> setAlignment(Qt::AlignRight | Qt::AlignCenter);

  QLabel * progGroupX0 = new QLabel("C0:", this);
  QLabel * progGroupa0 = new QLabel("a*:", this);
  QLabel * progGroupa0_1 = new QLabel("a*:", this);
  progGroupd0 = new QLabel("d0:", this);

  QLabel * progGroupTrig = new QLabel("func:", this);
  QLabel * progGroupOmega = new QLabel("Omega:", this);
  QLabel * progGroupK = new QLabel("k:", this);
  // Group matrixes and forms in layouts
  QLabel * progLabel11 = new QLabel(" + ", this);
  QLabel * progLabel11_1 = new QLabel(" + ", this);
  QLabel * progLabel13 = new QLabel(" + ( ", this);
  QLabel * progLabel15 = new QLabel(" ) * ", this);
  QLabel * progLabel17 = new QLabel(" ( ", this);
  QLabel * progLabel19 = new QLabel(" * t ^ ", this);
  QLabel * progLabel111 = new QLabel(" ) ", this);
  progGroupX0 -> setAlignment(Qt::AlignCenter);
  progGroupa0 -> setAlignment(Qt::AlignCenter);
  progGroupd0 -> setAlignment(Qt::AlignCenter);
  progGroupTrig -> setAlignment(Qt::AlignCenter);
  progGroupOmega -> setAlignment(Qt::AlignCenter);
  progGroupK -> setAlignment(Qt::AlignCenter);

  QHBoxLayout * progDialogCenterHLayout = new QHBoxLayout;
  QVBoxLayout * progDialogCenterVLayout[14];
  for (int vstep = 0; vstep < 14; vstep++) {
      progDialogCenterVLayout[vstep] = new QVBoxLayout;
  }

  progDialogCenterVLayout[0] -> addWidget(progGroupX0);
  progDialogCenterVLayout[0] -> addWidget(progMatrixC0);
  progDialogCenterVLayout[0] -> addSpacing(20);
  progDialogCenterVLayout[1] -> addWidget(progLabel11);
  progDialogCenterVLayout[2] -> addWidget(progGroupa0);
  progDialogCenterVLayout[2] -> addWidget(progMatrixa0);
  progDialogCenterVLayout[2] -> addSpacing(20);
  progDialogCenterVLayout[3] -> addWidget(progLabel13);
  progDialogCenterVLayout[4] -> addWidget(progGroupd0);
  progDialogCenterVLayout[4] -> addWidget(progMatrixd0);
  progDialogCenterVLayout[4] -> addSpacing(20);
  progDialogCenterVLayout[5] -> addWidget(progLabel11_1);
  progDialogCenterVLayout[6] -> addWidget(progGroupa0_1);
  progDialogCenterVLayout[6] -> addWidget(progMatrixa0_1);
  progDialogCenterVLayout[6] -> addSpacing(20);
  progDialogCenterVLayout[7] -> addWidget(progLabel15);
  progDialogCenterVLayout[8] -> addSpacing(60);
  progDialogCenterVLayout[8] -> addWidget(progGroupTrig);
  progDialogCenterVLayout[8] -> addWidget(progSelectFunc);
  progDialogCenterVLayout[8] -> addSpacing(80);
  progDialogCenterVLayout[9] -> addWidget(progLabel17);
  progDialogCenterVLayout[10] -> addSpacing(60);
  progDialogCenterVLayout[10] -> addWidget(progGroupOmega);
  progDialogCenterVLayout[10] -> addWidget(progOmega);
  progDialogCenterVLayout[10] -> addSpacing(80);
  progDialogCenterVLayout[11] -> addWidget(progLabel19);
  progDialogCenterVLayout[12] -> addSpacing(40);
  progDialogCenterVLayout[12] -> addWidget(progGroupK);
  progDialogCenterVLayout[12] -> addWidget(progK);
  progDialogCenterVLayout[12] -> addSpacing(100);
  progDialogCenterVLayout[13] -> addWidget(progLabel111);

  for (int vset = 0; vset < 14; vset++) {
      progDialogCenterHLayout -> addLayout(progDialogCenterVLayout[vset]);
  }

  progDialogGroup -> setLayout(progDialogCenterHLayout);
  progDialogUpperLayout -> addWidget(progDialogSelectLabel);
  progDialogUpperLayout -> addWidget(progDialogSelect);
  progDialogLowerLayout -> addSpacing(250);
  progDialogLowerLayout -> addWidget(progDialogProceed);
  progDialogLowerLayout -> addWidget(progDialogCancel);
  progDialogLowerLayout -> addSpacing(250);
  mainProgDialog -> addLayout(progDialogUpperLayout);
  mainProgDialog -> addWidget(progDialogGroup);
  mainProgDialog -> addLayout(progDialogLowerLayout);
  progDialog -> setLayout(mainProgDialog);
  progCurrentDialogSelect = progDialogSelect -> currentIndex();
  progSelect(progCurrentDialogSelect);
}

void DynamicSystem::progDialogResize() {
  C0_prog.setSize(systemSize, 1);
  a0.setSize(systemSize, 1);
  a0_1.setSize(systemSize, 1);
  d0.setSize(systemSize, 1);
  Ct.setSize(systemSize, 1);

  progMatrixC0 -> setRowCount(systemSize);
  progMatrixa0 -> setRowCount(systemSize);
  progMatrixd0 -> setRowCount(systemSize);
  progMatrixa0_1 -> setRowCount(systemSize);
  
  int fSize = 8;
  if (systemSize > 4) {
    fSize = 7;
    if (systemSize > 6) {
      fSize = 5;
      if (systemSize > 8) {
        fSize = 4;
      }
    }
  } else {
    fSize = 8;
  }

  QFont matrixFont("Tahoma", fSize);
  progMatrixC0 -> setFont(matrixFont);
  progMatrixa0 -> setFont(matrixFont);
  progMatrixd0 -> setFont(matrixFont);
  progMatrixa0_1 -> setFont(matrixFont);
  for (int i = 0; i < systemSize; i++) {
    progMatrixC0 -> horizontalHeader() -> resizeSection(i, 40);
    progMatrixC0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0 -> horizontalHeader() -> resizeSection(i, 40);
    progMatrixa0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixd0 -> horizontalHeader() -> resizeSection(i, 40);
    progMatrixd0 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
    progMatrixa0_1 -> horizontalHeader() -> resizeSection(i, 40);
    progMatrixa0_1 -> verticalHeader() -> resizeSection(i, 120 / systemSize);
  }
}

void DynamicSystem::progProceed() {
  bool ok1 = true;
  bool ok2 = true;
  progOmega -> text().toDouble( & ok1);
  progK -> text().toDouble( & ok2);

  auto isValidMatrixC0 = pharseMatrix(systemSize, 1, progMatrixC0) == 0;
  auto isValidMatrixa0 = pharseMatrix(systemSize, 1, progMatrixa0) == 0;
  auto isValidMatrixd0 = pharseMatrix(systemSize, 1, progMatrixd0) == 0;
  auto isValidMatrix0_1 = pharseMatrix(systemSize, 1, progMatrixa0_1) == 0;

  if (isValidMatrixC0 && isValidMatrixa0 && isValidMatrixd0 && isValidMatrix0_1) {
    if (ok1 == true)
      if (ok2 == true) {
        QString sTmp;

        for (int i = 0; i < systemSize; i++) {
          switch (progDialogSelect -> currentIndex()) {
          case 0:
            C0.setElement(i, 0, progMatrixd0 -> item(i, 0) -> text().toDouble());
            a0.setElement(i, 0, progMatrixa0_1 -> item(i, 0) -> text().toDouble());
            break;
          default:
            C0.setElement(i, 0, progMatrixC0 -> item(i, 0) -> text().toDouble());
            a0.setElement(i, 0, progMatrixa0_1 -> item(i, 0) -> text().toDouble());
            d0.setElement(i, 0, progMatrixd0 -> item(i, 0) -> text().toDouble());
            break;
          }

          sTmp.setNum(C0.getElement(i, 0));
          matrixC0 -> item(i, 0) -> setText(sTmp);
        }

        omega = progOmega -> text().toDouble();
        powK = progK -> text().toDouble();
        isProgControl = true;

        progCancel();
        takeNewData();
      }
    else
      QMessageBox::critical(this, "Error", "Field k contains invalid characters", "Continue");
    else
      QMessageBox::critical(this, "Error", "The omega field contains invalid characters", "Continue");
  }

}

void DynamicSystem::progCancel() {
  progDialog -> hide();
  progDialog -> close();
}

void DynamicSystem::progSelect(int option) {
  QTableWidgetItem * itemTmp = new QTableWidgetItem("1");
  progCurrentDialogSelect = option;
  QString sTmp;
  double dTmp;
  switch (option) {
  case 0:
    progMatrixC0 -> setEnabled(false);
    progMatrixa0 -> setEnabled(false);
    progMatrixd0 -> setEnabled(true);
    progMatrixa0_1 -> setEnabled(true);
    progSelectFunc -> setEnabled(false);
    progOmega -> setEnabled(false);
    progK -> setEnabled(true);
    progGroupd0 -> setText("C0:");
    break;
  case 1:
    for (int j = 0; j < systemSize; j++) {
      progMatrixd0 -> item(j, 0) -> setText("0.001");
    }
    progMatrixC0 -> setEnabled(true);
    progMatrixa0 -> setEnabled(true);
    progMatrixd0 -> setEnabled(true);
    progMatrixa0_1 -> setEnabled(false);
    progSelectFunc -> setEnabled(false);
    progOmega -> setEnabled(false);
    progK -> setEnabled(true);
    progGroupd0 -> setText("d0:");
    break;
  case 2:
    for (int k = 0; k < systemSize; k++) {
      progMatrixd0 -> item(k, 0) -> setText("0.001");
    }

    progMatrixC0 -> setEnabled(true);
    progMatrixa0 -> setEnabled(true);
    progMatrixd0 -> setEnabled(true);
    progMatrixa0_1 -> setEnabled(false);
    progSelectFunc -> setEnabled(true);
    progOmega -> setEnabled(true);
    progK -> setEnabled(false);
    progGroupd0 -> setText("d0:");
    break;
  }
}

bool DynamicSystem::progFiNegative() {
  if (isProgControl) {
    matrix firstSum(systemSize, systemSize);
    firstSum.setSingle(systemSize);
    matrix secondSum(A);
    firstSum = firstSum - B;
    firstSum.getInverse();
    secondSum = secondSum - B;

    matrix fi(systemSize, 1);
    matrix fi1(systemSize, 1);
    matrix tmpLin1(C0);
    matrix tmpLin2(C0);
    matrix tmpStep(C0);
    matrix tmpStep1(d0);
    matrix tmpStep2(C0);
    matrix tmpStep3(d0);
    matrix tmpHarm(C0);
    matrix tmpHarm1(d0);
    matrix tmpHarm2(C0);
    matrix tmpHarm3(d0);

    matrix result(systemSize, 1);
    matrix tmp_result1(systemSize, 1);
    matrix tmp_result2(systemSize, systemSize);

    switch (progDialogSelect -> currentIndex()) {
    case 0:
      fi = tmpLin1 * pow(0, powK);
      tmpLin2 = tmpLin2 + a0;
      fi1 = tmpLin2 * pow(1, powK);
      break;
    case 1:
      tmpStep1 = tmpStep1 * pow(0, powK);
      tmpStep = tmpStep + tmpStep1;
      fi = tmpStep;
      tmpStep3 = tmpStep3 * pow(1, powK);
      tmpStep2 = tmpStep2 + a0;
      tmpStep2 = tmpStep2 + tmpStep3;
      fi1 = tmpStep2;
      break;
    case 2:
      switch (progSelectFunc -> currentIndex()) {
      case 0:
        tmpHarm1 = tmpHarm1 * sin(omega * 0);
        tmpHarm3 = tmpHarm3 * sin(omega * (1));
        break;
      case 1:
        tmpHarm1 = tmpHarm1 * cos(omega * 0);
        tmpHarm3 = tmpHarm3 * cos(omega * (1));
        break;
      case 2:
        tmpHarm1 = tmpHarm1 * tan(omega * 0);
        tmpHarm3 = tmpHarm3 * tan(omega * (1));
        break;
      }
      tmpHarm = tmpHarm + tmpHarm1;
      fi = tmpHarm;
      tmpHarm2 = tmpHarm2 + a0;
      tmpHarm2 = tmpHarm2 + tmpHarm3;
      fi1 = tmpHarm2;
      break;
    }

    tmp_result1.initMatrix(fi1, systemSize, 1);
    tmp_result2.initMatrix(firstSum, systemSize, systemSize);

    tmp_result2 = tmp_result2 * secondSum;
    tmp_result2 = tmp_result2 * fi;

    for (int k = 0; k < systemSize; k++) {
      auto result = tmp_result1.getElement(k, 0) - tmp_result2.getElement(k, 0);
      if (result < 0) {
        return false;
      }
    }
  }
  return true;
}
