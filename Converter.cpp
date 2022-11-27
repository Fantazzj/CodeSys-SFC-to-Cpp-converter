//
// Created by Fantazz on 27/11/2022.
//

#include "Converter.hpp"

Converter::Converter(char* fileIn, char* fileOut) {
    _xmlFile = new QFile(fileIn);
    _xmlFile->open(QIODevice::ReadOnly | QIODevice::Text);
    _xml = new QXmlStreamReader(_xmlFile);

    //QFile outFile = QFile("_out.cpp");
    //_outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    //QTextStream _out = QTextStream(&_outFile);
    _out = new QTextStream(stdout);
}

Converter::~Converter() {
    _xmlFile->close();
    //_outFile->close();
}

QTextStream* Converter::exec() {
    _reachSFC();

    while(!(_xml->name() == QString("SFC") && _xml->tokenType() == QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_xml->name() == QString("step") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            QString name = _xml->attributes().value(QString("name")).toString();
            name.remove("Step");

            *_out << "changeStep(" << name << ");" << Qt::endl;
            _level--;

            _indent();

            *_out << "if(step == " << name << ") {" << Qt::endl;
            _level++;
        }

        if(_xml->name() == QString("inVariable") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            _reachCondition();
            QString condition = _xml->text().toString();

            _indent();

            *_out << "if(" << condition << ") ";
        }

        if(_xml->name() == QString("jumpStep") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            *_out << "jumpStep" << Qt::endl;
        }

        /*if(_xml->name() == QString("selectionDivergence") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            *_out << "selectionDivergence" << Qt::endl;
        }

        if(_xml->name() == QString("selectionConvergence") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            *_out << "selectionConvergence" << Qt::endl;
        }*/
    }

    return _out;
}

void Converter::_reachSFC() {
    while(_xml->name() != QString("SFC")) _xml->readNextStartElement();
}
void Converter::_indent() {
    for(quint8 i=0; i<_level; i++) *_out<<"\t";
}
void Converter::_reachCondition() {
    do _xml->readNext();
    while(_xml->name() != QString("expression"));
    do _xml->readNext();
    while(_xml->tokenType() != QXmlStreamReader::Characters);
}
