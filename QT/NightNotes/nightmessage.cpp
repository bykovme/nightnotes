#include "nightmessage.h"
#include "ui_nightmessage.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QPushButton>

NightMessage::NightMessage(QWidget *parent) :QDialog(parent), ui(new Ui::NightMessage) {
    ui->setupUi(this);
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
                   ~Qt::WindowMinimizeButtonHint &
                   ~Qt::WindowMaximizeButtonHint &
                   ~Qt::WindowContextHelpButtonHint);
    setFixedSize(width(), height());
    ui->graphicsView->setStyleSheet("background: transparent");
    this->setStyleSheet("QWidget{font-size:13px;font-family: Arial, Helvetica, Verdana, sans-serif;}");
    ui->labelTitle->setStyleSheet("QWidget{font-size:14px;font-family: Arial, Helvetica, Verdana, sans-serif;font-weight:bold;}");
}

const int NightMessage::MESSAGE_OK = 0;
const int NightMessage::MESSAGE_YESNO = 1;

void NightMessage::setMessage(QString title, QString text, bool error, int messageType) {

    QGraphicsScene* scene = new QGraphicsScene(0,0,64,64,this);
    QString iconName = ":/images/info.png";

    if (error == true) {
        iconName = ":/images/stop.png";
    }

    switch(messageType) {
    case MESSAGE_OK:
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        break;
    case MESSAGE_YESNO:
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
         QPushButton *YesButton=ui->buttonBox->button(QDialogButtonBox::Yes);
         YesButton->setShortcut(QKeySequence( Qt::Key_Y));
        break;
    }

    scene->addPixmap(QPixmap(iconName));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->ensureVisible(0,0,64,64);

    ui->labelTitle->setText(title);
    ui->labelText->setText(text);
    if (parent() == NULL) {
        QRect screenRect = QDesktopWidget().screen()->rect();
        setGeometry(screenRect.width()/2 - geometry().width() / 2,
                    screenRect.height()/2 - geometry().height() / 2,
                    geometry().width(), geometry().height());
    }

    int oldHeight = ui->labelText->size().height();
    ui->labelText->adjustSize();

    if ( ui->labelText->size().height() > oldHeight) { // Size became more than default
        int heightShift = ui->labelText->size().height() - oldHeight; // Calculating height difference
        int newHeight = this->size().height() + heightShift; // Calculating new height for label
        setFixedHeight(newHeight);
        ui->buttonBox->setGeometry(ui->buttonBox->geometry().x(),
                                   ui->buttonBox->geometry().y() + heightShift,
                                   ui->buttonBox->geometry().width(),
                                   ui->buttonBox->geometry().height());
    }

}

NightMessage::~NightMessage()
{
    delete ui;
}

void NightMessage::on_actionYes_triggered()
{
    this->accept();
}
