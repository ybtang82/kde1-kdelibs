#ifndef _KWVIEV_H_
#define _KWVIEV_H_

#include <qscrbar.h>
#include <qiodev.h>
#include <qpopmenu.h>

#include <kfm.h>
#include <kconfig.h>

#include "kwdialog.h"

class KWriteDoc;
class Highlight;

//search flags
const int sfCaseSensitive   = 1;
const int sfWholeWords      = 2;
const int sfFromCursor      = 4;
const int sfBackward        = 8;
const int sfSelected        = 16;
const int sfPrompt          = 32;
const int sfReplace         = 64;
const int sfAgain           = 128;
const int sfWrapped         = 256;
const int sfFinished        = 512;
//dialog results
const int srYes             = QDialog::Accepted;
const int srNo              = 10;
const int srAll             = 11;
const int srCancel          = QDialog::Rejected;

//config flags
const int cfAutoIndent      = 1;
const int cfBackspaceIndent = 2;
const int cfWordWrap        = 4;
const int cfReplaceTabs     = 8;
const int cfRemoveSpaces    = 16;
const int cfWrapCursor      = 32;
const int cfAutoBrackets    = 64;

const int cfPersistent      = 128;
const int cfKeepSelection   = 256;
const int cfVerticalSelect  = 512;
const int cfDelOnInput      = 1024;
const int cfXorSelect       = 2048;

const int cfOvr             = 4096;
const int cfMark            = 8192;

//update flags
const int ufDocGeometry     = 1;
const int ufUpdateOnScroll  = 2;
const int ufPos             = 4;

//load flags
const int lfInsert          = 1;
const int lfNewFile         = 2;
const int lfNoAutoHl        = 4;

void resizeBuffer(void *user, int w, int h);

struct PointStruc {
  int x;
  int y;
};

struct VConfig {
  PointStruc cursor;
  int flags;
  int wrapAt;
};

struct SConfig {
  PointStruc cursor;
  PointStruc startCursor;
  int flags;
};

class KWrite;

class KWriteView : public QWidget {
    Q_OBJECT
    friend KWriteDoc;
    friend KWrite;
  public:
    KWriteView(KWrite *, KWriteDoc *);
    ~KWriteView();

    void cursorLeft(VConfig &);
    void cursorRight(VConfig &);
    void cursorUp(VConfig &);
    void cursorDown(VConfig &);
    void home(VConfig &);
    void end(VConfig &);
    void pageUp(VConfig &);
    void pageDown(VConfig &);
    void top(VConfig &);
    void bottom(VConfig &);

  protected slots:
    void changeXPos(int);
    void changeYPos(int);

  protected:
    void getVConfig(VConfig &);
    void update(VConfig &);
//    void updateCursor(PointStruc &start, PointStruc &end, bool insert);
    void insLine(int line);
    void delLine(int line);
    void updateCursor();
    void updateCursor(PointStruc &newCursor);
    void updateView(int flags, int newXPos = 0, int newYPos = 0);
//  void scroll2(int, int);
    void tagLines(int start, int end);
    void tagAll();

    void paintTextLines(int xPos, int yPos);
    void paintCursor();

    void placeCursor(int x, int y, int flags);

    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void timerEvent(QTimerEvent *);

    KWrite *kWrite;
    KWriteDoc *kWriteDoc;
    QScrollBar *xScroll;
    QScrollBar *yScroll;

    int xPos;
    int yPos;

    int mouseX;
    int mouseY;
    int scrollX;
    int scrollY;
    int scrollTimer;

    PointStruc cursor;
    bool cursorOn;
    int cursorTimer;
    int cXPos;
    int cOldXPos;
    bool exposeCursor;//cursorMoved;

    int startLine;
    int endLine;
    int updateState;
    int updateLines[2];

    QPixmap *drawBuffer;
};

class KWBookmark {
  public:
    KWBookmark();
    int xPos;
    int yPos;
    PointStruc cursor;
    QString Name;
};


class KWrite : public QWidget {
    Q_OBJECT
    friend KWriteView;
    friend KWriteDoc;
  public:
    KWrite(KWriteDoc *, QWidget *parent = 0L);
    ~KWrite();

//status functions
    int currentLine();
    int currentColumn();
    int config();
    void setConfig(int);
//    bool isOverwriteMode();
    void setModified(bool);
    bool isModified();
    bool isLastView();
    KWriteDoc *doc();
    int undoState();
    void copySettings(KWrite *);
  public slots:
    void optDlg(); //options dialog
    void colDlg(); //color dialog
    void toggleVertical();
    void toggleOverwrite();
  signals:
    void newCurPos(); //line, column changed
    void newStatus(); //modified, config flags changed
    void statusMsg(const char *);
    void newCaption();
    void newUndo();
  protected:
    int configFlags;
    int wrapAt;

//url aware file functions
  public:
    enum action{GET, PUT}; //tells us what kind of job kwrite is waiting for
    void loadFile(QIODevice &, bool insert = false);
    void writeFile(QIODevice &);
    bool loadFile(const char *name, int flags = 0);
    bool writeFile(const char *name);
    void loadURL(const char *url, int flags = 0);
    void writeURL(const char *url, int flags = 0);
  protected slots:
    void kfmFinished();
    void kfmError(int, const char *);
  public:
    const char *fileName();
    bool canDiscard();
  public slots:
    void newDoc();
    void open();
    void insertFile();
    void save();
    void saveAs();
  protected:
    KFM *kfm;
    QString kfmURL;
    QString kfmFile;
    action kfmAction;
    int kfmFlags;

//edit functions
  public:
    void clear();
  public slots:
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void indent();
    void unIndent();
    void selectAll();
    void deselectAll();
    void invertSelection();

//search/replace functions
  public slots:
    void search();
    void replace();
    void searchAgain();
    void gotoLine();
  protected:
    void initSearch(SConfig &, int flags);
    void continueSearch(SConfig &);
    void searchAgain(SConfig &);
    void replaceAgain();
    void doReplaceAction(int result, bool found = false);
    void exposeFound(PointStruc &cursor, int slen, int flags, bool replace);
    void deleteReplacePrompt();
    bool askReplaceEnd();
  protected slots:
    void replaceSlot();
  protected:
    QString searchFor;
    QString replaceWith;
    int searchFlags;
    int replaces;
    SConfig s;
    QDialog *replacePrompt;

//right mouse button popup menu
  public:
    void installRBPopup(QPopupMenu *);
  protected:
    QPopupMenu *popup;

//bookmarks
  public:
    void installBMPopup(QPopupMenu *);//KWBookPopup *);
    void setBookmark(int n);
  public slots:
    void setBookmark();
    void addBookmark();
    void clearBookmarks();
    void gotoBookmark(int n);
  protected slots:
    void updateBMPopup();
  protected:
    QList<KWBookmark> bookmarks;
    int bmEntries;

//config file / session management functions
  public:
    void readConfig(KConfig *);
    void writeConfig(KConfig *);
    void readSessionConfig(KConfig *);
    void writeSessionConfig(KConfig *);

//syntax highlight
  public:
//    void setHighlight(Highlight *);
  public slots:
    void hlDef(); //highlight defaults dialog
    void hlDlg(); //highlight setup dialog
    void setHl(int n); //set highlight (number n)

//internal
  protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

    KWriteView *kWriteView;
    KWriteDoc *kWriteDoc;
};


#endif //KWVIEV_H

