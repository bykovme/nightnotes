#include "nightwindow.h"
#include "ui_nightwindow.h"
#include "nightnewfile.h"

#include <qdesktopservices.h>
#include <QProcess>
//#include <QProcessEnvironment>

NightWindow::NightWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::NightWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/icon256.png"));

    currentFile = "";
    rootItem = NULL;
    lastSearchTerm = "nothing";

    loadSettings();

    ui->pushButtonAddList->setVisible(true);   // For future use
    ui->pushButtonRemoveList->setVisible(false);// for future use

    connect(ui->pushButtonAddList, SIGNAL(clicked(bool)), this, SLOT(onChangeViewClicked(bool)));

    if (searchActive) {
        ui->lineEditFind->setVisible(true);
        ui->pushButtonCancelSearch->setVisible(true);
        ui->pushButtonFind->setVisible(true);
    } else {
        ui->lineEditFind->setVisible(false);
        ui->pushButtonCancelSearch->setVisible(false);
        ui->pushButtonFind->setVisible(false);
    }

    ui->treeWidget->setStyleSheet(NightNotePrefs::getThemeBackground());
    this->setStyleSheet(NightNotePrefs::getThemeHeader());

    // Custom theme for treeview
    // Details here: http://qt-project.org/doc/qt-4.8/stylesheet-examples.html#customizing-qtreeview
    ui->treeWidget->setStyleSheet(
      "QTreeView::branch:has-children:!has-siblings:closed { border-image: url(:/images/stylesheet-branch-closed-last.png); }"
      "QTreeView::branch:closed:has-children:has-siblings { border-image: url(:/images/stylesheet-branch-closed.png); }"
      "QTreeView::branch:open:has-children:!has-siblings { border-image: url(:/images/stylesheet-branch-open-last.png);}"
      "QTreeView::branch:open:has-children:has-siblings  { border-image: url(:/images/stylesheet-branch-open.png);}"
      "QTreeView::branch:has-siblings:!adjoins-item { border-image: url(:/images/stylesheet-vline.png) 0;}"
      "QTreeView::branch:has-siblings:adjoins-item {  border-image: url(:/images/stylesheet-branch-more.png) 0;}"
      "QTreeView::branch:!has-children:!has-siblings:adjoins-item {  border-image: url(:/images/stylesheet-branch-end.png) 0;}"
      "QTreeView { background: url(:/images/background.png) 0; color:#BBBBBB; selection-background-color: #242728;padding-left:10px;}"
      "QTreeView::item {  color:gray; }"
      "QLineEdit { background-color: yellow; color:black;}"
      "QTreeView::item:selected { border:none; border-width:0px; color:white; background-color: #242728;}"
      "QTreeView::item:selected:active{ border: none; border-width:0px;color:white; background-color: #242728;}"
      "QMenu { background-color: #DDDDDD; border: 1px solid black; color:black;}"
      "QMenu::item { background-color: transparent; } "
      "QMenu::item:selected { background-color: #999999;color:black;} "
      "QScrollBar:vertical  { border: 1px solid grey; background: #000000; width: 15px; margin: 0px 0 0px 0; ;}"
      "QScrollBar::handle:vertical  { border: 1px solid black; background: grey; min-height: 20px;max-height:100px; }"
    );

    ui->lineEditFind->setStyleSheet(NightNotePrefs::getLineEditTheme());

    ui->plainTextEdit->setFocus();
    // Custom theme for PlainTextEdit
    ui->plainTextEdit->setStyleSheet(
                " QPlainTextEdit  { background: url(:/images/background.png) 0; color:#BBBBBB;padding-left:3px;padding-top:2px;padding-right:0px; }"
                "QMenu { background-color: #DDDDDD; border: 1px solid black; color:black;}"
                "QMenu::item { background-color: transparent; } "
                "QMenu::item:selected { background-color: #999999;color:black;} "
    );

    ui->labelLastChanged->setStyleSheet(
                " QLabel  { background:transparent;color:#BEBEBE;}"
                );

    ui->pushButtonFind->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonAbout->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonAdd->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonAddFolder->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonCancelSearch->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonDelete->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonFind->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonPrefs->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonRefresh->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonWrapText->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonSearch->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonAddList->setStyleSheet(BUTTON_WHITE_CSS);

    ui->textBrowser->setOpenLinks(false);
    connect(ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));

    ui->textBrowser->setStyleSheet(
        " QTextBrowser  { background-color: #222222; color:#BBBBBB;padding-left:3px;padding-top:2px;padding-right:3px; font-family: verdana, sans-serif;}"
     );

    ui->textBrowser->document()->setDefaultStyleSheet("a:link {color: #5555FF; text-decoration:none;}" );

    loadFiles();

    connect(ui->treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SLOT(itemExpanded(QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SLOT(itemCollapsed(QTreeWidgetItem*)));

    savingTimer = new QTimer(this);
    connect(savingTimer, SIGNAL(timeout()), this, SLOT(saveTimeout()));

    moveTimer = new QTimer(this);
    connect(moveTimer,SIGNAL(timeout()), this, SLOT(moveTimeout()));

    connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(currentFileChanged()));
    dirty = false;
    loadingFile = false;
    dropItemEvent = false;

    changingFileName = false;

    ui->treeWidget->setMouseTracking( true );
    ui->treeWidget->viewport()->installEventFilter(this);

    ui->pushButtonWrapText->setChecked(NightNotePrefs::wrapTextByDefault());

    // Context menu for items in the tree
    nodeMenu = new QMenu(ui->treeWidget);
    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    createFolderActionTree = new QAction(tr("Create folder"), nodeMenu);
    createNoteActionTree = new QAction(tr("Create Note"), nodeMenu);
    QAction * convert_textMD = new QAction(tr("Convert Text<->Markdown"), nodeMenu);
    deleteActionTree = new QAction(tr("Delete"), nodeMenu);
    QAction * sep = new QAction(nodeMenu);
    sep->setSeparator(true);
    showInFinderActionTree = new QAction(tr("Show in Finder..."), nodeMenu);

    ui->treeWidget->addAction(createFolderActionTree);
    ui->treeWidget->addAction(createNoteActionTree);
    ui->treeWidget->addAction(convert_textMD);
    ui->treeWidget->addAction(deleteActionTree);
    ui->treeWidget->addAction(sep);
    ui->treeWidget->addAction(showInFinderActionTree);
    connect(convert_textMD, SIGNAL(triggered()),this, SLOT(onConvertFormats()));
    connect(createFolderActionTree, SIGNAL(triggered()),this, SLOT(onCreateFolderContextClickedTree()));
    connect(createNoteActionTree, SIGNAL(triggered()),this, SLOT(onCreateNoteContextClickedTree()));
    connect(deleteActionTree, SIGNAL(triggered()),this, SLOT(onDeleteContextClickedTree()));
    connect(showInFinderActionTree, SIGNAL(triggered()),this, SLOT(onShowInFinderContextClickedTree()));
    connect(nodeMenu, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));
}

