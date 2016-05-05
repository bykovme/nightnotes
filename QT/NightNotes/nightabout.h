#ifndef NIGHTABOUT_H
#define NIGHTABOUT_H

#include <QDialog>

namespace Ui {
class NightAbout;
}

class NightAbout : public QDialog
{
    Q_OBJECT

public:
    explicit NightAbout(QWidget *parent = 0);
    ~NightAbout();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonCheckUpdates_clicked();

private:
    Ui::NightAbout *ui;
};

#endif // NIGHTABOUT_H
