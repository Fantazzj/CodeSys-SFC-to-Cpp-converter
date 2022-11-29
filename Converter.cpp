//
// Created by Fantazz on 29/11/2022.
//

#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp) {
    _xml = xml;
    _xmlFile = xmlFile;
    _cpp = cpp;
    _hpp = hpp;
    _sfcConverter = new SFCConverter(_xml, _xmlFile, _cpp, _hpp);
}

void Converter::exec() {
    _reachBody();
    _xml->readNextStartElement();

    if(_xml->name() == QString("SFC"))
        _sfcConverter->exec();

}

void Converter::_reachBody() {
    while(_xml->name() != QString("body")) _xml->readNextStartElement();
}
