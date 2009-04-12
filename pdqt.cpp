/***
  pdqt.cpp

  Qtopia interface for Pure Data -- main file.
*/

#include "pdqt.h"

#define PDQTNAME "Pure Data for Qtopia"

#ifndef PD_COMMAND
#define PD_COMMAND "/opt/QtPalmtop/bin/pd"
#endif

#ifndef PATCH_DIRECTORY
#define PATCH_DIRECTORY "/opt/QtPalmtop/share/pdqt"
#endif

#define DPRINTF(x...) fprintf(stderr, x);


/** Scroll wheel constructor. */
ScrollWheel::ScrollWheel(int value_)
{
  value = value_;
}

/** Scroll wheel up for x steps. */
void ScrollWheel::scrollUp(unsigned int steps)
{
  value += steps;
}

/** Scroll wheel down for x steps. */
void ScrollWheel::scrollDown(unsigned int steps)
{
  value -= steps;
}

/** Get value of the scroll wheel. */
int ScrollWheel::getValue()
{
  return value;
}

/** Create bang widget. */
BangWidget::BangWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_BANG;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  w = (int) (parameters[5].toInt() * scale);
  h = w;

  // Set minimal and maximal values (i.e. false and true)
  min = 0;
  max = 1;

  // Set name
  name = parameters[10].latin1();
}

/** Paint bang widget. */
void BangWidget::paint(QPainter& p)
{
  // Backup brush
  QBrush backupBrush;

  // Draw contour
  p.drawRect(x, y, w-1, h-1);

  // If selected, backup current brush and set solid black one
  if(value == 1)
  {
    QBrush backupBrush = p.brush();
    p.setBrush(blackBrush);
  }

  // Draw ellipse, filled if selected
  p.drawEllipse(x, y, w-1, h-1);

  // If widget selected, deselect and restore brush
  if(value == 1)
  {
    value = 0;
    p.setBrush(backupBrush);
  }
}

/** Create horizontal slider widget. */
HorizontalSliderWidget::HorizontalSliderWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_HSLIDER;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  w = (int) (parameters[5].toInt() * scale);
  h = (int) (parameters[6].toInt() * scale);

  // Set minimal and maximal values
  min = parameters[7].toInt();
  max = parameters[8].toInt();

  // Set name
  name = parameters[12].latin1();
}

/** Paint horizontal slider widget. */
void HorizontalSliderWidget::paint(QPainter& p)
{
  // Draw contour
  p.drawRect(x, y, w, h);

  // Calculate position of the slider
  position = (int) ((float) w / max - min) * (int) (max - value);

  // Draw slider
  p.fillRect(x + w - position, y, 2, h, blackBrush);
}

/** Create vertical slider widget. */
VerticalSliderWidget::VerticalSliderWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_VSLIDER;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  w = (int) (parameters[5].toInt() * scale);
  h = (int) (parameters[6].toInt() * scale);

  // Set minimal and maximal values
  min = parameters[7].toInt();
  max = parameters[8].toInt();

  // Set name
  name = parameters[12].latin1();
}

/** Paint vertical slider widget. */
void VerticalSliderWidget::paint(QPainter& p)
{
  // Draw contour
  p.drawRect(x, y, w, h);

  // Calculate position of the slider
  position = (int) ((float) h / max - min) * (int) (max - value);

  // Draw slider
  p.fillRect(x, y + position, 2, h, blackBrush);
}

/** Create horizontal radio widget. */
HorizontalRadioWidget::HorizontalRadioWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_HRADIO;

  // Set minimal and maximal values first (needed for dimension calculations)
  min = 0;
  max = parameters[8].toInt();

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  h = (int) (parameters[5].toInt() * scale);
  w = h * max;

  // Set name
  name = parameters[10].latin1();

  // Save amount of radio buttons and adjust maximal value
  radioButtons = max;
  max--;
}

