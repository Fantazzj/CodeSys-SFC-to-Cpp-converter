#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile) :
	GeneralConverter(xml, xmlFile) {
	_xml = xml;
	_xmlFile = xmlFile;
}

void Converter::exec() {
	_reachElement("pous");

	do {
		_reachElement("pou");
		_convertPou();
	} while((_xml->name() == QString("body") && _xml->isEndElement()) &&
			!_xml->isEndDocument());
}

void Converter::_convertPou() {
	QString pouName = _xml->attributes().value(QString("name")).toString();

	QFile cppFile = QFile("../" + pouName + ".cpp");
	cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream* cpp = new QTextStream(&cppFile);

	QFile hppFile = QFile("../" + pouName + ".hpp");
	hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream* hpp = new QTextStream(&hppFile);

	*hpp << "#ifndef " << pouName.toUpper() << "_HPP"
		 << "\n"
		 << Qt::flush;
	*hpp << "#define " << pouName.toUpper() << "_HPP"
		 << "\n\n"
		 << Qt::flush;
	*cpp << "#include \"" << pouName.toUpper() << ".hpp\"\n\n"
		 << Qt::flush;

	VarsConverter varsConverter = VarsConverter(_xml, _xmlFile);
	varsConverter.exec();

	_reachElement("body");
	_xml->readNextStartElement();

	if(_xml->name() == QString("SFC")) {
		SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, pouName);
		sfcConverter.exec();


		*hpp << sfcConverter.enumStates()
			 << "class " << pouName << " {\n"
			 << "public:\n"
			 << varsConverter.pubVars
			 << "\tvoid autoCycle();\n"
			 << "\tvoid outputAnalysis();\n\n"
			 << "private:\n\tStep step;\n"
			 << "\tvoid changeStep(Step step);\n"
			 << "};\n\n"
			 << Qt::flush;

		*cpp << sfcConverter.autoCycleDef()
			 << sfcConverter.outputAnalysisDef()
			 << Qt::flush;
	}

	*hpp << "#endif\n"
		 << Qt::flush;

	cppFile.close();
	hppFile.close();
}
