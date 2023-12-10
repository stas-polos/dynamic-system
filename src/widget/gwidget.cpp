#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "gwidget.h"
#include "dynamic_system.h"

GLWidget::GLWidget(QWidget * parent): QGLWidget(parent) {
  tX0 = 0;
  tX1 = 5;
  tY0 = 0;
  tY1 = 5;
  isGraphBuild = true;
  isSavedOld = false;
}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL() {
  qglClearColor(QColor(255, 255, 255, 0));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 100, 0, 100, -1, 1);
  qglColor(Qt::white);
}

void GLWidget::paintGL() {
  if (isGraphBuild == true)
    glClear(GL_COLOR_BUFFER_BIT);
  if (isGraphBuild == true) {
    drawInfoLinesX();
    drawInfoLinesY();
  }
  if (isGraphBuild == false) {
    int step = 0; /**/
    for (float i = 0; i <= tX1 - tX0; i += stepIndex) {
      for (int ix = 0; ix < systemSize; ix++) {
        if (stepIndex < 1) {

          if (i == 0) {
            drawPointX(i * 100 / (tX1 - tX0), (pArray[step][ix] - tY0) * 100 / (tY1 - tY0), 1.0, 0.0, 0.0);
          } else {
            drawLineX((i - stepIndex) * 100 / (tX1 - tX0), (pArray[step - 1][ix] - tY0) * 100 / (tY1 - tY0), i * 100 / (tX1 - tX0), (pArray[step][ix] - tY0) * 100 / (tY1 - tY0), 1.0, 0.0, 0.0);
          }

        } else {
          drawPointX(i * 100 / (tX1 - tX0), (pArray[step][ix] - tY0) * 100 / (tY1 - tY0), 1.0, 0.0, 0.0);
        }
        drawPointXst(i * 100 / (tX1 - tX0), (pArray[step][ix + systemSize] - tY0) * 100 / (tY1 - tY0), 1.0, 0.0, 0.0);
      }
      if (isSavedOld == true) {
        for (int ix_old = 0; ix_old < systemSize; ix_old++) {
          if (stepIndex < 1) {
            if (i == 0) {
              drawPointX_old(i * 100 / (tX1 - tX0), (pArray_old[step][ix_old] - tY0) * 100 / (tY1 - tY0), 0.0, 0.0, 1.0);
            } else {
              drawLineX_old((i - stepIndex) * 100 / (tX1 - tX0), (pArray_old[step - 1][ix_old] - tY0) * 100 / (tY1 - tY0), i * 100 / (tX1 - tX0), (pArray_old[step][ix_old] - tY0) * 100 / (tY1 - tY0), 0.0, 0.0, 1.0);
            }

          } else {
            drawPointX_old(i * 100 / (tX1 - tX0), (pArray_old[step][ix_old] - tY0) * 100 / (tY1 - tY0), 0.0, 0.0, 1.0);
          }
          drawPointXst_old(i * 100 / (tX1 - tX0), (pArray_old[step][ix_old + systemSize] - tY0) * 100 / (tY1 - tY0), 0.0, 0.0, 1.0);
        }
      }
      step++;
    }
  }
}

void GLWidget::setLines(int cx0, int cx1, int cy0, int cy1) {
  tX0 = cx0;
  tX1 = cx1;
  tY0 = cy0;
  tY1 = cy1;
  updateGL();
}

void GLWidget::drawInfoLinesX() {
  int fsize = 6;
  for (int i = tX0; i <= tX1; i++) {
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.9, 0.9, 0.9);
    glVertex2i((i - tX0) * 100 / (tX1 - tX0), 0);
    glVertex2i((i - tX0) * 100 / (tX1 - tX0), 100);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2i((i - tX0) * 100 / (tX1 - tX0), 0);
    glVertex2i((i - tX0) * 100 / (tX1 - tX0), 3);
    glEnd();
    QString sTmp;
    QFont fTmp("Tahoma", fsize);
    sTmp.setNum(i, 10);
    renderText((i - tX0) * 100 / (tX1 - tX0) + 1.0, 4.0, 0.0, sTmp, fTmp);
  }
}

void GLWidget::drawInfoLinesY() {
  int fsize = 6;
  for (int i = tY0; i <= tY1; i++) {
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.9, 0.9, 0.9);
    glVertex2i(0, (i - tY0) * 100 / (tY1 - tY0));
    glVertex2i(100, (i - tY0) * 100 / (tY1 - tY0));
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2i(0, (i - tY0) * 100 / (tY1 - tY0));
    glVertex2i(2, (i - tY0) * 100 / (tY1 - tY0));
    glEnd();
    if (i != tY0) {
      QString sTmp;
      QFont fTmp("Tahoma", fsize);
      sTmp.setNum(i, 10);
      renderText(3.0, (i - tY0) * 100 / (tY1 - tY0) + 2.0, 0.0, sTmp, fTmp);
    }
  }
}

void GLWidget::drawPointXst(float x, float y, float R, float G, float B) {

  if (stepIndex < 1) {
    glColor3f(R, G, B);
    glLineWidth(1);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
  } else {
    glColor3f(R, G, B);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(x + 1, y + 2);
    glVertex2i(x - 1, y - 2);
    glVertex2i(x + 1, y - 2);
    glVertex2i(x - 1, y + 2);
    glEnd();
  }
}

void GLWidget::drawPointX(float x, float y, float R, float G, float B) {
  if (stepIndex < 1) {
    glColor3f(R, G, B);
    glLineWidth(1);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
  } else {
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    glVertex2i(x, y + 2);
    glVertex2i(x - 1, y);
    glVertex2i(x, y - 2);
    glVertex2i(x + 1, y);
    glEnd();
  }
}

void GLWidget::drawLineX(float x_old, float y_old, float x, float y, float R, float G, float B) {
  glColor3f(R, G, B);
  glLineWidth(1);
  glBegin(GL_LINES);
  glVertex2f(x_old, y_old);
  glVertex2f(x, y);
  glEnd();
}

void GLWidget::drawLineX_old(float x_old, float y_old, float x, float y, float R, float G, float B) {
  glColor3f(R, G, B);
  glLineWidth(2);
  glBegin(GL_LINES);
  glVertex2f(x_old, y_old);
  glVertex2f(x, y);
  glEnd();
}

void GLWidget::drawPointXst_old(float x, float y, float R, float G, float B) {
  if (stepIndex < 1) {
    glColor3f(R, G, B);
    glLineWidth(2);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
  } else {
    glColor3f(R, G, B);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(x + 1, y);
    glVertex2i(x - 1, y);
    glVertex2i(x, y + 2);
    glVertex2i(x, y - 2);
    glEnd();
  }
}

void GLWidget::drawPointX_old(float x, float y, float R, float G, float B) {
  glColor3f(R, G, B);
  glBegin(GL_POLYGON);
  glVertex2i(x - 0.5, y + 1);
  glVertex2i(x - 0.5, y - 1);
  glVertex2i(x + 0.5, y - 1);
  glVertex2i(x + 0.5, y + 1);
  glEnd();
}
