/**
  pdqt.h

  Qtopia interface for Pure Data -- header file.
*/

#ifndef _PDQT_H
#define _PDQT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef QTOPIA
  #include <qpe/qpeapplication.h>
  #include <qpe/config.h>
  #include "qtkfiledialog.h"
#else
  #include <qapplication.h>
  #include "config.h"
#endif

#include <qmainwindow.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qvaluelist.h>
#include <qmessagebox.h>
#include <qhostaddress.h>
#include <qsocketdevice.h>
#include <qsocketnotifier.h>
#include <qdialog.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qtimer.h>


enum pdWidgetType
{
  PD_BANG    = 0,
  PD_VSLIDER = 1,
  PD_HSLIDER = 2,
  PD_VRADIO  = 3,
  PD_HRADIO  = 4,
  PD_NUMBER  = 5,
  PD_SYMBOL  = 6,
  PD_TEXT    = 7
};

class PDWidget
{
  friend class PDQt;
public:
  PDWidget(enum pdWidgetType type_ = PD_TEXT,
           char* name_ = "",
	   int x_ = 0,
	   int y_ = 0,
	   int w_ = 0,
	   int h_ = 0,
	   int min_ = 0,
	   int max_ = 0,
           float value_ = 0) :
	   type(type_),
	   x(x_),
	   y(y_),
	   w(w_),
	   h(h_),
	   min(min_),
	   max(max_),
	   value(value_)
	   {
	     strncpy(name, name_, 128-1);
	   }
private:
  enum pdWidgetType type;
  char name[128];
  int x;
  int y;
  int w;
  int h;
  int min;
  int max;
  float value;
};

class PDPodButton
{
  friend class PDQt;
  PDPodButton() : pressed(false) {}
  PDPodButton(int key_) : key(key_), pressed(false) {}
  int key;
  bool pressed;
};

class PDQt : public QMainWindow
{
  Q_OBJECT
public:
  PDQt(QWidget* parent = 0, const char* name = 0);
  virtual ~PDQt();
public slots:
  void load();
  void load(const char* fileName);
  void about();
protected:
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
  void resizeEvent(QResizeEvent *);
  void paintEvent(QPaintEvent*);
  void closeEvent(QCloseEvent*);
private slots:
  void buttonActionBackpress();
  void receiveMessage();
private:
  void startPD();
  void connectPD();
  void disconnectPD();
  void stopPD();
  void sendMessage(const char*);
  //
  int screenWidth;
  int screenHeight;
  float screenMultiplier;
  QLabel* status;
  QPixmap paintPixmap;
  //
  QValueList<PDWidget> widgets;
  QString patch;
  //
  pid_t pdPid; // Process ID of the forked PD starter
  QSocketDevice* pdTx;
  QSocketDevice* pdRx;
  QSocketNotifier* pdReadNotifier;
  //
  bool loaded; // Is a patch loaded?
  bool running;	// Is PD running?
  bool connected; // Is GUI connected to PD?
  bool paused; // Is PD paused?
  bool shift; // Shift key emulation
  //
  PDPodButton buttonMenu;
  PDPodButton buttonPlay;
  PDPodButton buttonForward;
  PDPodButton buttonRewind;
  PDPodButton buttonAction;
  PDPodButton wheelClockwise;
  PDPodButton wheelCounterclockwise;
  int scrollValue; // Emulation of the scroll wheel
  //
  QFont font;
  QFontMetrics* fm;
  //
  QString pdPath;
  QString patchDirectory;
  Config* config;
};

#endif

