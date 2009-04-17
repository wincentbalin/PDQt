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
  #include <qfiledialog.h>
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

namespace pdqt
{
  enum WIDGETID
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

  enum BUTTONID
  {
    BUTTON_PLAY            = 0,
    BUTTON_MENU            = 1,
    BUTTON_ACTION          = 2,
    BUTTON_REWIND          = 3,
    BUTTON_FORWARD         = 4,
    WHEEL_CLOCKWISE        = 5,
    WHEEL_COUNTERCLOCKWISE = 6
  };

  // C-like data

  class PDPodButton
  {
    friend class PDQt;
    PDPodButton() : pressed(false) {}
    PDPodButton(int key_) : key(key_), pressed(false) {}
    int key;
    bool pressed;
  };

  // C++-like data

  class ScrollWheel
  {
  public:
    ScrollWheel(int value_ = 0);
    void scrollUp(unsigned int steps = 1);
    void scrollDown(unsigned int steps = 1);
    int getValue();
  private:
    int value;
  };

  class MessageSender
  {
  public:
    virtual void sendMessage(const char*) = 0;
  };

  class Main
  {
  public:
    virtual bool patchLoaded() = 0;
    virtual bool pdRunning() = 0;
    virtual bool pdPaused() = 0;
    virtual void setStatus(const char*) = 0;
  };

  class Button
  {
  public:
    Button(int key);
    void press(bool buttonpressed);
    bool pressed();
  private:
    int key_;
    bool pressed_;
  };

  class Controller
  {
  public:
    virtual void pressKey(int key) = 0;
    virtual void unpressKey(int key) = 0;
  };

  class SourAppleController : virtual public Controller
  {
  public:
    SourAppleController(MessageSender&);
  private:
    ScrollWheel wheel;
  };

  class BaseWidget
  {
  public:
    virtual ~BaseWidget() {}
    virtual enum WIDGETID getId() = 0;
    virtual void paint(QPainter&) = 0;
    virtual QString& getName()    = 0;
    virtual int minValue()        = 0;
    virtual int maxValue()        = 0;
    virtual float getValue()      = 0;
    virtual void setValue(float)  = 0;
  protected:
    enum WIDGETID id;
    int x;
    int y;
    QString name;
  };

  class GeometricWidget : virtual public BaseWidget
  {
  public:
    GeometricWidget() { blackBrush = QBrush("black"); }
    virtual ~GeometricWidget() {}
    virtual enum WIDGETID getId() { return id; }
    virtual void paint(QPainter&) {}
    virtual QString& getName() { return name; }
    virtual int minValue() { return min; }
    virtual int maxValue() { return max; }
    virtual float getValue() { return value; }
    virtual void setValue(float value_) { value = value_; }
  protected:
    int w;
    int h;
    int min;
    int max;
    float value;
    bool selected;
    QBrush blackBrush;
  };

  class TextualWidget : virtual public BaseWidget
  {
  public:
    TextualWidget() {}
    virtual ~TextualWidget() {}
    virtual enum WIDGETID getId() { return id; }
    virtual void paint(QPainter&) {}
    virtual QString& getName() { return name; }
    // Placeholders
    virtual int minValue() { return 0; }
    virtual int maxValue() { return 0; }
    virtual float getValue() { return 0; }
    virtual void setValue(float) {}
  protected:
    QFont font;
    QFontMetrics* fm;
  };

  class SliderWidget : virtual public GeometricWidget
  {
  protected:
    int position;
  };

  class RadioWidget : virtual public GeometricWidget
  {
  protected:
    unsigned int radioButtons;
  };

  class BangWidget : public GeometricWidget
  {
  public:
    BangWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter& p);
  };

  class HorizontalSliderWidget : public SliderWidget
  {
  public:
    HorizontalSliderWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter& p);
  };

  class VerticalSliderWidget : public SliderWidget
  {
  public:
    VerticalSliderWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter& p);
  };

  class HorizontalRadioWidget : public RadioWidget
  {
  public:
    HorizontalRadioWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter& p);
  };

  class VerticalRadioWidget : public RadioWidget
  {
  public:
    VerticalRadioWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter& p);
  };

  class NumberWidget : public GeometricWidget
  {
  public:
    NumberWidget(QStringList& parameters, float scale, QFont& widgetFont, QFontMetrics* widgetFontMetrics);
    virtual void paint(QPainter& p);
  protected:
    QFont font;
    QFontMetrics* fm;
  private:
    QPointArray contour;
    QString sv; // string value
  };

  class SymbolWidget : public TextualWidget
  {
  public:
    SymbolWidget(QStringList& parameters, float scale);
    virtual void paint(QPainter&);
  };

  class TextWidget : public TextualWidget
  {
  public:
    TextWidget(QStringList& parameters, float scale, QFont& widgetFont, QFontMetrics* widgetFontMetrics);
    virtual void paint(QPainter& p);
  private:
    QString text;
  };

  class View
  {
  public:
    virtual void repaint(QPainter& p) = 0;
  };

  class StandardView : virtual public View
  {
  public:
    void repaint(QPainter& p);
  };

  class CustomView : virtual public View
  {
  public:
    CustomView(QValueList<BaseWidget*>*);
    void repaint(QPainter&);
  private:
    QValueList<BaseWidget*>* widgets;
  };

  class PDQt : public QMainWindow, MessageSender
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
    QValueList<BaseWidget*> widgets;
    void createWidget(QString& line);
    View* view;
    Controller* controller;
    //
    QString patch;
    //
    pid_t pdPid; // Process ID of the forked PD starter
    QSocketDevice* pdTx;
    QSocketDevice* pdRx;
    QSocketNotifier* pdReadNotifier;
    //
    bool loaded; // Is a patch loaded?
    bool running; // Is PD running?
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
}

#endif