/** Paint horizontal radio widget. */
void HorizontalRadioWidget::paint(QPainter& p)
{
  for(unsigned int i = 0; i < radioButtons; i++)
  {
    // Draw a radio button
    p.drawRect(x + h * i, y, h, h);

    // Mark current radio button
    if(i == value)
      p.fillRect(x + h * i + 2, y + 2, h - 4, h - 4, blackBrush);
  }
}

/** Create vertical radio widget. */
VerticalRadioWidget::VerticalRadioWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_VRADIO;

  // Set minimal and maximal values first (needed for dimension calculations)
  min = 0;
  max = parameters[8].toInt();

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  w = (int) (parameters[5].toInt() * scale);
  h = w * max;

  // Set name
  name = parameters[10].latin1();

  // Save amount of radio buttons and adjust maximal value
  radioButtons = max;
  max--;
}

/** Paint vertical radio widget. */
void VerticalRadioWidget::paint(QPainter& p)
{
  for(unsigned int i = 0; i < radioButtons; i++)
  {
    // Draw a radio button
    p.drawRect(x, y + w * i, w, w);

    // Mark current radio button
    if(i == value)
      p.fillRect(x + 2, y + w * i + 2, w - 4, w - 4, blackBrush);
  }
}

/** Create number widget. */
NumberWidget::NumberWidget(QStringList& parameters, float scale, QFont& widgetFont, QFontMetrics* widgetFontMetrics)
{
  // Set id of the widget
  id = PD_NUMBER;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);
  w = (int) (parameters[4].toInt() * 7 * scale);
  h = (int) (                       16 * scale);

  // Set contour
  contour.setPoint(0, QPoint(x, y));
  contour.setPoint(1, QPoint(x + w - 5 * (int) scale, y));
  contour.setPoint(2, QPoint(x + w, y + 5 * (int) scale));
  contour.setPoint(3, QPoint(x + w, y + h));
  contour.setPoint(4, QPoint(x, y + h));
  contour.setPoint(5, QPoint(x, y));

  // Set value
  value = 0.0f;

  // Set name
  name = parameters[9].latin1();

  // Set font and it's metrics
  font = widgetFont;
  fm = widgetFontMetrics;
}

/** Paint number widget. */
void NumberWidget::paint(QPainter& p)
{
  // Convert value to string
  sv = QString::number(value, 'f', 1); 

  // Set font
  p.setFont(font);

  // Draw contour and text
  p.drawPolyline(contour);
  p.drawText(x + 12, y + fm->height() + 12, sv, sv.length());
}

/** Create symbol widget. */
SymbolWidget::SymbolWidget(QStringList& parameters, float scale)
{
  // Set id of the widget
  id = PD_SYMBOL;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);

  // Set name
  name = parameters[9].latin1();
}

/** Create text widget. */
TextWidget::TextWidget(QStringList& parameters, float scale, QFont& widgetFont, QFontMetrics* widgetFontMetrics)
{
  // Set id of the widget
  id = PD_TEXT;

  // Set dimensions
  x = (int) (parameters[2].toInt() * scale);
  y = (int) (parameters[3].toInt() * scale);

  // Copy fourth parameter to the text
  text = parameters[4].latin1();

  // Append all tokens to the text (which is the value)
  for(unsigned int i = 5; i < parameters.count(); i++)
    text.append(parameters[i].latin1());

  // Set font and it's metrics
  font = widgetFont;
  fm = widgetFontMetrics;
}

/** Paint text widget. */
void TextWidget::paint(QPainter& p)
{
  p.setFont(font);
  p.drawText(x + 12, y + fm->height() + 12, text, text.length());
}





