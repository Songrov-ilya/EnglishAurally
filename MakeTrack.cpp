#include "MakeTrack.h"

MakeTrack::MakeTrack() :
    strPathSubtitles_1(""),
    strPathSubtitles_2(""),
    strPathTrack_1(""),
    strPathTrack_2(""),
    strPathSave(""),
    baseSubtitles(1), // По дефолту опираемся на 1 субтитры
    phrasesVar1(0), // По дефолту фразы обрезаем начиная с 0
    phrasesVar2(0), // По дефолту фразы обрезаем начиная с 0 по 0
    valueFadeBefore(20), // По дефолту затухание перед обрезанным треком 20
    valueFadeAfter(25), // По дефолту затухание после обрезанного трека 25
    timeCut(5000) // Если выбран QRadioButton("Обрезать по отрезкам"), по дефолту это по 5 секунд.
{
    createProgressBarWidget();
}

void MakeTrack::setStrPathSubtitles_1(QString path)
{
    strPathSubtitles_1 = path;
}

void MakeTrack::setStrPathSubtitles_2(QString path)
{
    strPathSubtitles_2 = path;
}

void MakeTrack::setBaseSubtitles(int base)
{
    baseSubtitles = base;
    phrasesVar1 = 0;
    phrasesVar2 = vecPairCutTime.size();
}

void MakeTrack::setStrPathTrack_1(QString path)
{
    strPathTrack_1 = path;
}

void MakeTrack::setStrPathTrack_2(QString path)
{
    strPathTrack_2 = path;
}

void MakeTrack::setStrPathSave(QString path)
{
    strPathSave = path;
}

void MakeTrack::setValueFade(int fadeBefore, int fadeAfter)
{
    valueFadeBefore = fadeBefore;
    valueFadeAfter = fadeAfter;
}

void MakeTrack::setDataTab(QVector <QVector<int> > &vecDataTab)
{
    vecDataTabCut = vecDataTab;
}

void MakeTrack::createProgressBarWidget()
{
    progressBarWidget.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
        QPalette pal;
        pal.setBrush(QPalette::Background, QBrush(Qt::gray));
        progressBarWidget.setPalette(pal);
    QLabel *labelInfo = new QLabel("Обработанно фраз: ", &progressBarWidget);
    labelInfo->setObjectName("labelInfo");
    QPushButton *pushButton = new QPushButton("Отмена", &progressBarWidget);
    QProgressBar *progressBar = new QProgressBar(&progressBarWidget);
    progressBar->setRange(0, (phrasesVar2 - phrasesVar1) * vecDataTabCut.size());
    progressBar->setValue(0);
    progressBar->setObjectName("progressBar");
    QObject::connect(pushButton, &QPushButton::pressed, progressBar, &QProgressBar::reset);

        QHBoxLayout *hBoxLayout = new QHBoxLayout;
        hBoxLayout->addStretch();
        hBoxLayout->addWidget(pushButton);
        QVBoxLayout *vBoxLayout = new QVBoxLayout;
        vBoxLayout->addWidget(labelInfo);
        vBoxLayout->addWidget(progressBar);
        vBoxLayout->addLayout(hBoxLayout);
    progressBarWidget.setLayout(vBoxLayout);
    progressBarWidget.resize(400, 70);
}


int MakeTrack::showProgressBarWidget(int step, QString phrase)
{
    if(progressBarWidget.findChild <QProgressBar *>("progressBar")->value() == -1){
        progressBarWidget.hide();
        QApplication::processEvents();
        return -1;
    }
    progressBarWidget.show();
    progressBarWidget.findChild <QProgressBar *>("progressBar")->setValue(step);
    progressBarWidget.findChild <QLabel *>("labelInfo")->setText(phrase);
    QApplication::processEvents();
    return step;
}

void MakeTrack::readSubtitles(int intBaseSub)
{
    if(!vecPairCutTime.isEmpty()) vecPairCutTime.clear();
    QString pathSubtitles = (intBaseSub == 1 ? strPathSubtitles_1 : strPathSubtitles_2);

    if (!pathSubtitles.isEmpty())
    {

        QFile read_file(pathSubtitles);
        QString text;

        if(read_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&read_file);
            QRegExp rx("([0-2][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9].[0-9]");// Нашел в интернете
            do
            {
                text = stream.readLine();
                if(!text.isNull() && rx.indexIn(text) == 0){
                    vecPairCutTime.push_back(qMakePair(
                                            QTime::fromString(text.section(" --> ",0,0), "hh:mm:ss,zzz"), // Начало
                                            QTime::fromString(text.section(" --> ",1,1),"hh:mm:ss,zzz"))); // Конец
                }
            } while (!text.isNull());

        }

        read_file.close();
    }
    phrasesVar2 = phrasesVar2 < vecPairCutTime.size() ? phrasesVar2 : vecPairCutTime.size();
    if(phrasesVar2 == 0) phrasesVar2 = vecPairCutTime.size();// больше нужно для первого запуска приложения.
    phrasesVar1 = phrasesVar2 > phrasesVar1 ? phrasesVar1 : 0;

}

