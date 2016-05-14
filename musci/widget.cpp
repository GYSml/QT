#include <windows.h>
#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPalette>
#include <QFile>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{


    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);

    //设置列表样式
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条

    QPalette pll = ui->tableWidget->palette();
    pll.setBrush(QPalette::Base,QBrush(QPixmap(":/new/prefix1/background/background_1.png")));
    ui->tableWidget->setPalette(pll);



//    Sleep(1000);
    QFont font;
    font.setBold(true);
    font.setPointSize(15);

    QFont font1;
    font1.setBold(true);
    font1.setPointSize(20);

    setWindowIcon(QIcon(":/new/prefix1/icon/tuopan.png"));
    //当前歌词显示
    ui->lrcLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->lrcLabel->setFont(font1);
    ui->lrcLabel->setAlignment(Qt::AlignCenter);
    ui->lrcLabel->setStyleSheet("color: rgb(56, 212, 255)");
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    //当前播放音乐歌曲名
    ui->musicName->setAttribute(Qt::WA_TranslucentBackground);
    ui->musicName->setAlignment(Qt::AlignCenter);
    ui->musicName->setFont(font);
    ui->musicName->setText(tr("歌曲名"));
    //前一行歌词
    ui->privouslrc->setAttribute(Qt::WA_TranslucentBackground);
    ui->privouslrc->setFont(font);
    ui->privouslrc->setAlignment(Qt::AlignCenter);
    ui->privouslrc->setStyleSheet("color: rgb(255,255,255)");
    //后一行歌词
    ui->laterlrc->setAttribute(Qt::WA_TranslucentBackground);
    ui->laterlrc->setAlignment(Qt::AlignCenter);
    ui->laterlrc->setFont(font);
    ui->laterlrc->setStyleSheet("color: rgb(255,255,255)");
    //进度条当前时间
    ui->currTime->setAttribute(Qt::WA_TranslucentBackground);
    //歌曲总时间
    ui->totalTime->setAttribute(Qt::WA_TranslucentBackground);
    //进度条
   // ui->slider->setAttribute(Qt::WA_TranslucentBackground);
    //音量调节
    ui->volumBox->setAttribute(Qt::WA_TranslucentBackground);

    ui->pauseBtn->setToolTip("播放");
    ui->backBtn->setToolTip("上一首");
    ui->nextBtn->setToolTip("下一首");
    ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/image/play_hover.png);}"
                                "QPushButton:focus{padding:-1;}");
    ui->backBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/prev_hover1.png);}"
                               "QPushButton:hover{border-image: url(:/new/prefix1/icon/prev_hover.png);}"
                               "QPushButton:pressed{border-image: url(:/new/prefix1/icon/image/prev_hover1.png);}"
                               "QPushButton:focus{padding:-1;}");
    ui->nextBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/next_hover1.png);}"
                               "QPushButton:hover{border-image: url(:/new/prefix1/icon/next_hover.png);}"
                               "QPushButton:pressed{border-image: url(:/new/prefix1/icon/image/next_hover1.png);}"
                               "QPushButton:focus{padding:-1;}");
    ui->pushButton_3->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/add_2.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/add.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/add_2.png);}"
                                "QPushButton:focus{padding:-1;}");
    ui->pushButton->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/list_2.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/list.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/list_2.png);}"
                                "QPushButton:focus{padding:-1;}");
    ui->modelBtn->setToolTip("顺序播放");
    ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                            "QPushButton:hover{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                            "QPushButton:pressed{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                            "QPushButton:focus{padding:-1;}");
    ui->lrcShow->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/lrc_2.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/lrc.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/lrc_2.png);}"
                                "QPushButton:focus{padding:-1;}");
    ui->login->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/denglu_2.png);}"
                            "QPushButton:hover{border-image: url(:/new/prefix1/icon/denglu.png);}"
                            "QPushButton:pressed{border-image: url(:/new/prefix1/icon/denglu_2.png);}"
                            "QPushButton:focus{padding:-1;}");

    //定时器
    ui->timeEdit->setAttribute(Qt::WA_TranslucentBackground);
    ui->timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->timeLabel->hide();
    ui->timeEdit->hide();


    //定时
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(setPosition()));

    backTimer = new QTimer;
    connect(backTimer,SIGNAL(timeout()),this,SLOT(backGroungChange()));
    backTimer->start(3000);

    myLrc = new MyLrc();
    ui->tableWidget->hide();

    play = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    playList->addMedia(QUrl::fromLocalFile("E:/QT/musci/music/城府 - 许嵩.mp3"));
    playList->addMedia(QUrl::fromLocalFile("E:/QT/musci/music/shakira - try everything.mp3"));
    playList->addMedia(QUrl::fromLocalFile("E:/QT/musci/music/Maroon 5 - Maps.mp3"));
    play->setPlaylist(playList);
    play->setVolume(50);

    ui->volumBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->volumBox->setText(tr("静音"));
    ui->volumBox->setCheckable(true);
    ui->volumBox->setChecked(false);
    ui->volumBox->setVisible(false);

    ui->slider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->slider->setStyleSheet("  \
    QSlider::add-page:Horizontal\
    {  \
        background-color: rgb(87, 97, 106);\
       height:4px;\
    }\
    QSlider::sub-page:Horizontal \
   {\
        background-color: rgb(37, 168, 198);\
        height:4px;\
    }\
   QSlider::groove:Horizontal \
   {\
       background:transparent;\
       height:6px;\
   }\
   QSlider::handle:Horizontal \
   {\
       height:35px;\
       width:22px;\
       border-image: url(:/new/prefix1/icon/progress_thumb_2.png);\
       margin: -8 -6px; \
   }\
   ");

    ui->volumSlider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->volumSlider->setRange(1,100);
    ui->volumSlider->setSliderPosition(play->volume());
    ui->volumSlider->setToolTip("音量:50");
    ui->volumSlider->setVisible(false);
    ui->volumSlider->setStyleSheet("  \
                                   \
                                    QSlider::add-page:vertical\
                                    {     \
                                       background-color: rgb(37, 168, 198);\
                                       width:4px;\
                                    }\
                                    QSlider::sub-page:vertical \
                                   {\
                                       background-color: rgb(87, 97, 106);\
                                       width:4px;\
                                    }\
                                   QSlider::groove:vertical \
                                   {\
                                       background:transparent;\
                                       width:6px;\
                                   }\
                                   QSlider::handle:vertical \
                                   {\
                                        height: 22px;\
                                       width:22px;\
                                       border-image: url(:/new/prefix1/icon/progress_thumb_2.png);\
                                        margin: -6 -8px; \
                                    }\
                                   ");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui->pauseBtn,SIGNAL(clicked()),this,SLOT(pauseBtnClicked()));
    connect(play,SIGNAL(positionChanged(qint64)),this,SLOT(UpdateTime(qint64)));
    connect(play,SIGNAL(metaDataChanged()),this,SLOT(btnUpdate()));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(tableClick(int,int)));
    //定时器退出
    timer1 = new QTimer;
    connect(timer1,SIGNAL(timeout()),this,SLOT(subtime()));
    connect(ui->timeEdit,SIGNAL(currentIndexChanged(int)),this,SLOT(start(int)));



}
void Widget::start(int i)
{
    switch (i) {
    case 0:
        timer->stop();
        ui->timeLabel->setText("默认");
        return;
        break;
    case 1:
        settime(0,1,0);
        break;
    case 2:
        settime(0,3,0);
        break;
    case 3:
        settime(0,5,0);
        break;
    case 4:
        settime(0,10,0);
        break;
    case 5:
        settime(0,30,0);
        break;
    default:
        break;
    }

        //设定m_timer为1s发送一次信号
        while(m_time&&!timer1->isActive())
        {
            timer1->start(1000);
        }


}