/** PD Qt GUI constructor. */
PDQt::PDQt(QWidget* parent, const char* name) : QMainWindow(parent, name)
{
  // Initialize status variables
  loaded = false;
  running = false;
  connected = false;
  paused = false;

  // Initialize input variables
  buttonMenu.key = Key_R;
  buttonPlay.key = Key_C;
  buttonForward.key = Key_F;
  buttonRewind.key = Key_S;
  buttonAction.key = Key_D;
  wheelClockwise.key = Key_E;
  wheelCounterclockwise.key = Key_X;
  scrollValue = 0;

  // Initialize standard GUI font
  font = QFont("helvetica", 24, QFont::Bold);
  fm = new QFontMetrics(font);

  // Build menu bar
  menuBar()->insertItem("&Open", this, SLOT(load()), CTRL+Key_O);
  menuBar()->insertItem("&About", this, SLOT(about()), Key_F1);

/*
  // Get screen dimensions
  screenWidth = qApp->desktop()->width();
  screenHeight = qApp->desktop()->height();

  // Calculate size factor
  screenMultiplier = screenWidth / 160.0f;
*/

  // Get configuration entries
  config = new Config("PDQt");
  if(config->isValid())
  {
    pdPath = config->readEntry("pdPath");
    patchDirectory = config->readEntry("patchDirectory");
  }
  else
  {
    pdPath = PD_COMMAND;
    patchDirectory = PATCH_DIRECTORY;
  }

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
  config->writeEntry("pdPath", pdPath);
  config->writeEntry("patchDirectory", patchDirectory);

  // Remove status label
  statusBar()->removeWidget(status);
  delete status;

  // Delete created font metrics
  delete fm;
}

/** Mark key as pressed. */
void PDQt::keyPressEvent(QKeyEvent* k)
{
  int key = k->key();

  if(key == buttonMenu.key)
  {
    if(running && !paused)
    {
      sendMessage("m 1;\n");

      if(widgets.count() == 0)
        buttonMenu.pressed = true;
    }
  }
  else if(key == buttonAction.key)
  {
    if(running && !paused)
    {
      sendMessage("b;\n");

      if(widgets.count() == 0)
      {
        buttonAction.pressed = true;
	repaint(false);
	QTimer::singleShot(1000, this, SLOT(buttonActionBackpress()));
      }
    }

    if(running)
      shift = true;
  }
  else if(key == buttonRewind.key)
  {
    if(running && !paused)
    {
      sendMessage("w 1;\n");

      if(widgets.count() == 0)
        buttonRewind.pressed = true;
    }
  }
  else if(key == buttonForward.key)
  {
    if(running && !paused)
    {
      sendMessage("f 1;\n");

      if(widgets.count() == 0)
        buttonForward.pressed = true;
    }
  }
  else if(key == wheelCounterclockwise.key)
  {
    if(running && !paused)
    {
      if(shift)
      {
        sendMessage("l 10;\n");
	scrollValue -= 10;
      }
      else
      {
        sendMessage("l 1;\n");
	scrollValue -= 1;
      }
    }
  }
  else if(key == wheelClockwise.key)
  {
    if(running && !paused)
    {
      if(shift)
      {
        sendMessage("r 10;\n");
	scrollValue += 10;
      }
      else
      {
        sendMessage("r 1;\n");
	scrollValue += 1;
      }
    }
  }
  else if(key == buttonPlay.key)
  {
    if(running)
    {
      if(shift)
      {
        paused = !paused;

	if(paused)
	{
          sendMessage("p 0;\n");
	  status->setText("Paused");
	}
	else
	{
	  sendMessage("p 1;\n");
	  status->setText("Running patch");
	}
      }
      else if(!paused)
      {
        sendMessage("d 1;\n");

	if(widgets.count() == 0)
	  buttonPlay.pressed = true;
      }
    }
  }
  else
  {
    k->ignore();
  }
}

/** Mark key as released. */
void PDQt::keyReleaseEvent(QKeyEvent* k)
{
  int key = k->key();

  if(key == buttonMenu.key)
  {
    if(running && !shift && !paused)
    {
      sendMessage("m 0;\n");

      if(widgets.count() == 0)
        buttonMenu.pressed = false;
    }
  }
  else if(key == buttonAction.key)
  {
    if(running)
      shift = false;
  }
  else if(key == buttonRewind.key)
  {
    if(running && !paused)
    {
      sendMessage("w 0;\n");

      if(widgets.count() == 0)
        buttonRewind.pressed = false;
    }
  }
  else if(key == buttonForward.key)
  {
    if(running && !paused)
    {
      sendMessage("f 0;\n");

      if(widgets.count() == 0)
        buttonForward.pressed = false;
    }
  }
  else if(key == buttonPlay.key)
  {
    if(running && !shift && !paused)
    {
      sendMessage("d 0;\n");

      if(widgets.count() == 0)
        buttonPlay.pressed = false;
    }
  }
  else
  {
    k->ignore();
  }

  // Repaint everything, especially useful for repainting reset bang widgets
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
  screenMultiplier = screenWidth / 160.0f;
}

