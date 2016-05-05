#ifndef NIGHTMESSAGE_H
#define NIGHTMESSAGE_H

#include <QDialog>
#include <QShortcut>

namespace Ui {
class NightMessage;
}

class NightMessage : public QDialog
{
    Q_OBJECT
    
public:
    explicit NightMessage(QWidget *parent = 0);
    void setMessage(QString title, QString text, bool error, int messageType);
    ~NightMessage();
    static const int MESSAGE_OK;
    static const int MESSAGE_YESNO;
    
private slots:
    void on_actionYes_triggered();

private:
    Ui::NightMessage *ui;
    //QShortcut yesShortcut;
};

#endif // NIGHTMESSAGE_H
