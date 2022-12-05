#include "src/Converter.hpp"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>

int main(int argc, char* argv[]) {
	QString xmlFileName = QString(argv[1]);

	QFile xmlFile = QFile(xmlFileName);
	xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QXmlStreamReader* xml = new QXmlStreamReader(&xmlFile);

	Converter converter = Converter(xml, &xmlFile);
	converter.exec();

	xmlFile.close();

	return 0;
}
