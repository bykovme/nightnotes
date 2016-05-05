#include "nightsearchdialog.h"
#include "ui_nightsearchdialog.h"
#include "nightwindow.h"


NightSearchDialog::NightSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightSearchDialog)
{
    ui->setupUi(this);

    //parentWindow = (NightWindow *) parent;
    parentWindow = (NightWindow *) parent; //parent();

    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
                   ~Qt::WindowMinimizeButtonHint &
                   ~Qt::WindowMaximizeButtonHint ^ Qt::WindowStaysOnTopHint );
    setFixedSize(width(), height());
    ui->labelSearchResult->setText("");
}

NightSearchDialog::~NightSearchDialog() {
    delete ui;
}

void NightSearchDialog::on_pushButtonSearch_clicked() {
    QString searchTerm = ui->lineEdit->text();

    QString result = parentWindow->findText(searchTerm);
    ui->labelSearchResult->setText(result);
}
