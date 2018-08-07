#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    strTrack_1(""),
    strTrack_2(""),
    strSubtitles_1(""),
    strSubtitles_2(""),
    strObjNameRadioButtonChecked("radioButtonSubtitles"),// по дефолту программа опирается на субтитры
    strPathSave(""),
    buttonLottery(1),
    myIntRandom(0),
    ui(new Ui::MainWindow)
{
// Логика приложения описана в Помощи.

    ui->setupUi(this);

    setupBody();
    setupAction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupBody()
{
    this->setWindowTitle("English Aurally");

    QShortcut * shortcutR = new QShortcut(QKeySequence(Qt::Key_Return),ui->pushButton_begin,SLOT(animateClick()));
    QShortcut * shortcutE= new QShortcut(QKeySequence(Qt::Key_Enter),ui->pushButton_begin,SLOT(animateClick()));
    shortcutR->setAutoRepeat(false);
    shortcutE->setAutoRepeat(false);
    ui->pushButton_begin->setDisabled(true);

    ui->label_pixWave_1->setPixmap(QPixmap(":/Images/Final Materials/transparentRed.png").
                    scaled(QApplication::desktop()->width()/2, QApplication::desktop()->height()/5));
    ui->label_pixWave_1->setDisabled(true);
    ui->label_pixWave_2->setPixmap(QPixmap(":/Images/Final Materials/transparentBlue.png").
                    scaled(QApplication::desktop()->width() / 2, QApplication::desktop()->height() / 5));
    ui->label_pixWave_2->setDisabled(true);

    strPathSave = QDir::currentPath() + "/English Aurally";
    ui->lineEdit_save_path->setText(strPathSave);
    existsPathSave();

    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabCloseRequestedItem);

    for (int var = 0; var < ui->tabWidget->count(); ++var)
    {
        ui->tabWidget->setTabText(var, "Звук. дорожка 0");
        slotCreateTabWidget(var);
    }
    ui->tabWidget->setCurrentIndex(0);
    createButtonPlus();
}

void MainWindow::setupAction()
{
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->action_BaseSubtitles_1);
    actionGroup->addAction(ui->action_BaseSubtitles_2);
    ui->action_BaseSubtitles_1->setDisabled(true);
    ui->action_BaseSubtitles_2->setDisabled(true);

    ui->action_buttonLottery->setIcon(QIcon(":/Images/Final Materials/lotereya_shary_png_1.png"));
    ui->action_exit->setIcon(QIcon(":/Images/Final Materials/Exit.png"));
    ui->menu_subtitles->setIcon(QIcon(":/Images/Final Materials/gilbert bold.png"));
    ui->action_count_phrases->setIcon(QIcon(":/Images/Final Materials/number.png"));
    ui->action_Fade->setIcon(QIcon(":/Images/Final Materials/gradient.jpg"));
    ui->action_timeCut->setIcon(QIcon(":/Images/Final Materials/time.png"));
    ui->action_outputDirectory->setIcon(QIcon(":/Images/Final Materials/Open.png"));
    ui->action_administration->setIcon(QIcon(":/Images/Final Materials/Admin.png"));
    ui->action_help->setIcon(QIcon(":/Images/Final Materials/Note.png"));
    ui->action_developer->setIcon(QIcon(":/Images/Final Materials/C++Developer.png"));
    ui->action_AboutQt->setIcon(QIcon(":/Images/Final Materials/Qt.png"));

    connect(ui->action_exit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createButtonPlus()
{
// Выходит как: метод tabWidget->setTabsClosable(true); устанавливает крестик на каждой вкладке, а нам
// нужно крестики везде кроме последней вкладки. Благодаря тому что есть возможность крестик заменить собственной
// кнопкой, мы заменяем butPlus, впоследствии которую мы скроем, тем самым достигнем того что на последней
// вкладке будет отсутствовать крестик. Дальше на последнюю вкладку мы установим текст "+" и при нажатии
// на вкладку будет создаваться дополнительный таб.
    ui->tabWidget->addTab(new QWidget(), "+");
    QToolButton *butPlus = new QToolButton(ui->tabWidget->tabBar());
    butPlus->resize(0,0);
    butPlus->hide();
    ui->tabWidget->tabBar()->setTabButton(ui->tabWidget->tabBar()->count() - 1, QTabBar::RightSide, butPlus);
    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, &MainWindow::slotAddNewTab);
}

