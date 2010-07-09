/**
  pdqt.cpp

  Qtopia interface for Pure Data -- main file.
*/

#include "pdqt.h"

using namespace pdqt;

#define PDQTNAME "Pure Data for Qtopia"

#ifndef PD_COMMAND
#define PD_COMMAND "/opt/QtPalmtop/bin/pd"
#endif

#ifndef PATCH_DIRECTORY
#define PATCH_DIRECTORY "/opt/QtPalmtop/share/pdqt"
#endif

#ifndef CONFIG_FILE
#define CONFIG_FILE ".pdqtrc"
#endif

#define DPRINTF(x...) fprintf(stderr, x);


/** Create SourAppleController. */
SourAppleController::SourAppleController(Main* main_)
{
  // Save message sender
  main = main_;

  // No shift when started
  shift = false;

  // Initialize buttons
  buttons[BUTTON_PLAY]     = Button(Qt::Key_C);
  buttons[BUTTON_MENU]     = Button(Qt::Key_R);
  buttons[BUTTON_ACTION]   = Button(Qt::Key_D);
  buttons[BUTTON_REWIND]   = Button(Qt::Key_S);
  buttons[BUTTON_FORWARD]  = Button(Qt::Key_F);
  buttons[WHEEL_CLOCKWISE] = Button(Qt::Key_E);
  buttons[WHEEL_COUNTERCLOCKWISE] = Button(Qt::Key_X);
}

/** Get a button. */
Button& SourAppleController::getButton(enum ButtonID id)
{
  return buttons[id];
}

/** Press a key. */
bool SourAppleController::pressKey(int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("m 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_MENU].press();
    }
    return true;
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("b;\n");
      if(main->isStandardView())
        buttons[BUTTON_ACTION].press();
    }
    if(main->pdRunning())
      shift = true;
    return true;
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("w 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_REWIND].press();
    }
    return true;
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("f 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_FORWARD].press();
    }
    return true;
  }
  else if(buttons[WHEEL_COUNTERCLOCKWISE] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      if(shift) { wheel -= 10; main->sendMessage("l 10;\n"); }
      else      { wheel -=  1; main->sendMessage("l 1;\n");  }
    }
    return true;
  }
  else if(buttons[WHEEL_CLOCKWISE] == key)
  {
    buttons[WHEEL_CLOCKWISE].press();
    if(main->pdRunning() && ! main->pdPaused())
    {
      if(shift) { wheel += 10; main->sendMessage("r 10;\n"); }
      else      { wheel +=  1; main->sendMessage("r 1;\n");  }
    }
    return true;
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning())
    {
      if(shift)
      {
        main->pdPause( ! main->pdPaused());

        if(main->pdPaused()) 
        {
          main->setStatus("Paused");
          main->sendMessage("p 0;\n");
        }
        else
        {
          main->setStatus("Running patch");
          main->sendMessage("p 1;\n");
        }
      }
      else if( ! main->pdPaused())
      {
        main->sendMessage("d 1;\n");
        if(main->isStandardView())
          buttons[BUTTON_PLAY].press();
      }
    }
    return true;
  }

  return false;
}

/** Release a key. */
bool SourAppleController::unpressKey(int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && ! main->pdPaused() && ! shift)
    {
      main->sendMessage("m 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_MENU].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning())
      shift = false;
    return true;
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("w 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_REWIND].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("f 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_FORWARD].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning() && ! main->pdPaused() && ! shift)
    {
      main->sendMessage("d 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_PLAY].press(false);
    }
    return true;
  }

  return false;
}

/** Is any button pressed? */
bool SourAppleController::anyButtonPressed()
{
  // If any button pressed, return true
  for(unsigned int i = 0; i < BUTTONS; i++)
    if(buttons[i].pressed())
      return true;

  // No button pressed
  return false;
}

/** Is a button pressed? */
bool SourAppleController::buttonPressed(enum ButtonID button)
{
  return buttons[button].pressed();
}

/** Value of the wheel. */
int SourAppleController::wheelValue()
{
  return wheel.value();
}

SweetSourAppleController::SweetSourAppleController(Main* main_)
{
  // Store main interface
  main = main_;

  // No shift on start
  shift = false;

  // Initialize buttons
  buttons[BUTTON_PLAY]     = Button(Qt::Key_Space);
  buttons[BUTTON_MENU]     = Button(Qt::Key_Menu);
  buttons[BUTTON_ACTION]   = Button(Qt::Key_Shift);
  buttons[BUTTON_REWIND]   = Button(Qt::Key_Left);
  buttons[BUTTON_FORWARD]  = Button(Qt::Key_Right);
  buttons[WHEEL_CLOCKWISE] = Button(Qt::Key_Up);
  buttons[WHEEL_COUNTERCLOCKWISE] = Button(Qt::Key_Down);
}

/** Get a button. */
Button& SweetSourAppleController::getButton(enum ButtonID id)
{
  return buttons[id];
}

