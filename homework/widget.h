#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"cskill.h"
#include<QDialog>
#include"mypushbutton.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void entergame();
    bool eventFilter(QObject *, QEvent *evt);
    void slot();
    void slot2();
    void slot3();
    void slot4();
    void slot5();
    void getdiz(DizLabel **);
    void nextround();
    void ifwin();
    void ifdead();
    void back();
    void renew();
    int time;
    QDialog w;
    MyPushButton * startbtn;
    monsters* slime;

    void actslot1();
    void actslot2();
    void actslot3();
    void actslot4();
    void actslot5();

private:
    Ui::Widget *ui;

public slots:

};

#endif // WIDGET_H
