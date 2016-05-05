#include "nightwindow.h"
#include "ui_nightwindow.h"

void NightWindow::saveCurrentFile() {
    if (currentFile != "" && dirty == true) {
        QFile f(currentFile);
        if (f.open(QFile::ReadWrite | QFile::Text | QFile::Truncate) == true) {
                QTextStream out(&f);
                out.setCodec("UTF-8");
                out << ui->plainTextEdit->toPlainText();
        }
        f.close();

        QFileInfo fileInfo = QFileInfo( currentFile);
        if(fileInfo.fileName() == "New Night Note") {
            QString plainTextEditContents = ui->plainTextEdit->toPlainText();
            QStringList lines = plainTextEditContents.split("\n");
            if (lines.count() > 1) {
                QString line = lines.at(0) + ".txt";
                f.rename(line);
                fileInfo = QFileInfo(fileInfo.absolutePath() + QDir::separator() + line );
                currentFile = fileInfo.absoluteFilePath();
                loadFiles();
            }
        }
        ui->labelLastChanged->setText(fileInfo.lastModified().toString());
        this->setWindowTitle("Night Notes - " + fileInfo.fileName());
        dirty = false;
    }
}
