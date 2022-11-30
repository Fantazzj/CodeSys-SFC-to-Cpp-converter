//
// Created by Fantazz on 29/11/2022.
//

#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile/*, QTextStream* cpp, QTextStream* hpp*/) {
    _xml = xml;
    _xmlFile = xmlFile;
    //_cpp = cpp;
    //_hpp = hpp;
    //_sfcConverter = new SFCConverter(_xml, _xmlFile, _cpp, _hpp);
}

void Converter::exec() {
    _reachPous();

    _xml->readNextStartElement();
    _convertPou();

    //if(_xml->name() == QString("localVars"))


    //if(_xml->name() == QString("SFC"))
        //_sfcConverter->exec();

}

void Converter::_reachBody() {
    while(_xml->name() != QString("body")) _xml->readNextStartElement();
}
void Converter::_reachPous() {
    while(_xml->name() != QString("pous")) _xml->readNextStartElement();
}

void Converter::_convertPou() {
    QString pouName = _xml->attributes().value(QString("name")).toString();
    VarsConverter varsConverter = VarsConverter();

    QFile cppFile = QFile("..\\"+pouName+".cpp");
    cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* cpp = new QTextStream(&cppFile);

    QFile hppFile = QFile("..\\"+pouName+".hpp");
    hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* hpp = new QTextStream(&hppFile);

    if(1) {
        SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, cpp, hpp);
        sfcConverter.exec();
    }

    cppFile.close();
    hppFile.close();
}
