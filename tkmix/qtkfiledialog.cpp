/* -*- mode: c++; tab-width: 4; c-basic-offset: 4 -*- */

// ----------------------------------------
// QTK File Dialog 0.4.1 (License Free.)
// Copyright (C) 2004 by TKmix
// http://tkmixi.s58.xrea.com/
// compile: gcc 2.95.1
//
// Lots of changes by Andre Beckedorf
//			<evilJazz _AT_ katastrophos _DOT_ net>
// ----------------------------------------
#include <qapplication.h>
#include <qdialog.h>
#include <qtextcodec.h>
#ifdef QTOPIA
#include <qpe/resource.h>
#else
//#include <resource.h>
#include "../compat/resource.h"
#endif
#include <qmessagebox.h>
#include <qaccel.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include "qtkfiledialog.h"

#define TFD_NAME	QString("TkFileDialog")
#define TFD_NOTFILES	QString("/NotAvailable")
// ----------------------------------------
// Setting
// keymap
static int TFD_LNC_NORMAL[]	= { 4, 1, 10, 0, 2, 1, 10, 0, 2, 1, 10, 0, 2, 1, 10, 0 };
static int TFD_LNC_DIR[]	= { 11, 5, 0, 10, 11, 1, 0, 10, 11, 2, 0, 10, 11, 2, 0, 10 };
static int TFD_LNC_SAVE[]	= { 6, 1, 0, 0, 3, 1, 0, 0, 3, 1, 2, 0, 3, 1, 2, 0 };
static int TFD_LNC_SAVEDIR[]= { 11, 5, 0, 0, 11, 1, 0, 0, 11, 3, 0, 2, 11, 3, 0, 2 };
// drivesetting
static QString TFD_HOMEDIRECTORY[]	= { QDir::homeDirPath(), "/mnt/card", "/mnt/cf", "/mnt/cf2", "/mnt/usbstorage", "/mnt/net", "/hdd3" };
static QString TFD_DRIVECAPTION[]= { "Main Memory", "SD Card", "CF Card1", "CF Card2", "USB Storage", "Network", "Internal Drive" };
static QString TFD_DRIVEPIXMAP[]	= { "MainDeviceS", "SDDeviceS", "CFDeviceS", "CFDeviceS", "slfolder", "slfolder", "CFDeviceS" };
// default file filter
#define TFD_DEF_FILTER	"All Files(*.*)|*"
// interface caption
#define TFD_CAP_BTNOPEN	"Open"
#define TFD_CAP_BTNSAVE	"Save"
#define TFD_CAP_BTNCANCEL	"Cancel"
// Pixmap(icon)
#define TFD_PIXMAP_FILE	"slUnknown14"
#define TFD_PIXMAP_FOLDER	"slfolder"
#define TFD_PIXMAP_FOLDERUP	"folderup"
#define TFD_PIXMAP_FOLDERNEW "foldernew"
// ----------------------------------------
// Message
#define TFD_MSG_NOTFILES		"Could not open the specified file\nor directory."
#define TFD_MSG_SELERRFILE		"A file can not be selected.\nPlease select a directory."
#define TFD_MSG_NOTSELECTED	"Please select a file first."
#define TFD_MSG_NOTUSEMKDIR	"The directory could not be\ncreated. Perhaps you are\nmissing the necessary permissions."
#define TFD_MSG_NOINPUT		"Please enter a name\nfor the new directory."
#define TFD_MSG_NEWDIRECTORY	"Please enter the name\nof the new directory:"
#define TFD_MSG_SLASHERR		"The directory name can not\ncontain a / sign.\nPlease remove it and try again."
#define TFD_MSG_REWRITE		"The file already exists.\nDo you want to overwrite it?"