void NightWindow::onConvertFormats(){
    QFileInfo fInfo(currentFile);
    QString newFile = fInfo.fileName();


    if (currentFile.endsWith(".md", Qt::CaseInsensitive)) {
       newFile = newFile.mid(0, newFile.length() - 3) + ".txt";
    } else if (currentFile.endsWith(".txt", Qt::CaseInsensitive)) {
        newFile = newFile.mid(0, newFile.length() - 4) + ".md";
    }

    QString fullPath = fInfo.absolutePath() + QDir::separator() + newFile;
    if (QFile::copy(currentFile, fullPath) == true) {
        QFile::remove(fInfo.absoluteFilePath());
    }
    currentFile = fullPath;
    loadFiles();
}


void NightWindow::onCreateFolderContextClickedTree() {
    on_pushButtonAddFolder_clicked();
}

void NightWindow::onCreateNoteContextClickedTree() {
    on_pushButtonAdd_clicked();
}

void NightWindow::onDeleteContextClickedTree() {
    on_pushButtonDelete_clicked();
}

void NightWindow::onShowInFinderContextClickedTree() {
    QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
    if (selItems.count() == 1) {
        QFileInfo fInfo(selItems.at(0)->data(0, Qt::UserRole).toString());

        int os = -1; // 1 - mac, 0 - win
#if defined(Q_OS_WIN)
        os = 0;
#elif defined(Q_OS_MAC)
        os = 1;
#endif
        // Mac, Windows support folder or file.

        if (os == 0) {
                  /*
            const QString explorer = Env::systemEnvir .searchInPath(QLatin1String("explorer.exe"));
            if (explorer.isEmpty()) {

                QMessageBox::warning(parent,
                                     tr("Launching Windows Explorer failed"),
                                     tr("Could not find explorer.exe in path to launch Windows Explorer."));
                return;
            }
            QString param;
            if (!QFileInfo(fInfo.absoluteFilePath()).isDir())
                param = QLatin1String("/select,");
            param += QDir::toNativeSeparators(fInfo.absoluteFilePath());
            QProcess::startDetached(explorer, QStringList(param));*/
        } else if (os == 1) {
            QStringList scriptArgs;
            scriptArgs << QLatin1String("-e")
                       << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                          .arg(fInfo.absoluteFilePath());
            QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
            scriptArgs.clear();
            scriptArgs << QLatin1String("-e")
                       << QLatin1String("tell application \"Finder\" to activate");
            QProcess::execute("/usr/bin/osascript", scriptArgs);

        }
    }
}

