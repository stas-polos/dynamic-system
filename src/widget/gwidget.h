#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget: public QGLWidget {
  Q_OBJECT

  public:
    GLWidget(QWidget * parent = 0);
  ~GLWidget();
  int tX0;
  int tX1;
  int tY0;
  int tY1;
  int systemSize;
  float stepIndex;
  float ** pArray;
  float ** pArray_old;
  bool isGraphBuild;
  bool isSavedOld;
  void setLines(int cx0, int cx1, int cy0, int cy1);
  private:
    int curT;
  void drawInfoLinesX();
  void drawInfoLinesY();
  void drawPointXst(float x, float y, float R, float G, float B);
  void drawPointX(float x, float y, float R, float G, float B);
  void drawLineX(float x_old, float y_old, float x, float y, float R, float G, float B);
  void drawPointXst_old(float x, float y, float R, float G, float B);
  void drawPointX_old(float x, float y, float R, float G, float B);
  void drawLineX_old(float x_old, float y_old, float x, float y, float R, float G, float B);
  protected:
    void initializeGL();
  void paintGL();

};

#endif // GLWIDGET_H
