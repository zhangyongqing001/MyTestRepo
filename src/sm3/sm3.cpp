#include "sm3.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <QFile>

void OutStr(char* str, int len, QString& prompt)
{
    QFile file("out.txt");
    bool print_on_screen = false;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)){
        qDebug("Open file failure!");
        print_on_screen = true;
    }
    QByteArray outStr(str, len);
    QByteArray outHex(outStr.toHex());

    if(print_on_screen){
        qDebug() << prompt;
    }else{
        file.write(prompt.toStdString().c_str());
        file.write("\r");
    }

    int j=8 - (outHex.length()/8)%8;
    for(int i = outHex.length(); i > 0; i -= 8){
        if(j++ % 8 == 0){
            outHex.insert(i, '\r');
        }
        outHex.insert(i, ' ');
    }
    if(print_on_screen){
        qDebug() << outHex;
    }else{
        file.write(outHex);
        file.write("\r");
    }

    if(!print_on_screen){
        file.close();
    }
}
void OutStrU32(quint32 *p32, int len, QString& prompt)
{
    quint32 list[len];
    for(int i = 0; i<len; i++){
        list[i] = *(p32+i);
        list[i] = WORD_ORDER_SWITCH(list[i]);
    }
    OutStr((char*)list, 4*len, prompt);

    //OutStr((char*)p32, 4*len, prompt);
}