/** Paint GUI. */
void PDQt::paintEvent(QPaintEvent*)
{
  // If no patch loaded, nothing to draw
  if(!loaded)
    return;

  paintPixmap.fill();
  QPainter p(&paintPixmap);

  QBrush brush(black); // Solid black

  p.setBackgroundColor(white);
  p.setPen(black);

  if(widgets.count() > 0) // Custom interface
  {
    int v;
    unsigned int radioButtons;
    QPointArray numberContour(6);
    QString sv;

    for(QValueList<PDWidget>::Iterator widget = widgets.begin(); widget != widgets.end(); widget++)
    {
      p.save();

      switch((*widget).type)
      {
        case PD_BANG:
	  p.drawRect((*widget).x,
	             (*widget).y,
	             (*widget).w-1,
	             (*widget).h-1);
          if((*widget).value == 1)
	  {
	    p.setBrush(brush);
            p.drawEllipse((*widget).x,
                          (*widget).y,
                          (*widget).w-1,
                          (*widget).h-1);
            (*widget).value = 0;
	    p.setBrush(NoBrush);
	  }
	  else
            p.drawEllipse((*widget).x,
                          (*widget).y,
                          (*widget).w-1,
                          (*widget).h-1);
	  break;

	case PD_VSLIDER:
	  p.drawRect((*widget).x,
	             (*widget).y,
	             (*widget).w,
	             (*widget).h);
          v = (int) ((float) (*widget).h / ((*widget).max - (*widget).min)) *
	      (int) ((*widget).max - (*widget).value);
	  p.fillRect((*widget).x,
	             (*widget).y + v,
		     (*widget).w,
		     2,
		     brush);
	  break;

        case PD_HSLIDER:
	  p.drawRect((*widget).x,
	             (*widget).y,
	             (*widget).w,
	             (*widget).h);
          v = (int) ((float) (*widget).w / ((*widget).max - (*widget).min)) *
	      (int) ((*widget).max - (*widget).value);
	  p.fillRect((*widget).x + (*widget).w - v,
	             (*widget).y,
		     2,
		     (*widget).h,
		     brush);
          break;

        case PD_HRADIO:
	  radioButtons = (*widget).w / (*widget).h;
	  for(unsigned int i = 0; i < radioButtons; i++)
	  {
	    p.drawRect((*widget).x + (*widget).h * i,
	               (*widget).y,
		       (*widget).h,
		       (*widget).h);
            if((*widget).value == i)
	      p.fillRect((*widget).x + (*widget).h * i + 2,
	                 (*widget).y + 2,
			 (*widget).h - 4,
			 (*widget).h - 4,
			 brush);
	  }
	  break;

	case PD_VRADIO:
	  radioButtons = (*widget).h / (*widget).w;
          for(unsigned int i = 0; i < radioButtons; i++)
	  {
	    p.drawRect((*widget).x,
	               (*widget).y + (*widget).w * i,
		       (*widget).w,
		       (*widget).w);
            if((*widget).value == i)
	      p.fillRect((*widget).x + 2,
	                 (*widget).y + (*widget).w * i + 2,
			 (*widget).w - 4,
			 (*widget).w - 4,
			 brush);
	  }
	  break;

	case PD_NUMBER:
	  numberContour[0] = QPoint((*widget).x, (*widget).y);
	  numberContour[1] = QPoint((*widget).x + (*widget).w - 5 * (int) screenMultiplier,
	                            (*widget).y);
          numberContour[2] = QPoint((*widget).x + (*widget).w,
	                            (*widget).y + 5 * (int) screenMultiplier);
          numberContour[3] = QPoint((*widget).x + (*widget).w,
	                            (*widget).y + (*widget).h);
          numberContour[4] = QPoint((*widget).x,
	                            (*widget).y + (*widget).h);
	  numberContour[5] = numberContour[0];
          sv = QString::number((*widget).value, 'f', 1);
	  p.drawPolyline(numberContour);
          p.setFont(font);
	  p.drawText((*widget).x + (*widget).w - fm->width(sv) - 24,
	             (*widget).y + fm->height() + 12,
		     sv,
		     sv.length());

	  break;

	case PD_TEXT:
          p.setFont(font);
	  p.drawText((*widget).x + 12,
	             (*widget).y + fm->height() + 12,
		     (*widget).name,
		     strlen((*widget).name));
	  break;

	default:
	  break;
      }

      p.restore();
    }
  }
  else // Standard interface
  {
    // Draw filled ellipse
    p.setBrush(brush);
    p.drawEllipse(screenWidth / 2 - 2 * screenHeight / 5,
                  screenHeight / 10,
		  4 * screenHeight / 5,
		  4 * screenHeight / 5);

    if(buttonMenu.pressed)
    {
      p.setPen(gray);
      brush.setColor(gray);
      p.setBrush(brush);
      p.drawEllipse(screenWidth / 2 - screenHeight / 8,
                    screenHeight / 4 - screenHeight / 8,
		    screenHeight / 4,
		    screenHeight / 4);
    }

    if(buttonRewind.pressed)
    {
      p.setPen(gray);
      brush.setColor(gray);
      p.setBrush(brush);
      p.drawEllipse(screenWidth / 3 - screenHeight / 8,
                    screenHeight / 2 - screenHeight / 8,
		    screenHeight / 4,
		    screenHeight / 4);
    }

    if(buttonForward.pressed)
    {
      p.setPen(gray);
      brush.setColor(gray);
      p.setBrush(brush);
      p.drawEllipse(2 * screenWidth / 3  + 1 - screenHeight / 8,
                    screenHeight / 2 - screenHeight / 8,
		    screenHeight / 4,
		    screenHeight / 4);
    }

    if(buttonPlay.pressed)
    {
      p.setPen(gray);
      brush.setColor(gray);
      p.setBrush(brush);
      p.drawEllipse(screenWidth / 2 - screenHeight / 8,
                    3 * screenHeight / 4 - screenHeight / 8,
		    screenHeight / 4,
		    screenHeight / 4);
    }

    if(buttonAction.pressed)
    {
      p.setPen(gray);
      brush.setColor(gray);
      p.setBrush(brush);
    }
    else
    {
      p.setPen(lightGray);
      brush.setColor(lightGray);
      p.setBrush(brush);
    }
    p.drawEllipse(screenWidth / 2 - screenHeight / 8,
                  screenHeight / 2 - screenHeight / 8,
		  screenHeight / 4,
		  screenHeight / 4);

    p.setPen(black);
    QString sv(QString("%1").arg(scrollValue));
    p.setFont(font);
    p.drawText(screenWidth / 2 - fm->width(sv) / 2,
               screenHeight / 2 + fm->height() / 2,
	       sv,
	       sv.length());
  }

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
  QString fileName;

#ifdef USE_NATIVE_FILEDIALOGS
  fileName = QFileDialog::getOpenFileName(patchDirectory,
                                          "PureData patches (*.pd);;" \
					  "All files (*.*)",
					  this,
					  "open file dialog",
					  "Load PureData patches");
#else
  QTKFileDialog fileDialog(this, "Load PureData patches");

  fileDialog.setFilter("PureData patches|*.pd|" \
                       "All files|*.*");

  fileDialog.setHomeDirectoryCdUpEnable(true);

  // Set directory
  fileDialog.setDirectory(patchDirectory);

  fileDialog.showMaximized();

  if(fileDialog.exec() == QDialog::Accepted)
  {
     fileName  = fileDialog.getFileName();
  }
#endif

  if(fileName.length() > 0)
    load(fileName.latin1());

  // Repaint whole screen
  repaint(false);
}

