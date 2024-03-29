#ifndef CODESYS_SFC_TO_CPP_CONVERTER_GENERALCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_GENERALCONVERTER_HPP

#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QRegularExpression>

class GeneralConverter {
private:
public:
	GeneralConverter(QXmlStreamReader* xml, QFile* xmlFile);
	//virtual void exec();

protected:
	QXmlStreamReader* _xml;
	QFile* _xmlFile;
	quint64 _startLine;
	void _reachElement(QString name, QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
	bool _isElement(QString name, QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
	QString _getAttribute(QString attribute);
	void _backToLine(quint64 line);
	void _goToLine(quint64 line);
	static QStringList _convertTime(QString* time);
	static void _convertType(QString* type);
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_GENERALCONVERTER_HPP
