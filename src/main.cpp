#include <QApplication>
#include <QTextCodec>
#include "system/dynamic_system.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    DynamicSystem system;
    system.setWindowTitle("Modeling and control of a complex positive dynamic system");
    system.setFixedSize(700, 830);
    system.show();
    return app.exec();
}
