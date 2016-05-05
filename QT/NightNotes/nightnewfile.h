#ifndef NIGHTNEWFILE_H
#define NIGHTNEWFILE_H

#include <QDialog>

namespace Ui {
class NightNewFile;
}

class NightNewFile : public QDialog
{
    Q_OBJECT

public:
    explicit NightNewFile(QWidget *parent = 0);
    QString GetFileName();
    void SetPath(QString currentPath);
    void SetErrorText(QString errorText);
    ~NightNewFile();

private:
    Ui::NightNewFile *ui;
    QString path;
};

#endif // NIGHTNEWFILE_H
