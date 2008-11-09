#include <qdir.h>
#include <qfile.h>
#include <qpixmapcache.h>

#include "resource.h"

QPixmap Resource::loadPixmap(const QString & pix)
{
	QString path("/usr/share/quasar/pics");
	if (!QDir(path).exists())
	{
		path = "/usr/local/share/quasar/pics";
		if (!QDir(path).exists())
			path = "pics";
	}
			
	path += "/";
	
	QString filename(path + pix + ".png"); 
	if (!QFile(filename).exists())
	{
		filename = path + pix + ".jpg"; 
		if (!QFile(filename).exists())
			filename = "";
	}
	
	if (!filename.isEmpty())
	{
		QPixmap result;
		
		if (QPixmapCache::find(filename, result))
			return result;
		else
		{
			if (result.load(filename))
				QPixmapCache::insert(filename, result);

			return result;
		}
	}
	else
		return QPixmap();
}