// 扩展函数，将消息分组B扩展为132字，W0[0..15]是B本身,W0[16..67],W1[0..63]都是扩展
void SM3_Expand(quint32 Src[16], quint32 W0[68], quint32 W1[64])
{
    for(int i = 0; i < 16; i++){
        W0[i] = Src[i];
    }
//    QString prompt;
//    prompt = ("W0 pBuf from input");
//    OutStrU32(&W0[0], 16, prompt);
//    W0[0] = 0x61626380;
//    W0[15] = 0x00000018;
//    prompt = ("W0 pBuf reasigned");
//    OutStrU32(&W0[0], 16, prompt);

    for(int j = 16; j < 68; j++){
        quint32 t = BIT_3XOR(W0[j-16], W0[j-9], ROL_SHIFT(W0[j-3],15));
        W0[j] = BIT_3XOR(P1(t),ROL_SHIFT(W0[j-13],7),W0[j-6]);
    }
    for(int i = 0; i < 64; i++){
        W1[i] = BIT_XOR(W0[i],W0[i+4]);
    }

//    prompt = ("W0 pBuf");
//    OutStrU32(&W0[0], 68, prompt);
//    prompt = ("W1 pBuf");
//    OutStrU32(&W1[0], 64, prompt);

//    qDebug() << "W0[0]" << QString("%1").arg(W0[0],8,16,QChar('0'));
//    qDebug() << "W0[16]" << QString("%1").arg(W0[16],8,16,QChar('0'));
//    qDebug() << "W0[67]" << QString("%1").arg(W0[67],8,16,QChar('0'));
//    qDebug() << "W1[0]" << QString("%1").arg(W1[0],8,16,QChar('0'));
//    qDebug() << "W1[62]" << QString("%1").arg(W1[62],8,16,QChar('0'));
//    qDebug() << "W1[63]" << QString("%1").arg(W1[63],8,16,QChar('0'));
}
#define DEBUG_V 1
void SM3_Zip(quint32 Vin[8], quint32 W0[68], quint32 W1[64])
{
    quint32 SS1, SS2, TT1, TT2;
    quint32 VBak[8];
    for(int j = 0; j < 8; j++){
        VBak[j] = Vin[j];
    }

    QString prompt;
    prompt = ("SM3_Zip vin init value: ");
    OutStrU32(Vin, 8, prompt);

    //Debug[4][] = ;

    for(int j = 0; j < 64; j++){

#if (DEBUG_V==1)
        quint32 Debug[5][8];
        Debug[0][0] = Vin[0];
        Debug[0][1] = ROL_SHIFT(Vin[0],12);
        Debug[0][2] = Vin[4];
        Debug[0][3] = j;
        Debug[0][4] = j%32;
        Debug[0][5] = T0;
        Debug[0][6] = ROL_SHIFT(T0, j%32);
        Debug[0][7] = ROL_SHIFT(Vin[0],12) + Vin[4] + ROL_SHIFT(T0, j%32);
        prompt = QString("[D] Vin[0];  Vin[0]<12;  Vin[4];  j;  j mod 32;  T0;  T0<j mod;  sum: ");
        OutStrU32(Debug[0], 8, prompt);
#endif

        if(j < 16){
            SS1 = ROL_SHIFT(ROL_SHIFT(Vin[0],12) + Vin[4] + ROL_SHIFT(T0, j%32),7);
        }else{
            SS1 = ROL_SHIFT(ROL_SHIFT(Vin[0],12) + Vin[4] + ROL_SHIFT(T1, j%32),7);
        }

#if (DEBUG_V==1)
        Debug[1][0] = Vin[0];
        Debug[1][1] = ROL_SHIFT(Vin[0], 12);
        Debug[1][2] = SS1;
        Debug[1][3] = BIT_XOR(SS1, ROL_SHIFT(Vin[0], 12));
        prompt = QString("[D] Vin[0]; Vin[0]<12;  SS1;  SS2: ");
        OutStrU32(Debug[1], 4, prompt);
#endif

        SS2 = BIT_XOR(SS1, ROL_SHIFT(Vin[0], 12));
        if(j < 16){
            TT1 = FF0(Vin[0],Vin[1],Vin[2])+Vin[3] + SS2 + W1[j];
            TT2 = GG0(Vin[4],Vin[5],Vin[6])+Vin[7] + SS1 + W0[j];
        }else{
            TT1 = FF1(Vin[0],Vin[1],Vin[2])+Vin[3] + SS2 + W1[j];
            TT2 = GG1(Vin[4],Vin[5],Vin[6])+Vin[7] + SS1 + W0[j];
        }

#if (DEBUG_V==1)
        Debug[2][0] = Vin[0];
        Debug[2][1] = Vin[1];
        Debug[2][2] = Vin[2];
        Debug[2][3] = FF0(Vin[0],Vin[1],Vin[2]);
        Debug[2][4] = Vin[3];
        Debug[2][5] = SS2;
        Debug[2][6] = W1[j];
        Debug[2][7] = FF0(Vin[0],Vin[1],Vin[2])+Vin[3] + SS2 + W1[j];
        prompt = QString("[D] Vin[0];  Vin[1];  Vin[2];  FF0(0,1,2); Vin[3];  SS2;  W1[j];  TT1: ");
        OutStrU32(Debug[2], 8, prompt);

        Debug[3][0] = Vin[4];
        Debug[3][1] = Vin[5];
        Debug[3][2] = Vin[6];
        Debug[3][3] = GG0(Vin[4],Vin[5],Vin[6]);
        Debug[3][4] = Vin[7];
        Debug[3][5] = SS1;
        Debug[3][6] = W0[j];
        Debug[3][7] = GG1(Vin[4],Vin[5],Vin[6])+Vin[7] + SS1 + W0[j];
        prompt = QString("[D] Vin[4];  Vin[5];  Vin[6];  GG0(4,5,6); Vin[7];  SS1;  W0[j];  TT2: ");
        OutStrU32(Debug[3], 8, prompt);
#endif

        Vin[3] = Vin[2];
        Vin[2] = ROL_SHIFT(Vin[1],9);
        Vin[1] = Vin[0];
        Vin[0] = TT1;
        Vin[7] = Vin[6];
        Vin[6] = ROL_SHIFT(Vin[5],19);
        Vin[5] = Vin[4];
        Vin[4] = P0(TT2);

#if (DEBUG_V==1)
        Debug[4][0] = Vin[1];
        Debug[4][1] = ROL_SHIFT(Vin[1],9);
        Debug[4][2] = TT1;
        Debug[4][3] = Vin[5];
        Debug[4][4] = ROL_SHIFT(Vin[5],19);
        Debug[4][5] = TT2;
        Debug[4][6] = P0(TT2);
        prompt = QString("[D] Vin[1];Vin[1]<<<9;    TT1;  Vin[5]; Vin[5]<<<19;    TT2; P0[TT2]: ");
        OutStrU32(Debug[4], 7, prompt);
#endif
        //if(j < 2)
        {
            prompt = QString("%1 [----- Zip loop -----] value: ").arg(j);
            OutStrU32(Vin, 8, prompt);
        }
    }

    for(int j = 0; j < 8; j++){
        Vin[j] = BIT_XOR(Vin[j], VBak[j]);
    }
    prompt = QString("SM3_Zip final value:");
    OutStrU32(Vin, 8, prompt);
}
void SM3_Digest(char* pBufIn, quint32 bufLen, char* pBufOut)
{
    char buffer[64] = {0};
    quint32 Src[16];
    quint32 W0[68] = {0};
    quint32 W1[64] = {0};
    quint32 Vin[8];
    char *pAddr = pBufIn;
//    quint64 bitsLen = bufLen * 8;
//    switch_dword_order(&bitsLen);
    quint32 bitsLen = bufLen * 8;
    QString prompt;

    // 初始化V0
    for(int i = 0; i < 8; i++){
        //Vin[i] = WORD_ORDER_SWITCH(IV[i]);
        Vin[i] = (IV[i]);
    }
//    OutStrU32(Vin, 8, prompt);
    // 计算填充byte，要求总长(原长+1+填充+64)%512=0
    //int addLen = 448 - (bufLen+1) % 512;
    //int totalLen = bufLen + 1 + addLen + 64;
    int addLen = 56 - (bufLen+1) % 64;
    int totalLen = bufLen + 1 + addLen + 8;
    // 计算迭代次数
    int n = totalLen / 64;
    // 前n-1此迭代
    for(int i = 0; i < n-1; i++){
        memcpy((char*)&Src, pAddr, 64);
        for(int j = 0; j < 16; j++){
            // byte order convert
            //Src[j] = *(quint32*)(pAddr+4*j);
            Src[j] = WORD_ORDER_SWITCH(Src[j]);
        }
        prompt = "message list:";
        OutStrU32(Src, 16, prompt);

        SM3_Expand(Src, W0, W1);

        prompt = "W0 list:";
        OutStrU32(W0, 68, prompt);
        prompt = "W1 list:";
        OutStrU32(W1, 64, prompt);

        SM3_Zip(Vin, W0, W1);
        pAddr += 64;
    }
    // 最后一次迭代    
    memcpy(buffer, pAddr, bufLen%64);
    buffer[bufLen%64] = 0x80;
    // add 4 for 32bit length
    memset(buffer+bufLen%64+1, 0, addLen+4);
    bitsLen = WORD_ORDER_SWITCH(bitsLen);
    memcpy(buffer+bufLen%64+1+addLen+4, &bitsLen, sizeof(quint32));

    memcpy((char*)&Src, buffer, 64);
    for(int j = 0; j < 16; j++){
        Src[j] = WORD_ORDER_SWITCH(Src[j]);
    }

    SM3_Expand(Src, W0, W1);

    prompt = "W0 list:";
    OutStrU32(W0, 68, prompt);
    prompt = "W1 list:";
    OutStrU32(W1, 64, prompt);

    SM3_Zip(Vin, W0, W1);
    prompt = "Final Vin:";
    OutStrU32(Vin, 8, prompt);

    // convert before output
    for(int i = 0; i < 8; i++){
        Vin[i] = WORD_ORDER_SWITCH(Vin[i]);
        memcpy(pBufOut+i*sizeof(quint32), (quint8*)&Vin[i], sizeof(quint32));
    }
}

void switch_dword_order(quint64 *p64)
{
    quint64 _A = 0;
    quint64 _B = *p64;
    quint8 *_pA = (quint8 *)&_A;
    quint8 *_pB = (quint8 *)&_B;
    _pA[0] = _pB[7];
    _pA[1] = _pB[6];
    _pA[2] = _pB[5];
    _pA[3] = _pB[4];
    _pA[4] = _pB[3];
    _pA[5] = _pB[2];
    _pA[6] = _pB[1];
    _pA[7] = _pB[0];

    *p64 = _A;
}
