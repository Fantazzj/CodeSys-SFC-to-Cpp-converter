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
	auto* xml = new QXmlStreamReader(&xmlFile);

	QDir outDir = QDir("./out/");
	if(!outDir.exists()) outDir.mkpath(".");

	Converter converter = Converter(xml, &xmlFile, outDir);
	converter.exec();

	xmlFile.close();

	return 0;
}
