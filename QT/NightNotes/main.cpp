#include "nightwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY);
    QCoreApplication::setOrganizationDomain(DOMAIN);
    QCoreApplication::setApplicationName(NIGHT_NOTES);
    QCoreApplication::setApplicationVersion(APP_VERSION_TEXT);

    NightWindow w;
    w.show();
    
    return a.exec();
}