void NightWindow::menuAboutToShow() {

}

void NightWindow::itemExpanded(QTreeWidgetItem *expandedItem) {
    QSettings appSettings;
    QString openFolders = appSettings.value("general/open_folders",  "").toString();
    QString expandedFolder = expandedItem->data(0, Qt::UserRole).toString();
    if (expandedFolder.compare(NightNotePrefs::getCurrentPath()) == 0) return; // No save action for root
    if (openFolders.contains(expandedFolder + ":") == false) { // add folder if it is not there yet
        openFolders = openFolders + expandedFolder + ":";
        appSettings.setValue("general/open_folders", openFolders);
    }
}

void NightWindow::itemCollapsed(QTreeWidgetItem *collapseItem) {
    QSettings appSettings;
    QString openFolders = appSettings.value("general/open_folders",  "").toString();
    QString collapsedFolder = collapseItem->data(0, Qt::UserRole).toString();
    if (collapsedFolder.compare(NightNotePrefs::getCurrentPath()) == 0) {
        // root should not be collapsed, expand again
        ui->treeWidget->expandItem(collapseItem);
    } else {
        // usual folders
        if (openFolders.contains(collapsedFolder + ":") == true) { // remove folder if it is there
            openFolders = openFolders.replace(collapsedFolder + ":","");
            appSettings.setValue("general/open_folders", openFolders);
        }
    }
}

bool NightWindow::eventFilter( QObject* o, QEvent* e ) {

    if( o == ui->treeWidget->viewport() && e->type() == QEvent::Drop )
    {
        // do what you would do in the slot
        QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
        if (selItems.count() == 1) {

            droppedItem = selItems.at(0);

            moveTimer->setSingleShot(true);
            moveTimer->start(200); // 0.2 seconds
            dropItemEvent = true;
            return QMainWindow::eventFilter(o,e);;
        } else {
            return QMainWindow::eventFilter(o,e);;
        }

    } else {
        return QMainWindow::eventFilter(o,e);
    }
}

void NightWindow::currentFileChanged() {
    if (loadingFile == true) return;
    if (dirty == true && savingTimer->isActive()) return;
    dirty = true;
    if (currentFile != "") {
        savingTimer->setSingleShot(true);
        savingTimer->start(5000); // 5 seconds
    }
}

void NightWindow::saveTimeout() {
    saveCurrentFile();
    savingTimer->stop();
}

