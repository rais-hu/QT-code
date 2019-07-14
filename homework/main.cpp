#include "cskill.h"
#include"widget.h"

#include <QApplication>
int DizLabel::i=0;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.setWindowTitle(QObject::tr("Dicey Dungeons"));
    return a.exec();
}
