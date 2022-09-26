#include <QCoreApplication>
#include <QDebug>
#include "sm4.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello,SM4!";

    SM4_Test();

    return a.exec();
}
