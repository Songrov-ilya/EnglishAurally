#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <MakeTrack.h>

#include <QAudioBuffer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString strTrack_1;
    QString strTrack_2;
    QString strSubtitles_1;
    QString strSubtitles_2;
    QString strObjNameRadioButtonChecked;
    QString strPathSave;
    int buttonLottery;
    int myIntRandom;

    MakeTrack makeTrack;

    void setupBody();
    void setupAction();
    void createButtonPlus();
    void existsPathSave();
    void refreshInformationOnComboBox();
    void setDisabledPushButton_begin();
    void readSubtitles_OR_push_backTimeCut();
public slots:
    void slotCreateTabWidget(int var);
    void slotTabCloseRequestedItem(int index);
    void slotAddNewTab(int index);
    void slotOnComboBox(int indexCombo);
    void slotOnRadioButton(bool boolClicked);

private slots:
    void on_pushButton_track_1_clicked();

    void on_pushButton_subtitles_1_clicked();

    void on_pushButton_track_2_clicked();

    void on_pushButton_subtitles_2_clicked();

    void on_pushButton_begin_clicked();

    void on_action_BaseSubtitles_1_triggered();

    void on_action_BaseSubtitles_2_triggered();

    void on_action_count_phrases_triggered();

    void on_action_Fade_triggered();

    void on_action_timeCut_triggered();

    void on_action_outputDirectory_triggered();

    void on_action_help_triggered();

    void on_action_developer_triggered();

    void on_action_administration_triggered();

    void on_action_buttonLottery_triggered();

    void on_action_AboutQt_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
