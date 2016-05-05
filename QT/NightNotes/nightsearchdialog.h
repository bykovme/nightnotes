#ifndef NIGHTSEARCHDIALOG_H
#define NIGHTSEARCHDIALOG_H

#include <QDialog>
//#include "nightwindow.h"

namespace Ui {
class NightSearchDialog;
}

class NightWindow;


class NightSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NightSearchDialog(QWidget *parent = 0);
    ~NightSearchDialog();

private slots:
    void on_pushButtonSearch_clicked();

private:
    Ui::NightSearchDialog *ui;
    NightWindow * parentWindow;
};

#endif // NIGHTSEARCHDIALOG_H
