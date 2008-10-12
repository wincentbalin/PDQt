/****************************************************************************
** Form implementation generated from reading ui file 'examplebase.ui'
**
** Created: Fri Sep 19 15:48:26 2008
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "examplebase.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ExampleBase which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ExampleBase::ExampleBase( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ExampleBase" );
    resize( 196, 245 ); 
    setCaption( tr( "Example" ) );
    ExampleBaseLayout = new QVBoxLayout( this ); 
    ExampleBaseLayout->setSpacing( 6 );
    ExampleBaseLayout->setMargin( 11 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "<p>This is just an <i>example</i>. It doesn't do anything interesting at all." ) );
    ExampleBaseLayout->addWidget( TextLabel1 );

    quit = new QPushButton( this, "quit" );
    quit->setText( tr( "Quit" ) );
    ExampleBaseLayout->addWidget( quit );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ExampleBase::~ExampleBase()
{
    // no need to delete child widgets, Qt does it all for us
}

