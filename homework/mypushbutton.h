#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include<QString>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);


    MyPushButton(QString image1,QString image2="");

    QString image1path;
    QString image2path;


    void jump1();
    void jump2();

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
