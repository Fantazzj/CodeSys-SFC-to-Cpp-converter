#include "GeneralConverter.hpp"

GeneralConverter::GeneralConverter(QXmlStreamReader* xml, QFile* xmlFile) {
	_xml = xml;
	_xmlFile = xmlFile;
	_startLine = xml->lineNumber();
}

//void GeneralConverter::exec() {}

void GeneralConverter::_reachElement(QString name, QXmlStreamReader::TokenType tokenType) {
	while(!_xml->isEndDocument() && (_xml->name() != name || _xml->tokenType() != tokenType)) {
		if(tokenType == QXmlStreamReader::StartElement) _xml->readNextStartElement();
		else _xml->readNext();
	}
}

bool GeneralConverter::_isElement(QString name, QXmlStreamReader::TokenType tokenType) {
	return _xml->name() == name && _xml->tokenType() == tokenType;
}

void GeneralConverter::_goToLine(quint64 line) {
	do _xml->readNext();
	while(_xml->lineNumber() != line);
}

void GeneralConverter::_backToLine(quint64 line) {
	_xmlFile->seek(0);
	_xml->setDevice(_xmlFile);
	do _xml->readNext();
	while(_xml->lineNumber() != line);
}

QString GeneralConverter::_getAttribute(QString attribute) {
	return _xml->attributes().value(attribute).toString();
}

void GeneralConverter::_convertTime(QString* time) {

}

void GeneralConverter::_convertType(QString* type) {
	type->replace("BOOL", "bool");
	type->replace("INT", "int");
	type->replace("WORD", "int");
	type->replace("DWORD", "int");
	type->replace("TIME", "unsigned long long");
}