// ----------------------------------------
// TFDInputDlg - for TK File Dialog
TFDInputDlg::TFDInputDlg(QWidget *parent, const char *name): QDialog(parent, name, true, 0)
{
	setCaption("Create new directory");
	if ( !name ) setName(TFD_NAME + "input");
#ifdef QTOPIA	
	setWState(WState_Reserved1);
#endif
	edtInput = new QLineEdit(this);
	connect(edtInput, SIGNAL(returnPressed()), this, SLOT(edtFileName_returnPressed()));
	QPushButton *btnOK = new QPushButton("OK", this);
	connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	QPushButton *btnCancel = new QPushButton("Cancel", this);
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

	vbox = new QVBoxLayout(this, 10, 10);
	vbox->addWidget(new QLabel(tr(TFD_MSG_NEWDIRECTORY), this));
	vbox->addWidget(edtInput);
	hbox = new QHBoxLayout(vbox, 10);
	hbox->addWidget(btnOK);
	hbox->addWidget(btnCancel);
}

TFDInputDlg::~TFDInputDlg()
{

}

void TFDInputDlg::closeEvent( QCloseEvent *e )
{
	e->accept();
}

void TFDInputDlg::accept()
{
	if ( edtInput->text().contains('/') != 0 )
	{
		QMessageBox::warning(this, 0, tr(TFD_MSG_SLASHERR));
		return;
	}

	if ( edtInput->text().isEmpty() )
	{
		QMessageBox::warning(this, 0, tr(TFD_MSG_NOINPUT));
		return;
	}

	sInput = edtInput->text();
	emit done( Accepted );
}

// ----------------------------------------
// TK File Dialog - constructor and ...
QTKFileDialog::QTKFileDialog(QWidget *parent, const char *name): QDialog(parent, name, true, 0)
{
	// ----------------------------------------
	// Initialize
#ifdef QTOPIA	
	setWState(WState_Reserved1);
#endif
	if ( !name ) setName(TFD_NAME);
	bInit = false;
	qds.setSorting(QDir::DirsFirst|QDir::Name |QDir::IgnoreCase);
	qds.setMatchAllDirs(true);
	setSaveMode(false);
	setSaveMode_ReWriteMessageDisabled(false);
	setSaveMode_SaveMkDir(false);
	setDirectorySelectMode(false);
	setDirectory("");
	setSetDirectoryMkDir(false);
	setDirectoryDisabled(false);
	setMkDirButtonDisabled(false);
	setSystemFile(false);
	setHiddenFile(false);
	setMemoryCardDisabled(false);
	setFilter("");
	setHomeDirectoryCdUpEnable(false);
	setCdUpUpDisabled(false);

	// ----------------------------------------
	// Keyboard
	QAccel *qac = new QAccel(this);
	qac->connectItem(qac->insertItem(Key_Backspace), this, SLOT(Keyboard_BackspaceKey()));
	QAccel *qacHome = new QAccel(this);
	qacHome->connectItem(qacHome->insertItem(Key_Home), this, SLOT(Keyboard_HomeKey()));
	QAccel *qacEnd = new QAccel(this);
	qacEnd->connectItem(qacEnd->insertItem(Key_End), this, SLOT(Keyboard_EndKey()));
	QAccel *qacLeft = new QAccel(this);
	qacLeft->connectItem(qacLeft->insertItem(Key_Left), this, SLOT(Keyboard_LeftKey()));
	QAccel *qacRight = new QAccel(this);
	qacRight->connectItem(qacRight->insertItem(Key_Right), this, SLOT(Keyboard_RightKey()));

	// ----------------------------------------
	// hboxTop

	// �ɥ饤��
	cmbDrive = new QComboBox(this);
	connect(cmbDrive, SIGNAL(activated(int)), this, SLOT(cmbDrive_Activated()));

	// �����ե����
	btnMkDir = new QPushButton(this);
	btnMkDir->setPixmap(Resource::loadPixmap(TFD_PIXMAP_FOLDERNEW));
	btnMkDir->setAutoDefault(FALSE);
	connect(btnMkDir, SIGNAL(clicked()), this, SLOT(btnMkDir_Click()));

	// ��p�Υǥ��쥯�ȥ��ɽ��
	btnDirUp = new QPushButton(this);
	btnDirUp->setPixmap(Resource::loadPixmap(TFD_PIXMAP_FOLDERUP));
	btnDirUp->setAutoDefault(FALSE);
	connect(btnDirUp, SIGNAL(clicked()), this, SLOT(btnDirUp_Click()));

	// �ե�����ΰ���
	lstFiles = new QListBox(this);
	connect(lstFiles, SIGNAL(highlighted(const QString &)), this, SLOT(lstFiles_Highlighted(const QString &)));
	connect(lstFiles, SIGNAL(clicked(QListBoxItem *)), this, SLOT(lstFiles_Clicked()));
	connect(lstFiles, SIGNAL(doubleClicked(QListBoxItem *)), this, SLOT(lstFiles_Clicked()));
	connect(lstFiles, SIGNAL(returnPressed(QListBoxItem *)), this, SLOT(lstFiles_returnPressed()));

	// ----------------------------------------
	// hboxBottom1

	// Left
	edtFileName = new QLineEdit(this);
	connect(edtFileName, SIGNAL(textChanged(const QString &)), this, SLOT(edtFileName_textChanged(const QString &)));
	connect(edtFileName, SIGNAL(returnPressed()), this, SLOT(edtFileName_returnPressed()));
	btnOK = new QPushButton(0, this);
	btnOK->setAutoDefault(FALSE);
	btnOK->setFixedWidth(90);
	connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));

	// ----------------------------------------
	// hboxBottom2
	cmbFilter = new QComboBox(this);
	connect(cmbFilter, SIGNAL(activated(int)), this, SLOT(cmbFilter_Activated(int)));
	btnCancel = new QPushButton(0, this);
	btnCancel->setAutoDefault(FALSE);
	btnCancel->setFixedWidth(90);
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

	// ----------------------------------------
	// vbox - top layout
	vbox = new QVBoxLayout(this);
	hboxTop = new QHBoxLayout(vbox);
	hboxTop->addWidget(cmbDrive, 1);
	hboxTop->addWidget(btnMkDir);
	hboxTop->addWidget(btnDirUp);
	vbox->addWidget(lstFiles);
	hboxBottom1 = new QHBoxLayout(vbox);
	hboxBottom1->addWidget(edtFileName, 1);
	hboxBottom1->addWidget(btnOK);
	hboxBottom2 = new QHBoxLayout(vbox);
	hboxBottom2->addWidget(cmbFilter, 1);
	hboxBottom2->addWidget(btnCancel);
	vbox->addSpacing(1);
}

