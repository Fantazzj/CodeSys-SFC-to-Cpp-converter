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
};

class SFCConverter : public GeneralConverter {
private:
	QString _pouName;
	QString _reachCondition();
	QString _searchAfterConv();
	QVector<Step> _searchStepsInfo();
	QVector<QString> _searchStepsNames();
	QVector<Action> _searchActions();

public:
	SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir, QString pouName);
	QString enumStates();
	QString autoCycleDef();
	static QString autoCycleDec();
	QString outputAnalysisDef();
	static QString outputAnalysisDec();
	QString changeStepDef();
	static QString changeStepDec();
	static QString privateVars();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
