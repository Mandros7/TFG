#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicdialog.h"
#include "settingsdialog.h"
#include "inputdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void musicSettingsChanged();

private slots:
    void on_musicButton_clicked();
    void playerClosed();
    void on_settingsButton_clicked();
    void newMusicPath(QString newPath);
    void on_inputButton_clicked();

private:
    Ui::MainWindow *ui;
    MusicDialog *mDialog;
    SettingsDialog *sDialog;
    InputDialog *iDialog;
    bool musicPlayerRunning;
    QString musicPath;
};

#endif // MAINWINDOW_H
