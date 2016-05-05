#ifndef NIGHTNOTEPREFS_H
#define NIGHTNOTEPREFS_H

#include <QDialog>

namespace Ui {
class NightNotePrefs;
}

class NightNotePrefs : public QDialog
{
    Q_OBJECT

public:
    explicit NightNotePrefs(QWidget *parent = 0);
    ~NightNotePrefs();
    static QString getCurrentPath();
    static bool getWarnBeforeDelete();
    static bool sortFoldersFirst();
    static bool wrapTextByDefault();

    // theme preferences
    static int theme;
    static void setTheme(int themeParm);

    static QString getThemeBackground();
    static QString getThemeDivider();
    static QString getThemeHeader();
    static QString getLineEditTheme();
    static void checkForUpdate();

private slots:
    void on_pushButtonPath_clicked();
    void on_checkBoxAskIfdelete_stateChanged(int arg1);
    void on_checkBoxFolderSort_stateChanged(int arg1);
    void on_checkBoxWrapText_stateChanged(int arg1);

private:
    Ui::NightNotePrefs *ui;

    static QString widgetTheme;  // = BLACK_YELLOW_CSS;
    static QString dividerTheme; // = DIVIDER_YELLOW_CSS;
    static QString buttonTheme;  // = BUTTON_YELLOW_CSS;
    static QString headerTheme;  // = BLACK_YELLOW_HEADER_CSS;
    static QString lineditTheme; //

};

#endif // NIGHTNOTEPREFS_H
