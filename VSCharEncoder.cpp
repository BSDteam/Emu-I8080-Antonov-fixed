#include "VSCharEncoder.h"

const QChar additionalSymbols[]{
    QChar(L' '), //0
    QChar(L'█'),
    QChar(L'▓'),
    QChar(L'▒'),
    QChar(L'░'),
    QChar(L'│'), // 5
    QChar(L'─'),
    QChar(L'┌'),
    QChar(L'┐'),
    QChar(L'└'),
    QChar(L'┘'), // 10
    QChar(L'├'),
    QChar(L'┤'),
    QChar(L'┬'),
    QChar(L'┴'),
    QChar(L'┼'), // 15
    QChar(L'↑'),
    QChar(L'↓'),
    QChar(L'←'),
    QChar(L'→'),
    QChar(L'☼'), // 20
    QChar(L'♪'),
    QChar(L'♫'),
    QChar(L'╬'),
    QChar(L'║'),
    QChar(L'═'), // 25
    QChar(L'╔'),
    QChar(L'╚'),
    QChar(L'╝'),
    QChar(L'╠'),
    QChar(L'╦'), // 30
    QChar(L'╩'),
};

QString VSCharEncoder(const char &ch){
    if (ch>0 and ch <32){
        return additionalSymbols[ch];
    }
    else{
        static QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        //return QString::fromLocal8Bit(&ch,1);
        return codec ? codec->toUnicode(&ch, 1) : QString::fromLocal8Bit(&ch,1);
    }
}