QTKFileDialog::~QTKFileDialog()
{
	bInit = false;
}

void QTKFileDialog::showEvent( QShowEvent * )
{
	if (!bInit) initMode();
}

void QTKFileDialog::closeEvent( QCloseEvent *e )
{
	e->accept();
}

void QTKFileDialog::accept()
{
	LaunchEvent(4);
}

// ----------------------------------------
// ����ؿ� - ���
void QTKFileDialog::initMode()
{
	btnMkDir->setEnabled(!p_bMkDirButton);

	btnCancel->setText(tr(TFD_CAP_BTNCANCEL));
	if (p_bSaveMode) btnOK->setText(tr(TFD_CAP_BTNSAVE));
	else btnOK->setText(tr(TFD_CAP_BTNOPEN));

	bool bNamesExists = (!p_sNames.isEmpty() && qds.exists(p_sNames));
	if (!bNamesExists) if (!p_sNames.isEmpty() && p_sNames.left(1) != "/") p_sNames = "/" + p_sNames;

	int index = 0;
	for(int i = 0; i < TFD_DRIVECOUNT; ++i)
	{
		if ((i == 1) && (p_bMemCard)) break;
		if (QFileInfo(TFD_HOMEDIRECTORY[i] + TFD_NOTFILES).exists() ||
			!QFileInfo(TFD_HOMEDIRECTORY[i]).exists()) {
			sRootPath[i] = TFD_HOMEDIRECTORY[i] + TFD_NOTFILES;
			indexRootPathToCmbDrive[i] = 0;
			indexCmbDriveToRootPath[i] = i;
			continue;
		}
		cmbDrive->insertItem(Resource::loadPixmap(TFD_DRIVEPIXMAP[i]), tr(TFD_DRIVECAPTION[i]));
		indexRootPathToCmbDrive[i] = index;
		indexCmbDriveToRootPath[index] = i;
		sRootPath[i] = TFD_HOMEDIRECTORY[i];
		if (bNamesExists && i == getHomeDirectoryInitialPos(p_sNames)) sRootPath[i] = p_sNames;
		else sRootPath[i] += p_sNames;
		if (p_bSetDirMkDir) setForceMkDir(sRootPath[i]);
		if (!qds.exists(sRootPath[i])) sRootPath[i] = TFD_HOMEDIRECTORY[i];
		sRootPath[i] = qds.absFilePath(sRootPath[i]);
		++index;
	}

	// ----------------------------------------
	// Finalize
	if (p_sFilter.contains("|") == 0) p_sFilter = tr(TFD_DEF_FILTER);
	initFilter(p_sFilter, (bNamesExists)?p_sNames:sRootPath[0]);
	bInit = true;
#ifdef QTOPIA
	showMaximized();
#endif
}

