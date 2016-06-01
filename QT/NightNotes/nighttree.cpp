#include "nightwindow.h"
#include "ui_nightwindow.h"
#include "nightmessage.h"



void NightWindow::loadFiles() {

    disconnect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), 0, 0);

    ui->treeWidget->clear();
    rootItem = new NightTreeWidgetItem(ui->treeWidget);
    rootItem->setText(0, "Night Notes");

    rootItem->setIcon(0, QIcon(":/images/black_folder_star.png"));

    rootItem->setData(0, Qt::UserRole, NightNotePrefs::getCurrentPath());
    rootItem->setFlags(rootItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsDragEnabled);

    ui->treeWidget->addTopLevelItem(rootItem);

    NightTreeWidgetItem * selectedItem = loadTree(NightNotePrefs::getCurrentPath(), rootItem);

    rootItem->setExpanded(true);

    if (selectedItem != NULL) {
        NightTreeWidgetItem * item2expand = selectedItem;

        while(item2expand != rootItem) {
            ui->treeWidget->expandItem(item2expand);
            item2expand = (NightTreeWidgetItem *)item2expand->parent();
        }
        ui->treeWidget->scrollToItem(selectedItem, QAbstractItemView::PositionAtCenter);
    }

    if (rootItem->childCount() == 0) {
        //First start
        QString filename = NightNotePrefs::getCurrentPath() + QDir::separator() + "ReadMe.md";
        QFile f(filename);
        if (f.open(QFile::ReadWrite | QFile::Text | QFile::Truncate) == true) {
            QFile fText(":/readme_en.txt");
            if (fText.open(QFile::ReadOnly | QFile::Text) == true) {
                QTextStream in(&fText);
                in.setCodec("UTF-8");
                QTextStream out(&f);
                out.setCodec("UTF-8");
                out << in.readAll();
                out.flush();
                currentFile = filename;
                openTextFile(currentFile);
            }
        }
        f.close();
        loadFiles();
    } else {
        connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemNameChanged(QTreeWidgetItem *, int )));
    }
}

NightTreeWidgetItem * NightWindow::loadTree(QString folderPath, NightTreeWidgetItem * treeItem) {

    QSettings appSettings;
    QString openFolders = appSettings.value("general/open_folders",  "").toString();
    QDir dir;
    dir.setPath(folderPath);
    //qDebug() << "Found folder: " << folderPath;
    QFileInfoList files = dir.entryInfoList();
    NightTreeWidgetItem * foundItem = NULL;
    foreach(QFileInfo fileInfo, files) {
        if (fileInfo.fileName().startsWith(".")) continue;
        if (fileInfo.isDir() == true) {
            NightTreeWidgetItem * newItem = new NightTreeWidgetItem(treeItem);
            newItem->setText(0, fileInfo.fileName());
            newItem->setData(0, Qt::UserRole, fileInfo.absoluteFilePath());
            newItem->setIcon(0, QIcon(":/images/black_folder_light.png"));
            newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
            treeItem->addChild(newItem);
            NightTreeWidgetItem * localFoundItem;
            localFoundItem = loadTree(fileInfo.absoluteFilePath(), newItem);
            if (foundItem == NULL && localFoundItem != NULL) {
                foundItem = localFoundItem;
            }
            if (openFolders.contains(fileInfo.absoluteFilePath() + ":") == true) {
                ui->treeWidget->expandItem(newItem);
            }

            if (fileInfo.absoluteFilePath() == currentFile) {
                newItem->setSelected(true);
                foundItem = newItem;
            }

        } else {
            if (fileInfo.fileName().endsWith(".txt",Qt::CaseInsensitive) == true ||
                    fileInfo.fileName().endsWith(".md",Qt::CaseInsensitive) == true) {
                NightTreeWidgetItem * newItem = new NightTreeWidgetItem(treeItem);
                int extCount = 3;
                if (fileInfo.fileName().endsWith(".txt",Qt::CaseInsensitive) == true) {
                    extCount = 4;
                }
                newItem->setText(0, fileInfo.fileName().mid(0,fileInfo.fileName().count() - extCount));
                newItem->setData(0, Qt::UserRole, fileInfo.absoluteFilePath());
                if (fileInfo.fileName().endsWith(".md",Qt::CaseInsensitive)) {
                    newItem->setIcon(0, QIcon(":/images/notemd.png"));
                } else {
                    newItem->setIcon(0, QIcon(":/images/note.png"));
                }
                newItem->setFlags((newItem->flags() | Qt::ItemIsEditable) & ~Qt::ItemIsDropEnabled);

                treeItem->addChild(newItem);
                if (fileInfo.absoluteFilePath() == currentFile) {
                    newItem->setSelected(true);
                    openTextFile(fileInfo.absoluteFilePath());
                    ui->labelLastChanged->setText(fileInfo.lastModified().toString());
                    this->setWindowTitle( " " + QString( NIGHT_NOTES) + " - " + fileInfo.fileName());
                    currentFile = fileInfo.absoluteFilePath();
                    foundItem = newItem;
                }
            }
        }
    }
    return foundItem;
}