//显示时间
void Widget::showtime()
{   //将剩余时间转化为时，分，秒
    int h = m_time/3600;
    int m_time1 = m_time%3600;
    int m = m_time1/60;
    int s = m_time1%60;

    QTime time = QTime(h,m,s);
    QString text = time.toString("%1:%2:%3").arg(h).arg(m).arg(s);
    ui->timeLabel->setText(text);

    if(m_time<=0)
    {
        timer1->stop();
        exit(0);
    }

}
//提交当前剩余时间
void Widget::subtime()
{
    m_time--;//剩余时间减少一秒
    showtime();
}

//设置倒计时
void Widget::settime(const unsigned int time0, const unsigned int time, const unsigned int time1)
{
   m_time = time0*3600+time*60+time1;
}


void Widget::backGroungChange()
{
    QPixmap pixmap;
    QString path = ":/new/prefix1/background/background_";
    path =  path + QString::number((qrand()%11+1))+".png";
    pixmap.load(path);
    ui->backgroud->setPixmap(pixmap);

}

void Widget::tableClick(int row, int colum)
{
    playList->setCurrentIndex(row+3);
    pauseBtnClicked();
}
void Widget::setPosition()
{
    qint64 position = play->position();
    ui->slider->setMaximum(play->duration());
    ui->slider->setSliderPosition(position);

    ui->slider->setValue(position);
    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));

    qint64 maxPosition = play->duration();
    QTime sumTime(0, maxPosition / 60000, qRound((maxPosition % 60000) / 1000.0));
    ui->totalTime->setText(sumTime.toString(tr("mm:ss")));
    ui->currTime->setText(duration.toString(tr("mm:ss")));

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover1.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/image/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
       ui-> pauseBtn->setToolTip("暂停");
    }

}
void Widget::btnUpdate()
{
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    qDebug()<<title<<author;
    ui->musicName->setText(author+"-"+title);
    resolve_lrc(title);
}

