/***
  pdqt.cpp

  Qtopia interface for Pure Data -- main file.
*/

#include "pdqt.h"

#define PDQTNAME "Pure Data for Qtopia"
#define PD_COMMAND "/usr/local/PDa/bin/pd"

#define DPRINTF(x...) fprintf(stderr, x);


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
  standardGUIFont = QFont("helvetica", 36, QFont::Bold);
  standardGUIFontMetrics = new QFontMetrics(standardGUIFont);

  // Build menu bar
  menuBar()->insertItem("&Open", this, SLOT(load()), CTRL+Key_O);
  menuBar()->insertItem("&About", this, SLOT(about()), Key_F1);

  // Get screen dimensions
  screenWidth = qApp->desktop()->width();
  screenHeight = qApp->desktop()->height();

  // Calculate size factor
  screenMultiplier = screenWidth / 160.0f;

  // Create status bar
  status = new QLabel(statusBar());
  statusBar()->addWidget(status, 2, true);

  // Print status
  status->setText("Ready");
}

/** Destructor. */
PDQt::~PDQt()
{
  // Remove status label
  statusBar()->removeWidget(status);
  delete status;
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
      {
        buttonMenu.pressed = true;
	repaint();
      }
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
	repaint();
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
      {
        buttonRewind.pressed = true;
	repaint();
      }
    }
  }
  else if(key == buttonForward.key)
  {
    if(running && !paused)
    {
      sendMessage("f 1;\n");

      if(widgets.count() == 0)
      {
        buttonForward.pressed = true;
	repaint();
      }
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

      if(widgets.count() == 0)
        repaint();
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

      if(widgets.count() == 0)
        repaint();
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
	{
	  buttonPlay.pressed = true;
	  repaint();
	}
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
      {
        buttonMenu.pressed = false;
	repaint();
      }
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
      {
        buttonRewind.pressed = false;
	repaint();
      }
    }
  }
  else if(key == buttonForward.key)
  {
    if(running && !paused)
    {
      sendMessage("f 0;\n");

      if(widgets.count() == 0)
      {
        buttonForward.pressed = false;
	repaint();
      }
    }
  }
  else if(key == buttonPlay.key)
  {
    if(running && !shift && !paused)
    {
      sendMessage("d 0;\n");

      if(widgets.count() == 0)
      {
        buttonPlay.pressed = false;
	repaint();
      }
    }
  }
  else
  {
    k->ignore();
  }
}

/** Paint GUI. */
void PDQt::paintEvent(QPaintEvent*)
{
  // If no patch loaded, nothing to draw
  if(!loaded)
    return;

  QPainter p(this);
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
	  numberContour[1] = QPoint((*widget).x + (*widget).w - 5 * screenMultiplier,
	                            (*widget).y);
          numberContour[2] = QPoint((*widget).x + (*widget).w,
	                            (*widget).y + 5 * screenMultiplier);
          numberContour[3] = QPoint((*widget).x + (*widget).w,
	                            (*widget).y + (*widget).h);
          numberContour[4] = QPoint((*widget).x,
	                            (*widget).y + (*widget).h);
	  numberContour[5] = numberContour[0];
          sv = QString::number((*widget).value, 'f', 1);
	  p.drawPolyline(numberContour);
          p.setFont(standardGUIFont);
	  p.drawText((*widget).x + (*widget).w - standardGUIFontMetrics->width(sv) - 24,
	             (*widget).y + standardGUIFontMetrics->height() + 12,
		     sv,
		     sv.length());

	  break;

	case PD_TEXT:
          p.setFont(standardGUIFont);
	  p.drawText((*widget).x + 12,
	             (*widget).y + standardGUIFontMetrics->height() + 12,
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
    p.setFont(standardGUIFont);
    p.drawText(screenWidth / 2 - standardGUIFontMetrics->width(sv) / 2,
               screenHeight / 2 + standardGUIFontMetrics->height() / 2,
	       sv,
	       sv.length());
  }
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
  QTKFileDialog fileDialog(this, "Load PureData patches");

  fileDialog.setFilter("PureData patches|*.pd|" \
                       "All files|*.*");

  fileDialog.setHomeDirectoryCdUpEnable(true);

  // Set directory
  fileDialog.showMaximized();

  if(fileDialog.exec() == QDialog::Accepted)
  {
    load(fileDialog.getFileName().latin1());
  }
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
    enum pdWidgetType type = PD_TEXT;
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
                                              "(c) 2008 by Wincent Balin\n"
					      "PdPod: Martin Kaltenbrunner\n"
					      "PDa iPod port: Guenter Geiger\n"
					      "Pure Data (pd): Miller Puckette");
}

/** Press back action button. */
void PDQt::buttonActionBackpress()
{
  buttonAction.pressed = false;
  repaint();
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
    execl(PD_COMMAND, "pd", "-r", "22050", "-nogui", "-rt", "-nomidi", "-noadc", patch.latin1(), NULL);
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

	    if((*widget).type == PD_BANG)
	      (*widget).value = 1;
	    else
	      (*widget).value = argval;
	  }
	}
      }

      updateGUI = true;
    }
  }

  if(updateGUI)
    repaint();
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
  pdqt->showMaximized();
  // Load PD patch supplied as an argument
  if(a.argc() > 1)
    pdqt->load(a.argv()[1]);
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}

