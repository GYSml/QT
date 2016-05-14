#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "mylrc.h"
#include <QTimer>
#include <QMouseEvent>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void resolve_lrc(const QString &source_file_name);
    QMap<qint64, QString> lrc_map;
    MyLrc *myLrc;
    QTimer *timer;
    QTimer *backTimer;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint dragPos;
    //void likefile();
    void dislikefile();
    void setCurrentFile(const QString &fileName);
    QString curFile;
    QTimer *timer1;
    int m_time;
    void settime(const unsigned int time0,const unsigned int time,const unsigned int time1);

    void myTrayIcon();//托盘实现




private:
    QMediaPlayer *play;
    QMediaPlaylist *playList;

    QSystemTrayIcon *trayIcon;
    QMenu   *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *before;
    QAction *pause;
    QAction *next;
    QAction *aboutus;

private:
    Ui::Widget *ui;

private slots:

    void aboutUs();//关于我们
    void trayiconActivated(QSystemTrayIcon::ActivationReason reason);//托盘鼠标响应

    void backGroungChange();
    void pauseBtnClicked();
    void UpdateTime(qint64 time);
    void on_lrcShow_clicked();
    void on_nextBtn_clicked();
    void on_backBtn_clicked();
    void btnUpdate();
    void on_volumBtn_clicked();
    void on_volumBox_clicked();
    void on_volumSlider_valueChanged(int value);
    void setPosition();
    void on_slider_sliderMoved(int position);
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void tableClick(int row,int colum);
    void on_modelBtn_clicked();
    void on_login_clicked();
    void on_timeClock_clicked();
    //定时器退出
    void start(int);
    void subtime();
    void showtime();

    void on_tableWidget_activated(const QModelIndex &index);

    void contextMenuEvent(QContextMenuEvent *event);
    void likefile();
    void deleteSelectedMusic();
    void clearPlaylist();
};

#endif // WIDGET_H