/** Press a key. */
bool SweetSourAppleController::pressKey(int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("m 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_MENU].press();
    }
    return true;
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("b;\n");
      if(main->isStandardView())
        buttons[BUTTON_ACTION].press();
    }
    if(main->pdRunning())
      shift = true;
    return true;
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("w 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_REWIND].press();
    }
    return true;
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("f 1;\n");
      if(main->isStandardView())
        buttons[BUTTON_FORWARD].press();
    }
    return true;
  }
  else if(buttons[WHEEL_COUNTERCLOCKWISE] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      if(shift) { wheel -= 10; main->sendMessage("l 10;\n"); }
      else      { wheel -=  1; main->sendMessage("l 1;\n");  }
    }
    return true;
  }
  else if(buttons[WHEEL_CLOCKWISE] == key)
  {
    buttons[WHEEL_CLOCKWISE].press();
    if(main->pdRunning() && ! main->pdPaused())
    {
      if(shift) { wheel += 10; main->sendMessage("r 10;\n"); }
      else      { wheel +=  1; main->sendMessage("r 1;\n");  }
    }
    return true;
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning())
    {
      if(shift)
      {
        main->pdPause( ! main->pdPaused());

        if(main->pdPaused())
        {
          main->setStatus("Paused");
          main->sendMessage("p 0;\n");
        }
        else
        {
          main->setStatus("Running patch");
          main->sendMessage("p 1;\n");
        }
      }
      else if( ! main->pdPaused())
      {
        main->sendMessage("d 1;\n");
        if(main->isStandardView())
          buttons[BUTTON_PLAY].press();
      }
    }
    return true;
  }

  return false;
}