void NightWindow::moveTimeout() {

    if (droppedItem != NULL) {
        QTreeWidgetItem * parentItem = droppedItem->parent();
        QFileInfo fInfo(droppedItem->data(0, Qt::UserRole).toString());
        QFileInfo fInfoParentNew(parentItem->data(0, Qt::UserRole).toString());

        if (fInfo.absolutePath().compare(fInfoParentNew.absoluteFilePath()) != 0) {

            if (fInfo.isDir() == false) {
                QFile cpFile(fInfo.absoluteFilePath());
                QString newFilePath = fInfoParentNew.absoluteFilePath() + QDir::separator() + fInfo.fileName();
                if (cpFile.copy(newFilePath) == true) {

                    if (cpFile.remove() == true) {
                        droppedItem->setData(0, Qt::UserRole, newFilePath);
                        currentFile = newFilePath;
                    }
                }
            } else {

                QDir dstFolder(fInfoParentNew.absoluteFilePath() );
                if (dstFolder.mkdir(QFileInfo(fInfo).fileName()) == true) {

                    moveFolder(fInfo.absoluteFilePath(), fInfoParentNew.absoluteFilePath() + QDir::separator() + fInfo.fileName());
                    deleteFolder(fInfo.absoluteFilePath());

                } else {
                    // folder with the same name is already existing
                }

            }
        }
    }
    droppedItem = NULL;
    loadFiles();
}

void NightWindow::saveSettings() {
    saveCurrentFile();

    QSettings appSettings;

    // Window geometry
    QRect windowGeometry = geometry();
    appSettings.setValue("gui/w", windowGeometry.width());
    appSettings.setValue("gui/h", windowGeometry.height());
    appSettings.setValue("gui/x", windowGeometry.x());
    appSettings.setValue("gui/y", windowGeometry.y());

    // Splitter divider position
    QList<int> currentSizes = ui->splitter->sizes();
    appSettings.setValue("gui/divider",  currentSizes[0]);
    // Last edited file
    if (currentFile.isEmpty() == false) {
        appSettings.setValue("general/lastfile",  currentFile);
    }

    appSettings.setValue("general/search_active", searchActive);
}

void NightWindow::loadSettings() {

    QSettings appSettings;

    this->setWindowTitle(NIGHT_NOTES);
    QDir dataDir(NightNotePrefs::getCurrentPath());
    if (dataDir.exists() == false) {
        dataDir.mkpath(NightNotePrefs::getCurrentPath());
    }

    currentFile = appSettings.value("general/lastfile",  "").toString();

    QString expandedFoldersStr = appSettings.value("general/expanded_folders", "").toString();
    if (expandedFoldersStr.isEmpty() == false) {
        expandedFolders = expandedFoldersStr.split(":");
    }

    searchActive = appSettings.value("general/search_active", false).toBool();

    // Window geometry
    QRect userScreen = QApplication::desktop()->screenGeometry();

    int w = appSettings.value("gui/w", 640).toInt();
    int h = appSettings.value("gui/h", 400).toInt();
    int x = appSettings.value("gui/x", (int)((userScreen.width() - w)/2)).toInt();
    int y = appSettings.value("gui/y", (int)((userScreen.height() - h)/2)).toInt();
    setGeometry(x,y,w,h);
    show();

    // Splitter divider position
    int leftSize = appSettings.value("gui/divider", 150).toInt();

    QList<int> currentSizes = ui->splitter->sizes();
    currentSizes[0] = leftSize;
    currentSizes[1] = ui->splitter->width() - leftSize;
    ui->splitter->setSizes(currentSizes);

    // Last edited file
}

void NightWindow::closeEvent(QCloseEvent *) {
    saveSettings();
}

NightWindow::~NightWindow()
{
    delete ui;
}

