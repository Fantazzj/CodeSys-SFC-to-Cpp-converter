#ifndef CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP

#include "GeneralConverter.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QtGlobal>

enum SFC {
	StepEl,
	Transition,
	ActionEl,
	Jump,
	Convergence,
	Divergence
};

class Action {
public:
	QString variable;
	QVector<QString> steps;
	QString type;
	QString time;
};

class Step {
public:
	QString actual;
	QString next;
	QString transition;
	bool initial = false;
};

class SFCConverter : public GeneralConverter {
private:
	QString _pouName;
	QString _reachCondition();
	QString _searchAfterConv();
	QVector<Step> _searchStepsInfo();
	QVector<QString> _searchStepsNames();
	QVector<Action> _searchActions();
	void _sortActions(QVector<Action>);

public:
	SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir, QString pouName);
	QString enumStates();
	QString classDefinition();
	QString autoCycleDef();
	QString autoCycleDec();
	QString outputAnalysisDef();
	QString outputAnalysisDec();
	QString changeStepDef();
	QString changeStepDec();
	QString privateVars();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
