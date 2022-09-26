
#include <QDebug>
const quint8 SData[16][16]={
    //  0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    {0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05},
    {0x2B, 0x67, 0x9A, 0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99},
    {0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF, 0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62},
    {0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80, 0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6},
    {0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19, 0xE6, 0x85, 0x4F, 0xA8},
    {0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D, 0x35},
    {0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87},
    {0xD4, 0x00, 0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E},
    {0xEA, 0xBF, 0x8A, 0xD2, 0x40, 0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1},
    {0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55, 0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3},
    {0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23, 0xAB, 0x0D, 0x53, 0x4E, 0x6F},
    {0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C, 0x5B, 0x51},
    {0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8},
    {0x0A, 0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0},
    {0x89, 0x69, 0x97, 0x4A, 0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84},
    {0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D, 0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48},
};
const quint32 CK[32] = {
    0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
    0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
    0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
    0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
    0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
    0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
    0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
    0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279,
};
const quint32 FK[4] = {
    0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC,
};

#include "sm4.h"

quint32 rk[32];
quint32 MK[4];
quint32 K[36];
quint32 SM4_GetT1(quint32 x){
    quint32 tao = t(x);
    return L1(tao);
}
quint32 SM4_GetT(quint32 x){
    quint32 tao = t(x);
    return L(tao);
}
quint32 SM4_GetF(quint32 x0, quint32 x1, quint32 x2, quint32 x3, quint32 rk){
    quint32 sum = ((x1)^(x2)^(x3)^(rk));
    quint32 Tee = SM4_GetT(sum);
    return x0 ^ Tee;
}

void SM4_Init(const unsigned char *pKey)
{
    MK[0] = *(quint32*)pKey;
    MK[1] = *(quint32*)(pKey+4);
    MK[2] = *(quint32*)(pKey+8);
    MK[3] = *(quint32*)(pKey+12);
    MK[0] = CONVERT32_BITORDER(MK[0]);
    MK[1] = CONVERT32_BITORDER(MK[1]);
    MK[2] = CONVERT32_BITORDER(MK[2]);
    MK[3] = CONVERT32_BITORDER(MK[3]);

    K[0] = MK[0]^FK[0];
    K[1] = MK[1]^FK[1];
    K[2] = MK[2]^FK[2];
    K[3] = MK[3]^FK[3];

//    SM4_OutU32Inner(MK[0], "Inner org MK[0]:");
//    SM4_OutU32Inner(MK[1], "Inner org MK[1]:");
//    SM4_OutU32Inner(MK[2], "Inner org MK[2]:");
//    SM4_OutU32Inner(MK[3], "Inner org MK[3]:");
//    SM4_OutU32Inner(K[0], "Inner org K[0]:");
//    SM4_OutU32Inner(K[1], "Inner org K[1]:");
//    SM4_OutU32Inner(K[2], "Inner org K[2]:");
//    SM4_OutU32Inner(K[3], "Inner org K[3]:");

    QString prompt;
    for(int i = 0; i < 32; i++){
        quint32 mid = K[i+1]^K[i+2]^K[i+3]^CK[i];
        quint32 tmid = SM4_GetT1(mid);
        K[i+4] = K[i] ^ tmid;
//        prompt = QString("Inner org K[%1]:").arg(i+4);
//        SM4_OutU32Inner(K[i+4], prompt);
    }
    for(int i = 0; i < 32; i++){
        rk[i] = K[i+4];
//        prompt = QString("Inner org rk[%1]:").arg(i);
//        SM4_OutU32Inner(rk[i], prompt);
    }

//    int i = 26;
//    quint32 mid = K[i+1]^K[i+2]^K[i+3]^CK[i];
//    quint32 tmid = SM4_GetT1(mid);
//    quint32 qmid = K[i] ^ tmid;
//    prompt = QString("Inner org qmid26:");
//    SM4_OutU32Inner(qmid, prompt);

//    i = 25;
//    mid = K[i+1]^K[i+2]^K[i+3]^CK[i];
//    tmid = SM4_GetT1(mid);
//    qmid = K[i] ^ tmid;
//    prompt = QString("Inner org qmid25:");
//    SM4_OutU32Inner(qmid, prompt);

}
void SM4_OutU32Inner(quint32 data, QString prompt)
{
    QString tmp = QString("0x%1").arg(data, 8, 16, QChar('0'));
    qDebug() << prompt << tmp;
}