void NightWindow::resizeEvent(QResizeEvent *)
{
    if (searchActive == false) {
        ui->splitter->setGeometry(QRect(0, ui->splitter->y(), ui->centralWidget->width(), ui->centralWidget->height() - ui->splitter->y()));
        ui->lineEditFind->setVisible(false);
        ui->pushButtonCancelSearch->setVisible(false);
        ui->pushButtonFind->setVisible(false);
    } else {
        ui->splitter->setGeometry(QRect(0, ui->splitter->y(), ui->centralWidget->width(),
                                        ui->centralWidget->height() - ui->splitter->y() - ui->lineEditFind->height()*2));
        ui->pushButtonCancelSearch->setVisible(true);
        ui->pushButtonCancelSearch->setGeometry(QRect(ui->splitter->width() - ui->pushButtonCancelSearch->width() - 5,
                                                      ui->splitter->y() + ui->splitter->height() +
                                                      (ui->lineEditFind->height()*2 -ui->pushButtonCancelSearch->height())/2,
                                                      ui->pushButtonCancelSearch->width(), ui->pushButtonCancelSearch->height()));

        ui->pushButtonFind->setVisible(true);
        ui->pushButtonFind->setGeometry(QRect(ui->pushButtonCancelSearch->x() - ui->pushButtonCancelSearch->width() - 5,
                                                      ui->pushButtonCancelSearch->y(),
                                                      ui->pushButtonCancelSearch->width(), ui->pushButtonCancelSearch->height()));

        ui->lineEditFind->setVisible(true);
        ui->lineEditFind->setGeometry(QRect(5, ui->splitter->y() + ui->splitter->height() + ui->lineEditFind->height()/2,
                                          ui->splitter->width() -  (ui->splitter->width() - ui->pushButtonFind->x()) - 10, ui->lineEditFind->height()));



    }
 ui->labelLastChanged->setGeometry(QRect(this->width() -
                                        ui->labelLastChanged->width() - 10,
                                        ui->labelLastChanged->y(),
                                  ui->labelLastChanged->width(), ui->labelLastChanged->height()));
}

QString NightWindow::getNewFileName4Folder(QString path) {

    NightNewFile nNewFile;
    while(1) {

        nNewFile.SetPath(path);
        if (nNewFile.exec() == 1) {
            QString newFileName = nNewFile.GetFileName();
            if (newFileName.isEmpty()) {
                nNewFile.SetErrorText(tr("File name cannot be empty!"));
                continue;
            }
            if (QFile::exists(newFileName) == true) {
                nNewFile.SetErrorText(tr("File with this name exists!"));
                continue;
            }
            return newFileName;
        } else {
            return "";
        }
    }


    return "";
}


void NightWindow::on_pushButtonPrefs_clicked() {
    NightNotePrefs prefs;
    prefs.exec();
    loadSettings();
    // FIXME: expanded folders have to be cleared if folder was changed
    loadFiles(); // just in case
}

void NightWindow::on_pushButtonAbout_clicked() {
    NightAbout nightAbout;
    nightAbout.exec();
}


void NightWindow::on_pushButtonWrapText_toggled(bool checked) {
    if (checked) {
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    } else {
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}

void NightWindow::on_pushButtonRefresh_clicked() {
    loadFiles();
}



void NightWindow::on_pushButtonSearch_clicked()
{
    searchActive = true;
    resizeEvent(NULL);
    ui->lineEditFind->setFocus();
    ui->lineEditFind->selectAll();
}



void NightWindow::on_pushButtonCancelSearch_clicked()
{
    searchActive = false;
    resizeEvent(NULL);
}

void NightWindow::on_lineEditFind_returnPressed()
{
    findSearchTerm();
}

void NightWindow::on_pushButtonFind_clicked()
{
    findSearchTerm();
}

void NightWindow::findSearchTerm() {
    QString searchTerm = ui->lineEditFind->text().trimmed();

    QString plainText = ui->plainTextEdit->toPlainText();
    if (lastSearchTerm.compare(searchTerm) != 0 || lastSearchIndex < 0) {
        lastSearchIndex = 0;
        foundMatches = 0;
    } else {
        lastSearchIndex += searchTerm.length();
    }
    lastSearchIndex = plainText.indexOf(searchTerm, lastSearchIndex, Qt::CaseInsensitive);
    if (lastSearchIndex != -1) {
        foundMatches++;

        QTextCursor textCursor = ui->plainTextEdit->textCursor();
        textCursor.setPosition(lastSearchIndex, QTextCursor::MoveAnchor);
        textCursor.setPosition(lastSearchIndex + searchTerm.length(), QTextCursor::KeepAnchor);
        ui->plainTextEdit->setTextCursor(textCursor);
    } else {
        QTextCursor textCursor = ui->plainTextEdit->textCursor();
        textCursor.clearSelection();
        ui->plainTextEdit->setTextCursor(textCursor);

    }
    lastSearchTerm = searchTerm;
}