void MakeTrack::push_backTimeCut()
{
    if(!vecPairCutTime.isEmpty()) vecPairCutTime.clear();

    int intDurationTracks [2] = {0,0};
    for (int varDur = 0; varDur < 2; ++varDur) // Находим трек который самый маленький по продолжительности.
    {
        QString strPathTrack = (varDur == 0 ? strPathTrack_1 : strPathTrack_2);
        if(strPathTrack_1 == ""){// Если одного из треков нет, тогда мы опираемся на другой.
            strPathTrack = strPathTrack_2;
        }
        if(strPathTrack_2 == ""){
            strPathTrack = strPathTrack_1;
        }
        if(strPathTrack_1 == "" && strPathTrack_2 == ""){
            return;
        }
        QMediaPlayer *player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile(strPathTrack));

            QEventLoop eventLoop;// Ожидаем пока QMediaPlayer определит продолжительность. Это происходит не сразу.
            QObject::connect(player, SIGNAL(durationChanged(qint64)), &eventLoop, SLOT(quit()));

            QTimer timer; // На случай если не сработает первый сигнал
            QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
            timer.setSingleShot(true);
            timer.start(5000);

            eventLoop.exec();
        intDurationTracks [varDur] = player->duration();
        player->disconnect();
        delete player;
    }
    int Duration = intDurationTracks [0] < intDurationTracks [1] ? intDurationTracks [0] : intDurationTracks [1];
    int stepSegment = 0;

    for (int var = 0; var < Duration / timeCut; ++var)
    {
        vecPairCutTime.push_back(qMakePair(
                                QTime::fromMSecsSinceStartOfDay(stepSegment), // Начало
                                QTime::fromMSecsSinceStartOfDay(stepSegment + timeCut))); // Конец
    // Пример: QTime().toString("hh:mm:ss,,zzz") == выводить будет 00:23:45,,098
        stepSegment += timeCut;
    }
    phrasesVar2 = phrasesVar2 < vecPairCutTime.size() ? phrasesVar2 : vecPairCutTime.size();
    if(phrasesVar2 == 0) phrasesVar2 = vecPairCutTime.size();// больше нужно для первого запуска приложения.
    phrasesVar1 = phrasesVar2 > phrasesVar1 ? phrasesVar1 : 0;
}

void MakeTrack::cutTrack(int varTab, int var)
{
        QProcess process;
        QStringList strList;

        int intTrimTime1 = vecPairCutTime[var].first.msecsSinceStartOfDay() + vecDataTabCut[varTab][7];
        int intTrimTime2 = vecPairCutTime[var].second.msecsSinceStartOfDay() -
                           vecPairCutTime[var].first.msecsSinceStartOfDay() + vecDataTabCut[varTab][8];

        static int number;
        qDebug() << number++ << " Вошли intTrimTime2 " << vecPairCutTime[var].second.msecsSinceStartOfDay() << " - "
                 << vecPairCutTime[var].first.msecsSinceStartOfDay()
                 << " + " << vecDataTabCut[varTab][8] << " " << intTrimTime2 <<endl;
        if(intTrimTime1 < 0) intTrimTime1 = 0;
        if(intTrimTime2 < 0) intTrimTime2 = 1;

        strList << (vecDataTabCut[varTab][0] == 0 ? strPathTrack_1 : strPathTrack_2)
                << strPathSave + QString("/%1.%2Result.wav").arg(varTab + 1).arg(var)
                << "trim"
                << QTime::fromMSecsSinceStartOfDay(intTrimTime1).toString("hh:mm:ss.zzz")
                << QTime::fromMSecsSinceStartOfDay(intTrimTime2).toString("hh:mm:ss.zzz");

        process.start(QDir::currentPath() + "/sox-14-4-2/sox.exe", strList);

        if(!process.waitForStarted()) {
            return;
        }
        if(!process.waitForFinished()) {
            return;
        }
}

void MakeTrack::mergeTracks(int var)
{
    QProcess process;
    QStringList strList;

        if(var != phrasesVar1){// В первом входе в функцию обьедененного файла нет, поэтому он не добавляется.
            strList << strPathSave + QString("/English Aurally (%1 фраз).wav").arg((var - phrasesVar1) + 1 - 1);
        }

        for (int varTab = 0; varTab < vecDataTabCut.size(); ++varTab){
            strList << strPathSave + QString("/%1.%2ResultSpeedFadePad.wav").arg(varTab + 1).arg(var);
        }

    strList << strPathSave + QString("/English Aurally (%1 фраз).wav").arg((var - phrasesVar1) + 1);
    process.start(QDir::currentPath() + "/sox-14-4-2/sox.exe", strList);
    if(!process.waitForStarted()) {
        return;
    }
    if(!process.waitForFinished()) {
        return;
    }
}

void MakeTrack::deleteSegments(int var)
{
    foreach (QFileInfo fileInfo, QDir(strPathSave).entryInfoList())
    {
        if(fileInfo.isFile() && fileInfo.fileName() != QString("English Aurally (%1 фраз).wav").
                                    arg((var - phrasesVar1) + 1)){ // (var - phrasesVar1) + 1 для того если
                                                            // фраза всего одна, чтоб не отображалась "0 фраз".
            QFile(fileInfo.absoluteFilePath()).remove();
        }
    }
}