void MainWindow::existsPathSave()
{
    QDir dirSave(strPathSave);
    if(!dirSave.exists()){
        dirSave.mkdir(strPathSave);
    }
}

void MainWindow::refreshInformationOnComboBox()
{// При загрузке звукового файла, обновляется содержание комбобокса во всех вкладках.
    for (int var = 0; var < ui->tabWidget->count() - 1; ++var)
    {
        foreach (QObject *obj, ui->tabWidget->widget(var)->children())
        {
            if(obj->objectName() == "comboBoxNameSound")
            {
                int indexCombo = static_cast <QComboBox *> (obj)->currentIndex();
                static_cast <QComboBox *> (obj)->clear();
                static_cast <QComboBox *> (obj)->addItem(QFileInfo(strTrack_1).fileName());
                static_cast <QComboBox *> (obj)->addItem(QFileInfo(strTrack_2).fileName());
                static_cast <QComboBox *> (obj)->setCurrentIndex(indexCombo);
                if(strTrack_1 == ""){// Заходим сюда при первом запуске приложения. Если загрузили трек 2, тогда он -
                    indexCombo = 1;// - автоматически становиться текущим.
                    static_cast <QComboBox *> (obj)->setCurrentIndex(indexCombo);
                }
                if(indexCombo == 0 && strTrack_1 != ""){
                    ui->tabWidget->tabBar()->setTabTextColor(var, Qt::red);
                    ui->tabWidget->tabBar()->setTabText(var, "Звук. дорожка 1");
                }
                if(indexCombo == 1 && strTrack_2 != ""){
                    ui->tabWidget->tabBar()->setTabTextColor(var, Qt::blue);
                    ui->tabWidget->tabBar()->setTabText(var, "Звук. дорожка 2");
                }
            }
        }
    }
}

void MainWindow::setDisabledPushButton_begin()
{
    ui->pushButton_begin->setDisabled(true);
    if ((strTrack_1 != "" || strTrack_2 != "") && (strSubtitles_1 != "" || strSubtitles_2 != "")){
        ui->pushButton_begin->setDisabled(false);
    }
    if ((strTrack_1 != "" || strTrack_2 != "") && strObjNameRadioButtonChecked == "radioButtonSegment"){
        ui->pushButton_begin->setDisabled(false);
    }
}

void MainWindow::readSubtitles_OR_push_backTimeCut()
{
    if(strObjNameRadioButtonChecked == "radioButtonSegment"){
        makeTrack.push_backTimeCut();
    }
    else{
        makeTrack.readSubtitles(ui->action_BaseSubtitles_1->isChecked() ? 1 : 2);
    }
}

void MainWindow::slotAddNewTab(int index)
{
    if(index == ui->tabWidget->count() - 1){// Если это последняя вкладка, мы добавляем таб.
        ui->tabWidget->removeTab(ui->tabWidget->count() - 1);// Удаляем вкладку с "+"

        ui->tabWidget->addTab(new QWidget(), strTrack_1 == "" ? "Звук. дорожка 0" : "Звук. дорожка 1");
        slotCreateTabWidget(ui->tabWidget->count() - 1);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

        createButtonPlus();// Создаем вкладку с "+"
    }
}

void MainWindow::slotOnComboBox(int indexCombo)
{
    if(indexCombo == 0 && strTrack_1 != ""){
        ui->tabWidget->tabBar()->setTabTextColor(ui->tabWidget->currentIndex(), Qt::red);
        ui->tabWidget->tabBar()->setTabText(ui->tabWidget->currentIndex(), "Звук. дорожка 1");
    }
    if(indexCombo == 1 && strTrack_2 != ""){
        ui->tabWidget->tabBar()->setTabTextColor(ui->tabWidget->currentIndex(), Qt::blue);
        ui->tabWidget->tabBar()->setTabText(ui->tabWidget->currentIndex(), "Звук. дорожка 2");
    }
    setDisabledPushButton_begin();
}

void MainWindow::slotOnRadioButton(bool boolClicked)
{
    strObjNameRadioButtonChecked = sender()->objectName();
    for (int var = 0; var < ui->tabWidget->count() - 1; ++var)
    {//for для того чтобы во всех вкладках tabWidget radioButton был одинаков, нажат или нет.
        foreach (QObject *obj, ui->tabWidget->widget(var)->children())
        {
            if(obj->objectName() == strObjNameRadioButtonChecked)
            {
                static_cast <QRadioButton *> (obj)->setChecked(boolClicked);
            }
        }
    }
    setDisabledPushButton_begin();
}

