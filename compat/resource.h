#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <qstring.h>
#include <qpixmap.h>

class Resource
{
public:
	static QPixmap loadPixmap(const QString & pix);
};

#endif /*RESOURCE_H_*/
