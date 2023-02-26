#include "src/Converter.hpp"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>

int main(int argc, char* argv[]) {

	QString xmlFileName;
	if(argc > 1) {
		xmlFileName = QString(argv[1]);
		if(xmlFileName.last(4) != ".xml") {
			qCritical() << "input file is not a .xml file";
			return -1;
		}
	}
	else {
		qCritical() << "no input file given";
		return -1;
	}

	QString outDirName;
	if(argc > 2) {
		outDirName = QString(argv[2]);
		if(outDirName.contains("\\") || outDirName.contains("/")) {
			qCritical() << "sub-folders aren't currently supported";
			return -1;
		}
	}
	else outDirName = QString("out");


	QDir outDir = QDir(outDirName);
	QFile xmlFile = QFile(xmlFileName);
	if(!xmlFile.exists()) {
		qCritical() << xmlFileName << "does not exists";
		return -1;
	}

	xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
	auto* xml = new QXmlStreamReader(&xmlFile);

	if(!outDir.exists()) outDir.mkpath(".");

	Converter converter = Converter(xml, &xmlFile, outDir);
	converter.exec();

	xmlFile.close();

	return 0;
}
