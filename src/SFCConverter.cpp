#include "SFCConverter.hpp"

SFCConverter::SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QString pouName) :
    GeneralConverter(xml, xmlFile) {
    _pouName = pouName;
}

void SFCConverter::exec() {
    QVector<QString> stepsList;
    stepsList = _searchSteps();

    _printEnum(stepsList);

    privVars += QString("Step step;\n");

    autoCycle += QString("void ") + _pouName + QString("::autoCycle() {\n");

    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("step")) {
            _step = _getStepName();
            if(_getAttribute("initialStep") != "true") _printChangeStep(_step);
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

    autoCycle += QString("}\n");
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
        autoCycle += QString(" changeStep(") + step + QString(");\n");
}

void SFCConverter::_printIf(QString step, QString condition) {
    autoCycle += QString("\t") + QString("if(step == ") + step + QString(" && (") + condition + QString("))");
}

void SFCConverter::_printEnum(QVector<QString> stepsList) {
    enumStates += QString("enum Step: int {\n");
    for(QString S: stepsList) enumStates += QString("\t") + S + QString(",\n");
    enumStates += QString("};\n\n");
}