void QTKFileDialog::initFilter(const QString s, const QString dir)
{
	initQDirFilter();
	int iCount = s.contains("|") + 1;

	for(int i = 1; i <= (iCount/2); ++i)
	{
		cmbFilter->insertItem(Resource::loadPixmap(TFD_PIXMAP_FILE), getStringSeparate(s, (i*2)-1, "|"));
	}
	qds.cd(dir);
	cmbFilter_Activated(0);
}

void QTKFileDialog::initQDirFilter()
{
	int i = 0;
	if (p_bHiddenFile) i |= QDir::Hidden;
	if (p_bSystemFile) i |= QDir::System;
	if (p_bDirMode)
	{
		i |= QDir::Dirs;
	} else {
		i |= QDir::Files;
		if (!p_bDirDis) i |= QDir::Dirs;
	}
	qds.setFilter(i);
}

// ----------------------------------------
// ����ؿ� - ���٥��)��
void QTKFileDialog::LaunchEvent(const int ID)
{
	switch(ID)
	{
		case 1 ... 2:
			if (lstFiles->currentItem() == -1)
			{
				QMessageBox::warning(this, 0, tr(TFD_MSG_NOTSELECTED));
				return;
			}
			p_sSelFileName = lstFiles->currentText();
			break;

		case 3:
			if (edtFileName->text().isEmpty())
			{
				QMessageBox::warning(this, 0, tr(TFD_MSG_NOINPUT));
				return;
			}
			break;

		case 4:
			break;

		default:
			break;
	}

	int *p;
	if ( p_bSaveMode && p_bDirMode ) p = TFD_LNC_SAVEDIR;
	else if ( p_bSaveMode ) p = TFD_LNC_SAVE;
	else if ( p_bDirMode ) p = TFD_LNC_DIR;
	else p = TFD_LNC_NORMAL;

	const int iOffsetPos = ((ID - 1) * 4);
	LaunchCommand(getDirAndFileNameAppend(getCurrentDir(), p_sSelFileName), p[iOffsetPos+0], p[iOffsetPos+1], p[iOffsetPos+2], p[iOffsetPos+3]);
}

void QTKFileDialog::LaunchCommand(const QString FileName, const int FileCommand, const int DirCommand, const int NotFileCommand, const int NotDirCommand)
{
	QFileInfo fi(FileName);
	int iCommand = 0;

	if (fi.exists()) iCommand = (fi.isDir())?DirCommand:FileCommand;
	else iCommand = (NotDirCommand != 0)?NotDirCommand:NotFileCommand;

	switch(iCommand)
	{
		case 1: // CD
			setCurrentDir(FileName);
			break;
		case 2: // OK
			launchAccept(fi);
			break;
		case 3: // RW
			launchReWriteMessage(fi);
			break;
		case 4: // INSL
			if (edtFileName->text() == lstFiles->currentText()) launchAccept(fi);
			else launchSelectedTextToEdit();
			break;
		case 5: // INCD
			if (edtFileName->text() == lstFiles->currentText()) setCurrentDir(FileName);
			else launchSelectedTextToEdit();
			break;
		case 6: // INRW
			if (edtFileName->text() == lstFiles->currentText()) launchReWriteMessage(fi);
			else launchSelectedTextToEdit();
			break;
		case 10: // Err1
			QMessageBox::warning(this, 0, tr(TFD_MSG_NOTFILES));
			break;
		case 11: // Err2
			QMessageBox::warning(this, 0, tr(TFD_MSG_SELERRFILE));
			break;
		default: return;
	}
}