void Widget::likefile()
{
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("另存为"), curFile, tr("(*.txt)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, QStringLiteral("打开用户保存的列表"), QStringLiteral("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        }
        QTextStream out(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            out << ui->tableWidget->item(i, 2)->text();
            if (i < ui->tableWidget->rowCount() - 1)
            out << endl;
        }
        QApplication::restoreOverrideCursor();
        setCurrentFile(fileName);
    }

}

void Widget::setCurrentFile(const QString &fileName)
{
    // canonicalFilePath()可以除去路径中的符号链接，“.”和“..”等符号
    curFile = QFileInfo(fileName).canonicalFilePath();
    // 文档没有被更改过
    //savebutton->setDisabled(true);
}

void Widget::dislikefile()
{
    QString initialName=QDir::homePath();
    QString playListFileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), initialName, tr("(*.txt)"));
    bool flag = false;
    if (playList->isEmpty())
        flag = true;
    QFile file(playListFileName);
    QString path;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        QMessageBox::warning(this, QStringLiteral("打开用户保存列表"), QStringLiteral("无法打开文件 %1:\n%2.").arg(playListFileName).arg(file.errorString()));
    else{
        QTextStream stream(&file);
        int i =0;
        while (!stream.atEnd()){
            path = stream.readLine();
            playList->addMedia(QUrl::fromLocalFile(path));
            QString fileName = path.split("\\").last();
            int rownum = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem(fileName.split(".").front()));
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem(QDir::toNativeSeparators(path)));
            i = i + 1;
        }
        if (i != 0){
            initialName = path.left(path.lastIndexOf('/', -1));
        }
    }

}

