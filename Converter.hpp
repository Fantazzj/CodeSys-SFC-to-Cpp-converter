//
// Created by Fantazz on 27/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP


#include <QtGlobal>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

class Converter {
private:
    QFile* _xmlFile;
    QXmlStreamReader* _xml;
    QFile* _outFile;
    QTextStream* _out;
    quint8 _level = 0;
    void _reachSFC();
    void _reachCondition();
    void _indent();

public:
    Converter(QString fileIn, QString fileOut);
    ~Converter();
    QTextStream* exec();
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
