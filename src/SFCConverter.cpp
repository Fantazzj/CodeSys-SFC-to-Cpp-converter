#include "SFCConverter.hpp"

SFCConverter::SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir, QString pouName) :
	GeneralConverter(xml, xmlFile, outDir) {
	_pouName = pouName;
}

QString SFCConverter::enumStates() {
	QString out;
	QVector<QString> stepsList = _searchStepsNames();
	QVector<QString> done;
	out += QString("enum Step: int {\n");
	for(auto& S: stepsList) {
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
	for(auto& S: stepList) {
		out += QString("\tif(newStep==") + S.actual + QString(" && ") + S.transition + QString(")");
		out += QString(" changeStep(") + S.next + QString(");\n");
	}
	out += QString("}\n");
	return out;
}

QString SFCConverter::privateVars() {
	QString out;
	out += QString("\tStep oldStep;\n");
	out += QString("\tunsigned long long elapsedMillis = 0;\n");
	out += QString("\tunsigned long long previousMillis = 0;\n");

	QVector<Action> actionsList = _searchActions();
	for(auto& A: actionsList) {
		if(A.type == "SL" || A.type == "DS" || A.type == "SD")
			out += QString("\tunsigned long long ") + A.variable + QString("Start = 0;\n");
		if(A.type == "SL" || A.type == "DS" || A.type == "SD" || A.type == "S")
			out += QString("\tbool ") + A.variable + QString("Set = 0;\n");
	}

	return out;
}

QString SFCConverter::outputAnalysisDef() {
	QString out;

	QVector<Action> actionsList = _searchActions();

	out += QString("void ") + _pouName + QString("::outputAnalysis() {\n");
	for(auto& A: actionsList) {
		if(A.type == "N") {
			out += QString("\tif(");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(") ") + A.variable + QString(" = 1;\n");
			out += QString("\telse ") + A.variable + QString(" = 0;\n");
		}
		if(A.type == "S") {
			out += QString("\tif(");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(") ") + A.variable + QString(" = 1;\n");
		}
		if(A.type == "R") {
			out += QString("\tif(");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(") ") + A.variable + QString(" = 0;\n");
		}
		/*if(A.type == "P") {
			out += QString("\tif(newStep != oldStep && (");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(")) ") + A.variable + QString(" = 1;\n");
			out += QString("\telse ") + A.variable + QString(" = 0;\n");
		}
		if(A.type == "D") {
			out += QString("\tif(elapsedMillis >= ") + A.time + QString(" && (");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(")) ") + A.variable + QString(" = 1;\n");
			out += QString("\telse ") + A.variable + QString(" = 0;\n");
		}
		if(A.type == "L") {
			out += QString("\tif(elapsedMillis <= ") + A.time + QString(" && (");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(")) ") + A.variable + QString(" = 1;\n");
			out += QString("\telse ") + A.variable + QString(" = 0;\n");
		}
		if(A.type == "SL") {
			out += QString("\tif(");
			for(auto& S: A.steps) {
				out += QString("newStep == ") + S;
				if(&S != &A.steps.last()) out += QString(" || ");
			}
			out += QString(") ") + A.variable + QString(" = 1;\n");
			out += QString("\tif((") + A.variable + QString("Start + elapsedMillis) > ") + A.time + QString(") {\n");
			out += QString("\t\t") + A.variable + QString(" = 0;\n");
			out += QString("\t\t") + A.variable + QString("Start = Timer::milliseconds();\n");
			out += QString("\t}\n");
			out += QString("\tif((Timer::milliseconds() - ") + A.variable + QString("Start) > ") + A.time + QString(") ") + A.variable + QString(" = 0;\n");
		}
		if(A.type == "SD") {}
		if(A.type == "DS") {}
		 */
	}
	out += QString("\toldStep = newStep;\n");
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
	QString lastStepName;
	bool lastStepInitial = false;
	QVector<QString> divStep;
	QVector<QString> convStep;
	QVector<QString> stepsNames = _searchStepsNames();

	while(!_isElement("SFC", QXmlStreamReader::EndElement)) {
		_xml->readNextStartElement();

		if(_isElement("step")) {
			lastStepName = _getAttribute("name");
			if(_getAttribute("initialStep") == "true") {
				lastStepInitial = true;
			} else {
				newStep.next = lastStepName;
				newStep.initial = lastStepInitial;
				stepsList.append(newStep);
				lastStepInitial = false;
			}
			last = StepEl;
		}

		else if(_isElement("inVariable")) {
			QString condition = _reachCondition();
			condition.replace("and", "&&");
			condition.replace("or", "||");
			condition.replace("not ", "!");
			_convertTime(&condition);
			for(auto& S: stepsNames)
				condition.replace(S + QString(".t"), "elapsedMillis");

			if(last == Transition) {
				newStep.next = convStep.last();
				stepsList.append(newStep);
				newStep.actual = divStep.last();
				newStep.transition = condition;
			} else if(last == Jump) {
				newStep.actual = divStep.last();
				newStep.transition = condition;
			} else {
				newStep.actual = lastStepName;
				newStep.initial = lastStepInitial;
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
			divStep.append(lastStepName);
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
			do {
				_reachElement("action");
				QString type = _getAttribute("qualifier");
				QString time = _getAttribute("duration");
				_convertTime(&time);
				_reachElement("reference");
				QString variable = _getAttribute("name");

				bool found = false;
				for(auto& A: actionsList) {
					if(A.variable == variable && A.type == type) {
						A.steps.append(lastStep);
						found = true;
						break;
					}
				}
				if(!found) {
					Action newAction;
					newAction.type = type;
					newAction.variable = variable;
					newAction.time = time;
					newAction.steps.append(lastStep);
					actionsList.append(newAction);
				}
				_reachElement("action", QXmlStreamReader::EndElement);
				_xml->readNextStartElement();
			} while(!_isElement("actionBlock", QXmlStreamReader::EndElement));
		}
	}

	_backToLine(_startLine);

	return actionsList;
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

	out += QString("void ") + _pouName + QString("::changeStep(Step newStep){\n");
	out += QString("\tthis->newStep = newStep;\n");
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

void SFCConverter::_sortActions(QVector<Action>) {
	
}

QString SFCConverter::classDefinition() {
	QString out;

	out += _pouName + QString("::") + _pouName + QString("(){\n");

	QVector<Step> stepList;
	stepList = _searchStepsInfo();

	for(auto& S: stepList)
		if(S.initial)
			out += QString("\tnewStep = ") + S.actual + QString(";\n");

	out += QString("}\n");

	return out;
}
