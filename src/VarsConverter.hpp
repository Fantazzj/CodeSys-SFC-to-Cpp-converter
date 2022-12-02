#ifndef CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP

#include "GeneralConverter.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

class Variable {
public:
    QString name;
    QString type;
};

class VarsConverter : public GeneralConverter {
private:
    QVector<Variable> _variables;
    void _printVariables(QVector<Variable> variablesList);
    QString _getVarName();

public:
    VarsConverter(QXmlStreamReader* xml, QFile* xmlFile);
    void exec();
    QString pubVars;
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