void MainWindow::slotCreateTabWidget(int var)
{// Создается таб виджет.
        if(strTrack_1 != ""){// красный текст только в том случае, если уже указан путь к Track 1.
            ui->tabWidget->tabBar()->setTabTextColor(var, Qt::red);// По дефолту трек 1 становиться текущим.
        }
        QComboBox *comboBoxNameSound = new QComboBox(this);
        comboBoxNameSound->addItem(QFileInfo(strTrack_1).fileName());
        comboBoxNameSound->addItem(QFileInfo(strTrack_2).fileName());
        connect(comboBoxNameSound, SIGNAL(activated(int)), this, SLOT(slotOnComboBox(int)));

        QRadioButton *radioButtonSubtitles = new QRadioButton("Обрезать по субтитрам", this);
        radioButtonSubtitles->setChecked(strObjNameRadioButtonChecked == "radioButtonSubtitles");
        QRadioButton *radioButtonSegment = new QRadioButton("Обрезать по отрезкам", this);
        radioButtonSegment->setChecked(strObjNameRadioButtonChecked == "radioButtonSegment");
        connect(radioButtonSubtitles, SIGNAL(clicked(bool)), this, SLOT(slotOnRadioButton(bool)));
        connect(radioButtonSegment, SIGNAL(clicked(bool)), this, SLOT(slotOnRadioButton(bool)));
        QHBoxLayout *hBoxLayoutRadio = new QHBoxLayout;
        hBoxLayoutRadio->addWidget(radioButtonSubtitles);
        hBoxLayoutRadio->addWidget(radioButtonSegment);

        QSpinBox *spinBoxSlowDown = new QSpinBox(this);
        spinBoxSlowDown->setSuffix(" %");
        spinBoxSlowDown->setRange(0,99);
        QLabel *labelSlowDown = new QLabel("Замедление", this);
        QSpinBox *spinBoxSpeed = new QSpinBox(this);
        spinBoxSpeed->setSuffix(" %");
        spinBoxSpeed->setRange(0,99);
        QLabel *labelSpeed = new QLabel("Ускорение", this);
        QHBoxLayout *hBoxLayoutTempo = new QHBoxLayout;
        hBoxLayoutTempo->addWidget(spinBoxSlowDown);
        hBoxLayoutTempo->addWidget(labelSlowDown);
        hBoxLayoutTempo->addWidget(spinBoxSpeed);
        hBoxLayoutTempo->addWidget(labelSpeed);

        QSpinBox *spinBoxSilenceBefore = new QSpinBox(this);
        spinBoxSilenceBefore->setSuffix(" s");
        spinBoxSilenceBefore->setRange(0,60);
        QLabel *labelSilenceBefore = new QLabel("Тишина до фрагмента", this);
        QSpinBox *spinBoxSilenceAfter = new QSpinBox(this);
        spinBoxSilenceAfter->setSuffix(" s");
        spinBoxSilenceAfter->setRange(0,60);
        QLabel *labelSilenceAfter = new QLabel("Тишина после фрагмента", this);
        QHBoxLayout *hBoxLayoutDynamics = new QHBoxLayout;
        hBoxLayoutDynamics->addWidget(spinBoxSilenceBefore);
        hBoxLayoutDynamics->addWidget(labelSilenceBefore);
        hBoxLayoutDynamics->addWidget(spinBoxSilenceAfter);
        hBoxLayoutDynamics->addWidget(labelSilenceAfter);

        QSpinBox *spinBoxBiasBefore = new QSpinBox(this);
        spinBoxBiasBefore->setSuffix(" ms");
        spinBoxBiasBefore->setRange(-1000000, 1000000);
        spinBoxBiasBefore->setSingleStep(10);
        QLabel *labelBiasBefore = new QLabel("Смещение фрагмента", this);
        QSpinBox *spinBoxBiasAfter = new QSpinBox(this);
        spinBoxBiasAfter->setSuffix(" ms");
        spinBoxBiasAfter->setRange(-1000000, 1000000);
        spinBoxBiasAfter->setSingleStep(10);
        QLabel *labelBiasAfter = new QLabel("Смещение верхней границы фрагмента", this);
        QHBoxLayout *hBoxLayoutBias = new QHBoxLayout;
        hBoxLayoutBias->addWidget(spinBoxBiasBefore);
        hBoxLayoutBias->addWidget(labelBiasBefore);
        hBoxLayoutBias->addWidget(spinBoxBiasAfter);
        hBoxLayoutBias->addWidget(labelBiasAfter);

        comboBoxNameSound->setObjectName("comboBoxNameSound");
        radioButtonSubtitles->setObjectName("radioButtonSubtitles");
        radioButtonSegment->setObjectName("radioButtonSegment");
        spinBoxSlowDown->setObjectName("spinBoxSlowDown");
        spinBoxSpeed->setObjectName("spinBoxSpeed");
        spinBoxSilenceBefore->setObjectName("spinBoxSilenceBefore");
        spinBoxSilenceAfter->setObjectName("spinBoxSilenceAfter");
        spinBoxBiasBefore->setObjectName("spinBoxBiasBefore");
        spinBoxBiasAfter->setObjectName("spinBoxBiasAfter");

        QVBoxLayout *vBoxLayoutTabWidget = new QVBoxLayout;
        vBoxLayoutTabWidget->addWidget(comboBoxNameSound);
        vBoxLayoutTabWidget->addLayout(hBoxLayoutRadio);
        vBoxLayoutTabWidget->addLayout(hBoxLayoutTempo);
        vBoxLayoutTabWidget->addLayout(hBoxLayoutDynamics);
        vBoxLayoutTabWidget->addLayout(hBoxLayoutBias);
        ui->tabWidget->widget(var)->setLayout(vBoxLayoutTabWidget);

}

