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
  enum BUTTONID
  {
    BUTTON_PLAY            = 0,
    BUTTON_MENU            = 1,
    BUTTON_ACTION          = 2,
    BUTTON_REWIND          = 3,
    BUTTON_FORWARD         = 4,
    WHEEL_CLOCKWISE        = 5,
    WHEEL_COUNTERCLOCKWISE = 6,
    BUTTONS
  };

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


  class ScrollWheel
  {
  public:
    ScrollWheel(int value = 0) : value_(value) {}
    void scrollUp(unsigned int steps = 1) { value_ += steps; }
    void scrollDown(unsigned int steps = 1) { value_ -= steps; }
    int value() const { return value_; }
    inline ScrollWheel& operator += (const int i) { value_ += i; return *this; }
    inline ScrollWheel& operator -= (const int i) { value_ -= i; return *this; }
  private:
    int value_;
  };

  class MessageSender
  {
  public:
    virtual void sendMessage(const char*) = 0;
  };

  class Main : virtual public MessageSender
  {
  public:
    virtual bool patchLoaded() const = 0;
    virtual bool pdRunning() const = 0;
    virtual void pdPause(bool pause) = 0;
    virtual bool pdPaused() const = 0;
    virtual void setStatus(const char*) = 0;
    virtual bool isStandardView() const = 0;
  };

  class Button
  {
  public:
    Button() : key_(0), pressed_(false) {}
    Button(int key) : key_(key), pressed_(false) {}
    Button(const Button& b) : key_(b.key_), pressed_(b.pressed_) {}
    int key() const { return key_; }
    void press(bool buttonpressed = true) { pressed_ = buttonpressed; }
    bool pressed() const { return pressed_; }
    inline bool operator == (const int key) { return (key_ == key); }
  private:
    int key_;
    bool pressed_;
  };

  class Controller
  {
  public:
    virtual Button& getButton(enum BUTTONID id) = 0;
    virtual bool pressKey(int key) = 0;
    virtual bool unpressKey(int key) = 0;
    virtual bool anyButtonPressed() = 0;
    virtual bool buttonPressed(enum BUTTONID button) = 0;
    virtual int  wheelValue() = 0;
  };

  class SourAppleController : virtual public Controller
  {
  public:
    SourAppleController(Main*);
    Button& getButton(enum BUTTONID id);
    bool pressKey(int key);
    bool unpressKey(int key);
    bool anyButtonPressed();
    bool buttonPressed(enum BUTTONID button);
    int  wheelValue();
  private:
    bool shift;
    Button buttons[BUTTONS];
    ScrollWheel wheel;
    Main* main;
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

  class GeometricWidget : public BaseWidget
  {
  public:
    GeometricWidget() { blackBrush = QBrush(Qt::black); }
    virtual ~GeometricWidget() {}
    virtual enum WIDGETID getId() { return id; }
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

  class TextualWidget : public BaseWidget
  {
  public:
    TextualWidget() {}
    virtual ~TextualWidget() {}
    virtual enum WIDGETID getId() { return id; }
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

  class SliderWidget : public GeometricWidget
  {
  protected:
    int position;
  };

  class RadioWidget : public GeometricWidget
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
    virtual ~View() {}
    virtual void repaint(QPainter& p) = 0;
  protected:
    int width;
    int height;
    QFont* font;
    QFontMetrics* fm;
  };

  class StandardView : virtual public View
  {
  public:
    StandardView(Controller* controller_, int width_, int height_, QFont* font_, QFontMetrics* fm_);
    virtual ~StandardView() {}
    void repaint(QPainter& p);
  private:
    Controller* controller;
  };

  class CustomView : virtual public View
  {
  public:
    CustomView(QValueList<BaseWidget*>*);
    virtual ~CustomView() {}
    void repaint(QPainter&);
  private:
    QValueList<BaseWidget*>* widgets;
  };

  class PDQt : public QMainWindow, Main
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
    void resizeEvent(QResizeEvent*);
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
    //
    void sendMessage(const char*);
    bool patchLoaded() const;
    bool pdRunning() const;
    void pdPause(bool pause);
    bool pdPaused() const;
    void setStatus(const char*);
    bool isStandardView() const;
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
    QFont font;
    QFontMetrics* fm;
    //
    QString pdPath;
    QString patchDirectory;
    Config* config;
  };
}

#endif
