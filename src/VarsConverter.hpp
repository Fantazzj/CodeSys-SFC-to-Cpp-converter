#ifndef CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP

#include "GeneralConverter.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

enum VarEl {
	VariableEl,
	Type,
	InitialValue,
};

class Variable {
public:
	QString name;
	QString type;
	QString init;
};

class VarsConverter : public GeneralConverter {
private:
	QVector<Variable> _searchVariables();

public:
	VarsConverter(QXmlStreamReader* xml, QFile* xmlFile);
	QString publicVars();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_VARSCONVERTER_HPP