void QTKFileDialog::launchAccept(const QFileInfo FileData)
{
	p_sSelFileName = FileData.fileName();
	p_sFileName = FileData.absFilePath();
	p_iFilterIndex = cmbFilter->currentItem();
	if (p_bSaveMode && p_bSaveModeMkDir) setForceMkDir(p_sFileName);
	emit done( Accepted );
}

void QTKFileDialog::launchSelectedTextToEdit()
{
	edtFileName->setText(lstFiles->currentText());
}

void QTKFileDialog::launchReWriteMessage(const QFileInfo FileData)
{
	if (!p_bSaveModeReWrite)
	{
		if (FileData.exists())
		{
			if ( QMessageBox::information(this, 0, tr(TFD_MSG_REWRITE), 
			QMessageBox::Yes , QMessageBox::No | QMessageBox::Escape | QMessageBox::Default ) != QMessageBox::Yes) return;
		}
	}
	launchAccept(FileData);
}

// ----------------------------------------
// ����ؿ�
void QTKFileDialog::setCurrentDir(const QString dir)
{
	bool dirUp = dir == "..";
	QString oldDir = qds.absPath();
	
	if (!qds.cd(dir))
		return;
	
	if (lstFiles->count() != 0)
		lstFiles->clear();

	// ----------------------------------------
	// CdUp Items Insert Check
	bool bDirUpShow = false;
	if (!_bCdUp && !qds.isRoot())
	{
		bDirUpShow = true;
		if (!_bHomeCdUp)
		{
			int ic = (p_bMemCard) ? 1 : TFD_DRIVECOUNT;
			for(int i = 0; i < ic; ++i)
			{
				if ( getCurrentDir() == TFD_HOMEDIRECTORY[i] )
				{
					bDirUpShow = false;
					break;
				}
			}
		}
	}

	btnDirUp->setEnabled(bDirUpShow);
	if (bDirUpShow) 
		lstFiles->insertItem(Resource::loadPixmap(TFD_PIXMAP_FOLDERUP), "..");
	
	int oldRelDirIndex = 0;

	QString oldRelDir;
	if (dirUp)
		oldRelDir = oldDir.mid(qds.absPath().length() + 1);

	// ----------------------------------------
	// FileList Insert
	const QFileInfoList *lst = qds.entryInfoList();
	if(lst)
	{
		QFileInfoListIterator it(*lst);
		QFileInfo *fi;
		while( ( fi = it.current() ) != 0 )
		{
			++it;
			if( !(fi->fileName() == "." || fi->fileName() == "..") )
			{
				if (fi->isDir())
				{
					if (!p_bDirDis)
					{
						lstFiles->insertItem(Resource::loadPixmap(TFD_PIXMAP_FOLDER), fi->fileName());
						if (dirUp && fi->fileName() == oldRelDir)
							oldRelDirIndex = lstFiles->count() - 1;
					}
				} else {
					lstFiles->insertItem(Resource::loadPixmap(TFD_PIXMAP_FILE), fi->fileName());
				}
			}
		}
	}

	// ----------------------------------------
	// Fin.
	setCaption(getCurrentDir());
	cmbDrive->setCurrentItem(indexRootPathToCmbDrive[getHomeDirectoryInitialPos(getCurrentDir())]);
	edtFileName->setText("");
	lstFiles->setFocus();
	if (lstFiles->count() != 0) 
		lstFiles->setSelected(oldRelDirIndex, true);
}

void QTKFileDialog::setForceMkDir(const QString dir)
{
	int iPos = 0;
	QDir _qd;
	while (1)
	{
		iPos = dir.find("/", iPos, false);
		if (iPos == -1)
		{
			if (!_qd.exists(dir)) if (!_qd.mkdir(dir)) return;
			break;
		} else {
			if (!_qd.exists(dir.left(iPos+1))) if (!_qd.mkdir(dir.left(iPos+1))) return;
			++iPos;
		}
	}
}

int QTKFileDialog::getHomeDirectoryInitialPos(const QString SearchStr)
{
	int iPos = 0;

	if (!p_bMemCard)
	{
		for(int i = 0; i < TFD_DRIVECOUNT; i++)
		{
			if ( SearchStr.find(QRegExp("^" + TFD_HOMEDIRECTORY[i]), 0) != -1 )
			{
				iPos = i;
				break;
			}
		}
	}

	return iPos;
}


