#ifndef MAKETRACK_H
#define MAKETRACK_H

#include  <QtWidgets>
#include  <QString>
#include  <QMediaPlayer>


class MakeTrack
{
public:
    MakeTrack();

    QWidget progressBarWidget;
    QString strPathSubtitles_1;
    QString strPathSubtitles_2;
    QString strPathTrack_1;
    QString strPathTrack_2;
    QString strPathSave;
    int baseSubtitles = 1;
    int phrasesVar1 = 0;
    int phrasesVar2 = 0;
    int valueFadeBefore = 20;
    int valueFadeAfter = 25;
    int timeCut = 5000;

    QVector <QPair <QTime, QTime> > vecPairCutTime;
    QVector <QVector <int> > vecDataTabCut;

    void setStrPathSubtitles_1 (QString path = QString());
    void setStrPathSubtitles_2 (QString path = QString());
    void setBaseSubtitles (int base);
    void setStrPathTrack_1 (QString path = QString());
    void setStrPathTrack_2 (QString path = QString());
    void setStrPathSave (QString path = QString());
    void setValueFade (int fadeBefore, int fadeAfter);
    void setDataTab(QVector <QVector <int> > &vecDataTab);

    void createProgressBarWidget();
     int showProgressBarWidget(int step, QString phrase);
    void readSubtitles(int intBaseSub);
    void push_backTimeCut();
    void cutTrack(int varTab, int var);
    void mergeTracks(int var);
    void deleteSegments(int var);
    void makeEffects(QString effect, int varTab, int var);
    void startToMake();

    void showDilogPhrases();

};

#endif // CUTTRACK_H