void MainWindow::slotTabCloseRequestedItem(int index)
{
    // чтоб не удалялась последняя вкладка которая осталась и вкладка с "+". Количество вкладок всегда будет 2.
    if(ui->tabWidget->count() == 2) return;
//    delete ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1  == index ? index-1 : index);
}


void MainWindow::on_pushButton_track_1_clicked()
{
    QString strTrack = QFileDialog::getOpenFileName(0, tr("Open Track"), "", "*.wav");
        if(strTrack.isEmpty()) return;
    strTrack_1 = strTrack;
    ui->label_pixWave_1->setDisabled(false);
    refreshInformationOnComboBox();
    makeTrack.setStrPathTrack_1(strTrack_1);
    setDisabledPushButton_begin();
}

void MainWindow::on_pushButton_subtitles_1_clicked()
{
    QString str = QFileDialog::getOpenFileName(0,tr("Open questions"), QDir::currentPath(),"*.srt ;; *.txt");
        if(str.isEmpty()) return;
    strSubtitles_1 = str;
    ui->action_BaseSubtitles_1->setDisabled(false);
    ui->action_BaseSubtitles_1->setChecked(true);
    makeTrack.setStrPathSubtitles_1(strSubtitles_1);
    readSubtitles_OR_push_backTimeCut();// вызов нужен чтоб коректно отображались данные в Action count_phrases
    setDisabledPushButton_begin();

}

void MainWindow::on_pushButton_track_2_clicked()
{
    QString strTrack = QFileDialog::getOpenFileName(0, tr("Open Track"), "", "*.wav");
        if(strTrack.isEmpty()) return;
    strTrack_2 = strTrack;
    ui->label_pixWave_2->setDisabled(false);
    refreshInformationOnComboBox();
    makeTrack.setStrPathTrack_2(strTrack_2);
    setDisabledPushButton_begin();
}

void MainWindow::on_pushButton_subtitles_2_clicked()
{
    QString str = QFileDialog::getOpenFileName(0,tr("Open questions"), QDir::currentPath(),"*.srt ;; *.txt");
        if(str.isEmpty()) return;
    strSubtitles_2 = str;
    ui->action_BaseSubtitles_2->setDisabled(false);
    ui->action_BaseSubtitles_2->setChecked(true);
    makeTrack.setStrPathSubtitles_2(strSubtitles_2);
    readSubtitles_OR_push_backTimeCut();// вызов нужен чтоб коректно отображались данные в Action count_phrases
    setDisabledPushButton_begin();
}