/** Release a key. */
bool SweetSourAppleController::unpressKey(int key)
{
  // Elaborate control logic
  if(buttons[BUTTON_MENU] == key)
  {
    if(main->pdRunning() && ! main->pdPaused() && ! shift)
    {
      main->sendMessage("m 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_MENU].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_ACTION] == key)
  {
    if(main->pdRunning())
      shift = false;
    return true;
  }
  else if(buttons[BUTTON_REWIND] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("w 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_REWIND].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_FORWARD] == key)
  {
    if(main->pdRunning() && ! main->pdPaused())
    {
      main->sendMessage("f 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_FORWARD].press(false);
    }
    return true;
  }
  else if(buttons[BUTTON_PLAY] == key)
  {
    if(main->pdRunning() && ! main->pdPaused() && ! shift)
    {
      main->sendMessage("d 0;\n");
      if(main->isStandardView())
        buttons[BUTTON_PLAY].press(false);
    }
    return true;
  }

  return false;
}

/** Is any button pressed? */
bool SweetSourAppleController::anyButtonPressed()
{
  // If any button pressed, return true
  for(unsigned int i = 0; i < BUTTONS; i++)
    if(buttons[i].pressed())
      return true;

  // No button pressed
  return false;
}

/** Is a button pressed? */
bool SweetSourAppleController::buttonPressed(enum ButtonID button)
{
  return buttons[button].pressed();
}

/** Value of the wheel. */
int SweetSourAppleController::wheelValue()
{
  return wheel.value();
}


widget::Bang::Bang(QStringList& parameters)
{
  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  width = (int) (parameters[5].toInt() * scale);
  height = width;

  // Set minimal and maximal values (i.e. false and true)
  min = 0;
  max = 1;

  // Set name
  name_ = parameters[10];

  // Set black brush
  GraphicProperties gp = GraphicProperties::getInstance();
  blackBrush = gp.getBlackBrush();
}

void widget::Bang::paint(QPainter& p)
{
  // Draw contour
  p.drawRect(x, y, width, height);

  // If selected, backup current brush and set solid black one
  if(value == 1)
  {
    backupBrush = p.brush();
    p.setBrush(blackBrush);
  }

  // Draw ellipse, filled if selected
  p.drawEllipse(x, y, width, height);

  // If widget selected, reset it and restore brush
  if(value == 1)
  {
    value = 0;
    p.setBrush(backupBrush);
  }
}

void widget::Bang::setValue(float f)
{
  (void) f;
  value = 1;
}

void widget::properties::Slider::setValue(float f)
{
  // Restrict range of values
  if(f > max)
  {
    f = max;
  }
  else if(f < min)
  {
    f = min;
  }

  // Set new value
  value = f;

  // Calculate position of the slider
  position = (int) ((float) width / max - min) * (int) (max - value);
}

widget::HorizontalSlider::HorizontalSlider(QStringList& parameters)
{
  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  width = (int) (parameters[5].toInt() * scale);
  height = (int) (parameters[6].toInt() * scale);

  // Set minimal and maximal values
  min = parameters[7].toInt();
  max = parameters[8].toInt();

  // Set name
  name_ = parameters[12];

  // Set black brush
  GraphicProperties gp = GraphicProperties::getInstance();
  blackBrush = gp.getBlackBrush();
}

void widget::HorizontalSlider::paint(QPainter& p)
{
  // Draw field
  p.drawRect(x, y, width, height);

  // Draw indicator
  p.fillRect(x + width - position, y, IndicatorThickness, height, blackBrush);
}

widget::VerticalSlider::VerticalSlider(QStringList& parameters)
{
  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  width = (int) (parameters[5].toInt() * scale);
  height = (int) (parameters[6].toInt() * scale);

  // Set minimal and maximal values
  min = parameters[7].toInt();
  max = parameters[8].toInt();

  // Set name
  name_ = parameters[12];

  // Set black brush
  GraphicProperties gp = GraphicProperties::getInstance();
  blackBrush = gp.getBlackBrush();
}

void widget::VerticalSlider::paint(QPainter& p)
{
  // Draw field
  p.drawRect(x, y, width, height);

  // Draw indicator
  p.fillRect(x, y + position, IndicatorThickness, height, blackBrush);
}

void widget::properties::Radio::setValue(float f)
{
  // Restrict range of values
  if(f > max)
  {
    f = max;
  }
  else if(f < min)
  {
    f = min;
  }

  // Set new value
  value = f;
}

widget::HorizontalRadio::HorizontalRadio(QStringList& parameters)
{
  // Set minimal and maximal values first (needed for dimension calculations)
  min = 0;
  max = parameters[8].toInt();

  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  height = (int) (parameters[5].toInt() * scale);
  width = height * max;

  // Set name
  name_ = parameters[10];

  // Save amount of radio buttons and adjust maximal value
  buttons = max;
  max = max - 1; // Because first index is 0

  // Set black brush
  GraphicProperties gp = GraphicProperties::getInstance();
  blackBrush = gp.getBlackBrush();
}

void widget::HorizontalRadio::paint(QPainter& p)
{
  for(unsigned int i = 0; i < buttons; i++)
  {
    // Draw a radio button
    p.drawRect(x + height * i, y, height, height);

    // Mark current radio button
    if(i == value)
    {
      p.fillRect(x + height * i + MarkMargin,
                 y + MarkMargin,
                 height - MarkMargin * 2,
                 height - MarkMargin * 2,
                 blackBrush);
    }
  }
}

widget::VerticalRadio::VerticalRadio(QStringList& parameters)
{
  // Set minimal and maximal values first (needed for dimension calculations)
  min = 0;
  max = parameters[8].toInt();

  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  width = (int) (parameters[5].toInt() * scale);
  height = width * max;

  // Set name
  name_ = parameters[10];

  // Save amount of radio buttons and adjust maximal value
  buttons = max;
  max = max - 1; // Because first index is 0

  // Set black brush
  GraphicProperties gp = GraphicProperties::getInstance();
  blackBrush = gp.getBlackBrush();
}

void widget::VerticalRadio::paint(QPainter& p)
{
  for(unsigned int i = 0; i < buttons; i++)
  {
    // Draw a radio button
    p.drawRect(x, y + width * i, width, width);

    // Mark current radio button
    if(i == value)
    {
      p.fillRect(x + MarkMargin,
                 y + width * i + MarkMargin,
                 width - MarkMargin * 2,
                 width - MarkMargin * 2,
                 blackBrush);
    }
  }
}

widget::Number::Number(QStringList& parameters)
{
  // Set dimensions
  GraphicProperties gp = GraphicProperties::getInstance();
  float scale = gp.getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  width = (int) (parameters[4].toInt() * 7 * scale);
  height = (int) (16 * scale);

  // Create contour
  contour = QPointArray(6);

  // Set contour
  contour.setPoint(0, QPoint(x, y));
  contour.setPoint(1, QPoint(x + width - 5 * (int) scale, y));
  contour.setPoint(2, QPoint(x + width, y + 5 * (int) scale));
  contour.setPoint(3, QPoint(x + width, y + height));
  contour.setPoint(4, QPoint(x, y + height));
  contour.setPoint(5, QPoint(x, y));

  // Set value
  value = 0.0f;

  // Set name
  name_ = parameters[9];
}

void widget::Number::setValue(float f)
{
  value = f;
}

void widget::Number::paint(QPainter& p)
{
  // Convert value to string
  sv = QString::number(value, 'f', 1);

  // Draw contour and text
  p.drawPolyline(contour);
  p.drawText(x + 12, y + p.fontMetrics().height() + 12, sv, sv.length());
}

void widget::properties::Textual::setValue(float f)
{
  (void) f; // No new value possible here
}

widget::Text::Text(QStringList& parameters)
{
  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);

  // Copy fourth parameter to the text
  text = parameters[4];

  // Append all tokens to the text (which is the value)
  for(unsigned int i = 5; i < parameters.count(); i++)
  {
    // Append whitespace
    text.append(' ');

    // Append text
    text.append(parameters[i]);
  }

  // Cut off the last semicolon
  text.truncate(text.length()-1);

  // Set length of the resulting text
  textLength = text.length();
}

void widget::Text::paint(QPainter& p)
{
  p.drawText(x + 12, y + p.fontMetrics().height() + 12, text, textLength);
}

widget::Symbol::Symbol(QStringList& parameters)
{
  // Set dimensions
  float scale = GraphicProperties::getInstance().getScale();
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);

  // Set name
  name_ = parameters[9];
}

void widget::Symbol::setValue(float f)
{
  (void) f; // No setting of a new value possible
}

void widget::Symbol::paint(QPainter& p)
{
  (void) p; // Symbols do not get painted
}


/** Create standard user interface. */
StandardView::StandardView(Controller* controller_, int width_, int height_)
{
  controller = controller_;
  width = width_;
  height = height_;
}

/** Paint standard user interface. */
void StandardView::repaint(QPainter& p)
{
  QBrush brush(Qt::black); // Solid black

  // For drawing filled ellipses
  p.setBrush(brush);

  // Outer circle
  p.drawEllipse(width / 2 - 2 * height / 5,
                height / 10,
                4 * height / 5,
                4 * height / 5);

  // Draw buttons, if any pressed
  if(controller->anyButtonPressed())
  {
    p.setPen(Qt::gray);
    brush.setColor(Qt::gray);
    p.setBrush(brush);
  }

  if(controller->buttonPressed(BUTTON_MENU))
  {
    p.drawEllipse(width / 2 - height / 8,
                  height / 4 - height / 8,
                  height / 4,
                  height / 4);
  }

  if(controller->buttonPressed(BUTTON_REWIND))
  {
    p.drawEllipse(width / 3 - height / 8,
                  height / 2 - height / 8,
                  height / 4,
                  height / 4);
  }

  if(controller->buttonPressed(BUTTON_FORWARD))
  {
    p.drawEllipse(2 * width / 3  + 1 - height / 8,
                  height / 2 - height / 8,
                  height / 4,
                  height / 4);
  }

  if(controller->buttonPressed(BUTTON_PLAY))
  {
    p.drawEllipse(width / 2 - height / 8,
                  3 * height / 4 - height / 8,
                  height / 4,
                  height / 4);
  }

  // Draw inner circle
  QColor innerCircleColor =
    controller->buttonPressed(BUTTON_ACTION) ? Qt::gray : Qt::lightGray;
  p.setPen(innerCircleColor);
  brush.setColor(innerCircleColor);
  p.setBrush(brush);
  p.drawEllipse(width / 2 - height / 8,
                height / 2 - height / 8,
                height / 4,
                height / 4);

  // Draw scroll wheel value
  p.setPen(Qt::black);
  QString sv(QString("%1").arg(controller->wheelValue()));
  QFontMetrics fm = p.fontMetrics();
  p.drawText(width / 2 - fm.width(sv) / 2,
             height / 2 + fm.height() / 2,
             sv,
             sv.length());
}

/** Create custom user interface. */
CustomView::CustomView(QValueList<widget::Widget*>* widgets_)
{
  widgets = widgets_;
}

/** Paint custom user interface. */
void CustomView::repaint(QPainter& p)
{
  // Paint widgets
  for(QValueList<widget::Widget*>::ConstIterator widget = widgets->begin();
      widget != widgets->end();
      widget++)
  {
    (*widget)->paint(p);
  }
}


/** Configuration class constructor. */
Config::Config(const char* filename_, bool check_only_)
{
  // Store name of the configuration file
  filename = filename_;
  // Store also the contents check parameter
  check_only = check_only_;

  // Read entries
  read();

  // Check for empty entries. Replace them with default value
  if(pdPath_.isEmpty() || patchDirectory_.isEmpty())
  {
    pdPath_ = PD_COMMAND;
#ifdef UNIX
    pdStart_ = true;
#else /* WIN32 */
    pdStart_ = false;
#endif
    patchDirectory_ = PATCH_DIRECTORY;
  }

  // If configuration file does not exist, write it
  QFile configFile(filename);
  if(!configFile.exists())
    write();
}

/** Configuration class destructor. */
Config::~Config()
{
  // If this configuration was used for checking only, return
  if(check_only)
    return;

  // Re-read configuration into another object
  Config* originalConfig = new Config(filename, true);

  // Compare objects, write config into the config file if not equal
  if(this->pdPath() != originalConfig->pdPath() ||
     this->pdStart() != originalConfig->pdStart() ||
     this->patchDirectory() != originalConfig->patchDirectory())
    write();

  // Clean up
  delete originalConfig;
}

/** Copy data from another configuration. */
void Config::copyFrom(Config& otherConfig)
{
  // Copy data
  pdPath_ = otherConfig.pdPath_;
  pdStart_ = otherConfig.pdStart_;
  patchDirectory_ = otherConfig.patchDirectory_;
}

/** Read configuration file. */
void Config::read()
{
  QFile f(filename);
  QTextStream t(&f);
  QString line;
  QStringList parameters;
  QString key;
  QString value;

  // Open file
  if(!f.open(IO_ReadOnly))
    return;

  // Read patch file
  while(!t.atEnd())
  {
    // Reading line
    line = t.readLine().stripWhiteSpace();

    // Check for empty lines
    if(line.isEmpty())
      continue;

    // Parse the line and store value of parameter if one was supplied

    // Split line at the '=' character
    parameters = QStringList::split('=', line);

    // If the are no two parts, go to the next line
    if(parameters.count() != 2)
      continue;

    // Get key and value
    key = (*(parameters.at(0))).stripWhiteSpace();
    value = (*(parameters.at(1))).stripWhiteSpace();

    // Assign values to keys
    if(key == "PDPath")
      pdPath_ = value;
    else if(key == "PDStart")
      pdStart_ = (value == "yes");
    else if(key == "PatchDirectory")
      patchDirectory_ = value;
  }

  // Close file
  f.close();
}

/** Write configuration file. */
void Config::write()
{
  QFile f(filename);
  QTextStream t(&f);

  // Open file
  if(!f.open(IO_WriteOnly))
    return;

  // Store values
  t << "PDPath" << " = " << pdPath_ << endl;
  t << "PDStart" << " = " << (pdStart_ ? "yes" : "no") << endl;
  t << "PatchDirectory" << " = " << patchDirectory_ << endl;

  // Close file
  f.close();
}

/** Configuration dialog constructor. */
ConfigDialog::ConfigDialog(Config* currentConfig, QString filename,
                           QWidget* parent, const char* name) :
                                       QDialog(parent, name, true)
{
  // Set up initial parameters
  setCaption("Configuration");

  // Initialize configuration
  config = new Config(filename, true);
  config->copyFrom(*currentConfig);

  // Create main dialog box
  QGridLayout* grid = new QGridLayout(this, 3, 9, 4, 4, "grid layout");

  // Create PD path group
  QHGroupBox* pdPathGroup = new QHGroupBox("Pure Data binary location", this);
  grid->addMultiCellWidget(pdPathGroup, 0, 0, 0, 8);

  pdPathView = new QLineEdit(config->pdPath(), pdPathGroup);
  connect(pdPathView, SIGNAL(textChanged(const QString&)),
          this, SLOT(setPDPath(const QString&)));

  QPushButton* pdPathChoose = new QPushButton("&Choose", pdPathGroup);
  connect(pdPathChoose, SIGNAL(clicked()), this, SLOT(choosePDPath()));

  QPushButton* pdPathDefault = new QPushButton("&Default", pdPathGroup);
  connect(pdPathDefault, SIGNAL(clicked()), this, SLOT(defaultPDPath()));

  // Create PD start group
  QHGroupBox* pdStartGroup = new QHGroupBox("Start Pure Data", this);
  grid->addMultiCellWidget(pdStartGroup, 1, 1, 0, 8);

  pdStartView = new QCheckBox("Start Pure Data when loading patch", pdStartGroup);
  pdStartView->setChecked(config->pdStart());
  connect(pdStartView, SIGNAL(toggled(bool)), this, SLOT(setPDStart(bool)));

#ifndef QTOPIA
  // Create (default) OK button, Qtopia makes one automatically
  QPushButton* configOK = new QPushButton("&OK", this);
  configOK->setDefault(true);
  grid->addWidget(configOK, 2, 8);
  connect(configOK, SIGNAL(clicked()), this, SLOT(accept()));
#endif /* QTOPIA */
}

/** Set PD binary path. */
void ConfigDialog::setPDPath(const QString& path)
{
  config->setPDPath(path);
}

/** Choose PD binary path. */
void ConfigDialog::choosePDPath()
{
  QString filename;
  QString pdDir = QFileInfo(pdPathView->text()).filePath();

#ifdef USE_NATIVE_FILEDIALOGS
  filename = QFileDialog::getOpenFileName(pdDir,
#ifdef UNIX
                                          "All files (*)",
#else /* WIN32 */
                                          "Executable files (*.exe;*.com)",
#endif
                                          this,
                                          "open file dialog",
                                          "Choose Pure Data binary");
#else
  QTKFileDialog fileDialog(this, "Choose Pure Data binary");
  fileDialog.setFilter("All files|*.*");
  fileDialog.setHomeDirectoryCdUpEnable(true);

  // Set directory
  fileDialog.setDirectory(pdDir);
  fileDialog.showMaximized();

  if(fileDialog.exec() == QDialog::Accepted)
  {
     filename  = fileDialog.getFileName();
  }
#endif

  if(filename.length() > 0)
    pdPathView->setText(filename);
}

/** Set default PD binary path. */
void ConfigDialog::defaultPDPath()
{
  pdPathView->setText(PD_COMMAND);
}

/** Set whether to start Pure Data when loading patch. */
void ConfigDialog::setPDStart(bool state)
{
  config->setPDStart(state);
}


/** Help dialog contructor. */
HelpDialog::HelpDialog(QWidget* parent, const char* name, WFlags f) :
                                             QDialog(parent, name, f)
{
  // Set up dialog
  setCaption("PDQt help");
  setWFlags(getWFlags() | Qt::WDestructiveClose);

  // Create main box
  QVBox* mainBox = new QVBox(this);
  mainBox->setSpacing(4);

  // Add copyright group
  QTextBrowser* help = new QTextBrowser(mainBox);
  help->setTextFormat(Qt::RichText);

  // Create help text
  help->setText
  (
    "<h1>PDQt help</h1><br/>"
    "<h2><a name=\"contents\">Contents</a></h2>"
    "<ul>"
      "<li><a href=\"#generalcosiderations\">General considerations</a></li>"
      "<li><a href=\"#standardinterface\">Standard interface</a></li>"
      "<li><a href=\"#custominterface\">Custom interface</a></li>"
    "</ul><br/>"
    ""
    "<h2><a name=\"generalconsiderations\">General considerations</a></h2><br/>"
    "  The aim of the user interface was to emulate the user interface"
    "  of an audio player with a certain fame. While trying to do so,"
    "  the control keys had to be chosen carefully, lest they do not"
    "  land wide apart on different keyboards.<br/>"
    "  <center>"
    "    <table>"
    "    <caption>The keys are:</caption>"
    "      <tbody>"
    "        <tr><td><h3>Key</h3></td><td><h3>Function</h3></td></tr>"
    "        <tr><td>Space</td><td>Play</td></tr>"
    "        <tr><td>Menu</td><td>Menu</td></tr>"
    "        <tr><td>Shift</td><td>Action</td></tr>"
    "        <tr><td>Left</td><td>Rewind</td></tr>"
    "        <tr><td>Right</td><td>Forward</td></tr>"
    "        <tr><td>Up</td><td>Rotate clockwise</td></tr>"
    "        <tr><td>Down</td><td>Rotate counterclockwise</td></tr>"
    "      </tbody>"
    "    </table>"
    "  </center><br/>"
    "  Some controls maintain their function in all interfaces."
    "  Rotation buttons are used quite often to increase and decrease"
    "  a value. Pressing <b>Action</b> button together with rotation buttons"
    "  multiplies value change by 10. Pressing <b>Action</b> button together"
    "  with <b>Play</b> button pauses and unpauses the current patch.<br/>"
    "  <a href=\"#contents\">Back to contents</a><br/>"
    ""
    "<h2><a name=\"standardinterface\">Standard interface</a></h2><br/>"
    "  The standard interface presents itself like a wheel. You may use"
    "  four buttons (<b>Menu</b>, <b>Forward</b>, <b>Play</b> and <b>Rewind</b>)"
    "  to activate four functions, and if you look at the table in the previous"
    "  section, you will notice that the plays of the mentioned keys on the wheel"
    "  correspond with their place on the keyboard. The actual functions"
    "  of the keys depend on the playing patch.<br/>"
    "  In the middle of the wheel there is a value that you can change with"
    "  rotation buttons. <b>Action</b>&nbsp;button may activate, depending on the"
    "  current patch, some secondary functions.<br/>"
    "  <a href=\"#contents\">Back to contents</a><br/>"
    ""
    "<h2><a name=\"custominterface\">Custom interface</a></h2><br/>"
    "  You will have to figure out what the controls do by yourself, "
    "  as the functions of the control keys are entirely customized here.<br/>"
    "  <a href=\"#contents\">Back to contents</a><br/>"
  );

  // Add text view to the view port
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(mainBox);

  // Set size and show
#ifdef QTOPIA
  showMaximized();
#else
  if (qApp->desktop()->width() < 640 || qApp->desktop()->height() < 480)
  {
    showMaximized();
  }
  else
  {
    resize(640, 480);
    show();
  }
#endif
}

/** About dialog constructor. */
AboutDialog::AboutDialog(QWidget* parent, const char* name, WFlags f) :
                                             QDialog(parent, name, f)
{
  // Set up dialog
  setCaption("About PDQt");
  setWFlags(getWFlags() | Qt::WDestructiveClose);

  // Create scroll bars
  QScrollView* scrollView = new QScrollView(this);
  scrollView->setFrameStyle(QScrollView::NoFrame);
  scrollView->setMargin(4);
  scrollView->setResizePolicy(QScrollView::AutoOneFit);

  // Create main box
  QVBox* mainBox = new QVBox(scrollView->viewport());
  mainBox->setSpacing(4);
  scrollView->addChild(mainBox);

  // Add copyright group
  QVGroupBox* copyrightBox = new QVGroupBox("Copyright information", mainBox);
  QTextView* about = new QTextView(copyrightBox);
  about->setTextFormat(Qt::RichText);

  // Create "About..." text
  about->setText
  (
    "<h1>Pure Data for Qt</h1><br/>"
    "Copyright (C) 2008, 2009 by Wincent Balin "
      "<a href=\"mailto:wincent.balin@gmail.com\">(wincent.balin@gmail.com)</a><br/>"
    "<br/>"
    "This software is licensed under the terms of the GNU General Public License version 2.<br/>"
    "<br/>"
    "For more information please visit: "
    "<a href=\"http://katastrophos.net/wincent/blog/software/pdqt/\">"
      "http://katastrophos.net/wincent/blog/software/pdqt/</a><br/>"
    "<br/>"
    "This software is based upon PdPod "
      "(<a href=\"http://www.ipodlinux.org/wiki/PdPod\">"
      "http://www.ipodlinux.org/wiki/PdPod</a>) by Martin Kaltenbrunner<br/>"
    "PdPod is a graphical user interface for Pure Data Anywhere "
      "(<a href=\"http://pd-anywhere.sf.net/\">http://pd-anywhere.sf.net/</a>) "
      "by Guenter Geiger<br/>"
    "Pure Data Anywhere is a fixed-point implementation "
      "of the Pure Data audio environment "
      "(<a href=\"http://puredata.info/\">"
      "http://puredata.info/</a>) by Miller Puckette et al.<br/>"
    "TK File Dialog (C) by TKmix, modified by AGAWA Koji and Andre Beckedorf<br/>"
    "<br/>"
    "Many thanks go to Andre Beckedorf, "
      "author of the very inspiring audio player package called Quasar "
      "(<a href=\"http://katastrophos.net/quasar/\">http://katastrophos.net/quasar/</a>), "
      "for providing both his knowledge and his resources. "
      "Without him this piece of software would not be."
  );

  // Add text view to the view port
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(scrollView);

  // Set size and show
#ifdef QTOPIA
  showMaximized();
#else
  if (qApp->desktop()->width() < 640 || qApp->desktop()->height() < 480)
  {
    showMaximized();
  }
  else
  {
    resize(640, 480);
    show();
  }
#endif
}


/** PD Qt GUI constructor. */
PDQt::PDQt(QWidget* parent, const char* name) : QMainWindow(parent, name)
{
  // Initialize status variables
  loaded = false;
  running = false;
  connected = false;
  paused = false;

  // Initialize input
  controller = new SweetSourAppleController(this);

  // Initialize standard GUI font
  font = QFont("helvetica", 24, QFont::Bold);


  // Initialize graphic properties
  GraphicProperties gp = GraphicProperties::getInstance();

  // Build menu bar
  menuBar()->insertItem("&Open", this, SLOT(load()), CTRL+Key_O);
#ifdef UNIX
  menuBar()->insertItem("&Config", this, SLOT(configure()));
#endif
  menuBar()->insertItem("&Help", this, SLOT(help()), Key_F1);
  menuBar()->insertItem("&About", this, SLOT(about()), SHIFT+Key_F1);

  // Get configuration entries
  configFilename = QDir::homeDirPath() + QDir::separator() + CONFIG_FILE;
  config = new Config(configFilename);

  // Set widget flags for double-buffering 
  setWFlags(getWFlags() |
            Qt::WNorthWestGravity | Qt::WRepaintNoErase | Qt::WResizeNoErase);

  // Create status bar
  status = new QLabel(statusBar());
  statusBar()->addWidget(status, 2, true);

  // Print status
  status->setText("Ready");
}

/** Destructor. */
PDQt::~PDQt()
{
  // Save configuration
  delete config;

  // Remove view if needed
  if(view)
    delete view;

  // Remove status label
  statusBar()->removeWidget(status);
  delete status;

  // Delete controller
  delete controller;
}

/** Mark key as pressed. */
void PDQt::keyPressEvent(QKeyEvent* k)
{
  int key = k->key();

  bool known_key = controller->pressKey(key);

  if(!known_key)
    k->ignore();

  // Release action button automatically after 10 msec
  if(controller->getButton(BUTTON_ACTION) == key && isStandardView() &&
      loaded && ! paused)
    QTimer::singleShot(10, this, SLOT(buttonActionBackpress()));

  // Repaint everything
  repaint(false);
}

/** Mark key as released. */
void PDQt::keyReleaseEvent(QKeyEvent* k)
{
  int key = k->key();

  bool known_key = controller->unpressKey(key);

  if(!known_key)
    k->ignore();

  // Repaint everything
  repaint(false);
}

void PDQt::resizeEvent(QResizeEvent *)
{
  // Get screen dimensions
  screenWidth = this->width();
  screenHeight = this->height();

  if(screenWidth != paintPixmap.width() || screenHeight != paintPixmap.height())
  {
    paintPixmap.resize(screenWidth, screenHeight);
  }

  // Calculate size factor
  float screenMultiplier = screenWidth / 160.0f;
  GraphicProperties::getInstance().setScale(screenMultiplier);
}

/** Paint GUI. */
void PDQt::paintEvent(QPaintEvent*)
{
  // If no patch loaded, nothing to draw
  if(!loaded)
    return;

  paintPixmap.fill();
  QPainter p(&paintPixmap);

  p.setBackgroundColor(white);
  p.setPen(black);
  p.setFont(font);

  view->repaint(p);

  QPainter pp(this);
  pp.drawPixmap(0, 0, paintPixmap);
}

/** Processes closing events. */
void PDQt::closeEvent(QCloseEvent* ce)
{
  if(!loaded)
  {
    ce->accept();
  }
  else
  {
    switch(QMessageBox::information(this, "PDQt", "Really quit?", "Yes", "No", 0, 1))
    {
      case 0:
        if(connected)
          disconnectPD();
        if(running)
          stopPD();
        ce->accept();
        break;
      case 1:
      default:
        ce->ignore();
        break;
    }
  }
}

/** Get PD patch filename. */
void PDQt::load()
{
  QString filename;

#ifdef USE_NATIVE_FILEDIALOGS
  filename = QFileDialog::getOpenFileName(config->patchDirectory(),
#ifdef UNIX
                                          "PureData patches (*.pd);;" \
                                          "All files (*)",
#else /* WIN32 */
                                          "PureData patches (*.pd);;" \
                                          "All files (*.*)",
#endif
                                          this,
                                          "open file dialog",
                                          "Load PureData patches");
#else
  QTKFileDialog fileDialog(this, "Load PureData patches");

  fileDialog.setFilter("PureData patches|*.pd|" \
                       "All files|*.*");

  fileDialog.setHomeDirectoryCdUpEnable(true);

  // Set directory
  fileDialog.setDirectory(config->patchDirectory());

  fileDialog.showMaximized();

  if(fileDialog.exec() == QDialog::Accepted)
  {
     filename  = fileDialog.getFileName();
  }
#endif

  if(filename.length() > 0)
    load(filename.latin1());

  // Repaint whole screen
  repaint(false);
}

/** Load PD patch. */
void PDQt::load(const char* filename)
{
  QFile f(filename);
  QFileInfo fi(f);
  QTextStream t(&f);
  QString line;

  // Set patch name and directory of the patch
  patch = filename;
  fi = QFileInfo(patch);
  config->setPatchDirectory(fi.dirPath());

  // Open patch file
  if(!f.open(IO_ReadOnly))
    return;

  // If needed, delete view
  if(loaded)
    delete view;

  // Reset status
  loaded = false;

  // Erase widget objects
  for(QValueList<widget::Widget*>::Iterator widget = widgets.begin();
      widget != widgets.end();
      widget++)
  {
    delete *widget;
  }

  // Clear widgets
  widgets.clear();

  // Read patch file
  while(!t.atEnd())
  {
    // Reading line
    line = t.readLine();

    // Check for empty lines
    if(line.isEmpty())
      continue;

    // Parse line and add the new widget, if one was created
    widget::Widget* widget = createWidget(line);

    if(widget)
      widgets.append(widget);
  }

  f.close();

  // Restart PD
  if(connected)
    disconnectPD();
  if(running)
    stopPD();
  startPD();
  connectPD();
  // (Re-)Create view
  if(isStandardView())
    view = new StandardView(controller, screenWidth, screenHeight);
  else
    view = new CustomView(&widgets);

  // Update status
  loaded = true;
  setCaption(QString(PDQTNAME" - %1").arg(filename));
  setStatus("Running patch");
}

/** Show configuration dialog. */
void PDQt::configure()
{
  // Make configuration dialog
  ConfigDialog configDialog(config, configFilename, this, "Configuration dialog");

  // Show dialog, do configuration
  int result = configDialog.exec();

  // Dependent on result, store new configuration or drop it
  switch(result)
  {
    case QDialog::Accepted:
      config->copyFrom(configDialog.getConfig());
      break;

    case QDialog::Rejected:
    default:
      break;
  }
}

/** Show Help dialog. */
void PDQt::help()
{
  HelpDialog* helpDialog = new HelpDialog();
  helpDialog->show();
}

/** Show About dialog. */
void PDQt::about()
{
  AboutDialog* aboutDialog = new AboutDialog();
  aboutDialog->show();
}

/** Press back action button. */
void PDQt::buttonActionBackpress()
{
  (controller->getButton(BUTTON_ACTION)).press(false);
  repaint(false);
}

/** Start PD core. */
void PDQt::startPD()
{
  // If no start required, do as if
  if(!config->pdStart())
  {
    running = true;
    return;
  }

#ifdef UNIX
  running = false;

  pdPid = vfork();

  if(pdPid == 0)
  {
    running = true;
    status->setText("Running PDa");
    execl(config->pdPath(), "pd", "-r", "22050", "-nogui", "-rt", "-nomidi", "-noadc", patch.latin1(), NULL);
    // If execution fails, display message and exit forked process
    running = false;
    status->clear();
    QMessageBox::critical(this, "PD start failed", "Failed to start PD engine!");
    _exit(0);
  }
  else if(pdPid < 0)
  {
    // Failed to fork
    return;
  }
#endif /* UNIX */
}

/** Connect to the PD core. */
void PDQt::connectPD()
{
  QHostAddress loopback(0x7F000001);
  bool result;

  // Reset status
  connected = false;

  // Initialize sending socket
  pdTx = new QSocketDevice(QSocketDevice::Datagram);
  result =
  pdTx->connect(loopback, 3333);
  if(!result)
  {
    connected = false;
    return;
  }

  // Initialize receiving socket
  pdRx = new QSocketDevice(QSocketDevice::Datagram);
  pdRx->setBlocking(false);
  result =
  pdRx->bind(loopback, 3334);
  if(!result)
  {
    connected = false;
    return;
  }

  // Create receiving notifier
  pdReadNotifier = new QSocketNotifier(pdRx->socket(), QSocketNotifier::Read, this);
  connect(pdReadNotifier, SIGNAL(activated(int)), this, SLOT(receiveMessage()));

  // connected to PD core
  connected = true;
  setStatus("Connected to PDa");
}

/** Disconnect from the PD core. */
void PDQt::disconnectPD()
{
  // Disconnect receive notifier
  disconnect(pdReadNotifier, SIGNAL(activated(int)), this, SLOT(receiveMessage()));

  // Close sockets
  pdRx->close();
  pdTx->close();

  // Delete notifier
  delete pdReadNotifier;

  // Delete sockets
  delete pdRx;
  delete pdTx;

  connected = false;
  status->setText("Disconnected from PDa");
}

/** Stop PD core. */
void PDQt::stopPD()
{
#ifdef UNIX
  int pd_status;
  int result;

  result = kill(pdPid, SIGKILL);
  wait(&pd_status);

  running = false;
  status->setText("Stopped PDa");
#endif /* UNIX */
}

/** Send message to PD core. */
void PDQt::sendMessage(const char* message)
{
  pdTx->writeBlock(message, strlen(message));
}

/** Receive and process message from PD core. */
void PDQt::receiveMessage()
{
#define DATA_LEN 1024
  char data[DATA_LEN];
  int bytes;
  bool updateGUI = false;

  QStringList lines;
  QStringList messages;
  QStringList tokens;

  float argval;

  bytes =
  pdRx->readBlock(data, DATA_LEN);
  if(bytes < 1) // No data received? Data length == 0?
    return;

  // Append last 0
  if(bytes < DATA_LEN)
  {
    data[bytes] = '\0';
  }
  else
  {
    data[DATA_LEN-1] = '\0';
  }

  // Dissect a line
  lines = QStringList::split('\n', data);

  for(QStringList::Iterator line = lines.begin(); line != lines.end(); line++)
  {
    messages = QStringList::split(';', *line);

    for(QStringList::Iterator message = messages.begin(); message != messages.end(); message++)
    {
      // Process message
      tokens = QStringList::split(' ', *message);

      // Get name of the widget
      QString addressedWidgetName = (*tokens.at(0));

      if(tokens.count() > 1)
      {
        argval = (*tokens.at(1)).toFloat();
      }
      else
      {
        argval = 0;
      }

      for(QValueList<widget::Widget*>::Iterator widget = widgets.begin();
          widget != widgets.end();
          widget++)
      {
        // Get widget from the iterator
        widget::Widget* w = *widget;

        // If widget is addressed, set it's value
        if(w->name() == addressedWidgetName)
        {
          // Set new value of the widget
          w->setValue(argval);

          // Update GUI to reflect change of the value
          updateGUI = true;
        }
      }
    }
  }

  // Repaint everything of needed
  if(updateGUI)
    repaint(false);
}

/** Is a patch loaded? */
bool PDQt::patchLoaded() const
{
  return loaded;
}

/** Is PD running? */
bool PDQt::pdRunning() const
{
  return running;
}

/** Pause PD (status only). */
void PDQt::pdPause(bool pause)
{
  paused = pause;
}

/** Is PD paused? */
bool PDQt::pdPaused() const
{
  return paused;
}

/** Set status text. */
void PDQt::setStatus(const char* text)
{
  status->setText(text);
}

/** Is this a standard view? */
bool PDQt::isStandardView() const
{
  return widgets.isEmpty();
}


/** Widget builder (pattern of the same name). */
widget::Widget* PDQt::createWidget(QString& line)
{
  widget::Widget* widget = NULL;

  // Split line to tokens
  QStringList tokens = QStringList::split(' ', line.stripWhiteSpace());

  // Check whether line contains widget information; if so, create it
  if(line.contains("floatatom") && line.contains("pod_"))
  {
    widget = new widget::Number(tokens);
  }
  else if(line.contains("symbolatom") && line.contains("pod_"))
  {
    widget = new widget::Symbol(tokens);
  }
  else if(line.contains("vsl") && line.contains("pod_"))
  {
    widget = new widget::VerticalSlider(tokens);
  }
  else if(line.contains("hsl") && line.contains("pod_"))
  {
    widget = new widget::HorizontalSlider(tokens);
  }
  else if(line.contains("vradio") && line.contains("pod_"))
  {
    widget = new widget::VerticalRadio(tokens);
  }
  else if(line.contains("hradio") && line.contains("pod_"))
  {
    widget = new widget::HorizontalRadio(tokens);
  }
  else if(line.contains("bng") && line.contains("pod_"))
  {
    widget = new widget::Bang(tokens);
  }
  else if(line.contains("text"))
  {
    widget = new widget::Text(tokens);
  }

  return widget;
}

/** Program entry. */
int main(int argc, char** argv)
{
#ifdef QTOPIA
  QPEApplication a(argc, argv);
#else
  QApplication a(argc, argv);
#endif
  PDQt* pdqt = new PDQt();
  pdqt->setCaption(PDQTNAME);

  // Show main widget and resize it if needed
#ifdef QTOPIA
  a.showMainWidget(pdqt);
#else
  a.setMainWidget(pdqt);
  if (qApp->desktop()->width() < 640 || qApp->desktop()->height() < 480)
  {
    pdqt->showMaximized();
  }
  else
  {
    pdqt->resize(640, 480);
    pdqt->show();
  }
#endif

  // Load PD patch supplied as an argument
  if(a.argc() > 1)
    pdqt->load(a.argv()[1]);

  // Connect closing signal to the quit slot and execute the app
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  int result = a.exec();

  // Clean up after execution
  delete pdqt;
  return result;
}
