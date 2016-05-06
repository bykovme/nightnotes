#include "nightwindow.h"
#include "ui_nightwindow.h"
//#include "markdown.h"
#include <iostream>
#include <sstream>
#include "discount/discount-wrapper.hpp"

QString NightWindow::convertMD2HTML(QString mdText) {
    //markdown::Document doc;
    //doc.read(mdText);
    //std::stringstream out;
    //QString htmlText = doc.write(out);
    std::string mdIn = mdText.toStdString();
    Markdown mkd(mdIn);
    try {
        // throws a std::runtime_error if discount fails to parse
        // the markdown string, ie. mkd_string() returns a nullptr.
        mkd.compile();
        // html <- "<h1>Hello Markdown!</h1>"
        std::string html = mkd.html();
        return QString::fromStdString( html);
    }
    catch(const std::runtime_error& e) {
        return "Markdown to HTML conversion error";
    }
}

void NightWindow::anchorClicked(const QUrl & link) {
    NightMessage nm;
    nm.setMessage(tr("Link 2 open"), tr("Link: '") + link.toString(), false, NightMessage::MESSAGE_OK);
    nm.exec();

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

                if (filename.endsWith(".md", Qt::CaseInsensitive)) {
                    ui->textBrowser->setVisible(true);
                     ui->plainTextEdit->setVisible(false);
                    ui->textBrowser->setHtml(convertMD2HTML(plainText));
                } else {
                    ui->textBrowser->setVisible(false);
                    ui->plainTextEdit->setVisible(true);
                }
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
    ui->plainTextEdit->setFocus();
    //changingFileName = false;
}

