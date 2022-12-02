//
// Created by Fantazz on 01/12/2022.
//

#include "GeneralConverter.hpp"

GeneralConverter::GeneralConverter(QXmlStreamReader* xml, QFile* xmlFile) {
    _xml = xml;
    _xmlFile = xmlFile;
}

void GeneralConverter::exec() { }

void GeneralConverter::_reachElement(QString name, QXmlStreamReader::TokenType tokenType) {
    while(!_xml->isEndDocument() && (_xml->name() != name || _xml->tokenType() != tokenType)) {
        if(tokenType == QXmlStreamReader::StartElement) _xml->readNextStartElement();
        else _xml->readNext();
    }
}

bool GeneralConverter::_isElement(QString name, QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == name && _xml->tokenType() == tokenType;
}

void GeneralConverter::_goToLine(qint64 line) {
    do _xml->readNext();
    while(_xml->lineNumber() != line);
}

void GeneralConverter::_backToLine(qint64 line) {
    _xmlFile->seek(0);
    _xml->setDevice(_xmlFile);
    do _xml->readNext();
    while(_xml->lineNumber() != line);
}

QString GeneralConverter::_getAttribute(QString attribute) {
    return _xml->attributes().value(attribute).toString();
}
