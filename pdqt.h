/**
  pdqt.h

  Qtopia interface for Pure Data -- header file.
*/

#ifndef _PDQT_H
#define _PDQT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#define UNIX 1
#endif

#ifdef UNIX
  #include <unistd.h>
  #include <signal.h>
  #include <sys/types.h>
  #include <sys/wait.h>
#endif

#ifdef QTOPIA
  #include <qpe/qpeapplication.h>
  #include "qtkfiledialog.h"
#else
  #include <qapplication.h>
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
#include <qlayout.h>
#include <qhgroupbox.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qscrollview.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qtextview.h>
#include <qtextbrowser.h>


namespace pdqt
{
  enum ButtonID
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

  enum WidgetID
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


  class WidgetProperties
  {
  public:
    WidgetProperties() {}
    int getX() { return x; }
    void setX(int x_) { x = x_; }
    int getY() { return y; }
    void setY(int y_) { y = y_; }
    QString& getName() { return name; }
    void setName(QString& name_) { name = name_; }
  private:
    int x;
    int y;
    QString name;
    //
    QPointArray contour;
    QString sv; // string value
    //
    QString text;
  };

  class GeometricWidgetProperties : public WidgetProperties
  {
  public:
    GeometricWidgetProperties() {}
    void setWidth(int width) { w = width; }
    int getWidth() { return w; }
    void setHeight(int height) { h = height; }
    int getHeight() { return h; }
    void setMinValue(int v) { min = v; }
    int getMinValue() { return min; }
    void setMaxValue(int v) { max = v; }
    int getMaxValue() { return max; }
    void setValue(float v) { value = v; }
    float getValue() { return value; }
  private:
    int w;
    int h;
    int min;
    int max;
    float value;
  };

  class GraphicProperties
  {
  public:
    GraphicProperties() { blackBrush = QBrush(Qt::black); }
    QBrush getBlackBrush() { return blackBrush; }
    void setFont(QFont* f) { font = f; }
    QFont* getFont() { return font; }
    void setFontMetrics(QFontMetrics* fm) { fontMetrics = fm; }
    QFontMetrics* getFontMetrics() { return fontMetrics; }
  private:
    QBrush blackBrush;
    //
    QFont* font;
    QFontMetrics* fontMetrics;
  };

  class Widget
  {
  public:
    virtual ~Widget() {}
    virtual enum WidgetID getId() = 0;
    virtual void paint(QPainter&) = 0;
    virtual QString& getName()    = 0;
    virtual int minValue()        = 0;
    virtual int maxValue()        = 0;
    virtual float getValue()      = 0;
    virtual void setValue(float)  = 0;
  protected:
    enum WidgetID id;
    WidgetProperties properties;
    int x;
    int y;
    QString name;
  };

  class GeometricWidget : public Widget
  {
  public:
    GeometricWidget() { blackBrush = QBrush(Qt::black); }
    virtual ~GeometricWidget() {}
    virtual enum WidgetID getId() { return id; }
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

  class TextualWidget : public Widget
  {
  public:
    TextualWidget() {}
    virtual ~TextualWidget() {}
    virtual enum WidgetID getId() { return id; }
    virtual QString& getName() { return name; }
    // Place holders
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
    virtual Button& getButton(enum ButtonID id) = 0;
    virtual bool pressKey(int key) = 0;
    virtual bool unpressKey(int key) = 0;
    virtual bool anyButtonPressed() = 0;
    virtual bool buttonPressed(enum ButtonID button) = 0;
    virtual int  wheelValue() = 0;
  };

  class Main;
  class SourAppleController : virtual public Controller
  {
  public:
    SourAppleController(Main*);
    Button& getButton(enum ButtonID id);
    bool pressKey(int key);
    bool unpressKey(int key);
    bool anyButtonPressed();
    bool buttonPressed(enum ButtonID button);
    int  wheelValue();
  private:
    bool shift;
    Button buttons[BUTTONS];
    ScrollWheel wheel;
    Main* main;
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
    CustomView(QValueList<Widget*>*);
    virtual ~CustomView() {}
    void repaint(QPainter&);
  private:
    QValueList<Widget*>* widgets;
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


  class Config
  {
  public:
    Config(const char* filename_, bool check_only_ = false);
    ~Config();
    //
    void copyFrom(Config& otherConfig);
    QString pdPath() const { return pdPath_; }
    void setPDPath(QString path) { pdPath_ = path; }
    bool pdStart() const { return pdStart_; }
    void setPDStart(bool state) { pdStart_ = state; }
    QString patchDirectory() const { return patchDirectory_; }
    void setPatchDirectory(QString dir) { patchDirectory_ = dir; }
  private:
    void read();
    void write();
    QString filename;
    bool check_only;
    //
    QString pdPath_;
    bool pdStart_;
    QString patchDirectory_;
  };

  class ConfigDialog : public QDialog
  {
    Q_OBJECT
  public:
    ConfigDialog(Config* currentConfig, QString filename,
                 QWidget *parent = 0, const char *name = 0);
    ~ConfigDialog() { delete config; }
    Config& getConfig() { return *config; }
  private slots:
    void setPDPath(const QString& path);
    void choosePDPath();
    void defaultPDPath();
    void setPDStart(bool state);
  private:
    Config* config;
    QLineEdit* pdPathView;
    QCheckBox* pdStartView;
  };


  class HelpDialog : public QDialog
  {
    Q_OBJECT
  public:
    HelpDialog(QWidget* parent = 0, const char* name = 0, WFlags f = 0);
  };

  class AboutDialog : public QDialog
  {
    Q_OBJECT
  public:
    AboutDialog(QWidget* parent = 0, const char* name = 0, WFlags f = 0);
  };


  class PDQt : public QMainWindow, Main
  {
    Q_OBJECT
  public:
    PDQt(QWidget* parent = 0, const char* name = 0);
    virtual ~PDQt();
  public slots:
    void load();
    void load(const char* filename);
    void configure();
    void help();
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
    QValueList<Widget*> widgets;
    void createWidget(QString& line);
    View* view;
    Controller* controller;
    //
    QString patch;
    //
#ifdef UNIX
    pid_t pdPid; // Process ID of the forked PD starter
#endif
    QSocketDevice* pdTx;
    QSocketDevice* pdRx;
    QSocketNotifier* pdReadNotifier;
    //
    bool loaded; // Is a patch loaded?
    bool running; // Is PD running?
    bool connected; // Is GUI connected to PD?
    bool paused; // Is PD paused?
    //
    QFont font;
    QFontMetrics* fm;
    //
    QString configFilename;
    Config* config;
  };
}

#endif
