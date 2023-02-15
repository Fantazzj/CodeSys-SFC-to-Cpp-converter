#include "GeneralConverter.hpp"

GeneralConverter::GeneralConverter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir) {
	_xml = xml;
	_xmlFile = xmlFile;
	_startLine = xml->lineNumber();
	_outDir = outDir;
}

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

QStringList GeneralConverter::_convertTime(QString* time) {
	QRegularExpression reg = QRegularExpression(R"((TIME|T|t|time)#(\d+d)?(\d+h)?(\d+ms*)?(\d+s)?(\d+ms)?)", QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatchIterator j = reg.globalMatch(*time);
	QStringList valueList;

	while(j.hasNext()) {
		QRegularExpressionMatch match = j.next();
		QString value = match.captured();
		valueList.append(value);

		//qDebug() << *time << value;

		value = value.toLower();
		value.remove("t#");
		value.remove("time#");
		qint64 tot = 0;
		for(qint64 i = 0, nPos = 0; i < value.length(); i++) {
			if(value.at(i).isLetter()) {
				qint64 val = value.sliced(nPos, i - nPos).toUInt();
				if(value.at(i) == 'd') val = val * 24 * 60 * 60 * 1000;
				if(value.at(i) == 'h') val = val * 60 * 60 * 1000;
				if(value.at(i) == 'm' && value.at(i + 1) != 's') val = val * 60 * 1000;
				if(value.at(i) == 's') val = val * 1000;
				tot += val;
				nPos = i + 1;
			}
		}
		time->replace(match.captured(), QString::number(tot));
	}

	return valueList;
}

void GeneralConverter::_convertType(QString* type) {
	type->replace("BOOL", "bool");
	type->replace("INT", "int");
	type->replace("WORD", "int");
	type->replace("DWORD", "int");
	type->replace("TIME", "unsigned long long");
}
