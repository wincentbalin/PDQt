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
#ifndef QTKFILEDIALOG_LIB_H
#define QTKFILEDIALOG_LIB_H
#include <qvariant.h>
#include <qdialog.h>
#include <qdir.h>
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QListBox;
class QListBoxItem;
class QPushButton;
class QLabel;
class QLineEdit;

class TFDInputDlg: public QDialog
{
Q_OBJECT
public:
	TFDInputDlg(QWidget *parent = 0, const char *name = 0);
	~TFDInputDlg();
	QString getInput() { return sInput; }

protected:
	void closeEvent( QCloseEvent * );

protected slots:
	void accept();

private slots:
	void edtFileName_returnPressed() { accept(); }

private:
	QVBoxLayout	*vbox;
	QHBoxLayout	*hbox;
	QLineEdit	*edtInput;
	QString	sInput;
};

#define TFD_DRIVECOUNT	7

class QTKFileDialog: public QDialog
{
Q_OBJECT

public:
	QTKFileDialog(QWidget *parent = 0, const char *name = 0);
	~QTKFileDialog();
	QString getCurrentDir() { return qds.absPath(); }
	QString getFileName(const bool b = false) { if (b) return p_sSelFileName; else return p_sFileName; }
	int getFilterIndex() { return p_iFilterIndex; }
	// Property
	void setSaveMode(const bool b) { p_bSaveMode = b; }
	void setSaveMode_ReWriteMessageDisabled(const bool b) { p_bSaveModeReWrite = b; }
	void setSaveMode_SaveMkDir(const bool b) { p_bSaveModeMkDir = b; }
	void setDirectorySelectMode(const bool b) { p_bDirMode = b; }
	void setDirectory(const QString d) { p_sNames = d; }
	void setSetDirectoryMkDir(const bool b) { p_bSetDirMkDir = b; }
	void setDirectoryDisabled(const bool b) { p_bDirDis = b; }
	void setMkDirButtonDisabled(const bool b) { p_bMkDirButton = b; }
	void setSystemFile(const bool b) { p_bSystemFile = b; }
	void setHiddenFile(const bool b) { p_bHiddenFile = b; }
	void setMemoryCardDisabled(const bool b) { p_bMemCard = b; }
	void setFilter(const QString s) { p_sFilter = s; }
	void setHomeDirectoryCdUpEnable(const bool b) { _bHomeCdUp = b; }
	void setCdUpUpDisabled(const bool b) { _bCdUp = b; }
	//static
	static void setForceMkDir(const QString dir);
	static QString getStringSeparate(const QString s, const int ID, const QString Delimiter);

protected:
	void closeEvent( QCloseEvent * );
	void showEvent( QShowEvent * );

protected slots:
	void accept();

private slots:
	void Keyboard_BackspaceKey();
	void Keyboard_HomeKey();
	void Keyboard_EndKey();
	void Keyboard_LeftKey();
	void Keyboard_RightKey();
	void cmbDrive_Activated();
	void cmbDrive_Previous();
	void cmbDrive_Next();
	void btnDirUp_Click();
	void btnMkDir_Click();
	void lstFiles_Highlighted(const QString &s);
	void lstFiles_Clicked();
	void lstFiles_returnPressed();
	void edtFileName_textChanged(const QString &s);
	void edtFileName_returnPressed();
	void cmbFilter_Activated(int index);

private:
	void initMode();
	void initFilter(const QString s, const QString dir);
	void initQDirFilter();
	void LaunchEvent(const int ID);
	void LaunchCommand(const QString FileName, const int FileCommand, const int DirCommand, const int NotFileCommand, const int NotDirCommand);
	void launchAccept(const QFileInfo FileData);
	void launchSelectedTextToEdit();
	void launchReWriteMessage(const QFileInfo FileData);
	void setCurrentDir(const QString dir);
	int getHomeDirectoryInitialPos(const QString SearchStr);
	QString getDirAndFileNameAppend(const QString dir, const QString filename);
	QComboBox	*cmbDrive, *cmbFilter;
	QListBox	*lstFiles;
	QLineEdit	*edtFileName;
	QPushButton	*btnMkDir, *btnDirUp, *btnOK, *btnCancel;
	QVBoxLayout	*vbox;
	QHBoxLayout	*hboxTop, *hboxBottom1, *hboxBottom2;
	QDir		qds;
	QString	p_sNames, sRootPath[TFD_DRIVECOUNT], p_sFileName, p_sSelFileName, p_sFilter;
	int indexCmbDriveToRootPath[TFD_DRIVECOUNT];
	int indexRootPathToCmbDrive[TFD_DRIVECOUNT];
	int		p_iFilterIndex;
	bool		bInit, p_bSaveMode, p_bSaveModeReWrite, p_bDirMode, p_bSetDirMkDir, p_bDirDis, p_bMkDirButton, p_bHiddenFile, p_bSystemFile, p_bMemCard, p_bSaveModeMkDir, _bCdUp, _bHomeCdUp;
};
#endif
