#include "SFCConverter.hpp"

SFCConverter::SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QString pouName) :
    GeneralConverter(xml, xmlFile) {
    _pouName = pouName;
}

void SFCConverter::exec() {
    _searchSteps();
}

QString SFCConverter::enumStates() {
    QString out;
    QVector<Step> stepsList = _searchSteps();
    QVector<QString> done;
    out += QString("enum Step: int {\n");
    for(Step S: stepsList) {
        if(!done.contains(S.actual)) {
            done.append(S.actual);
            out += QString("\t") + S.actual + QString(",\n");
        }
    }
    out += QString("};\n\n");
    return out;
}

QString SFCConverter::autoCycleDef() {
    QString out;
    out += QString("void ") + _pouName + QString("::autoCycle() {\n");
    QVector<Step> stepList = _searchSteps();
    for(Step S: stepList) {
        out += QString("\tif(step==") + S.actual + QString(" && ") + S.transition + QString(")");
        out += QString(" changeStep(") + S.next + QString(");\n");
    }
    out += QString("}\n");
    return out;
}

QString SFCConverter::privVars() {
    QString out;
    out += QString("Step step;\n");
    return out;
}

QString SFCConverter::outputAnalysisDef() {
    QString out;

    QVector<Actions> actionsList = _searchActions();
    _sortActions(&actionsList);

    out += QString("void ") + _pouName + QString("::outputAnalysis() {\n");
    for(quint64 i = 0, vPos = i; i < actionsList.size(); i++) {
        if(actionsList.at(vPos).variable != actionsList.at(i).variable || i == 0) {
            if(i != 0) {
                out += QString(") ") + actionsList.at(vPos).variable + QString(" = 1;\n\telse ") + actionsList.at(vPos).variable + QString(" = 0;\n");
            }
            out += QString("\tif(step == ") + actionsList.at(i).step;
            vPos = i;
        } else {
            out += QString(" || step == ") + actionsList.at(i).step;
        }
        if(i == actionsList.size() - 1) {
            out += QString(") ") + actionsList.at(vPos).variable + QString(" = 1;\n\telse ") + actionsList.at(vPos).variable + QString(" = 0;\n");
        }
    }
    out += QString("}\n");

    return out;
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

QVector<Step> SFCConverter::_searchSteps() {
    QVector<Step> stepsList;
    Step newStep;
    SFC last;
    QString lastStep;
    QVector<QString> divStep;
    QVector<QString> convStep;

    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("step")) {
            lastStep = _getStepName();
            if(_getAttribute("initialStep") != "true") {
                //_printChangeStep(lastStep);
                newStep.next = lastStep;
                stepsList.append(newStep);
            }
            last = StepEl;
        }

        else if(_isElement("inVariable")) {
            QString condition = _reachCondition();
            condition.replace("and", "&&");
            condition.replace("or", "||");
            condition.replace("not ", "!");

            if(last == Transition) {
                //_printChangeStep(convStep.last());
                newStep.next = convStep.last();
                stepsList.append(newStep);
                //_printIf(divStep.last(), condition);
                newStep.actual = divStep.last();
                newStep.transition = condition;
            } else if(last == Jump) {
                //_printIf(divStep.last(), condition);
                newStep.actual = divStep.last();
                newStep.transition = condition;
            } else {
                //_printIf(lastStep, condition);
                newStep.actual = lastStep;
                newStep.transition = condition;
            }

            last = Transition;
        }

        else if(_isElement("jumpStep")) {
            QString name = _getJumpStepName();
            //_printChangeStep(name);
            newStep.next = name;
            stepsList.append(newStep);
            last = Jump;
        }

        else if(_isElement("selectionDivergence")) {
            divStep.append(lastStep);
            convStep.append(_searchAfterConv());
            last = Divergence;
        }

        else if(_isElement("selectionConvergence")) {
            divStep.removeLast();
            convStep.removeLast();
            last = Convergence;
        }
    }

    _backToLine(_startLine);

    qDebug() << "STEPS:";
    for(Step S: stepsList) {
        qDebug() << S.actual << " " << S.transition << " " << S.next << Qt::flush;
    }

    return stepsList;
}

QVector<Actions> SFCConverter::_searchActions() {
    QVector<Actions> actionsList;
    QString lastStep;
    SFC last;

    while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("step")) {
            lastStep = _getStepName();
            last = StepEl;
        }

        else if(_isElement("actionBlock")) {
            last = Action;
        }
    }

    return actionsList;
}

QString SFCConverter::_getStepName() {
    return _xml->attributes().value(QString("name")).toString();
}

QString SFCConverter::_getJumpStepName() {
    return _xml->attributes().value(QString("targetName")).toString();
}

/*
QString SFCConverter::_printChangeStep(QString step) {
    QString out;
    if(!step.isEmpty())
        out += QString(" changeStep(") + step + QString(");\n");
    return out;
}

QString SFCConverter::_printIf(QString step, QString condition) {
    QString out;
    out += QString("\t") + QString("if(step == ") + step + QString(" && (") + condition + QString("))");
    return out;
}
*/

void SFCConverter::_sortActions(QVector<Actions>* actionsList) {
    for(quint64 i = 0; i < actionsList->size() - 1; i++) {
        quint64 minPos = i;
        for(quint64 j = i; j < actionsList->size(); j++) {
            minPos = j;
        }
        actionsList->swapItemsAt(i, minPos);
    }
}
