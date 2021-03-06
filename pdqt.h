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
    BUTTON_SHIFT           = 7,
    BUTTONS
  };

  /*
   * Quadrants as usual:
   *
   *       ^
   *       |
   *    II | I
   *       |
   *  -----+---->
   *       |
   *   III | IV
   *       |
   *
   *  Begin with quadrant, where the point (0, 0) of the screen is.
   *  Enumerate counting clockwise.
   */
  enum Quadrant
  {
    SECOND_QUADRANT,
    FIRST_QUADRANT,
    FOURTH_QUADRANT,
    THIRD_QUADRANT
  };

  class ScreenProperties
  {
  public:
    virtual int getScreenWidth() const = 0;
    virtual int getScreenHeight() const = 0;
    virtual float getScreenMultiplier() const = 0;
    virtual QBrush& getBlackBrush() = 0;
  };

  namespace widget
  {
    namespace properties
    {
      class Named
      {
      public:
        /** Get name of the widget. */
        virtual QString name(void) const = 0;
      };

      class Base : public virtual Named
      {
      public:
        virtual ~Base() {}
        QString name(void) const { return name_; }
      protected:
        QString name_;
        int x;
        int y;
      };

      class Geometric : public Base
      {
      public:
      protected:
        float value;
        int min;
        int max;
        int width;
        int height;
      };

      class Textual : public Base
      {
      public:
        void setValue(float f);
      };

      class Bang : public Geometric
      {
      public:
      };

      class Slider : public Geometric
      {
      public:
        void setValue(float f);
      protected:
        int position;
        static const unsigned int IndicatorThickness = 2;
      };

      class Radio : public Geometric
      {
      public:
        void setValue(float f);
      protected:
        unsigned int buttons;
        static const unsigned int MarkMargin = 2;
      };
    }

    class Widget : public virtual properties::Named
    {
    public:
      /** Set value of the widget. */
      virtual void setValue(float f) = 0;
      /** Paint the widget. */
      virtual void paint(QPainter&) = 0;
    };

    class Bang : public Widget, public properties::Bang
    {
    public:
      Bang(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f);
      void paint(QPainter&);
    private:
      QBrush backupBrush;
      QBrush blackBrush;
    };

    class HorizontalSlider : public Widget, public properties::Slider
    {
    public:
      HorizontalSlider(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f) { properties::Slider::setValue(f); }
      void paint(QPainter&);
    private:
      QBrush blackBrush;
    };

    class VerticalSlider : public Widget, public properties::Slider
    {
    public:
      VerticalSlider(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f) { properties::Slider::setValue(f); }
      void paint(QPainter&);
    private:
      QBrush blackBrush;
    };

    class HorizontalRadio : public Widget, public properties::Radio
    {
    public:
      HorizontalRadio(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f) { properties::Radio::setValue(f); }
      void paint(QPainter&);
    private:
      QBrush blackBrush;
    };

    class VerticalRadio : public Widget, public properties::Radio
    {
    public:
      VerticalRadio(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f) { properties::Radio::setValue(f); }
      void paint(QPainter&);
    private:
      QBrush blackBrush;
    };

    class Number : public Widget, public properties::Geometric
    {
    public:
      Number(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f);
      void paint(QPainter&);
    private:
      QPointArray contour;
      QString sv;
    };

    class Text : public Widget, public properties::Textual
    {
    public:
      Text(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f) { properties::Textual::setValue(f); }
      void paint(QPainter&);
    private:
      QString text;
      unsigned int textLength;
    };

    class Symbol : public Widget, public properties::Base
    {
    public:
      Symbol(QStringList& parameters, ScreenProperties* sp);
      QString name(void) const { return properties::Base::name(); }
      void setValue(float f);
      void paint(QPainter& p);
    };
  }


  class MessageSender
  {
  public:
    virtual void sendMessage(const char*) = 0;
  };

  class Main : public MessageSender
  {
  public:
    virtual bool patchLoaded() const = 0;
    virtual bool pdRunning() const = 0;
    virtual void pdPause(bool pause) = 0;
    virtual bool pdPaused() const = 0;
    virtual void setStatus(const char*) = 0;
    virtual bool isStandardView() const = 0;
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
    void press(bool buttonPressed = true) { pressed_ = buttonPressed; }
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
    virtual void pressKey(const int key) = 0;
    virtual void unpressKey(const int key) = 0;
    virtual bool knownKey(const int key) = 0;
    virtual bool anyButtonPressed() = 0;
    virtual bool buttonPressed(enum ButtonID button) = 0;
    virtual int  wheelValue() = 0;
    virtual void addToWheel(const int i) = 0;
  };

  class SweetAppleController : virtual public Controller
  {
  public:
    SweetAppleController(Main*);
    Button& getButton(enum ButtonID id);
    void pressKey(const int key);
    void unpressKey(const int key);
    bool knownKey(const int key);
    bool anyButtonPressed();
    bool buttonPressed(enum ButtonID button);
    int  wheelValue();
    void addToWheel(const int i);
  private:
    bool shift;
    Button buttons[BUTTONS];
    ScrollWheel wheel;
    Main* main;
  };

  class TurningGestureRecognizer
  {
  public:
    TurningGestureRecognizer();
    void setController(Controller* c) { controller = c; }
    void setScreenProperties(ScreenProperties* sp) { screenProperties = sp; }
    bool processNextCoordinates(const int x, const int y);
  private:
    enum Quadrant previousQuadrant;
    Controller* controller;
    ScreenProperties* screenProperties;
  };


  class View
  {
  public:
    virtual ~View() {}
    virtual void repaint(QPainter& p) = 0;
  protected:
    int width;
    int height;
  };

  class StandardView : virtual public View
  {
  public:
    StandardView(Controller* controller_, int width_, int height_);
    virtual ~StandardView() {}
    void repaint(QPainter& p);
  private:
    Controller* controller;
  };

  class CustomView : virtual public View
  {
  public:
    CustomView(QValueList<widget::Widget*>*);
    virtual ~CustomView() {}
    void repaint(QPainter&);
  private:
    QValueList<widget::Widget*>* widgets;
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


  class PDQt : public QMainWindow, Main, ScreenProperties
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
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
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
    // Screen properties
  public:
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    float getScreenMultiplier() const { return screenMultiplier; }
    QBrush& getBlackBrush() { return blackBrush; }
  private:
    int screenWidth;
    int screenHeight;
    float screenMultiplier;
    QBrush blackBrush;
    //
    QLabel* status;
    QPixmap paintPixmap;
    //
    QValueList<widget::Widget*> widgets;
    widget::Widget* createWidget(QString& line);
    View* view;
    Controller* controller;
    TurningGestureRecognizer turningGestureRecognizer;
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
    QFont font;
    //
    bool loaded; // Is a patch loaded?
    bool running; // Is PD running?
    bool connected; // Is GUI connected to PD?
    bool paused; // Is PD paused?
    //
    QString configFilename;
    Config* config;
  };
}

#endif
