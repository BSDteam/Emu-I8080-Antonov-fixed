#ifndef VSCHARENCODER_H
#define VSCHARENCODER_H
#include <Qt>
#include <QTextCodec>

extern const QChar additionalSymbols[];
extern QString VSCharEncoder(const char &ch);
#endif // VSCHARENCODER_H
