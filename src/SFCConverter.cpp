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

    *_hpp << "Step step;\n"
          << Qt::flush;

    *_cpp << "void autoCycle() {\n"
          << Qt::flush;
    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("step")) {
            _step = _getStepName();
            if(_getAttribute("initialStep")!="true") _printChangeStep(_step);
            _last = Step;
        }

        else if(_isElement("inVariable")) {
            QString condition = _reachCondition();
            condition.replace("and", "&&");
            condition.replace("or", "||");

            if(_last == Transition) {
                _printChangeStep(_convStep.last());
                _printIf(_divStep.last(), condition);
            } else if(_last == Jump) {
                _printIf(_divStep.last(), condition);
            } else {
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
            _last = Divergence;
        }

        else if(_isElement("selectionConvergence")) {
            _divStep.removeLast();
            _convStep.removeLast();
            _last = Convergence;
        }
    }
    *_cpp << "}\n"
          << Qt::flush;
}

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

    _backToLine(startLine + 1);

    return stepsList;
}

QString SFCConverter::_getStepName() {
    return _xml->attributes().value(QString("name")).toString();
}

QString SFCConverter::_getJumpStepName() {
    return _xml->attributes().value(QString("targetName")).toString();
}

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
