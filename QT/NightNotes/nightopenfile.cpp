#include "nightwindow.h"
#include "ui_nightwindow.h"
//#include "markdown.h"
#include <iostream>
#include <sstream>
#include <QClipboard>
#include <QDesktopServices>
#include "discount/discount-wrapper.hpp"

QString NightWindow::convertMD2HTML(QString mdText) {
    std::string mdIn = mdText.toStdString();
    Markdown mkd(mdIn);
    try {
        mkd.compile();
        std::string html = mkd.html();
        return QString::fromStdString( html);
    } catch(const std::runtime_error& e) {
        return "Markdown to HTML conversion error";
    }
}

void NightWindow::onChangeViewClicked(bool) {
    if (currentFile.endsWith(".md",Qt::CaseInsensitive)) {
        if (ui->textBrowser->isVisible()) {

            QList<int> currentSizes = ui->splitter->sizes();
            int c0 = currentSizes[0];
            int c1 = currentSizes[1];
            int c2 = currentSizes[2];

            ui->textBrowser->setVisible(false);
            ui->plainTextEdit->setVisible(true);
            ui->plainTextEdit->setFocus();

            currentSizes[0] = c0;
            currentSizes[1] = c2;
            currentSizes[2] = c1;
            ui->splitter->setSizes(currentSizes);
            ui->pushButtonWrapText->setVisible(true);
            ui->pushButtonAddList->setIcon(QIcon(":/images/view.png"));

        } else {

            ui->textBrowser->setVisible(true);

            saveCurrentFile(true);

            QList<int> currentSizes = ui->splitter->sizes();
            int c0 = currentSizes[0];
            int c1 = currentSizes[1];
            int c2 = currentSizes[2];


            ui->plainTextEdit->setVisible(false);

            currentSizes[0] = c0;
            currentSizes[1] = c2;
            currentSizes[2] = c1;
            ui->splitter->setSizes(currentSizes);
            ui->pushButtonWrapText->setVisible(false);
            ui->pushButtonAddList->setIcon(QIcon(":/images/edit.png"));
        }
    }
}

void NightWindow::anchorClicked(const QUrl & link) {

    QString linkStr = link.toString();

    if (linkStr.startsWith("copy:",Qt::CaseSensitive)) {
        QClipboard *clipboard = QApplication::clipboard();
        linkStr = linkStr.replace("copy:", "");
        clipboard->setText(linkStr);
        NightMessage nm;
        nm.setMessage(tr("Copied to clipboard"), linkStr, false, NightMessage::MESSAGE_OK);
        nm.exec();
    } else if (linkStr.startsWith("http://", Qt::CaseInsensitive) || linkStr.startsWith("https://", Qt::CaseInsensitive) ) {

        QDesktopServices::openUrl(link);

    } else if (linkStr.endsWith(".md", Qt::CaseInsensitive)) {
        QFileInfo fileCurrent = QFileInfo(currentFile);
        QString filename = fileCurrent.absolutePath() + QDir::separator() + linkStr;
        QFileInfo fileInfo = QFileInfo(filename);
        if (fileInfo.exists()) {
            currentFile = filename;
            loadFiles();
        } else {
            NightMessage nm;
            nm.setMessage(tr("Create file '") + linkStr + "'",
                          tr("File i snot found, do you want to create it?"),
                          false, NightMessage::MESSAGE_YESNO);
            if (nm.exec() == 1) {
                QList<QTreeWidgetItem *> selItems = ui->treeWidget->selectedItems();
                if (selItems.count() == 1) {
                    QFileInfo fInfo(selItems.at(0)->data(0, Qt::UserRole).toString());
                    QString newFileName;
                    if (fInfo.isDir() == true) {
                        newFileName = fInfo.absoluteFilePath() + QDir::separator() + linkStr;
                    } else {
                        newFileName = fInfo.absolutePath() + QDir::separator() + linkStr;
                    }
                    if (newFileName.isEmpty()) {
                        return;
                    }
                    QFile newFile(newFileName);
                    if (newFile.open(QFile::ReadWrite) == true) {
                        currentFile = newFileName;
                    }
                    newFile.close();
                    loadFiles();
                    onChangeViewClicked(true);
                    ui->plainTextEdit->setFocus();
                }

            }
        }
    }



}

void NightWindow::currentItemChanged(QTreeWidgetItem* newItem,QTreeWidgetItem*) {
    saveCurrentFile();
    if (newItem == NULL) return;
    QFileInfo fileInfo = QFileInfo( newItem->data(0, Qt::UserRole).toString());
    if ((fileInfo.fileName().endsWith(".txt") == true || fileInfo.fileName().endsWith(".md") == true) && fileInfo.size() < MAX_FILE) { // do not open files larger than max_file (256k)
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

                ui->pushButtonAddList->setEnabled(true);

                ui->plainTextEdit->setPlainText(plainText);

                QList<int> currentSizes = ui->splitter->sizes();
                int c1 = currentSizes[1];
                int c2 = currentSizes[2];

                if (c1 == 0) {
                    c1 = c2;
                }


                if (filename.endsWith(".md", Qt::CaseInsensitive)) {
                    ui->textBrowser->setVisible(true);
                     ui->plainTextEdit->setVisible(false);
                    ui->textBrowser->setHtml(convertMD2HTML(plainText));
                    currentSizes[1] = 0;
                    currentSizes[2] = c1;
                    ui->pushButtonAddList->setVisible(true);
                    ui->pushButtonWrapText->setVisible(false);
                    ui->pushButtonAddList->setIcon(QIcon(":/images/edit.png"));
                } else {
                    ui->textBrowser->setVisible(false);
                    ui->plainTextEdit->setVisible(true);
                    currentSizes[1] = c1;
                    currentSizes[2] = 0;
                    ui->pushButtonAddList->setVisible(false);
                    ui->pushButtonWrapText->setVisible(true);
                }
                ui->splitter->setSizes(currentSizes);

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
            if (newFileName.isEmpty()) {
                return;
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
                if (newFileName.isEmpty()) {
                    return;
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
    onChangeViewClicked(true);
    ui->plainTextEdit->setFocus();

    //changingFileName = false;
}

