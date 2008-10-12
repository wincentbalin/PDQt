/****************************************************************************
** Form interface generated from reading ui file 'examplebase.ui'
**
** Created: Fri Sep 19 15:48:24 2008
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef EXAMPLEBASE_H
#define EXAMPLEBASE_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QPushButton;

class ExampleBase : public QWidget
{ 
    Q_OBJECT

public:
    ExampleBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ExampleBase();

    QLabel* TextLabel1;
    QPushButton* quit;

protected:
    QVBoxLayout* ExampleBaseLayout;
};

#endif // EXAMPLEBASE_H
