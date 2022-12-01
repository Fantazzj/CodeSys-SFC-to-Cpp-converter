//
// Created by Fantazz on 29/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP

#include <QXmlStreamReader>
#include <QTextStream>
#include "VarsConverter.hpp"
#include "SFCConverter.hpp"

class Converter {
private:
    QXmlStreamReader* _xml;
    QFile* _xmlFile;
    //QTextStream* _cpp;
    //QTextStream* _hpp;
    //SFCConverter* _sfcConverter;

    void _reachBody();
    void _reachPous();
    void _reachNextPou();
    void _convertPou();
public:
    Converter(QXmlStreamReader* xml, QFile* xmlFile/*, QTextStream* cpp, QTextStream* hpp*/);
    void exec();
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
