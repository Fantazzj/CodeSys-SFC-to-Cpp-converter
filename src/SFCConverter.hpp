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
	QString step;
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
	QVector<Step> _searchSteps();
	QVector<Action> _searchActions();
	//QString _getStepName();
	//QString _getJumpStepName();
	static void _sortActions(QVector<Action>* actionsList);

public:
	SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QString pouName);
	void exec();
	QString enumStates();
	QString autoCycleDef();
	QString autoCycleDec();
	QString outputAnalysisDef();
	QString outputAnalysisDec();
	QString privVars();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
