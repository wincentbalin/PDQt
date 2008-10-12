/****************************************************************************
** Example meta object code from reading C++ file 'example.h'
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

#include "example.h"
#include <qmetaobject.h>
#include <qapplication.h>



const char *Example::className() const
{
    return "Example";
}

QMetaObject *Example::metaObj = 0;

void Example::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(ExampleBase::className(), "ExampleBase") != 0 )
	badSuperclassWarning("Example","ExampleBase");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString Example::tr(const char* s)
{
    return qApp->translate( "Example", s, 0 );
}

QString Example::tr(const char* s, const char * c)
{
    return qApp->translate( "Example", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* Example::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) ExampleBase::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (Example::*m1_t0)();
    typedef void (QObject::*om1_t0)();
    m1_t0 v1_0 = &Example::goodBye;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "goodBye()";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"Example", "ExampleBase",
	slot_tbl, 1,
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
