/***
  pdqt.h

  Qtopia interface for Pure Data -- header file.
*/

#ifndef _PDQT_H
#define _PDQT_H

#include <csignal>

#include <qmainwindow.h>
#include <qcanvas.h>
#include <qvector.h>
#include <qstatusbar.h>

#include <qpe/qpeapplication.h>

#define PDQTNAME "Pure Data for Qtopia"

#define PD_BANG 0
#define PD_VSLIDER 1
#define PD_HSLIDER 2
#define PD_VRADIO 3
#define PD_HRADIO 4
#define PD_NUMBER 5
#define PD_SYMBOL 6
#define PD_TEXT 7

class PDWidget
{
public:
  int type;
  char name[128];
  int x;
  int y;
  int w;
  int h;
  int min;
  int max;
  float value;
};

class PDQt : public QMainWindow
{
  Q_OBJECT
public:
  PDQt();
  ~PDQt();
protected:
  void closeEvent(QCloseEvent*);
private:
  QCanvas* canvas;
  QCanvasView* canvas_view;
  QVector<PDWidget>* pd_widgets;
};

#endif

