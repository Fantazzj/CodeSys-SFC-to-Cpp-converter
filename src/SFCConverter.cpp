#include "SFCConverter.hpp"

SFCConverter::SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QString pouName) :
	GeneralConverter(xml, xmlFile) {
	_pouName = pouName;
}

QString SFCConverter::enumStates() {
	QString out;
	QVector<QString> stepsList = _searchStepsNames();
	QVector<QString> done;
	out += QString("enum Step: int {\n");
	for(QString S: stepsList) {
		if(!done.contains(S)) {
			done.append(S);
			out += QString("\t") + S + QString(",\n");
		}
	}
	out += QString("};\n\n");
	return out;
}

QString SFCConverter::autoCycleDef() {
	QString out;
	out += QString("void ") + _pouName + QString("::autoCycle() {\n");
	out += QString("\telapsedMillis = Timer::milliseconds() - previousMillis;\n");
	QVector<Step> stepList = _searchStepsInfo();
	for(Step S: stepList) {
		out += QString("\tif(step==") + S.actual + QString(" && ") + S.transition + QString(")");
		out += QString(" changeStep(") + S.next + QString(");\n");
	}
	out += QString("}\n");
	return out;
}

QString SFCConverter::privateVars() {
	QString out;
	out += QString("\tStep step;\n");
	out += QString("\tunsigned long elapsedMillis = 0;\n");
	out += QString("\tunsigned long previousMillis = 0;\n");
	return out;
}

QString SFCConverter::outputAnalysisDef() {
	QString out;

	QVector<Action> actionsList = _searchActions();
	_sortActions(&actionsList);

	out += QString("void ") + _pouName + QString("::outputAnalysis() {\n");
	for(qint64 i = 0, vPos = i; i < actionsList.size(); i++) {
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
	if(_isElement("step")) name = _getAttribute("name");
	else if(_isElement("jumpStep")) name = _getAttribute("targetName");

	_xmlFile->seek(0);
	_xml->setDevice(_xmlFile);
	do _xml->readNext();
	while(_xml->lineNumber() != startLine + 1);

	return name;
}

QVector<Step> SFCConverter::_searchStepsInfo() {
	QVector<Step> stepsList;
	Step newStep;
	SFC last;
	QString lastStep;
	QVector<QString> divStep;
	QVector<QString> convStep;
	QVector<QString> stepsNames = _searchStepsNames();

	while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
		_xml->readNext();

		if(_isElement("step")) {
			lastStep = _getAttribute("name");
			if(_getAttribute("initialStep") != "true") {
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
			_convertTime(&condition);
			for(QString S: stepsNames)
				condition.replace(S+QString(".t"), "elapsedMillis");

			if(last == Transition) {
				newStep.next = convStep.last();
				stepsList.append(newStep);
				newStep.actual = divStep.last();
				newStep.transition = condition;
			} else if(last == Jump) {
				newStep.actual = divStep.last();
				newStep.transition = condition;
			} else {
				newStep.actual = lastStep;
				newStep.transition = condition;
			}

			last = Transition;
		}

		else if(_isElement("jumpStep")) {
			QString name = _getAttribute("targetName");
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

	return stepsList;
}

QVector<Action> SFCConverter::_searchActions() {
	QVector<Action> actionsList;
	QString lastStep;

	while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
		_xml->readNextStartElement();

		if(_isElement("step")) {
			lastStep = _getAttribute("name");
		}

		else if(_isElement("actionBlock")) {
			_reachElement("reference");
			QString variable = _getAttribute("name");
			Action newAction;
			newAction.variable = variable;
			newAction.step = lastStep;
			actionsList.append(newAction);
		}
	}

	_backToLine(_startLine);

	return actionsList;
}

void SFCConverter::_sortActions(QVector<Action>* actionsList) {
	for(qint64 i = 0; i < actionsList->size() - 1; i++) {
		qint64 minPos = i;
		for(qint64 j = i; j < actionsList->size(); j++) {
			minPos = j;
		}
		actionsList->swapItemsAt(i, minPos);
	}
}

QString SFCConverter::autoCycleDec() {
	return "\tvoid autoCycle();\n";
}

QString SFCConverter::outputAnalysisDec() {
	return "\tvoid outputAnalysis();\n";
}

QString SFCConverter::changeStepDec() {
	return "\tvoid changeStep(Step step);\n";
}

QString SFCConverter::changeStepDef() {
	QString out;

	out += QString("void ") + _pouName + QString("::changeStep(Step step){\n");
	out += QString("\tthis->step = step;\n");
	out += QString("\telapsedMillis = 0;\n");
	out += QString("\tpreviousMillis = Timer::milliseconds();\n");
	out += QString("}\n");

	return out;
}

QVector<QString> SFCConverter::_searchStepsNames() {
	QVector<QString> stepsList;

	while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
		_xml->readNext();
		if(_isElement("step"))
			stepsList.append(_getAttribute("name"));
	}

	_backToLine(_startLine);

	return stepsList;
}
