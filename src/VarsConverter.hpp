#ifndef CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP

#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QDebug>
#include "GeneralConverter.hpp"

class Variable{
public:
    QString name;
    QString type;
};

class VarsConverter : public GeneralConverter{
private:
    QTextStream* _cpp;
    QTextStream* _hpp;
    QVector<Variable> _variables;
    void _printVariables(QVector<Variable> variablesList);
    QString _getVarName();

public:
    VarsConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp);
    void exec();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