void MainWindow::on_pushButton_begin_clicked()
{
    QVector <QVector<int> > vecDataTab;
    for (int var = 0; var < ui->tabWidget->count() - 1; ++var)
    {// Перед запуском считываем информацию и записываем ее в вектор.
        QVector <int> vecDataTabLittle = {0,0,0,0,0,0,0,0,0};
        foreach (QObject *obj, ui->tabWidget->widget(var)->children())
        {
            if(obj->objectName() == "comboBoxNameSound"){
                vecDataTabLittle [0] = static_cast <QComboBox *> (obj)->currentIndex();
            }
            if(obj->objectName() == "radioButtonSubtitles"){
                vecDataTabLittle [1] = static_cast <QRadioButton *> (obj)->isChecked();
            }
            if(obj->objectName() == "radioButtonSegment"){
                vecDataTabLittle [2] = static_cast <QRadioButton *> (obj)->isChecked();
            }
            if(obj->objectName() == "spinBoxSlowDown"){
                vecDataTabLittle [3] = static_cast <QSpinBox *> (obj)->value();
            }
            if(obj->objectName() == "spinBoxSpeed"){
                vecDataTabLittle [4] = static_cast <QSpinBox *> (obj)->value();
            }
            if(obj->objectName() == "spinBoxSilenceBefore"){
                vecDataTabLittle [5] = static_cast <QSpinBox *> (obj)->value();
            }
            if(obj->objectName() == "spinBoxSilenceAfter"){
                vecDataTabLittle [6] = static_cast <QSpinBox *> (obj)->value();
            }
            if(obj->objectName() == "spinBoxBiasBefore"){
                vecDataTabLittle [7] = static_cast <QSpinBox *> (obj)->value();
            }
            if(obj->objectName() == "spinBoxBiasAfter"){
                vecDataTabLittle [8] = static_cast <QSpinBox *> (obj)->value();
            }
        }
        vecDataTab.push_back(vecDataTabLittle);
    }
    makeTrack.setDataTab(vecDataTab);
    makeTrack.setStrPathSave(strPathSave);
    readSubtitles_OR_push_backTimeCut();// Решается чем заполнить вектор, данными от субтитров или по отрезкам.
    existsPathSave();
    makeTrack.startToMake();
}

void MainWindow::on_action_BaseSubtitles_1_triggered()
{
    makeTrack.setBaseSubtitles(1);
    readSubtitles_OR_push_backTimeCut();// Нужен вызов функции чтоб коректно отображались данные в Action
}

void MainWindow::on_action_BaseSubtitles_2_triggered()
{
    makeTrack.setBaseSubtitles(2);
    readSubtitles_OR_push_backTimeCut();
}

void MainWindow::on_action_count_phrases_triggered()
{
    readSubtitles_OR_push_backTimeCut();// Нужен вызов функции чтоб обновились данные
    makeTrack.showDilogPhrases();
}

void MainWindow::on_action_Fade_triggered()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
        QLabel *labBefore = new QLabel("Перед:",&dialog);
        QLabel *labAfter = new QLabel("После:",&dialog);
    QSpinBox *spinBoxBefore = new QSpinBox(&dialog);
    spinBoxBefore->setPrefix("0.");
    spinBoxBefore->setSuffix(" s");
    spinBoxBefore->setRange(0,999);
    spinBoxBefore->setMinimumHeight(45);
    spinBoxBefore->setValue(makeTrack.valueFadeBefore);
    QSpinBox *spinBoxAfter = new QSpinBox(&dialog);
    spinBoxAfter->setPrefix("0.");
    spinBoxAfter->setSuffix(" s");
    spinBoxAfter->setRange(0,999);
    spinBoxAfter->setMinimumHeight(45);
    spinBoxAfter->setValue(makeTrack.valueFadeAfter);

    QPushButton *pushButton1 = new QPushButton(&dialog);
    pushButton1->setText("Ok");
    QPushButton *pushButton2 = new QPushButton(&dialog);
    pushButton2->setText("Cancel");
    QGridLayout gridLayout;
//    gridLayout.addWidget(spinBoxBefore,0,0,1,0);
    gridLayout.addWidget(labBefore,0,0);
    gridLayout.addWidget(labAfter,0,1);
    gridLayout.addWidget(spinBoxBefore,1,0);
    gridLayout.addWidget(spinBoxAfter,1,1);
    gridLayout.addWidget(pushButton1,2,0);
    gridLayout.addWidget(pushButton2,2,1);
    dialog.setLayout(&gridLayout);

    dialog.setFixedSize(dialog.minimumSize());
    connect(pushButton1,SIGNAL(clicked()),&dialog,SLOT(accept()));
    connect(pushButton2,SIGNAL(clicked()),&dialog,SLOT(reject()));
    if(dialog.exec() == QDialog::Accepted)
    {
        makeTrack.setValueFade(spinBoxBefore->value(), spinBoxAfter->value());
    }
}

