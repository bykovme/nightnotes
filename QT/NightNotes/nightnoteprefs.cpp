#include "nightnoteprefs.h"
#include "ui_nightnoteprefs.h"
#include <QSettings>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include "const.h"
#include "themes.h"
#include "nightmessage.h"
#include "nightwindow.h"

NightNotePrefs::NightNotePrefs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightNotePrefs)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &
                   ~Qt::WindowMinimizeButtonHint &
                   ~Qt::WindowMaximizeButtonHint );
    setFixedSize(width(), height());


    QString currentPath = getCurrentPath();
    ui->lineEditPath->setText(currentPath);

    ui->checkBoxAskIfdelete->setChecked(getWarnBeforeDelete());
    ui->checkBoxFolderSort->setChecked(sortFoldersFirst());
    ui->checkBoxWrapText->setChecked(wrapTextByDefault());

}

NightNotePrefs::~NightNotePrefs()
{
    delete ui;
}

void NightNotePrefs::on_pushButtonPath_clicked()
{
    QSettings settings;
    QString currentPath =  getCurrentPath();
    qDebug() << "currentPath: " << currentPath;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select working folder"),
                                                  currentPath + "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty() == false) {
        NightMessage nm;
        nm.setMessage(tr("Warning!"), tr("You changed the folder for your text files! Would like to copy your text files from current folder to the newly selected one?"),
                      true, NightMessage::MESSAGE_YESNO);
        if (nm.exec() == 1) {
            // Move files here
            NightWindow::moveFolder(currentPath, dir);
            NightMessage nmDelete;
            nmDelete.setMessage(tr("Warning!"), tr("Do you want to remove all the files from your old folder?") + "\n " + currentPath ,
                          true, NightMessage::MESSAGE_YESNO);
            if (nmDelete.exec() == 1) {
                NightWindow::deleteFolder(currentPath);
                // recover top folder (just in case :)
                QDir newFolder(currentPath);
                newFolder.mkdir(currentPath);
            }
        }
        // otherwise just change folder
        ui->lineEditPath->setText(dir);
        settings.setValue("general/working_path", dir);
        settings.setValue("general/open_folders", "");
    }

}

bool NightNotePrefs::getWarnBeforeDelete() {
    QSettings settings;
    bool warn;
    warn = settings.value("general/ask_before_delete", true).toBool();
    return warn;
}

bool NightNotePrefs::sortFoldersFirst() {
    QSettings settings;
    bool foldersFirst;
    foldersFirst = settings.value("general/folders_first", true).toBool();
    return foldersFirst;
}

bool NightNotePrefs::wrapTextByDefault() {
    QSettings settings;
    bool wrapText;
    wrapText = settings.value("general/wrap_text", true).toBool();
    return wrapText;
}

QString NightNotePrefs::getCurrentPath() {
    QSettings settings;
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString currentPath = settings.value("general/working_path", homePath +  QDir::separator() + DEFAULT_PATH).toString();
    return currentPath;
}

void NightNotePrefs::on_checkBoxAskIfdelete_stateChanged(int newState) {
    QSettings settings;
    bool stateChecked2;
    switch(newState) {
    case Qt::Unchecked:
        stateChecked2 = false;
        break;
    case Qt::Checked:
        stateChecked2 = true;
        break;
    default:
        stateChecked2 = false;
        break;
    }

    if( stateChecked2 != getWarnBeforeDelete()) {
        settings.setValue("general/ask_before_delete", stateChecked2);
    }
}

void NightNotePrefs::on_checkBoxFolderSort_stateChanged(int newState)
{
    QSettings settings;
    bool stateChecked2;
    switch(newState) {
    case Qt::Unchecked:
        stateChecked2 = false;
        break;
    case Qt::Checked:
        stateChecked2 = true;
        break;
    default:
        stateChecked2 = false;
        break;
    }

    if( stateChecked2 != sortFoldersFirst()) {
        settings.setValue("general/folders_first", stateChecked2);
    }
}

void NightNotePrefs::on_checkBoxWrapText_stateChanged(int newState)
{
    QSettings settings;
    bool stateChecked2;
    switch(newState) {
    case Qt::Unchecked:
        stateChecked2 = false;
        break;
    case Qt::Checked:
        stateChecked2 = true;
        break;
    default:
        stateChecked2 = false;
        break;
    }

    if( stateChecked2 != wrapTextByDefault()) {
        settings.setValue("general/wrap_text", stateChecked2);
    }
}

int NightNotePrefs::theme = -1;
QString NightNotePrefs::widgetTheme = BLACK_WHITE_CSS;
QString NightNotePrefs::dividerTheme = DIVIDER_WHITE_CSS;
QString NightNotePrefs::buttonTheme = BUTTON_WHITE_CSS;
QString NightNotePrefs::headerTheme = BLACK_WHITE_HEADER_CSS;
QString NightNotePrefs::lineditTheme = LINEEDIT_WHITE_CSS;

void NightNotePrefs::setTheme(int themeParm) {
    if (themeParm == -1 ) {
        theme = BLACK_WHITE_ID; // TODO: temporary (permanently? :) ) hardcoded
    } else {
        theme = themeParm;
    }

    switch(theme) {
    case BLACK_GREEN_ID:
        widgetTheme = BLACK_GREEN_CSS;
        dividerTheme = DIVIDER_GREEN_CSS;
        buttonTheme = BUTTON_GREEN_CSS;
        headerTheme = BLACK_GREEN_HEADER_CSS;
        break;
    case BLACK_YELLOW_ID:
        widgetTheme = BLACK_YELLOW_CSS;
        dividerTheme = DIVIDER_YELLOW_CSS;
        buttonTheme = BUTTON_YELLOW_CSS;
        headerTheme = BLACK_YELLOW_HEADER_CSS;
    case BLACK_WHITE_ID: default:
        widgetTheme = BLACK_WHITE_CSS;
        dividerTheme = DIVIDER_WHITE_CSS;
        buttonTheme = BUTTON_WHITE_CSS;
        headerTheme = BLACK_WHITE_HEADER_CSS;
        break;
    }
}

QString NightNotePrefs::getThemeBackground() {
    //if (getTheme() == -1) setTheme(getTheme());
    return widgetTheme;
}

QString NightNotePrefs::getThemeDivider() {
    //if (getTheme() == -1) setTheme(getTheme());
    return dividerTheme;
}

QString NightNotePrefs::getThemeHeader() {
    //if (getTheme() == -1) setTheme(getTheme());
    return headerTheme;
}

QString NightNotePrefs::getLineEditTheme() {
    return lineditTheme;
}

void NightNotePrefs::checkForUpdate() {

}
