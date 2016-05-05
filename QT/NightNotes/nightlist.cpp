#include "nightwindow.h"
#include "ui_nightwindow.h"

void NightWindow::on_pushButtonAddList_clicked() {
    // Create new list

    ui->pushButtonAddList->setEnabled(false);
/*
    ui->listWidget->addItem("wItem");
    ui->listWidget->setVisible(true);
*/
}


void NightWindow::buildList(QString plainText) {
    /*
    ui->listWidget->clear();
    int listStart = plainText.indexOf(LIST_TAG);
    int tagLength  = QString(LIST_TAG).length();
    int listRealStart = listStart + tagLength;
    if (listRealStart + tagLength < plainText.length()) {
        QString strList = plainText.mid(listRealStart, plainText.length() - listRealStart);
        int endIndex = strList.indexOf(LIST_TAG);
        strList = strList.mid(0, endIndex);
        QStringList strItems = strList.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        foreach(QString str, strItems) {
            qDebug() << str;
            QListWidgetItem * wi;
            wi = new QListWidgetItem(ui->listWidget);
            wi->setText(str);
            ui->listWidget->addItem(wi);
        }
    }
*/

}
