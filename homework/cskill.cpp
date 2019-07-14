#include "cskill.h"
#include "ui_widget.h"
#include<QtGlobal>
#include<QTime>
#include<QString>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include<QDebug>
extern Cskill* skill;
extern monsters* monster1;
extern fighter* solider1;
Cskill::Cskill()
{

}
//Cskill::Cskill(QWidget * parent): QLabel(parent)
//{

//}
void Cskill::use(fighter* hero,monsters* monster,DizLabel* diz){

        if(damage>0){
           monster->hurt(damage*diz->num);
        }
        if(shield>0){
           hero->shield+=shield*diz->num;
        }
        if(redamage>0){
           hero->hurt(redamage);
        }
        if(recover>0){
          if(hero->hp+recover>=hero->F_hp){
           hero->hp=hero->F_hp;
        }else{
           hero->hp+=recover*diz->num;
           }
       }

}


DizLabel::DizLabel(QWidget *parent) : QLabel(parent)
{
    j=i;
    i++;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+i*100);
    num=qrand()%6+1;
    installEventFilter(this);
    resize(65,65);
    setPixmap(QPixmap(QString(":/diz%1.png").arg(num)));
    move(500+i*100,600);
    this->hide();
}
bool DizLabel::eventFilter(QObject *, QEvent *evt)
{
    static QPoint lastPnt;
    static bool isHover = false;
    // 鼠标按下
    if(evt->type() == QEvent::MouseButtonPress)
    {
    QMouseEvent* e = static_cast<QMouseEvent *>(evt);
    if(rect().contains(e->pos()) && //is the mouse is clicking the key
        (e->button() == Qt::LeftButton)) //if the mouse click the right key
    {
        lastPnt = e->pos();
        isHover = true;
    }
    }
    // 鼠标移动
    else if(evt->type() == QEvent::MouseMove && isHover)
    {
    // 鼠标位置
    QMouseEvent* e = static_cast<QMouseEvent*>(evt);
    int dx = e->pos().x() - lastPnt.x();
    int dy= e->pos().y()-lastPnt.y();
    // 修改对象位置
    move(x()+dx,y()+dy);

    }
    else if(evt->type() == QEvent::MouseButtonRelease && isHover)
    {
        if(x()>150&&y()>270&&x()<180&&y()<300&&skill[0].f(this))
        {
            skill[0].use(solider1,monster1,this);
            sendsignal();
            this->hide();
        }
        if(x()>430&&y()>270&&x()<460&&y()<300&&skill[1].f(this))
        {
            skill[1].use(solider1,monster1,this);
            sendsignal2();
            this->hide();
        }
        if(x()>740&&y()>430&&x()<770&&y()<460&&skill[2].f(this))
        {
            skill[2].use(solider1,monster1,this);
            sendsignal3();
            this->hide();
        }
        if(x()>740&&y()>220&&x()<770&&y()<250&&skill[3].f(this))
        {
            skill[3].use(solider1,monster1,this);
            sendsignal4();
            this->hide();
        }
        if(x()>1060&&y()>310&&x()<1080&&y()<350){
            this->hide();
            sendsignal5();
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            this->num=qrand()%6+1;
            this->setPixmap(QPixmap(QString(":/diz%1.png").arg(this->num)));
            this->move(500+this->j*100,600);
            this->show();
        }
    isHover = false;
    }
    return false;
}
DizLabel::~DizLabel()
{
    i--;
}

fighter::fighter()
{
    F_hp=24;
    hp=24;
    num=2;
    LV=1;
    shield=0;

}


monsters::monsters()
{
    fullhp = 20;
    hp = fullhp;
    shield = 0;
    LV=1;
}
void monsters::init(int lv,int h)
{
    fullhp=h;
    LV=lv;
    hp=fullhp;
}