void MainWindow::on_action_timeCut_triggered()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
        QLabel *labUp = new QLabel("Обрезать по отрезкам:",&dialog);
    QDoubleSpinBox *doubleSpinBoxTimeCut = new QDoubleSpinBox(&dialog);
    doubleSpinBoxTimeCut->setSuffix(" s");
    doubleSpinBoxTimeCut->setRange(0,999.0);
    doubleSpinBoxTimeCut->setMinimumHeight(45);
    doubleSpinBoxTimeCut->setValue(double(makeTrack.timeCut) / double(1000));

    QPushButton *pushButton1 = new QPushButton(&dialog);
    pushButton1->setText("Ok");
    QPushButton *pushButton2 = new QPushButton(&dialog);
    pushButton2->setText("Cancel");
    QGridLayout gridLayout;
    gridLayout.addWidget(labUp,0,0,1,0);
    gridLayout.addWidget(doubleSpinBoxTimeCut,1,0,1,0);
    gridLayout.addWidget(pushButton1,2,0);
    gridLayout.addWidget(pushButton2,2,1);
    dialog.setLayout(&gridLayout);

    dialog.setFixedSize(dialog.minimumSize());
    connect(pushButton1,SIGNAL(clicked()),&dialog,SLOT(accept()));
    connect(pushButton2,SIGNAL(clicked()),&dialog,SLOT(reject()));
    if(dialog.exec() == QDialog::Accepted){
        makeTrack.timeCut = doubleSpinBoxTimeCut->value() * 1000; // переводим в миллисекунды
    }
}

void MainWindow::on_action_outputDirectory_triggered()
{
    QString strSave = QFileDialog::getExistingDirectory(0, tr("Open Track"), "");
    if(strSave.isEmpty()) return;
    strPathSave = strSave + "/English Aurally";
    ui->lineEdit_save_path->setText(strPathSave);
}

