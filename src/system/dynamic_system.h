#ifndef DSYS_H
#define DSYS_H

#include "matrix.h"
#include "gwidget.h"
#include <QWidget>
#include <QDialog>

class QPushButton;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QTableWidget;
class QTableWidgetItem;
class QTableView;
class QFont;
class GLWidget;

class DynamicSystem: public QDialog {
  Q_OBJECT

  public:
    DynamicSystem(QWidget * parent = 0);
  ~DynamicSystem();
  matrix A;
  matrix B;
  matrix C0;
  matrix X0;
  matrix X;
  matrix Xst;
  matrix k;

  matrix A_old;
  matrix B_old;
  matrix C0_old;
  matrix X0_old;
  matrix X_old;
  matrix Xst_old;
  matrix k_old;

  matrix tempM;
  matrix tempM_old;
  int systemSize;
  int x0;
  int x1;
  int y0;
  int y1;
  int tableRowCount;
  private slots:
    void toggleEditA();
  void toggleSave();
  void toggleStepOff();
  void toggleStepOn();
  void progControlOpen();
  void calculateInPoint();
  void resizeX();
  void resizeY();
  void incMatrixA();
  void decMatrixA();
  void incMatrixB();
  void decMatrixB();
  void resizeData();
  void takeNewData();
  void saveData();
  void progProceed();
  void progCancel();
  void progSelect(int option);

  private:
    QLineEdit * editSize;
  QPushButton * resize;
  QRadioButton * radioDiscrete;
  QRadioButton * radioContinuity;
  QCheckBox * checkChangeA;
  QCheckBox * checkPrevIter;
  QComboBox * stepList;
  QTableWidget * matrixA;
  QTableWidget * matrixB;
  QTableWidget * matrixC0;
  QTableWidget * matrixX0;
  GLWidget * graphArea;
  QTableWidget * tableArea;
  QLineEdit * editX0;
  QLineEdit * editX1;
  QLineEdit * editY0;
  QLineEdit * editY1;
  QPushButton * decA;
  QPushButton * incA;
  QPushButton * decB;
  QPushButton * incB;
  QLineEdit * editA;
  QLineEdit * editB;
  QLineEdit * editCalc;
  QPushButton * buttonCalc;
  QPushButton * buttonProgControl;
  QPushButton * launch;
  QPushButton * save;
  bool systemType;
  bool isFirstStart;
  bool isFirstStartSave;
  bool canSave;
  int stepMultipler;
  float stepIndex;
  bool isSave;
  bool isChangable;
  bool isUsedFeedback;
  int pharseMatrix(int n, int m, QTableWidget * matrix);
  bool isAllProductive();
  void addNewLine(matrix tmp, matrix tmp_old, int t);
  void getX_st(float t);
  void getX(float t);
  void getX_st_old(float t);
  void getX_old(float t);
  void getC(float t);
  void takeIncK();
  void takeDecK();
  void takeEqvK();
  int takeAttr;
  void saveOldData();
  void resizeSystem(bool clear);
  void calculateDiscreteX(float t);
  void calculateContinuosX(float t);

  QDialog * progDialog;
  QPushButton * progDialogProceed;
  QPushButton * progDialogCancel;
  QComboBox * progDialogSelect;
  QTableWidget * progMatrixC0;
  QTableWidget * progMatrixa0;
  QTableWidget * progMatrixd0;
  QTableWidget * progMatrixa0_1;
  QLineEdit * progOmega;
  QLineEdit * progK;
  QComboBox * progSelectFunc;
  QLabel * progGroupd0;
  matrix C0_prog;
  matrix a0;
  matrix a0_1;
  matrix d0;
  matrix Ct;
  float omega;
  float powK;
  bool isProgControl;
  void progDialogInit();
  void progDialogResize();
  bool progFiNegative();
  int progCurrentDialogSelect;
};

#endif
