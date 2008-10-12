/****************************************************************************
** ExampleBase meta object code from reading C++ file 'examplebase.h'
**
** Created: Fri Sep 19 15:48:27 2008
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.3   edited 2001-10-17 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "examplebase.h"
#include <qmetaobject.h>
#include <qapplication.h>



const char *ExampleBase::className() const
{
    return "ExampleBase";
}

QMetaObject *ExampleBase::metaObj = 0;

void ExampleBase::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("ExampleBase","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString ExampleBase::tr(const char* s)
{
    return qApp->translate( "ExampleBase", s, 0 );
}

QString ExampleBase::tr(const char* s, const char * c)
{
    return qApp->translate( "ExampleBase", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* ExampleBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"ExampleBase", "QWidget",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