/** Load PD patch. */
void PDQt::load(const char* fileName)
{
  QFile f(fileName);
  QFileInfo fi(f);
  QTextStream t(&f);
  QString line;
  QStringList parameters;

  // Set patch name and directory of the patch
  patch = fileName;
  fi = QFileInfo(patch);
  patchDirectory = fi.dirPath();

  // Open patch file
  if(!f.open(IO_ReadOnly))
    return;

  // Reset status
  loaded = false;

  // Clear widgets
  widgets.clear();

  while(!t.atEnd())
  {
    // PD widget parameters
    enum WIDGETID type = PD_TEXT;
    char name[128];
    int x = 0,
        y = 0,
        w = 0,
        h = 0;
    int min = 0,
        max = 0;
    float value = 0;

    // Reading line
    line = t.readLine();

    // Check for empty lines
    if(line.isEmpty())
      continue;

    // Parse line

    // Type of widget?
    if(line.contains("floatatom") && line.contains("pod_")) type = PD_NUMBER;
    else if(line.contains("symbolatom") && line.contains("pod_")) type = PD_SYMBOL;
    else if(line.contains("vsl") && line.contains("pod_")) type = PD_VSLIDER;
    else if(line.contains("hsl") && line.contains("pod_")) type = PD_HSLIDER;
    else if(line.contains("vradio") && line.contains("pod_")) type = PD_VRADIO;
    else if(line.contains("hradio") && line.contains("pod_")) type = PD_HRADIO;
    else if(line.contains("bng") && line.contains("pod_")) type = PD_BANG;
    else if(line.contains("text")) type = PD_TEXT;
    // Type unknown, continue with the next line
    else continue;

    // Split line into list of parameters
    parameters = QStringList::split(' ', line.stripWhiteSpace());

    // Parse parameters
    // QStringList::split() takes the first token in too -- different to strtok()!
    switch(type)
    {
      case PD_NUMBER:
      case PD_SYMBOL:
        x = (int) (parameters[2].toInt() * screenMultiplier);
	y = (int) (parameters[3].toInt() * screenMultiplier);
	w = (int) (parameters[4].toInt() * 7 * screenMultiplier);
	h = (int) (                       16 * screenMultiplier);
	strncpy(name, parameters[9].latin1(), 128-1);
	break;

      case PD_VSLIDER:
      case PD_HSLIDER:
        x = (int) (parameters[2].toInt() * screenMultiplier);
        y = (int) (parameters[3].toInt() * screenMultiplier);
        w = (int) (parameters[5].toInt() * screenMultiplier);
        h = (int) (parameters[6].toInt() * screenMultiplier);
	min = parameters[7].toInt();
	max = parameters[8].toInt();
	strncpy(name, parameters[12].latin1(), 128-1);
        break;

      case PD_VRADIO:
      case PD_HRADIO:
        x = (int) (parameters[2].toInt() * screenMultiplier);
	y = (int) (parameters[3].toInt() * screenMultiplier);
	min = 0;
	max = parameters[8].toInt();
	if(type == PD_VRADIO)
	{
	  w = (int) (parameters[5].toInt() * screenMultiplier);
	  h = w * max;
	}
	else if(type == PD_HRADIO)
	{
          h = (int) (parameters[5].toInt() * screenMultiplier);
          w = h * max;
	}
	strncpy(name, parameters[10].latin1(), 128-1);
	max--;
	break;

      case PD_BANG:
        x = (int) (parameters[2].toInt() * screenMultiplier);
	y = (int) (parameters[3].toInt() * screenMultiplier);
	w = (int) (parameters[5].toInt() * screenMultiplier);
	h = w;
	min = 0;
	max = 1;
	strncpy(name, parameters[10].latin1(), 128-1);
	break;

      case PD_TEXT:
        x = (int) (parameters[2].toInt() * screenMultiplier);
	y = (int) (parameters[3].toInt() * screenMultiplier);
	strncpy(name, parameters[4].latin1(), 128-1);
	for(int i = 5, n = 128-1; i < (int) parameters.count(); i++)
	{
	  n = n - parameters[i].length();
	  if(n < 0)
	    break;
	  strcat(name, parameters[i].latin1());
	}
	name[strlen(name)-1] = '\0';
        break;
    }

    // Add new widget
    widgets.append(PDWidget(type, name, x, y, w, h, min, max, value));
  }

  f.close();

  // Restart PD
  if(connected)
    disconnectPD();
  if(running)
    stopPD();
  startPD();
  connectPD();

  // Update status
  loaded = true;
  setCaption(QString(PDQTNAME" - %1").arg(fileName));
  status->setText("Running patch");
}

