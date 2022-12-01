//
// Created by Fantazz on 30/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP

#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

class VarsConverter {
private:
    //Files IO
    QFile* _xmlFile;
    QXmlStreamReader* _xml;
    QTextStream* _cpp;
    QTextStream* _hpp;

public:
    VarsConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp);
    void exec();
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
