#include "nightwindow.h"
#include "ui_nightwindow.h"

void NightWindow::currentItemChanged(QTreeWidgetItem* newItem,QTreeWidgetItem*) {
    saveCurrentFile();
    if (newItem == NULL) return;
    QFileInfo fileInfo = QFileInfo( newItem->data(0, Qt::UserRole).toString());
    if (fileInfo.fileName().endsWith(".txt") == true && fileInfo.size() < MAX_FILE) { // do not open files larger than max_file (256k)
        openTextFile(fileInfo.absoluteFilePath());
        ui->labelLastChanged->setText(fileInfo.lastModified().toString());
        this->setWindowTitle("Night Notes - " + fileInfo.fileName());
        currentFile = fileInfo.absoluteFilePath();
    }
}

QString NightWindow::getFileText(QString plainText) {
    int endOfText = plainText.indexOf(LIST_TAG);
    if (endOfText > 0)
        return plainText.mid(0, endOfText);
    else
        return "";
}

void NightWindow::openTextFile(QString filename) {
    QFile f(filename);
    if (f.open(QFile::ReadOnly | QFile::Text) == true) {
            QTextStream in(&f);
            in.setCodec("UTF-8");
            loadingFile = true;
            QString plainText = in.readAll();
            if (plainText.contains(LIST_TAG)) {
                ui->pushButtonAddList->setEnabled(false);
                ui->pushButtonRemoveList->setEnabled(true);
                //ui->listWidget->setVisible(true);

                QString realText = getFileText(plainText);
                ui->plainTextEdit->setPlainText(realText);

                buildList(plainText);

            } else {
                ui->pushButtonAddList->setEnabled(true);
                ui->pushButtonRemoveList->setEnabled(false);
                //ui->listWidget->setVisible(false);

                ui->plainTextEdit->setPlainText(plainText);
            }
            loadingFile = false;
    }
    f.close();
}

void NightWindow::on_pushButtonAdd_clicked()
{
    {
        QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
        if (selItems.count() == 1) {
            QFileInfo fInfo(selItems.at(0)->data(0, Qt::UserRole).toString());
            QString newFileName;
            if (fInfo.isDir() == true) {
                newFileName = getNewFileName4Folder(fInfo.absoluteFilePath());
            } else {
                newFileName = getNewFileName4Folder(fInfo.absolutePath());
            }
            QFile newFile(newFileName);
            if (newFile.open(QFile::ReadWrite) == true) {
                currentFile = newFileName;
            }
            newFile.close();
        } else {
            if (rootItem != NULL) {
                QFileInfo fInfo(NightNotePrefs::getCurrentPath());
                QString newFileName;
                if (fInfo.isDir() == true) {
                    newFileName = getNewFileName4Folder(fInfo.absoluteFilePath());
                } else {
                    newFileName = getNewFileName4Folder(fInfo.absolutePath());
                }
                QFile newFile(newFileName);
                if (newFile.open(QFile::ReadWrite) == true) {
                    currentFile = newFileName;
                }
                newFile.close();
            }
        }
    }
    //changingFileName = true;
    loadFiles();
    ui->plainTextEdit->setFocus();
    //changingFileName = false;
}

