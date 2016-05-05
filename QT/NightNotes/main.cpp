#include "nightwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Alexander Bykov");
    QCoreApplication::setOrganizationDomain("bykov.me");
    QCoreApplication::setApplicationName("Night Note");

    NightWindow w;
    w.show();
    
    return a.exec();
}
