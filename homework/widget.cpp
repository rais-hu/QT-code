#include "widget.h"
#include "ui_widget.h"
#include"mypushbutton.h"
#include<QPushButton>
#include "cskill.h"
#include<QString>
#include<QEvent>
#include<QMouseEvent>
#include<QLabel>
#include<QTimer>
#include<QPropertyAnimation>
Cskill* skill=new Cskill[5];
fighter* solider1=new fighter;
monsters* monster1=new monsters[5];
DizLabel **diz=new DizLabel*[5];

static int p=monster1->fullhp;
static int q=solider1->F_hp;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->winpage->hide();
    ui->stackedWidget->setCurrentWidget(ui->page);

    ui->page->setAutoFillBackground(true);
        QPalette palette = ui->page->palette();
        palette.setBrush(QPalette::Window,
                         QBrush(QPixmap(":/cover.jpg").scaled(
                                 1280,720,
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation)));
        ui->page->setPalette(palette);

    ui->fightpage->setAutoFillBackground(true);
        QPalette palette1 = ui->fightpage->palette();
        palette1.setBrush(QPalette::Window,
                         QBrush(QPixmap(":/timg4.jpg").scaled(
                                 1280,720,
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation)));
        ui->fightpage->setPalette(palette1);

        ui->winpage->setAutoFillBackground(true);
            QPalette palette2 = ui->winpage->palette();
            palette2.setBrush(QPalette::Window,
                             QBrush(QPixmap(":/back.jpg").scaled(
                                     500,500,
                                     Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation)));
            ui->winpage->setPalette(palette2);


        startbtn=new MyPushButton(":/startbtn2.png");

        startbtn->setParent(this);
        startbtn->move(this->width()*0.5-startbtn->width()*0.5,this->height()*0.75);

        connect(startbtn,&QPushButton::clicked,[=](){
            startbtn->jump1();
            startbtn->jump2();
        QTimer::singleShot(500,this,[=](){
            entergame();
            if(ui->stackedWidget->currentIndex()!=0)
            {
                startbtn->hide();
            }
        });
       });


        for(int ij=0;ij<5;ij++){
            diz[ij]=new DizLabel(ui->fightpage);
        }
    getdiz(diz);

    time=0;
    skill[0].Init(1,0,"剑","造成等同骰子点数的伤害",0,0,0,false);
    skill[1].Init(0,1,"木盾","增加等同骰子点数的护盾");
    skill[2].Init(2,0,"回旋镖","造成2*骰子点数的伤害，自身受到3点伤害",0,3);
    skill[3].Init(0,0,"治疗水金","恢复等同骰子点数的血量（最大3）",3,0,1);
    skill[4].Init(0,0,"投掷","重新投出一颗骰子",0,0,0,true);
    ui->skill2->hide();
    ui->skill3->hide();
    ui->skill4->hide();
    monster1[0].init(1,20);
    slime=&monster1[0];
    monster1[1].init(2,30);
    monster1[2].init(3,40);
    monster1[3].init(4,50);
    monster1[4].init(5,60);
    ui->hero_blood->setMaximum(solider1->F_hp);
    ui->hero_blood->setMinimum(0);
    ui->hero_blood->setValue(solider1->hp);
    ui->hero_blood->setFormat(QString("%v/%1").arg(solider1->F_hp));
    ui->hero_blood->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
    ui->hero_blood->setAlignment(Qt::AlignCenter);
    ui->hero_shield->setText(QString("护盾*%1   骰子*%2").arg(solider1->shield).arg(solider1->num));
    ui->mon_blood->setMaximum(monster1->fullhp);
    ui->mon_blood->setMinimum(0);
    ui->mon_blood->setValue(monster1->hp);
    ui->mon_blood->setFormat(QString("%v/%1").arg(monster1->fullhp));
    ui->mon_blood->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
    ui->mon_blood->setAlignment(Qt::AlignCenter);
    ui->mon_shield->setText(QString("护盾*%1").arg(monster1->shield));

    QTimer::singleShot(2000,[=](){
        connect(ui->next,&QPushButton::clicked,this,&Widget::nextround);
    });



    connect(ui->btn,&QPushButton::clicked,this,&Widget::ifwin);

    ui->msk->move(this->width()*0.5-ui->msk->width()*0.5,-ui->msk->height());
    ui->msk->setText(QString("轮到%1行动了！").arg(ui->mon_name->text()));
    ui->mnsk->move(this->width()*0.4-ui->mnsk->width()*0.5,-ui->mnsk->height());

         connect(ui->next,&QPushButton::clicked,[=](){
        QPropertyAnimation* act=new QPropertyAnimation(ui->msk,"geometry");
        act->setDuration(1000);
        act->setStartValue(QRect(ui->msk->x(),ui->msk->y(),ui->msk->width(),ui->msk->height()));
        act->setEndValue(QRect(ui->msk->x(),ui->msk->y()+ui->msk->height()*1.5,ui->msk->width(),ui->msk->height()));
        act->setEasingCurve(QEasingCurve::Linear);
        act->start();
        QTimer::singleShot(1500,this,[=](){
            act->setDuration(1000);
            act->setStartValue(QRect(ui->msk->x(),ui->msk->y(),ui->msk->width(),ui->msk->height()));
            act->setEndValue(QRect(ui->msk->x(),ui->msk->y()-ui->msk->height()*1.5,ui->msk->width(),ui->msk->height()));
            act->setEasingCurve(QEasingCurve::Linear);
            act->start();
        });
    });
    for (int ij=0;ij<5;ij++) {
        connect(diz[ij],&DizLabel::send,this,&Widget::slot);
        connect(diz[ij],&DizLabel::sendvanish,this,&Widget::slot2);
        connect(diz[ij],&DizLabel::send3,this,&Widget::slot3);
        connect(diz[ij],&DizLabel::send4,this,&Widget::slot4);
        connect(diz[ij],&DizLabel::send5,this,&Widget::slot5);
    }

}

