#include "nightwindow.h"
#include "ui_nightwindow.h"
#include "nightmessage.h"

bool NightWindow::deleteFolder(const QString &dirPath) {
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!deleteFolder(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

bool NightWindow::moveFolder(QString src, QString dest) {
    QDir parentDstDir(QFileInfo(dest).path());
    parentDstDir.mkdir(QFileInfo(dest).fileName());

    QDir srcDir(src);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = src + QDir::separator() + info.fileName();
        QString dstItemPath = dest + QDir::separator() + info.fileName();
        if (info.isDir()) {
            if (!moveFolder(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            QFileInfo fi(dstItemPath);
            if (fi.exists() == true) {
                // skip now
                // TODO: skip, overwrite, overwrite all, skip all dialog required
            } else {
                if (!QFile::copy(srcItemPath, dstItemPath)) {
                    return false;
                }
            }
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
        }
    }

    return true;
}

void NightWindow::recursiveFolderDelete(QString folderPath) {
    qDebug() << "Recursive enter, folder name: " << folderPath;
    QDir dir(folderPath);
    QFileInfoList files = dir.entryInfoList();
    foreach(QFileInfo fileInfo, files) {
        if (fileInfo.isDir() == true) {
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;
            //qDebug() << "Child folder name: " << fileInfo.absoluteFilePath();
            recursiveFolderDelete(fileInfo.absoluteFilePath());
            //
        } else {
            //qDebug() << "Child file name: " << fileInfo.absoluteFilePath();
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }
    dir.rmdir(folderPath);
}


void NightWindow::on_pushButtonDelete_clicked()
{
    bool removed = false;
    {
        QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
        if (selItems.count() == 1) {
            QFileInfo fInfo(selItems.at(0)->data(0, Qt::UserRole).toString());

            int filesFound = 0;
            if (fInfo.isDir() == true) {
                QDir dir(fInfo.absoluteFilePath());
                QFileInfoList files = dir.entryInfoList();
                foreach(QFileInfo fileInfo, files) {
                    if (fileInfo.isDir() == true) {
                        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;
                        filesFound++;
                    } else {
                        filesFound++;
                    }
                }
                if (filesFound == 0) {
                    removed = dir.rmdir(fInfo.absoluteFilePath()); //fInfo.absoluteFilePath());
                    if (removed == false) {
                        NightMessage nm;
                        nm.setMessage(tr("Error!"), tr("Folder is not deleted, check permissions for: '") +
                                      fInfo.absoluteFilePath() + "'",
                                      true, NightMessage::MESSAGE_OK);
                        nm.exec();
                    }
                } else {
                    NightMessage nm;
                    nm.setMessage(tr("Warning!"), tr("You want to delete not empty folder, are you sure?"),
                                  true, NightMessage::MESSAGE_YESNO);
                    if (nm.exec() == 1) {
                        recursiveFolderDelete(fInfo.absoluteFilePath());
                        removed = true;
                    }
                }
            } else {

                bool deleteFile = false;
                if (NightNotePrefs::getWarnBeforeDelete() == true) {
                    NightMessage nm;
                    int extLen = 4;
                    if (fInfo.fileName().endsWith(".md",Qt::CaseInsensitive)) {
                        extLen = 3;
                    }
                    nm.setMessage(tr("Warning!"), tr("Are you sure you want to delete this note: '") +
                                  fInfo.fileName().mid(0,fInfo.fileName().count() - extLen) + "'?",
                                  true, NightMessage::MESSAGE_YESNO);
                    if (nm.exec() == 1) {
                        deleteFile = true;
                    }
                } else {
                    deleteFile = true;
                }
                if (deleteFile == true) {
                    removed = QFile::remove(fInfo.absoluteFilePath());
                }
            }
        }
    }
    if (removed == true) {
        loadFiles();
    } else {
        // show error
    }
}


void NightWindow::on_pushButtonAddFolder_clicked() {
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("Create new folder"),
                                               tr("Folder name:"), QLineEdit::Normal,
                                               "New Folder", &ok);
    if (ok && !folderName.isEmpty()) {
        ok = false;
        QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
        if (selItems.count() == 1) {
            QFileInfo fInfo(selItems.at(0)->data(0, Qt::UserRole).toString());
            QString newFileName;
            if (fInfo.isDir() == true) {
                newFileName = fInfo.absoluteFilePath() + QDir::separator() + folderName;
            } else {
                newFileName = fInfo.absolutePath() + QDir::separator() + folderName;
            }
            QDir newFolder(newFileName);
            ok = newFolder.mkdir(newFileName);
            currentFile = newFileName;

        } else {
            if (rootItem != NULL) {
                QFileInfo fInfo(NightNotePrefs::getCurrentPath());
                QString newFileName;
                if (fInfo.isDir() == true) {
                    newFileName = fInfo.absoluteFilePath() + QDir::separator() + folderName;
                } else {
                    newFileName = fInfo.absolutePath() + QDir::separator() + folderName;
                }
                QDir newFolder(newFileName);
                ok = newFolder.mkdir(newFileName);
            }
        }
        if (ok) {
            loadFiles();
        }
    }
}

void NightWindow::itemNameChanged(QTreeWidgetItem * item, int column) {
    if (dropItemEvent == true) {
        // it goes here when drop event triggered
        // we do not rename anything, just go away
        // nightwindow.h: eventFilter()
        dropItemEvent = false;
        return;
    }
    if (changingFileName == true) return;
    if (column == 0) {
        changingFileName = true;
        saveCurrentFile();
        QFileInfo fileInfo = QFileInfo( item->data(0, Qt::UserRole).toString());
        QDir fileDir = QDir(fileInfo.absolutePath());
        if (fileInfo.isDir() == true) {
            fileDir.rename(fileInfo.absoluteFilePath(), fileInfo.absolutePath() + QDir::separator() + item->text(0));
        } else {
            QString newFileName = "";
            QString ext = ".md";
            if(fileInfo.fileName().endsWith(".txt", Qt::CaseInsensitive)) {
                ext = ".txt";
            }
            if (currentFile == fileInfo.absoluteFilePath()) newFileName = item->text(0) + ext;
            if (fileDir.rename(fileInfo.absoluteFilePath(), fileInfo.absolutePath() + QDir::separator() + item->text(0) + ext) == true) {
                if (newFileName.isEmpty() == false) {
                    currentFile = fileInfo.absolutePath() + QDir::separator() + newFileName;
                }
            } else { // TODO: rename error
                NightMessage nm;
                nm.setMessage(tr("Error!"), tr("Name was not changed. Do not use some forbidden symbols like: ") +
                              "'/', '\\', '?', '%', '*', ':', '|', '', '\"', '<', '>'",
                              true, NightMessage::MESSAGE_OK);
                nm.exec();

            }
        }
        loadFiles();
        changingFileName = false;
    }
}