void MainWindow::on_action_help_triggered()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
    dialog.setEnabled(true);
    QScrollArea scr(&dialog);
    QWidget wgt(&dialog);

    QPalette palette;
    palette.setBrush(QPalette::Normal, QPalette::Background,QBrush(Qt::lightGray));
    dialog.setPalette(palette);

    QVBoxLayout vbox(&wgt);
    QVBoxLayout vbox2;
        QLabel lab1;
        lab1.setPixmap(QPixmap(":/Images/Final Materials/icoPngMergeTransparent.png").
                       scaled(500,200,Qt::KeepAspectRatio));
        QLabel lab2;
        lab2.setText("<span style=\"color: rgb(21, 103, 202);\">"
            "Программа предназначена для обучения языкам на слух. Чтобы начать это делать нужно<br>"
            "сделать несколько операций, а именно:<br>"
            "  <b>1.</b> Найти подходящий фильм для обучения интересующего языка.<br>"
            "  <b>2.</b> Скачать фильм. Он должен иметь несколько аудио дорожек, вашего родного языка и звуковую<br>"
            "дорожку обучающего языка. Плюс к этому, в видео файле должны быть субтитры вашего родного<br>"
            "языка и субтитры обучающего языка, или хотя бы субтитры одного языка.<br>"
            "  <b>3.</b> Извлечь аудио дорожку из фильма. В этом может помочь программа AVCFree<br>"
            "(Free Video Converter). Сначала извлекается одна звуковая дорожка, после другая. Важно!<br>"
            "Звуковые дорожки должны иметь формат (.wav).<br>"
            "  <b>4.</b> Извлечь субтитры из фильма. Здесь я ничего не могу посоветовать. Однако нужно извлечь<br>"
            "сначала одни субтитры, после другие. Есть вариант скачать отдельно субтитры в интернете.<br>"
            "Формат субтитров (.srt). После скачивания субтитров откройте файл в обычном блокноте<br>"
            "и посмотрите, совпадает ли время появления субтитров с аудио дорожкой (Просто включите<br>"
            "фильм и посмотрите, если на такое время 00:01:21,212 говорятся слова такие же как<br>"
            "написаны в субтитрах и есть совпадение по времени, тогда все нормально.) Если все<br>"
            "совпадает, но есть смещение во времени, это не страшно, смещение можно будет поправить.<br>"
            "  <b>5.</b> После соблюдений выше написанных шагов, можно запустить программу и воспользоваться<br>"
            "ее функционалом. Если конечно программа не запущенна.</span><br><br>"

            "<span style=\"color: rgb(228, 87, 39);\"><b>Пример:<br></b>"
            "<b>Шаг 1.</b> Мне нравиться фильм (Кунг фу панда). Я его скачиваю.<br>"
            "<b>Шаг 2.</b> Перед скачиванием посмотрел, что в фильме есть русская и английская дорожки.<br>"
            "Также я посмотрел, чтоб были русские и английские субтитры. Если каких-то субтитров нет,<br>"
            "ничего страшного, их можно будет скачать отдельно в интернете.<br>"
            "<b>Шаг 3.</b> У меня на компьютере уже установлена программа AVCFree. Я запускаю ее и извлекаю<br>"
            "сначала русскую дорожку, потом английскую.<br>"
            "<b>Шаг 4.</b> В фильме субтитров не оказалось, я пошел их искать в интернете. Нашел. Скачал. Дальше<br>"
            "открываю скачанный файл с помощью блокнота, запускаю фильм и начинаю проверять на совпадение.<br>"
            "Если свопадают слова и не совпадает время, не страшно, в программе это можно будет подправить.<br>"
            "<b>Шаг 5.</b> На этом этапе уже все готово. Запускаю программу, загружаю в нее звуковые дорожки,<br>"
            "загружаю субтитры, произвожу некоторые настройки, и могу наслаждаться результатом.</span><br><br>"

            "<span style=\"color: rgb(21, 103, 202);\"><b>Что делает программа?<br></b>"
            "Программа обрезает аудиодорожку на фрагменты. То есть, сначала берет фрагмент с английской<br>"
            "дорожки, вырезает его, потом берет точно такой же фрагмент с русской дорожки и вырезает его.<br>"
            "После соединяет эти фрагменты вместе. В итоге получается эдакая английская речь с переводом.<br>"
            "Это все происходит с опорой на субтитры. То есть, программа считывает время, где начинаются и<br>"
            "заканчиваются фразы в фильме, и обрезает аудиодорожки, ссылаясь на указанное время. В этом смысле,<br>"
            "лучше брать фильмы с ярко выраженными диалогами, однако все зависит от ваших предпочтений."
            "</span><br><br>"

            "<span style=\"color: rgb(228, 87, 39);\"><b>Какие бы я сделал настройки?<br></b>"
            "Для начала я бы определил спираль - 11211. То есть, чтобы сначала говорилась английская фраза,<br>"
            "потом снова английская фраза, потом русская, и потом опять английская, и еще раз английская.<br>"
            "Мне кажется, для начала это было бы хорошо для восприятия. Следующим шагом настроил бы<br>"
            "тишину после фрагмента. Сделал бы одну секунду тишины. Ну и в конце, если субтитры не<br>"
            "совпадают по времени, настроил бы смещение.</span><br>");

        vbox.addWidget(&lab1);
        vbox.addWidget(&lab2);

        wgt.setLayout(&vbox);

        scr.setWidget(&wgt);
        vbox2.addWidget(&scr);

    dialog.setLayout(&vbox2);
    dialog.setFixedSize(610,654);
    dialog.exec();
}

void MainWindow::on_action_developer_triggered()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
    QScrollArea scr(&dialog);
    QWidget wgt(&dialog);

    QPalette palette;
    palette.setBrush(QPalette::Normal, QPalette::Background,QBrush(Qt::lightGray));
    dialog.setPalette(palette);

    QVBoxLayout vbox(&wgt);
    QVBoxLayout vbox2;
        QLabel lab1;
        lab1.setPixmap(QPixmap(":/Images/Final Materials/Logo_temporarily.png"));
        QLabel lab2;
        lab2.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab2.setText("Разработчик: \n"
                     "  Сонгров Илья. На данный момент получающий образование в музыкальной области и \n"
                     "  интересующийся программированием.");
        QLabel lab3;
        lab3.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab3.setText("Мои контакты: \n"
                     "  телефон:  +380983008644\n"
                     "Автор логотипа: \n"
                     "  Костенко Марк (vk - https://vk.com/id305604444)");

        QLabel lab4;
        lab4.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab4.setText("Программа является бесплатной и может распространяться по лицензии свободных программ,\n"
                     "если не приносит вред и не нарушает лицензию Qt.");
        vbox.addWidget(&lab1);
        vbox.addWidget(&lab2);
        vbox.addWidget(&lab3);
        vbox.addWidget(&lab4);

        wgt.setLayout(&vbox);
        scr.setWidget(&wgt);
        vbox2.addWidget(&scr);

    dialog.setLayout(&vbox2);
    dialog.setFixedSize(570,390);
    dialog.exec();
}

