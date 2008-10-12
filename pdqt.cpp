/***
  pdqt.cpp

  Qtopia interface for Pure Data -- main file.
*/

#include "pdqt.h"



char pd_patch[128];
char pd_command[64];
char patch_directory[64];
int screen_width, screen_height;
float screen_multiplier;
int scroll_value, last_scroll, scroll_diff;
char loaded, running, connected, paused, about, shift;
char action_button, forward_button, rewind_button, play_button, menu_button;
pid_t pd_pid;
int sockfd_out, sockfd_in;
int patch_id;
int menu_entries;
char directory_entries[128][64];
int last_cpu;



/** Constructor. */
PDQt::PDQt() : QMainWindow(0, PDQTNAME, WDestructiveClose)
{
  // Print status
  statusBar()->message("Starting...");

  // Create PD widgets
  pd_widgets = new QVector<PDWidget>(128);

  // Create PD canvas
  canvas = new QCanvas(630, 455);
  canvas_view = new QCanvasView(canvas, this, "PD canvas", 0);
  canvas_view->setFocus();
  setCentralWidget(canvas_view);

  // Resize window
  resize(sizeHint());

  // Print status
  statusBar()->message("Ready");

}

/** Destructor. */
PDQt::~PDQt()
{
  // Delete canvas
  delete canvas_view;
  delete canvas;

  // Delete PD widgets
  delete pd_widgets;
}

/** Processes closing events. */
void PDQt::closeEvent(QCloseEvent* ce)
{
  ce->accept();
}

/** Program entry. */
int main(int argc, char** argv)
{
  QPEApplication a(argc, argv);
  PDQt* pdqt = new PDQt();
  pdqt->setCaption(PDQTNAME);
  pdqt->show();
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}

