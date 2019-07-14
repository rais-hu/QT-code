#ifndef CSKILL_H
#define CSKILL_H

#include <QLabel>
#include<QProgressBar>
#include <QWidget>
#include <QObject>
#include<QTime>
#include<QtGlobal>
#include<QDebug>
class fighter;
class monsters;
class DizLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DizLabel(QWidget *parent = nullptr);
    ~DizLabel();
    bool eventFilter(QObject *, QEvent *);
    int num;
    static int i;
    int j;
    void sendsignal(){emit send();}
    void sendsignal2(){emit sendvanish();}
    void sendsignal3(){emit send3();}
    void sendsignal4(){emit send4();}
    void sendsignal5(){emit send5();}

signals:void send();
    void sendvanish();
    void send3();
    void send4();
    void send5();

public slots:
};

class Cskill
{

public:
//    explicit Cskill(int _damage,int _limit,QWidget * parent=nullptr);
    Cskill();
    int damage;
    int limit;
    int redamage;
    int shield;
    int recover;
    QString name;
    QString describe;
    bool special;
    bool f(DizLabel* diz){
        if(limit==0||diz->num<=limit){
            return true;
        }else {
            return false;
        }
    }
    void use(fighter* hero,monsters* monster,DizLabel* diz);
    void Init(int _damage,int _shield,QString _name,QString _describe,int _limit=0,int _redamage=0,int _recover=0,bool _special=false){
        damage=_damage;
        limit=_limit;
        redamage=_redamage;
        shield=_shield;
        recover=_recover;
        name=_name;
        describe=_describe;
        special=_special;
    }
signals:

public slots:
};


class fighter
{
public:
//    explicit fighter(Cskill** _skill,QWidget *parent = nullptr);
    fighter();

    int hp;//当前hp
    int F_hp;//满hp
    int LV;
    int num;//骰子数
    int shield;//盾
    Cskill* skill[4];//技能
//    QLabel character;
//    QLabel *hero;
//    QWidget *condition;
//    QProgressBar *con_blood;
//    QLabel *con_name;
//    QLabel *con_shield;
//    QLabel *con_diz_num;
    DizLabel **diz;



    void hurt(int damage){
        if(shield>=damage){shield-=damage;}
        else{hp-=damage-shield;shield=0;}
        if(hp<=0){
            hp=0;
        }
    }
    void LVup(){
        LV++;
        F_hp+=5;
        if(hp+10<F_hp)hp+=10;
        else hp=F_hp;
        shield=0;
        if(num<5){
            num++;
        }
  }
};

class monsters
{

//signals:void win();
public:
     monsters();
     void init(int lv, int h);

    //当前hp
    int hp;
    //血量
    int fullhp;
    //护甲
    int shield;
    int LV;

//    void act(){emit acter();}

    void hurt(int damage){       
       if(shield>=damage){shield-=damage;}
       else{hp-=damage-shield;shield=0;}
       if(hp<=0){
           hp=0;
       }
    }
    void recover(int re)
    {
        if(hp+re<fullhp)
            hp+=re;
        else {
            hp=fullhp;
        }
    }
    int action(fighter* h){
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int act=qrand()%4;
        qDebug()<<act;
        switch(act){
        case 0: qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+1);
                h->hurt(3+(qrand()%LV+1)*2);
                break;
        case 1: qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+1);
                shield+=(2+qrand()%LV*2);
                break;
        case 2: qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+1);
                if(hp==fullhp){h->hurt(3+qrand()%LV);}
                else {recover(3+qrand()%LV);}
                break;
        case 3: qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+1);
                h->hurt(2*(qrand()%LV+1)+2);shield+=2+2*(LV-qrand()%LV);
                break;

        }
        return act;
    }
};







#endif // CSKILL_H