////函数功能:解析歌词
void Widget::resolve_lrc(const QString &source_file_name)
{
    lrc_map.clear();
    //判断文件是否存在
    if(source_file_name.isEmpty())
        return;
    QString lrc_file_name = "E:/QT/musci/music/" + source_file_name + ".lrc";
//    int index = playList->currentIndex();
//    QString path = ui->tableWidget->item(index,2)->text();
//    qDebug()<<path;
    QFile file(lrc_file_name);
    if(!file.open(QIODevice::ReadOnly)){
        ui->lrcLabel->setText(tr("歌词未找到"));
        return;
    }

    //读取文件所有内容
    QString all_text = QString(file.readAll());
    file.close();
    QStringList lrc_line = all_text.split("\n");
    //正则表达式d{2}表示匹配2个数字
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    //遍历
    foreach (QString oneline, lrc_line) {
        QString temp = oneline;
        temp.replace(rx,"");
        int pos = rx.indexIn(oneline,0);
        while (pos != -1) {
            QString cap = rx.cap(0);//返回第0个表达式匹配的内容
            // 将时间标签转换为时间数值，以毫秒为单位
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
            lrc_map.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneline, pos);//匹配全部
        }
    }
    if(lrc_map.isEmpty()){
        ui->lrcLabel->setText(tr("歌词未找到"));
        return;
    }
}
//////将歌词显示在主界面myMusic上
void Widget::UpdateTime(qint64 time)
{
    qint64 total_time_value = play->duration();
    QTime total_time(0, (total_time_value/60000)%60, (total_time_value/1000)%60);
    QTime current_time(0, (time/60000)%60, (time/1000)%60);
    QString str = current_time.toString("mm:ss") + "/" + total_time.toString("mm:ss");
    if(!lrc_map.isEmpty()) {
        // 获取当前时间在歌词中的前后两个时间点
        qint64 previous = 0;
        qint64 later = 0;
        //keys()方法返回lrc_map列表
        qint64 count = 0;
        foreach (qint64 value, lrc_map.keys()) {
            if (time >= value) {
                count ++;
                previous = value;
            } else {
                later = value;
                break;
            }
        }

     if (later == 0)
            later = total_time_value;
     qint64 front=0;
     qint64 front1 = 0;
     qint64 later1;
     qint64 a=0;
     foreach ( qint64 item, lrc_map.keys()) {
         a++;
         if( ((a==(count-1)) && item < previous) )
                    front = item;
         if( ((a==(count-2)) && item < previous) )
                    front1 = item;
         if( ((a==(count+2)) && item > previous) )
                    later1 = item;
     }
     QString frontLrc = lrc_map.value(front)+"\n"+"\n"+lrc_map.value(front1);
     QString later_lrc = lrc_map.value(later)+"\n"+"\n"+lrc_map.value(later1);
     QString current_lrc = lrc_map.value(previous);
     if(current_lrc != myLrc->text()) {
                ui->laterlrc->setText(later_lrc);
                myLrc->setText(current_lrc);
                ui->lrcLabel->setText(current_lrc);

                ui->privouslrc->setText(frontLrc);
                qint64 interval_time = later - previous;
                myLrc->start_lrc_mask(interval_time);
            }
      }
    else {  // 如果没有歌词文件，则在顶部标签中显示歌曲标题
            QString title = play->metaData("Title").toString()+".mp3";
            ui->musicName->setText(title);
     }
}
void Widget::pauseBtnClicked()
{


    if(ui->pauseBtn->toolTip() == tr("暂停"))
    {
        timer->stop();
        play->pause();
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/play_pres.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("播放");
    }
    else
    {
        QString title = play->metaData("Title").toString();
        QString author = play->metaData("Author").toString();
        QString musicname = title + "-" + author;
        ui->musicName->setText(musicname);
        resolve_lrc(title);

        timer->start();
        play->play();
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover1.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");

    }

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lrcShow_clicked()
{


    if(myLrc->isHidden())
        myLrc->show();
    else
        myLrc->hide();
}

void Widget::on_nextBtn_clicked()
{
    playList->next();
    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover1.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }
    timer->start();
    play->play();
    //updaInfo();
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    QString musicname = title + "-" + author;
    qDebug()<<title;
    ui->musicName->setText(musicname);
    resolve_lrc(title);
}

void Widget::on_backBtn_clicked()
{
    play->stop();
    playList->previous();

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover1.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }
    timer->start();
    play->play();
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    QString musicname = title + "-" + author;
    qDebug()<<title;
    ui->musicName->setText(musicname);
    resolve_lrc(title);

}

void Widget::on_volumBtn_clicked()
{
    if(ui->volumSlider->isVisible()&&ui->volumBox->isVisible())
    {
        ui->volumSlider->setVisible(false);
        ui->volumBox->setVisible(false);
    }
    else
    {
        ui->volumSlider->setVisible(true);
        ui->volumBox->setVisible(true);
    }
}

void Widget::on_volumBox_clicked()
{
    if(ui->volumBox->isChecked())
    {
        play->setVolume(0);
        ui->volumBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/mute_2.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/mute_2.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/mute_2.png);}"
                                "QPushButton:focus{padding:-1;}");

        ui->volumBox->setToolTip(tr("静音"));
        ui->volumBtn->setToolTip(tr("静音"));
    }
    else
    {
        play->setVolume(ui->volumSlider->value());
        ui->volumBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/mute_1.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/mute_1.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/mute_1.png);}"
                                "QPushButton:focus{padding:-1;}");

        QString valueStr = QString("%1").arg(ui->volumSlider->value());
        ui->volumBox->setToolTip("音量:"+valueStr);
        ui->volumBtn->setToolTip("音量:"+valueStr);
    }
}

void Widget::on_volumSlider_valueChanged(int value)
{
    play->setVolume(value);
    QString valueStr = QString("%1").arg(value);
    ui->volumSlider->setToolTip("音量:"+valueStr);
    ui->volumBox->setToolTip("音量:"+valueStr);
    ui->volumBtn->setToolTip("音量:"+valueStr);
}

void Widget::on_slider_sliderMoved(int position)
{
    play->stop();
    timer->stop();
    play->setPosition(position);
    timer->start();
    play->play();

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover1.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }

}

void Widget::on_pushButton_clicked()
{

    if(ui->tableWidget->isHidden())
        ui->tableWidget->show();
    else
        ui->tableWidget->hide();
}
void Widget::mousePressEvent(QMouseEvent *event)
{
     if(event->button()==Qt::LeftButton)
     {
        dragPos = event->globalPos()-frameGeometry().topLeft();
        event->accept();
     }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        move(event->globalPos()-dragPos);
        event->accept();
    }
}

