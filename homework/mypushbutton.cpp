#include "mypushbutton.h"
#include<QPropertyAnimation>

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}
MyPushButton::MyPushButton(QString image1,QString image2)
{

    this->image1path=image1;
    this->image2path=image2;

    QPixmap pix;
    pix.load(this->image1path);

    this->setFixedSize(pix.width()*0.48,pix.height()*0.42);

    this->setStyleSheet("QPushbutton{border:0px;}");

    this->setIcon(pix);

    this->setIconSize(QSize(pix.width(),pix.height()));

}

void MyPushButton::jump1()
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");

    animation->setDuration(200);

    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));

    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();

}

void MyPushButton::jump2()
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");

    animation->setDuration(200);

    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();

}