bool SM4_Encrypt(const unsigned char *pIn, unsigned char *pOut)
{
    if (pIn == nullptr || pOut == nullptr){
        return false;
    }
    quint32 x[36] = {0};
    x[0] = *(quint32*)pIn;
    x[1] = *(quint32*)(pIn+4);
    x[2] = *(quint32*)(pIn+8);
    x[3] = *(quint32*)(pIn+12);

    x[0] = CONVERT32_BITORDER(x[0]);
    x[1] = CONVERT32_BITORDER(x[1]);
    x[2] = CONVERT32_BITORDER(x[2]);
    x[3] = CONVERT32_BITORDER(x[3]);

    QString prompt;
    for(int i = 0; i < 32; i++){
        x[i+4] = SM4_GetF(x[i],x[i+1],x[i+2],x[i+3],rk[i]);
//        prompt = QString("Inner org X[%1]:").arg(i+4);
//        SM4_OutU32Inner(x[i+4], prompt);
    }

    x[35] = CONVERT32_BITORDER(x[35]);
    x[34] = CONVERT32_BITORDER(x[34]);
    x[33] = CONVERT32_BITORDER(x[33]);
    x[32] = CONVERT32_BITORDER(x[32]);

    memcpy(pOut, &x[35], sizeof(quint32));
    memcpy(pOut+4, &x[34], sizeof(quint32));
    memcpy(pOut+8, &x[33], sizeof(quint32));
    memcpy(pOut+12, &x[32], sizeof(quint32));

    return true;
}


bool SM4_Decrypt(const unsigned char *pIn, unsigned char *pOut)
{
    if (pIn == nullptr || pOut == nullptr){
        return false;
    }
    quint32 x[36] = {0};
    x[0] = *(quint32*)pIn;
    x[1] = *(quint32*)(pIn+4);
    x[2] = *(quint32*)(pIn+8);
    x[3] = *(quint32*)(pIn+12);

    x[0] = CONVERT32_BITORDER(x[0]);
    x[1] = CONVERT32_BITORDER(x[1]);
    x[2] = CONVERT32_BITORDER(x[2]);
    x[3] = CONVERT32_BITORDER(x[3]);

    QString prompt;
    for(int i = 0; i < 32; i++){
        x[i+4] = SM4_GetF(x[i],x[i+1],x[i+2],x[i+3],rk[31-i]);
//        prompt = QString("Inner org X[%1]:").arg(i+4);
//        SM4_OutU32Inner(x[i+4], prompt);
    }

    x[35] = CONVERT32_BITORDER(x[35]);
    x[34] = CONVERT32_BITORDER(x[34]);
    x[33] = CONVERT32_BITORDER(x[33]);
    x[32] = CONVERT32_BITORDER(x[32]);

    memcpy(pOut, &x[35], sizeof(quint32));
    memcpy(pOut+4, &x[34], sizeof(quint32));
    memcpy(pOut+8, &x[33], sizeof(quint32));
    memcpy(pOut+12, &x[32], sizeof(quint32));

    return true;
}


void SM4_OutStr(const unsigned char *pOutStr, int size, QString &prompt){
    qDebug() << prompt;
    QByteArray out(QByteArray((const char*)pOutStr, size).toHex());
    for(int i = out.length(); i > 0; i-=2){
        if (i%32 == 0){
            //out.insert(i, '\r');
        }
        out.insert(i, ' ');
    }
    qDebug() << out;
}

void SM4_Test()
{
    qDebug() << "SM4 Test" ;

    unsigned char key[16] = {
        0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
    };
    unsigned char in[16] = {
        0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
    };
    unsigned char out[16] = {0};

    QString prompt;
    qDebug() << "Encrypt:";
    prompt = "Input: ";
    SM4_OutStr(in, 16, prompt);
    SM4_Init(key);
    for(int i = 0; i < 1000000; i++){
        SM4_Encrypt(in, out);
        memcpy(in, out, sizeof(char)*16);
    }
    prompt = "Output: ";
    SM4_OutStr(out, 16, prompt);

    qDebug() << "Decrypt:";
    prompt = "Input: ";
    memcpy(in, out, sizeof(char)*16);
    SM4_OutStr(in, 16, prompt);
    SM4_Init(key);
    for(int i = 0; i < 1000000; i++){
        SM4_Decrypt(in, out);
        memcpy(in, out, sizeof(char)*16);
    }
    prompt = "Output: ";
    SM4_OutStr(out, 16, prompt);

//    prompt = "SBox: ";
//    SM4_OutStr((const char *)SData, 256, prompt);

}

void SM4_Test2()
{
    qDebug() << "SM4 Test" ;

    unsigned char key[16] = {
        0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10
    };
    unsigned char in[16] = {
        0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10
    };
    unsigned char out[16] = {0};

    QString prompt;
    qDebug() << "Encrypt:";
    prompt = "Input: ";
    SM4_OutStr(in, 16, prompt);
    SM4_Init(key);
    SM4_Encrypt(in, out);
    prompt = "Output: ";
    SM4_OutStr(out, 16, prompt);

    qDebug() << "Decrypt:";
    prompt = "Input: ";
    memcpy(in, out, sizeof(char)*16);
    SM4_OutStr(in, 16, prompt);
    SM4_Init(key);
    SM4_Decrypt(in, out);
    prompt = "Output: ";
    SM4_OutStr(out, 16, prompt);

//    prompt = "SBox: ";
//    SM4_OutStr((const char *)SData, 256, prompt);

}