Widget::~Widget()
{
    delete ui;
}
void Widget::entergame(){
    ui->stackedWidget->setCurrentWidget(ui->fightpage);
}
void Widget::renew(){
    int m=monster1->hp;
    int n=solider1->hp;
    if(p>m)
    {
        QTimer::singleShot(700,[=](){
            actslot1();
            actslot2();
        });
    }
    if(q>n)
    {
        QTimer::singleShot(700,[=](){
            actslot3();
            actslot4();
        });
    }
    p=m;
    q=n;
    ui->mon_blood->setValue(monster1->hp);
    if(monster1->hp==0)
    {
        solider1->LVup();
        ui->winpage->show();
        if(solider1->LV<6)monster1++;
    }
    else if(solider1->hp==0)
    {
        ui->hero_blood->setValue(solider1->hp);
        ifdead();
     QTimer::singleShot(5000,this,[=](){back();});
    }else {
        ui->hero_blood->setValue(solider1->hp);
   ui->msk->setText(QString("轮到%1行动了！").arg(ui->mon_name->text()));
   ui->hero_shield->setText(QString("护盾*%1   骰子*%2").arg(solider1->shield).arg(solider1->num));
   ui->mon_shield->setText(QString("护盾*%1").arg(monster1->shield));

   m=monster1->fullhp;
   n=solider1->F_hp;}
}
void Widget::slot(){
    ui->skill1->hide();
    renew();
}
void Widget::slot2(){
    ui->skill2->hide();
    renew();
}
void Widget::slot3(){
    ui->skill4->hide();
    renew();
}
void Widget::slot4(){
    ui->skill3->hide();
    renew();
}
void Widget::slot5(){
    if(time<2){
        time++;
    }else{
     ui->skill5->hide();
    }
//    renew();
}
void Widget::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
    startbtn->show();
    ui->skill1->show();
    ui->skill5->show();
    ui->hero->move(100,500);
    solider1->LV=1;
    monster1=slime;
    solider1->hp=24;
    solider1->F_hp=24;
    solider1->num=2;
    solider1->shield=0;
    solider1->LV=1;
    time=0;
    for (int ij=0;ij<5;ij++) {
        monster1[ij].hp=monster1[ij].fullhp;
    }
    ui->mon_name->setText("史莱姆");
    ui->monster->setPixmap(QPixmap(":/slime.png"));
    ui->hero_blood->setMaximum(solider1->F_hp);
    ui->hero_blood->setMinimum(0);
    ui->hero_blood->setValue(solider1->hp);
    ui->mon_blood->setMaximum(monster1->fullhp);
    ui->mon_blood->setMinimum(0);
    ui->mon_blood->setValue(monster1->hp);
    ui->stackedWidget->setCurrentWidget(ui->page);
    getdiz(diz);
}
void Widget::ifdead()
{
//    ui->mon_blood->setMaximum(monster1->fullhp);
//    ui->mon_blood->setValue(monster1->hp);
//    ui->hero_blood->setMaximum(solider1->F_hp);
//    ui->hero_blood->setValue(solider1->hp);
//    ui->hero_shield->setText(QString("护盾*%1   骰子*%2").arg(solider1->shield).arg(solider1->num));
//    ui->mon_shield->setText(QString("护盾*%1").arg(monster1->shield));
//    ui->mon_blood->setFormat(QString("%v/%1").arg(monster1->fullhp));
//    ui->hero_blood->setFormat(QString("%v/%1").arg(solider1->F_hp));
    QTimer::singleShot(2500,[=](){
        actslot5();
    });
}
void Widget::ifwin()
{
    ui->winpage->hide();
    ui->skill1->show();
    ui->skill5->show();
    time=0;
        if(solider1->LV==2)
        {
            ui->skill2->show();
            ui->mon_name->setText("小火苗");
            ui->monster->setPixmap(QPixmap(":/fire.png"));

        }
        if(solider1->LV==3)
        {
            ui->skill3->show();
            ui->mon_name->setText("雪人");
            ui->monster->setPixmap(QPixmap(":/snowman.png"));
        }
        if(solider1->LV==4)
        {
            ui->skill4->show();
            ui->mon_name->setText("女巫");
            ui->monster->setPixmap(QPixmap(":/wizard.png"));
        }
        if(solider1->LV==5)
        {
            ui->mon_name->setText("盗贼");
            ui->monster->setPixmap(QPixmap(":/thief.png"));
        }
        if(solider1->LV==6)
        {
            back();
        }

        ui->mon_blood->setMaximum(monster1->fullhp);
        ui->mon_blood->setValue(monster1->hp);
        ui->hero_blood->setMaximum(solider1->F_hp);
        ui->hero_blood->setValue(solider1->hp);
        ui->hero_shield->setText(QString("护盾*%1   骰子*%2").arg(solider1->shield).arg(solider1->num));
        ui->mon_shield->setText(QString("护盾*%1").arg(monster1->shield));
        ui->mon_blood->setFormat(QString("%v/%1").arg(monster1->fullhp));
        ui->hero_blood->setFormat(QString("%v/%1").arg(solider1->F_hp));
        getdiz(diz);


}
void Widget::getdiz(DizLabel **a)
{
    for (int ij=0;ij<solider1->num;ij++) {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+ij*100);
        a[ij]->num=qrand()%6+1;
        a[ij]->setPixmap(QPixmap(QString(":/diz%1.png").arg(a[ij]->num)));
        a[ij]->move(500+ij*100,600);
        a[ij]->show();
        a[ij]->installEventFilter(a[ij]);
        connect(a[ij],&DizLabel::send,this,&Widget::slot);
    }
}
void Widget::nextround()
{
    ui->next->hide();
    for(int ij=0;ij<5;ij++){
        if(!diz[ij]->isHidden()){
            diz[ij]->hide();
        }
    }
    time=0;
    ui->skill1->hide();
    ui->skill2->hide();
    ui->skill3->hide();
    ui->skill3->hide();
    ui->skill4->hide();
    ui->skill5->hide();

    int k=monster1->action(solider1);
    if(k==0)
    {
        ui->mnsk->setText(QString("%1攻击了你，效果拔群！").arg(ui->mon_name->text()));
        QTimer::singleShot(3000,[=](){
            QPropertyAnimation* act=new QPropertyAnimation(ui->mnsk,"geometry");
            act->setDuration(1000);
            act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
            act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()+ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
            act->setEasingCurve(QEasingCurve::Linear);
            act->start();
            QTimer::singleShot(1500,this,[=](){
                act->setDuration(1000);
                act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
                act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()-ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
                act->setEasingCurve(QEasingCurve::Linear);
                act->start();
            });
        });
    }
    if(k==1)
    {
        ui->mnsk->setText(QString("%1护盾增加，效果拔群！").arg(ui->mon_name->text()));
        QTimer::singleShot(3000,[=](){
            QPropertyAnimation* act=new QPropertyAnimation(ui->mnsk,"geometry");
            act->setDuration(1000);
            act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
            act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()+ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
            act->setEasingCurve(QEasingCurve::Linear);
            act->start();
            QTimer::singleShot(1500,this,[=](){
                act->setDuration(1000);
                act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
                act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()-ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
                act->setEasingCurve(QEasingCurve::Linear);
                act->start();
            });
        });
    }
    if(k==2)
    {
        ui->mnsk->setText(QString("%1治疗了自己，效果拔群！").arg(ui->mon_name->text()));
        QTimer::singleShot(3000,[=](){
            QPropertyAnimation* act=new QPropertyAnimation(ui->mnsk,"geometry");
            act->setDuration(1000);
            act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
            act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()+ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
            act->setEasingCurve(QEasingCurve::Linear);
            act->start();
            QTimer::singleShot(1500,this,[=](){
                act->setDuration(1000);
                act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
                act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()-ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
                act->setEasingCurve(QEasingCurve::Linear);
                act->start();
            });
        });
    }
    if(k==3)
    {
        ui->mnsk->setText(QString("%1使用了刺盾，护甲增加的同时攻击了你！").arg(ui->mon_name->text()));
        QTimer::singleShot(3000,[=](){
            QPropertyAnimation* act=new QPropertyAnimation(ui->mnsk,"geometry");
            act->setDuration(1000);
            act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
            act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()+ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
            act->setEasingCurve(QEasingCurve::Linear);
            act->start();
            QTimer::singleShot(1500,this,[=](){
                act->setDuration(1000);
                act->setStartValue(QRect(ui->mnsk->x(),ui->mnsk->y(),ui->mnsk->width(),ui->mnsk->height()));
                act->setEndValue(QRect(ui->mnsk->x(),ui->mnsk->y()-ui->mnsk->height()*1.5,ui->mnsk->width(),ui->mnsk->height()));
                act->setEasingCurve(QEasingCurve::Linear);
                act->start();
            });
        });
    }
    slot();
    QTimer::singleShot(6000,[=](){
        getdiz(diz);
        ui->skill1->show();
        if(solider1->LV>=2){
         ui->skill2->show();
         if(solider1->LV>=3){
            ui->skill3->show();
            if(solider1->LV>=4){
               ui->skill4->show();
            }
         }
        }
        ui->skill5->show();
        ui->next->show();
    });

}
bool Widget::eventFilter(QObject *, QEvent *evt)
{
    static bool isHover = false;
    if(evt->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(evt);
        if(rect().contains(e->pos()) && //is the mouse is clicking the key
            (e->button() == Qt::LeftButton)) //if the mouse click the right key
        {
            isHover = true;
        }
    }
    else if(evt->type() == QEvent::MouseMove && isHover)
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(evt);
    }
    else if(evt->type() == QEvent::MouseButtonRelease && isHover)
    {
        isHover = false;
    }
    return false;
}

