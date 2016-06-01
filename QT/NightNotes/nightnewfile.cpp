#include "nightwindow.h"
#include "nightnewfile.h"
#include "ui_nightnewfile.h"
#include <QPushButton>

NightNewFile::NightNewFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightNewFile)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
                   ~Qt::WindowMinimizeButtonHint &
                   ~Qt::WindowMaximizeButtonHint &
                   ~Qt::WindowContextHelpButtonHint);
    setFixedSize(width(), height());


    this->setStyleSheet("QWidget{font-size:13px;font-family: Arial, Helvetica, Verdana, sans-serif;}");
    ui->labelError->setStyleSheet(" QLabel  { background:transparent;color:#FF0000;}");
    ui->lineEdit->setFocus();
}
void NightNewFile::SetErrorText(QString errorText) {
    ui->labelError->setText(errorText);
}

void NightNewFile::SetPath(QString currentPath) {
    path = currentPath;
    ui->label->setText("Path: " + currentPath);
}

QString NightNewFile::GetFileName() {

    QString filename = ui->lineEdit->text();
    if (filename.isEmpty() == true) return "";
    QString fileType = ui->comboBox->currentText();
    if (fileType.compare("Markdown", Qt::CaseInsensitive) == 0) {
        filename = path + QDir::separator() + filename + ".md";
    } else {
        filename = path + QDir::separator() + filename + ".txt";
    }
    return filename;
}

NightNewFile::~NightNewFile()
{
    delete ui;
}