void MainWindow::on_action_administration_triggered()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
    QLabel *labUp = new QLabel("<b>Введите пароль. Это секретные настройки:</b>",&dialog);
    QLineEdit *lineEdit= new QLineEdit(&dialog);
    lineEdit->setPlaceholderText("Год написания");
    QLabel *labDown = new QLabel(&dialog);

    QPushButton *pushButton1 = new QPushButton(&dialog);
    pushButton1->setText("Ok");
    pushButton1->setDisabled(true);
    QPushButton *pushButton2 = new QPushButton(&dialog);
    pushButton2->setText("Cancel");
    QGridLayout gridLayout;
    gridLayout.addWidget(labUp,0,0,1,0);
    gridLayout.addWidget(lineEdit,1,0,1,0);
    gridLayout.addWidget(labDown,2,0,1,0);
    gridLayout.addWidget(pushButton1,3,0);
    gridLayout.addWidget(pushButton2,3,1);
    dialog.setLayout(&gridLayout);

    dialog.setFixedSize(dialog.minimumSize());
    int step = 0;
    connect(lineEdit, QLineEdit::textChanged, this, [&](const QString &str)
    {
        QString arrNoYes[6] = {"Нет ", "Нет Нет ", "Нет Нет Нет ", "Да ", "Да Да ", "Да Да Да "};
            pushButton1->setDisabled(str.length() < 18);
            labDown->setText(arrNoYes[step++ + (str.length() < 18 ? 0 : 3)]);
        if(step == 3) step = 0;
    });
    connect(pushButton1,SIGNAL(clicked()),&dialog,SLOT(accept()));
    connect(pushButton2,SIGNAL(clicked()),&dialog,SLOT(reject()));
    if(dialog.exec() == QDialog::Accepted)
    {
        QDialog dialog;
        dialog.setWindowFlags(Qt::WindowCloseButtonHint);
            QLabel *labUp = new QLabel("Контроль рандома:",&dialog);
        QSpinBox *spinBoxRandom = new QSpinBox(&dialog);
        spinBoxRandom->setSuffix(" - билет");
        spinBoxRandom->setRange(0,100);
        spinBoxRandom->setMinimumHeight(45);
        spinBoxRandom->setValue(6);

        QPushButton *pushButton1 = new QPushButton("Ok",&dialog);
        QPushButton *pushButton2 = new QPushButton("Cancel",&dialog);
        QGridLayout gridLayout;
        gridLayout.addWidget(labUp,0,0,1,0);
        gridLayout.addWidget(spinBoxRandom,1,0,1,0);
        gridLayout.addWidget(pushButton1,2,0);
        gridLayout.addWidget(pushButton2,2,1);
        dialog.setLayout(&gridLayout);

        dialog.setFixedSize(dialog.minimumSize());
        connect(pushButton1,SIGNAL(clicked()),&dialog,SLOT(accept()));
        connect(pushButton2,SIGNAL(clicked()),&dialog,SLOT(reject()));
        if(dialog.exec() == QDialog::Accepted){
            myIntRandom = spinBoxRandom->value();
        }
    }
}

void MainWindow::on_action_buttonLottery_triggered()
{
    int intRandom = qrand() % 100;

    if(myIntRandom != 0){
        intRandom = myIntRandom;
        myIntRandom = 0;
    }
    if(intRandom == 6){
        QMessageBox::about(this,"Surprise", "Поздравляю! Номер выпавшего билета = " + QString::number(intRandom)
        + ". И он оказался счастливым! У вас это вышло с " + QString::number(buttonLottery) + " раза. Позвоните "
        "мне на мой номер +380983008644 и я пополню ваш счет на 50гр. Если вы отказываетесь, тогда вы пополняете"
                                                                                              " мне счет.");
        return;
    }
    QMessageBox::information(this,"Lottery", "Вы " + QString::number(buttonLottery) + " раз нажали на эту кнопку."
        " Это кнопка Лотерея. Продолжайте нажимать. Есть вероятность 1 к 100 что вы выиграете."
        " Номер выпавшего билета = " + QString::number(intRandom) + ". Увы, но он не оказался счастливым.");

    buttonLottery++;
}

void MainWindow::on_action_AboutQt_triggered()
{
    QMessageBox::aboutQt(this,"AboutQt");
}
