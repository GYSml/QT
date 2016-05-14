#ifndef MYLRC_H
#define MYLRC_H
#include <QLabel>
#include <QTimer>
#include <QPalette>
#include <QPushButton>
class QTimer;
class MyLrc : public QLabel
{
    Q_OBJECT
public:
     MyLrc(QWidget *parent=0);
     void start_lrc_mask(qint64 intervaltime);
     void stop_lrc_mask();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);
    void paintEvent(QPaintEvent *);
private:
    QPoint offset;
    QFont font;
    QLinearGradient lineargradient;
    QLinearGradient masklineargradient;
    QTimer *timer;
    qreal lrc_mask_width;
    qreal lrc_mask_width_interval;

private slots:
    void timeout();
};

#endif // MYLRC_H