// ----------------------------------------
//  SLOT
void QTKFileDialog::Keyboard_BackspaceKey()
{
	if (edtFileName->hasFocus()) edtFileName->backspace();
	else btnDirUp_Click();
}

void QTKFileDialog::Keyboard_HomeKey()
{
	if (edtFileName->hasFocus()) edtFileName->home(false);
	else cmbDrive_Previous();
}

void QTKFileDialog::Keyboard_EndKey()
{
	if (edtFileName->hasFocus()) edtFileName->end(false);
	else cmbDrive_Next();
}

void QTKFileDialog::Keyboard_LeftKey()
{
	if (lstFiles->hasFocus() && btnDirUp->isEnabled())
		setCurrentDir("..");
}

void QTKFileDialog::Keyboard_RightKey()
{
	if (lstFiles->hasFocus())
		LaunchEvent(2);
}

void QTKFileDialog::cmbDrive_Activated()
{
	setCurrentDir(sRootPath[indexCmbDriveToRootPath[cmbDrive->currentItem()]]);
}

void QTKFileDialog::cmbDrive_Previous()
{
	int index = cmbDrive->currentItem();
	if (index == 0)
		index = cmbDrive->count() - 1;
	else
		--index;

	cmbDrive->setCurrentItem(index);
	setCurrentDir(sRootPath[indexCmbDriveToRootPath[index]]);
}

void QTKFileDialog::cmbDrive_Next()
{
	int index = cmbDrive->currentItem();
	if (index == cmbDrive->count() - 1)
		index = 0;
	else
		++index;

	cmbDrive->setCurrentItem(index);
	setCurrentDir(sRootPath[indexCmbDriveToRootPath[index]]);
}

void QTKFileDialog::btnDirUp_Click()
{
	if (btnDirUp->isEnabled()) setCurrentDir("..");
}

void QTKFileDialog::btnMkDir_Click()
{
	// ���¥e��å�
	QFileInfo fi(getCurrentDir());
	if (!fi.isWritable())
	{
		QMessageBox::warning(this, 0, tr(TFD_MSG_NOTUSEMKDIR));
		return;
	}

	TFDInputDlg dlg(this, 0);
	if (dlg.exec());
	{
		qds.mkdir(dlg.getInput());
		setCurrentDir(getCurrentDir());
	}
}

void QTKFileDialog::lstFiles_Highlighted(const QString &s)
{
	p_sSelFileName = s;
}

void QTKFileDialog::lstFiles_Clicked()
{
	LaunchEvent(1);
}

void QTKFileDialog::lstFiles_returnPressed()
{
	LaunchEvent(2);
}

void QTKFileDialog::edtFileName_textChanged(const QString &s)
{
	p_sSelFileName = s;
}

void QTKFileDialog::edtFileName_returnPressed()
{
	LaunchEvent(3);
}

void QTKFileDialog::cmbFilter_Activated(int index)
{
	QString s = edtFileName->text();
	qds.setNameFilter(getStringSeparate(p_sFilter, ((index+1)*2), "|"));
	setCurrentDir("./");
	edtFileName->setText(s);
}

// ----------------------------------------
// ���Ѵؿ�
QString QTKFileDialog::getDirAndFileNameAppend(const QString dir, const QString filename)
{
	return qds.cleanDirPath(dir + "/" + filename);
}

QString QTKFileDialog::getStringSeparate(const QString s, const int ID, const QString Delimiter)
{
	QString sData = s;
	int iPos = 0, iPastpos = 0, iCount = 0;
	bool bCheck = false;

	iPos = sData.find(Delimiter, iPos, false);
	while(iPos >= 0)
	{
		if (iCount == ID -1)
		{
			sData = sData.mid(iPastpos, iPos - iPastpos);
			bCheck = true;
			break;
		}
		iCount++;
		iPos += Delimiter.length();
		iPastpos = iPos;
		iPos = sData.find(Delimiter, iPos, false);
	}

	if ( ! bCheck ) if (iCount == ID -1)
		sData = sData.mid(iPastpos, sData.length() - iPastpos);

	return sData;
}