/** Show About dialog. */
void PDQt::about()
{
    QMessageBox::about(this, "About PDQt...", "Qtopia GUI for PDa\n"
                                              "(c) 2008, 2009 by Wincent Balin\n"
					      "            and Andre Beckedorf\n"
					      "PdPod: Martin Kaltenbrunner\n"
					      "PDa iPod port: Guenter Geiger\n"
					      "Pure Data (pd): Miller Puckette");
}

/** Press back action button. */
void PDQt::buttonActionBackpress()
{
  buttonAction.pressed = false;
  repaint(false);
}

/** Start PD core. */
void PDQt::startPD()
{
  running = false;

  pdPid = vfork();

  if(pdPid == 0)
  {
    running = true;
    status->setText("Running PDa");
    execl(pdPath.latin1(), "pd", "-r", "22050", "-nogui", "-rt", "-nomidi", "-noadc", patch.latin1(), NULL);
    // If exec fails display message and exit forked process
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
  status->setText("Connected to PDa");
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
  int pd_status;
  int result;

  result = kill(pdPid, SIGKILL);
  wait(&pd_status);

  running = false;
  status->setText("Stopped PDa");
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

      if(tokens.count() > 1)
      {
        argval = atof((*(tokens.at(1))).latin1());
      }
      else
      {
        argval = 0;
      }

      for(QValueList<PDWidget>::Iterator widget = widgets.begin(); widget != widgets.end(); widget++)
      {
        if(strncmp((*tokens.at(0)).latin1(), (*widget).name, strlen((*widget).name)) == 0)
	{
	  if((*widget).type != PD_NUMBER)
	  {
	    if(argval > (*widget).max) argval = (*widget).max;
	    if(argval < (*widget).min) argval = (*widget).min;
	  }

	  if((*widget).type == PD_BANG)
	    (*widget).value = 1;
	  else
	    (*widget).value = argval;
	}
      }

      updateGUI = true;
    }
  }

  if(updateGUI)
    repaint(false);
}

