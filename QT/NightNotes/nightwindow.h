#ifndef NIGHTWINDOW_H
#define NIGHTWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QDropEvent>
#include <QList>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QMessageBox>
#include <QDropEvent>
#include <QMimeData>
#include <QByteArray>
#include <QTableWidgetItem>
#include <QDrag>
#include <QMenu>
#include <QTextBrowser>

#include <QDebug>

#include "const.h"
#include "themes.h"
#include "nightnoteprefs.h"
#include "nightabout.h"
#include "nightmessage.h"

#define LIST_TAG "{[NIGHT_NOTES_TODO]}"

namespace Ui {
class NightWindow;
}

class NightTreeWidgetItem : public QTreeWidgetItem
{
public:
    NightTreeWidgetItem(QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
    NightTreeWidgetItem(NightTreeWidgetItem *treeItem) : QTreeWidgetItem(treeItem)  {}
    NightTreeWidgetItem(NightTreeWidgetItem * parent, const QStringList & strings)
        : QTreeWidgetItem (parent,strings)  {}
    bool operator< (const QTreeWidgetItem &other) const
    {
        QFileInfo fileInfoOther = QFileInfo( other.data(0, Qt::UserRole).toString());
        QFileInfo fileInfoOwn = QFileInfo( data(0, Qt::UserRole).toString());

        if (NightNotePrefs::sortFoldersFirst() == false) {
            if (fileInfoOwn.fileName().toLower().compare(fileInfoOther.fileName().toLower()) > 0)
                return true;
            else
                return false;
        } else {

            if (fileInfoOwn.isDir() && fileInfoOther.isDir()) {
                if (fileInfoOwn.fileName().toLower().compare(fileInfoOther.fileName().toLower()) > 0)
                    return true;
                else
                    return false;
            } else if (!fileInfoOwn.isDir() && !fileInfoOther.isDir()) {
                if (fileInfoOwn.fileName().toLower().compare(fileInfoOther.fileName().toLower()) > 0)
                    return true;
                else
                    return false;
            } else {
                if (fileInfoOwn.isDir() == false)
                    return true;
                else
                    return false;
            }
        }
    }
};

class NightWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit NightWindow(QWidget *parent = 0);
    ~NightWindow();
    static bool moveFolder(QString src, QString dest);
    static bool deleteFolder(const QString &dirPath);

public slots:

    
private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonPrefs_clicked();
    void currentItemChanged(QTreeWidgetItem* newItem,QTreeWidgetItem*);
    void saveTimeout();
    void moveTimeout();
    void currentFileChanged();
    void itemNameChanged(QTreeWidgetItem * item, int column);
    void on_pushButtonAbout_clicked();
    void on_pushButtonDelete_clicked();
    void on_pushButtonAddFolder_clicked();
    bool eventFilter( QObject* o, QEvent* e );
    //void dropEvent(QDropEvent *);
    void itemExpanded(QTreeWidgetItem * expandedItem);
    void itemCollapsed(QTreeWidgetItem * collapsedItem);


    void on_pushButtonWrapText_toggled(bool checked);
    void on_pushButtonRefresh_clicked();
    void on_pushButtonAddList_clicked();
    void on_pushButtonSearch_clicked();
    void on_pushButtonCancelSearch_clicked();
    void on_lineEditFind_returnPressed();
    void on_pushButtonFind_clicked();

    void onCreateFolderContextClickedTree();
    void onCreateNoteContextClickedTree();
    void onDeleteContextClickedTree();
    void onShowInFinderContextClickedTree();
    void menuAboutToShow();
    void anchorClicked(const QUrl & link);
private:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
    //void dropEvent(QDropEvent *);
    void saveSettings();
    void loadSettings();
    void loadFiles();
    void saveCurrentFile();
    NightTreeWidgetItem * loadTree(QString folderPath, NightTreeWidgetItem * treeItem);
    QString getNewFileName4Folder(QString path);
    void openTextFile(QString filename);
    void recursiveFolderDelete(QString folderPath);
    QString getFileText(QString plainText);
    void buildList(QString plainText);
    void findSearchTerm();
    QString convertMD2HTML(QString mdText);
    // void expandMe(QTreeWidgetItem * item);

    Ui::NightWindow *ui;
    int theme;


    QString dataPath;
    QTimer * savingTimer;
    QTimer * moveTimer;
    QString currentFile;
    QString currentParent;
    bool dirty;
    bool changingFileName;
    bool loadingFile;
    NightTreeWidgetItem * rootItem;
    QTreeWidgetItem * droppedItem;
    QPoint poss;
    QStringList expandedFolders;
    int lastSearchIndex;
    QString lastSearchTerm;
    bool searchActive;
    bool dropItemEvent;
    int foundMatches;
    QMenu * nodeMenu;
    QAction * createFolderActionTree;
    QAction * createNoteActionTree;
    QAction * deleteActionTree;
    QAction * showInFinderActionTree;
};

#endif // NIGHTWINDOW_H
