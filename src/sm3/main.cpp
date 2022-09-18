#include <QCoreApplication>
#include <QDebug>
#include "stdio.h"
#include "string.h"

#include "sm3.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello, SM3";

    char digest[32];
    QString prompt = "Disest output:";

//    char strIn[] = "abc";
//    SM3_Digest(strIn, 3, digest);
//    OutStr(digest, 32, prompt);
//    qDebug() << prompt ;
//    QByteArray outStr(digest, 32);
//    QByteArray outHex(outStr.toHex());
//    qDebug() << outHex;
//    qDebug() << "Finished";



    char strIn2[] = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    SM3_Digest(strIn2, 64, digest);
    OutStr(digest, 32, prompt);
    qDebug() << prompt ;
    QByteArray outStr(digest, 32);
    QByteArray outHex(outStr.toHex());
    qDebug() << outHex;
    qDebug() << "Finished";

    return a.exec();
}