void Widget::on_pushButton_4_clicked()
{
    exit(0);
}

void Widget::on_pushButton_5_clicked()
{
    myTrayIcon();//托盘
    //this->showMinimized();
    this->hide();
}

void Widget::on_pushButton_3_clicked()
{

    QString initialName=QDir::homePath();
    QStringList pathList=QFileDialog::getOpenFileNames(this, tr("选择文件"), initialName, tr("*.mp3 *.wma *.m4a *.ogg *.aac *.ape"));
    for(int i=0; i<pathList.size(); ++i) {
        QString path=QDir::toNativeSeparators(pathList.at(i));
        if(!path.isEmpty()) {
            playList->addMedia(QUrl::fromLocalFile(path));
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem(fileName.split(".").front()));
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem(QDir::toNativeSeparators(pathList.at(i))));
        }
    }
}

void Widget::on_modelBtn_clicked()
{
    if(ui->modelBtn->toolTip() == tr("顺序播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::Random);//随机
            ui->modelBtn->setToolTip("随机播放");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/suiji.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/suiji.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/suiji.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
       else if(ui->modelBtn->toolTip() == tr("随机播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲
            ui->modelBtn->setToolTip("单曲循环");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/xunhuan.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/xunhuan.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/xunhuan.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
      else
        {
            playList->setPlaybackMode(QMediaPlaylist::Loop); //顺序
            ui->modelBtn->setToolTip("顺序播放");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/shunxu.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
}


void Widget::on_login_clicked()
{

    dislikefile();
}

void Widget::on_timeClock_clicked()
{
    if(ui->timeEdit->isHidden())
        ui->timeEdit->show();
    else
        ui->timeEdit->hide();
    if(ui->timeLabel->isHidden())
        ui->timeLabel->show();
    else
        ui->timeLabel->hide();
}

//托盘
void Widget::myTrayIcon()
{

    QIcon icon = QIcon(":/new/prefix1/icon/tuopan.png");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("隐藏到托盘");
    trayIcon->show();
    //弹出气泡提示
    trayIcon->showMessage("随心而动","隐藏到托盘",QSystemTrayIcon::Information,5000);
    //添加单/双击鼠标相应
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
    //行为
    minimizeAction = new QAction(tr("最小化 (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    before = new QAction(tr("上一曲 (&B)"), this);
    connect(before, SIGNAL(triggered()), this, SLOT(on_backBtn_clicked()));
    pause = new QAction(tr("播放/暂停 (&P)"), this);
    connect(pause, SIGNAL(triggered()), this, SLOT(pauseBtnClicked()));
    next = new QAction(tr("下一曲 (&N)"), this);
    connect(next, SIGNAL(triggered()), this, SLOT(on_nextone_clicked()));
    restoreAction = new QAction(tr("还原 (&R)"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    aboutus = new QAction(tr("关于我们"), this);
    connect(aboutus, SIGNAL(triggered()), this, SLOT(aboutUs()));
    quitAction = new QAction(tr("退出 (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    //创建右键弹出菜单
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);

    trayIconMenu->addAction(before);
    trayIconMenu->addAction(pause);
    trayIconMenu->addAction(next);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutus);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);


}

//列表右键选择
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QStringLiteral("保存播放列表"), this, SLOT(likefile()));
    menu.addAction(QStringLiteral("删除歌曲"), this, SLOT(deleteSelectedMusic()));
    menu.addAction(QStringLiteral("清空列表"), this, SLOT(clearPlaylist()));
    menu.exec(event->globalPos());
}

void Widget::deleteSelectedMusic()
{
    QTableWidgetItem * item = ui->tableWidget->currentItem();
    if (item == Q_NULLPTR)
        return;
    playList->removeMedia(ui->tableWidget->currentRow());
    ui->tableWidget->removeRow(item->row());
    //emit isModified();
    //emit playlistReduced();
}

void Widget::clearPlaylist()
{
    while (ui->tableWidget->rowCount()){
        ui->tableWidget->removeRow(0);
        playList->removeMedia(0);
    }
    //emit isModified();
    //emit playlistReduced();
}

//鼠标响应
void Widget::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        this->showNormal();
        this->raise();
        break;
    default:
        break;
    }
}

void Widget::aboutUs()
{
    QMessageBox::information(this,"关于我们","成员:\n蔡伯伟、郭英生、杨爽、曹恒伟");
}
