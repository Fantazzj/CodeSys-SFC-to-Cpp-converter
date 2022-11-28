//
// Created by Fantazz on 27/11/2022.
//

#include "Converter.hpp"

Converter::Converter(QString fileIn, QString fileOut) {
    _xmlFile = new QFile(fileIn);
    _xmlFile->open(QIODevice::ReadOnly | QIODevice::Text);
    _xml = new QXmlStreamReader(_xmlFile);

    //_outFile = new QFile(fileOut);
    //_outFile->open(QIODevice::WriteOnly | QIODevice::Text);
    //_out = new QTextStream(_outFile);
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
            _step = _xml->attributes().value(QString("name")).toString().remove("Step");
            *_out << " changeStep(" << _step << ");" << Qt::endl;
            _last = Step;
        }

        else if(_xml->name() == QString("inVariable") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            QString condition = _reachCondition();

            if(_last == Trans) {
                *_out << " changeStep(" << _convStep.last() << ");" << Qt::endl;
                *_out << "if(step == " << _divStep.last() << " && (" << condition << "))";
            }
            else *_out << "if(step == " << _step << " && (" << condition << "))";

            _last = Trans;
        }

        else if(_xml->name() == QString("jumpStep") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            QString name = _xml->attributes().value(QString("targetName")).toString().remove("Step");
            *_out << " changeStep(" << name << ");" << Qt::endl;
            _last = Jump;
        }

        else if(_xml->name() == QString("selectionDivergence") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            _level++;
            _divStep.append(_step);
            _convStep.append(_searchConvStep());

            _last = Divergence;
        }

        else if(_xml->name() == QString("selectionConvergence") && _xml->tokenType() == QXmlStreamReader::StartElement) {
            _divStep.removeLast();
            _convStep.removeLast();
            _last = Convergence;
        }

    }

    return _out;
}

void Converter::_reachSFC() {
    while(_xml->name() != QString("SFC")) _xml->readNextStartElement();
}

void Converter::_indent() {
    for(quint8 i=0; i<_level; i++) *_out<<"\t";
}

QString Converter::_reachCondition() {
    do _xml->readNext();
    while(_xml->name() != QString("expression"));
    do _xml->readNext();
    while(_xml->tokenType() != QXmlStreamReader::Characters);
    return _xml->text().toString();
}

QString Converter::_searchConvStep() {
    qint64 startLine = _xml->lineNumber();

    do _xml->readNext();
    while(!(_xml->name() == QString("selectionConvergence") && _xml->tokenType() == QXmlStreamReader::StartElement));
    _xml->readNext();
    QString name = _xml->attributes().value(QString("name")).toString().remove("Step");

    _xmlFile->seek(0);
    _xml->setDevice(_xmlFile);
    do _xml->readNext();
    while(_xml->lineNumber() != startLine+1);

    return name;
}