/** Widget builder (pattern of the same name). */
bool PDQt::createWidget(QString& line, BaseWidget& widget)
{
  // Split line to tokens
  QStringList tokens = QStringList::split(' ', line.stripWhiteSpace());

  // Check whether line contains widget information; if so, create it
  if(line.contains("floatatom") && line.contains("pod_"))
    widget = NumberWidget(tokens, screenMultiplier, font, fm);
  else if(line.contains("symbolatom") && line.contains("pod_"))
    widget = SymbolWidget(tokens, screenMultiplier);
  else if(line.contains("vsl") && line.contains("pod_"))
    widget = VerticalSliderWidget(tokens, screenMultiplier);
  else if(line.contains("hsl") && line.contains("pod_"))
    widget = HorizontalSliderWidget(tokens, screenMultiplier);
  else if(line.contains("vradio") && line.contains("pod_"))
    widget = VerticalRadioWidget(tokens, screenMultiplier);
  else if(line.contains("hradio") && line.contains("pod_"))
    widget = HorizontalRadioWidget(tokens, screenMultiplier);
  else if(line.contains("bng") && line.contains("pod_"))
    widget = BangWidget(tokens, screenMultiplier);
  else if(line.contains("text"))
    widget = TextWidget(tokens, screenMultiplier, font, fm);
  else return false;

  // Widget created
  return true;
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
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}