void MakeTrack::makeEffects(QString effect, int varTab, int var)
{
    QProcess process;
    QStringList strList;

    if(effect == "speed")
    {
        int intSpeed = vecDataTabCut[varTab][3] - vecDataTabCut[varTab][4];
        QString strSpeed = intSpeed <= 0 ? QString("1.%1").arg(intSpeed * -1) : QString("0.%1").arg(100 - intSpeed);
        strList << strPathSave + QString("/%1.%2Result.wav").arg(varTab + 1).arg(var)
                << strPathSave + QString("/%1.%2ResultSpeed.wav").arg(varTab + 1).arg(var)
                << "speed"
                << strSpeed;
    }
    if(effect == "fade")
    {
        strList << strPathSave + QString("/%1.%2ResultSpeed.wav").arg(varTab + 1).arg(var)
                << strPathSave + QString("/%1.%2ResultSpeedFade.wav").arg(varTab + 1).arg(var)
                << "fade"
                << QString("0.%1").arg(valueFadeBefore)
                << "0"
                << QString("0.%1").arg(valueFadeAfter);
    }
    if(effect == "pad") // silence
    {
        strList << strPathSave + QString("/%1.%2ResultSpeedFade.wav").arg(varTab + 1).arg(var)
                << strPathSave + QString("/%1.%2ResultSpeedFadePad.wav").arg(varTab + 1).arg(var)
                << "pad"
                << QString::number(vecDataTabCut[varTab][5])
                << QString::number(vecDataTabCut[varTab][6]);
    }
    process.start(QDir::currentPath() + "/sox-14-4-2/sox.exe", strList);
    if(!process.waitForStarted()) {
        return;
    }
    if(!process.waitForFinished()) {
        return;
    }
}

void MakeTrack::startToMake()
{
    int progressStep = 0;
    progressBarWidget.findChild <QProgressBar *>("progressBar")->setValue(progressStep);
    progressBarWidget.findChild <QProgressBar *>("progressBar")->setRange(0,
                                        (phrasesVar2 - phrasesVar1) * vecDataTabCut.size());

    for (int var = phrasesVar1; var < phrasesVar2; ++var)
    {
        for (int varTab = 0; varTab < vecDataTabCut.size(); ++varTab)
        {
        QString phrase = QString("Обработанно фраз: %1 из %2").arg(var-phrasesVar1+1).arg(phrasesVar2-phrasesVar1);
            if(showProgressBarWidget(++progressStep, phrase) == -1){// Выходим когда нажата отмена
                deleteSegments(var - 1); // удаляем все кроме неполного результата
                return;
            }
            cutTrack(varTab, var);
            makeEffects("speed", varTab, var);
            makeEffects("fade", varTab, var);
            makeEffects("pad", varTab, var);
        }
        mergeTracks(var);
        deleteSegments(var);
    }
    progressBarWidget.hide();
}


void MakeTrack::showDilogPhrases()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);

    QLabel *labelMain = new QLabel(&dialog);
    labelMain->setText(QString("Программой обнаруженно <b>%1</b> смысловых отрезков. "
                "Укажите диапозон фраз которые нужно обрезать.").arg(vecPairCutTime.size()));
    labelMain->setWordWrap(true);
        QLabel *label = new QLabel(&dialog);
        label->setText(" - ");
    QIntValidator *intValidator = new QIntValidator(&dialog);
    intValidator->setRange(0, vecPairCutTime.size());
    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    lineEdit1->setText(QString::number(phrasesVar1));
    lineEdit1->setValidator(intValidator);
    QLineEdit *lineEdit2 = new QLineEdit(&dialog);
    lineEdit2->setText(QString::number(phrasesVar2));
    lineEdit2->setValidator(intValidator);

    QPushButton *pushButton1 = new QPushButton(&dialog);
    pushButton1->setText("Ok");
    QPushButton *pushButton2 = new QPushButton(&dialog);
    pushButton2->setText("Cancel");

    QHBoxLayout *hBoxLayout1 = new QHBoxLayout;
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout;
    QVBoxLayout vBoxLayout;
    hBoxLayout1->addWidget(lineEdit1);
    hBoxLayout1->addWidget(label);
    hBoxLayout1->addWidget(lineEdit2);
    hBoxLayout2->addWidget(pushButton1);
    hBoxLayout2->addWidget(pushButton2);
    vBoxLayout.addWidget(labelMain);
    vBoxLayout.addLayout(hBoxLayout1);
    vBoxLayout.addLayout(hBoxLayout2);
    dialog.setLayout(&vBoxLayout);

    QObject::connect(pushButton1,SIGNAL(clicked()),&dialog,SLOT(accept()));
    QObject::connect(pushButton2,SIGNAL(clicked()),&dialog,SLOT(reject()));
    if(dialog.exec() == QDialog::Accepted)
    {
        phrasesVar1 = lineEdit1->text().toInt();
        phrasesVar2 = lineEdit2->text().toInt();
    }
}
