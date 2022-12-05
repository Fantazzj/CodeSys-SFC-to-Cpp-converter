#ifndef CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP

#include "GeneralConverter.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QtGlobal>

enum SFC {
    Step,
    Transition,
    Action,
    Jump,
    Convergence,
    Divergence
};

class Output {
public:
    QString variable;
    QString step;
};

class SFCConverter : public GeneralConverter {
private:
    QString _pouName;
    QString _step;
    QVector<QString> _divStep;
    QVector<QString> _convStep;
    SFC _last;
    QVector<Output> _outputs;

    QString _reachCondition();
    QString _searchAfterConv();
    QVector<QString> _searchSteps();
    QString _getStepName();
    QString _getJumpStepName();
    void _printChangeStep(QString step);
    void _printIf(QString step, QString condition);
    void _printEnum(QVector<QString> stepsList);
    QString _assembleOutputAnalysis();
    void _sortOutputs();

public:
    SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QString pouName);
    void exec();
    QString enumStates;
    QString autoCycle;
    QString outputAnalysis;
    QString privVars;
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
