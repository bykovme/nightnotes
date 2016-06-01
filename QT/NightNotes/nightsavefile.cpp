#include "nightwindow.h"
#include "ui_nightwindow.h"
#include <QSaveFile>

void NightWindow::saveCurrentFile(bool forceWrite) {
    if (currentFile != "" && (dirty == true || forceWrite == true)) {
        QSaveFile f(currentFile);
        if (f.open(QSaveFile::WriteOnly | QSaveFile::Text | QSaveFile::Truncate) == true) {
                QTextStream out(&f);
                out.setCodec("UTF-8");
                out << ui->plainTextEdit->toPlainText();
                out.flush();
                if (out.status() != QTextStream::Ok) {
                    NightMessage nm;
                    nm.setMessage(tr("Save error"), "Error!!!!", true, NightMessage::MESSAGE_OK);
                    nm.exec();
                }
        }
        f.commit();

        QFileInfo fileInfo = QFileInfo( currentFile);

        ui->labelLastChanged->setText(fileInfo.lastModified().toString());
        this->setWindowTitle("Night Notes - " + fileInfo.fileName());
        if (ui->textBrowser->isVisible()) {
            ui->textBrowser->setHtml(convertMD2HTML(ui->plainTextEdit->toPlainText()));
        }
        dirty = false;
    }
}
