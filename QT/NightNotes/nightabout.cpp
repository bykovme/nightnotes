#include "nightabout.h"
#include "ui_nightabout.h"

#include <QGraphicsPixmapItem>
#include "nightnoteprefs.h"
#include "themes.h"
#include "const.h"

NightAbout::NightAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightAbout)
{

    ui->setupUi(this);
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
                   ~Qt::WindowMinimizeButtonHint &
                   ~Qt::WindowMaximizeButtonHint );

    setFixedSize(width(), height());
    this->setStyleSheet(NightNotePrefs::getThemeHeader() + ";background-color:black; color:#BBBBBB; ");


    ui->graphicsViewIcon->setStyleSheet("background: transparent;border:none;");
    ui->graphicsViewIcon->setVisible(true);
    QGraphicsScene* sceneBack = new QGraphicsScene();
    QGraphicsPixmapItem * pBack = sceneBack->addPixmap(QPixmap(":/images/icon1024.png").scaledToWidth(
                                                         ui->graphicsViewIcon->size().width(), Qt::SmoothTransformation));
    ui->graphicsViewIcon->setRenderHint(QPainter::HighQualityAntialiasing, true);
    ui->graphicsViewIcon->setRenderHint(QPainter::SmoothPixmapTransform, true);
    ui->graphicsViewIcon->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewIcon->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewIcon->setScene(sceneBack);
    ui->graphicsViewIcon->fitInView(pBack, Qt::KeepAspectRatio);

    ui->labelMail->setOpenExternalLinks(true);
    ui->labelMailWeb->setOpenExternalLinks(true);

    ui->labelProgramVersion->setStyleSheet(
                " QLabel  { background:transparent;color:#BEBEBE;}"
                );
    QString appVersion = (QString) NIGHT_NOTES + " " + (QString)APP_VERSION_TEXT + " (" + (QString)APP_VERSION_INTERNAL + ")" ;
    ui->labelProgramVersion->setText(appVersion);
    ui->labelDevelopedBy->setStyleSheet(
                " QLabel  { background:transparent;color:#BEBEBE;}"
                );
    ui->pushButton->setStyleSheet(BUTTON_WHITE_CSS);
    ui->pushButtonCheckUpdates->setStyleSheet(BUTTON_WHITE_CSS);

    ui->labelLoad->setText(tr("Checking new version..."));
    ui->labelLoad->setVisible(false);
    ui->pushButtonCheckUpdates->setVisible(false);

#ifdef Q_OS_OSX
    QFont f( "Verdana", 16, QFont::Normal);
    ui->labelProgramVersion->setFont(f);
    //ui->labelProgramVersion->setSt
#endif
}

NightAbout::~NightAbout()
{
    delete ui;
}

void NightAbout::on_pushButton_clicked()
{
    reject();
}

void NightAbout::on_pushButtonCheckUpdates_clicked() {
    ui->pushButtonCheckUpdates->setVisible(false);
    ui->labelLoad->setVisible(true);
}