void Widget::actslot1()
{
    QPropertyAnimation *animation=new QPropertyAnimation(ui->monster,"geometry");

    animation->setDuration(400);

    animation->setStartValue(QRect(ui->monster->x(),ui->monster->y(),ui->monster->width(),ui->monster->height()));

    animation->setEndValue(QRect(ui->monster->x(),ui->monster->y()+15,ui->monster->width(),ui->monster->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void Widget::actslot2()
{
    QPropertyAnimation *animation=new QPropertyAnimation(ui->monster,"geometry");

    animation->setDuration(400);

    animation->setStartValue(QRect(ui->monster->x(),ui->monster->y()+15,ui->monster->width(),ui->monster->height()));

    animation->setEndValue(QRect(ui->monster->x(),ui->monster->y(),ui->monster->width(),ui->monster->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void Widget::actslot3()
{
    QPropertyAnimation *animation=new QPropertyAnimation(ui->hero,"geometry");

    animation->setDuration(400);

    animation->setStartValue(QRect(ui->hero->x(),ui->hero->y(),ui->hero->width(),ui->hero->height()));

    animation->setEndValue(QRect(ui->hero->x(),ui->hero->y()+15,ui->hero->width(),ui->hero->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void Widget::actslot4()
{
    QPropertyAnimation *animation=new QPropertyAnimation(ui->hero,"geometry");

    animation->setDuration(400);

    animation->setStartValue(QRect(ui->hero->x(),ui->hero->y()+15,ui->hero->width(),ui->hero->height()));

    animation->setEndValue(QRect(ui->hero->x(),ui->hero->y(),ui->hero->width(),ui->hero->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void Widget::actslot5(){
    QPropertyAnimation *animation=new QPropertyAnimation(ui->hero,"geometry");

    animation->setDuration(2000);

    animation->setStartValue(QRect(ui->hero->x(),ui->hero->y(),ui->hero->width(),ui->hero->height()));

    animation->setEndValue(QRect(ui->hero->x(),ui->hero->y()+400,ui->hero->width(),ui->hero->height()));

//    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->setEasingCurve(QEasingCurve::Linear);

    animation->start();

}

