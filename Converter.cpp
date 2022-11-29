//
// Created by Fantazz on 27/11/2022.
//

#include "Converter.hpp"

Converter::Converter(QString fileIn, QString fileOut) {
    _xmlFile = new QFile(fileIn);
    _xmlFile->open(QIODevice::ReadOnly | QIODevice::Text);
    _xml = new QXmlStreamReader(_xmlFile);

    _outFile = new QFile(fileOut);
    _outFile->open(QIODevice::WriteOnly | QIODevice::Text);
    _out = new QTextStream(_outFile);
    //_out = new QTextStream(stdout);
}

Converter::~Converter() {
    _xmlFile->close();
    _outFile->close();
}

void Converter::exec() {
    _reachSFC();

    while(!(_xml->name() == QString("SFC") && _xml->tokenType() == QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isStep()) {
            _step = _getStepName();
            _printChangeStep(_step);
            _last = Step;
        }

        else if(_isTransition()) {
            QString condition = _reachCondition();

            if(_last == Trans || _last == Jump) {
                _level=_divLevel.last();
                _printChangeStep(_convStep.last());
                _indent(_divLevel.last()++);
                _printIf(_divStep.last(), condition);
            } else {
                _indent(_level++);
                _printIf(_step, condition);
            }

            _last = Trans;
        }

        else if(_isJumpStep()) {
            QString name = _getJumpStepName();
            _printChangeStep(name);
            _last = Jump;
        }

        else if(_isDivergence()) { //TODO non per forza in fondo ci deve essere una convergenza
            _divStep.append(_step);
            _convStep.append(_searchAfterConv());
            _divLevel.append(_level);
            _last = Divergence;
        }

        else if(_isConvergence()) {
            _divStep.removeLast();
            _convStep.removeLast();
            _divLevel.removeLast();
            _last = Convergence;
        }

    }

}

void Converter::_reachSFC() {
    while(_xml->name() != QString("SFC")) _xml->readNextStartElement();
}

void Converter::_indent(quint8 level) {
    //for(quint8 i=0; i<level; i++) *_out<<"    ";
}

QString Converter::_reachCondition() {
    do _xml->readNext();
    while(_xml->name() != QString("expression"));
    do _xml->readNext();
    while(_xml->tokenType() != QXmlStreamReader::Characters);
    return _xml->text().toString();
}

QString Converter::_searchAfterConv() {
    qint64 startLine = _xml->lineNumber();

    do _xml->readNext();
    while(!_isConvergence(QXmlStreamReader::EndElement));

    do _xml->readNext();
    while(!_isStep() && !_isJumpStep());

    QString name;
    if(_isStep()) name = _getStepName();
    else if(_isJumpStep()) name = _getJumpStepName();

    _xmlFile->seek(0);
    _xml->setDevice(_xmlFile);
    do _xml->readNext();
    while(_xml->lineNumber() != startLine+1);

    return name;
}

QString Converter::_getStepName() {
    return _xml->attributes().value(QString("name")).toString().remove("Step");
}

QString Converter::_getJumpStepName() {
    return _xml->attributes().value(QString("targetName")).toString().remove("Step");
}

bool Converter::_isConvergence(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("selectionConvergence") && _xml->tokenType() == tokenType;
}

bool Converter::_isDivergence(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("selectionDivergence") && _xml->tokenType() == tokenType;
}

bool Converter::_isStep(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("step") && _xml->tokenType() == tokenType;
}

bool Converter::_isJumpStep(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("jumpStep") && _xml->tokenType() == tokenType;
}

bool Converter::_isTransition(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("inVariable") && _xml->tokenType() == tokenType;
}

void Converter::_printChangeStep(QString step) {
    *_out << " changeStep(" << step << ");" << "\n" << Qt::flush;
}

void Converter::_printIf(QString step, QString condition) {
    *_out << "if(step == " << step << " && (" << condition << "))";
}
