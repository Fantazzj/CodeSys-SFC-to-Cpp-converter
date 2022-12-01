//
// Created by Fantazz on 27/11/2022.
//

#include "SFCConverter.hpp"

SFCConverter::SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp) :
    GeneralConverter(xml, xmlFile) {
    _cpp = cpp;
    _hpp = hpp;
}

void SFCConverter::exec() {
    QVector<QString> stepsList;
    stepsList = _searchSteps();
    _printEnum(stepsList);

    int i = 0;

    *_cpp << "void autoCycle() {\n"
          << Qt::flush;
    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("step")) {
            _step = _getStepName();
            if(!_isInitialStep()) _printChangeStep(_step);
            //if(!_stepsList.contains(_step)) _stepsList.append(_step);
            _last = Step;
        }

        else if(_isElement("inVariable")) {
            QString condition = _reachCondition();
            condition.replace("and", "&&");
            condition.replace("or", "||");

            if(_last == Transition) {
                //_level=_divLevel.last();
                _printChangeStep(_convStep.last());
                //_indent(_divLevel.last()++);
                _printIf(_divStep.last(), condition);
            } else if(_last == Jump) {
                //_level=_divLevel.last();
                //_indent(_divLevel.last()++);
                _printIf(_divStep.last(), condition);
            } else {
                //_indent(_level++);
                _printIf(_step, condition);
            }

            _last = Transition;
        }

        else if(_isElement("jumpStep")) {
            QString name = _getJumpStepName();
            _printChangeStep(name);
            _last = Jump;
        }

        else if(_isElement("selectionDivergence")) {
            _divStep.append(_step);
            _convStep.append(_searchAfterConv());
            //_divLevel.append(_level);
            _last = Divergence;
        }

        else if(_isElement("selectionConvergence")) {
            _divStep.removeLast();
            _convStep.removeLast();
            //_divLevel.removeLast();
            _last = Convergence;
        }
    }
    *_cpp << "}\n"
          << Qt::flush;
}

/*void SFCConverter::_reachSFC() {
    while(_xml->name() != QString("SFC")) _xml->readNextStartElement();
}


void SFCConverter::_indent(quint8 level) {
    for(quint8 i=0; i<level; i++) *_cpp<<"    ";
}
*/

QString SFCConverter::_reachCondition() {
    do _xml->readNext();
    while(_xml->name() != QString("expression"));
    do _xml->readNext();
    while(_xml->tokenType() != QXmlStreamReader::Characters);
    return _xml->text().toString();
}

QString SFCConverter::_searchAfterConv() {
    qint64 startLine = _xml->lineNumber();

    do _xml->readNext();
    while(!_isElement("selectionConvergence", QXmlStreamReader::EndElement));

    do _xml->readNext();
    while(!_isElement("step") && !_isElement("jumpStep") && !_xml->atEnd());

    QString name;
    if(_isElement("step")) name = _getStepName();
    else if(_isElement("jumpStep")) name = _getJumpStepName();

    _xmlFile->seek(0);
    _xml->setDevice(_xmlFile);
    do _xml->readNext();
    while(_xml->lineNumber() != startLine + 1);

    return name;
}

QVector<QString> SFCConverter::_searchSteps() {
    QVector<QString> stepsList;

    qint64 startLine = _xml->lineNumber();

    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();
        if(_isElement("step")) {
            QString step = _getStepName();
            if(!stepsList.contains(step)) stepsList.append(step);
        }
    }

    /*
    _xmlFile->seek(0);
    _xml->setDevice(_xmlFile);
    do _xml->readNext();
    while(_xml->lineNumber() != startLine + 1);
     */

    _backToLine(startLine+1);

    return stepsList;
}

QString SFCConverter::_getStepName() {
    return _xml->attributes().value(QString("name")).toString();
}

QString SFCConverter::_getJumpStepName() {
    return _xml->attributes().value(QString("targetName")).toString();
}

/*
bool SFCConverter::_isConvergence(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("selectionConvergence") && _xml->tokenType() == tokenType;
}

bool SFCConverter::_isDivergence(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("selectionDivergence") && _xml->tokenType() == tokenType;
}

bool SFCConverter::_isStep(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("step") && _xml->tokenType() == tokenType;
}*/

bool SFCConverter::_isInitialStep() {
    return _xml->attributes().value(QString("initialStep")).toString() == "true";
}
/*
bool SFCConverter::_isJumpStep(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("jumpStep") && _xml->tokenType() == tokenType;
}

bool SFCConverter::_isTransition(QXmlStreamReader::TokenType tokenType) {
    return _xml->name() == QString("inVariable") && _xml->tokenType() == tokenType;
}
 */

void SFCConverter::_printChangeStep(QString step) {
    if(!step.isEmpty())
        *_cpp << " changeStep(" << step << ");"
              << "\n"
              << Qt::flush;
}

void SFCConverter::_printIf(QString step, QString condition) {
    *_cpp << "\t"
          << "if(step == " << step << " && (" << condition << "))" << Qt::flush;
}

void SFCConverter::_printEnum(QVector<QString> stepsList) {
    *_hpp << "enum Step: int {\n";
    for(QString S: stepsList) *_hpp << "\t" << S << ",\n";
    *_hpp << "};\n\n"
          << Qt::flush;
}
