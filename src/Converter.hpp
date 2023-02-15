#ifndef CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP

#include "GeneralConverter.hpp"
#include "SFCConverter.hpp"
#include "VarsConverter.hpp"
#include <QTextStream>
#include <QXmlStreamReader>

class Converter : public GeneralConverter {
private:
	void _convertPou();
	void _createTimerClass();

public:
	Converter(QXmlStreamReader* xml, QFile* xmlFile);
	void exec();

protected:
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
