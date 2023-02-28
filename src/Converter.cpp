#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir) :
	GeneralConverter(xml, xmlFile) {
	_outDir = outDir;
}

void Converter::exec() {
	_createTimerClass();

	_reachElement("pous");
	qInfo() << "reached \"pous\"";
	qInfo() << "-----";

	do {
		_reachElement("pou");
		qInfo() << "reached \"pou\"";
		qInfo() << "-----";
		qInfo() << "converting...";
		qInfo() << "-----";
		_convertPou();
	} while((_isElement("body", QXmlStreamReader::EndElement)) && !_xml->isEndDocument());
}

void Converter::_convertPou() {
	QString pouName = _getAttribute("name");

	QFile cppFile = QFile(_outDir.dirName() + "/" + pouName + ".cpp");
	cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream cpp = QTextStream(&cppFile);

	QFile hppFile = QFile(_outDir.dirName() + "/" + pouName + ".hpp");
	hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream hpp = QTextStream(&hppFile);

	hpp << "#ifndef " << pouName.toUpper() << "_HPP\n"
		<< "#define " << pouName.toUpper() << "_HPP\n"
		<< "\n"
		<< "#include \"Timer.hpp\"\n"
		<< "\n"
		<< Qt::flush;

	cpp << "#include \"" << pouName.toUpper() << ".hpp\"\n"
		<< "\n"
		<< Qt::flush;

	_reachElement("interface");
	qInfo() << "reached \"interface\"";
	qInfo() << "-----";

	_reachElement("localVars");
	qInfo() << "reached \"localVars\"";
	qInfo() << "-----";
	VarsConverter varsConverter = VarsConverter(_xml, _xmlFile);

	_reachElement("body");
	qInfo() << "reached \"body\"";
	qInfo() << "-----";
	_xml->readNextStartElement();

	if(_isElement("SFC")) {
		qInfo() << "found \"SFC\" program";
		qInfo() << "-----";
		SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, pouName);

		hpp << sfcConverter.enumStates()
			<< "class " << pouName << " {\n"
			<< "public:\n"
			<< "\t" << pouName << "();\n"
			<< sfcConverter.autoCycleDec()
			<< sfcConverter.outputAnalysisDec()
			<< varsConverter.publicVars()
			<< sfcConverter.publicVars()
			<< "private:\n"
			<< sfcConverter.privateVars()
			<< sfcConverter.changeStepDec()
			<< "};\n"
			<< "\n"
			<< Qt::flush;

		cpp << sfcConverter.classDefinition()
			<< sfcConverter.autoCycleDef()
			<< sfcConverter.outputAnalysisDef()
			<< sfcConverter.changeStepDef()
			<< Qt::flush;
	}

	hpp << "#endif\n"
		<< Qt::flush;

	cppFile.close();
	hppFile.close();
}

void Converter::_createTimerClass() {
	QFile cppFile = QFile(_outDir.dirName() + QString("/") + QString("Timer.cpp"));
	cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream cpp = QTextStream(&cppFile);

	QFile hppFile = QFile(_outDir.dirName() + QString("/") + QString("Timer.hpp"));
	hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream hpp = QTextStream(&hppFile);

	hpp << "#ifndef TIMER_HPP\n"
		<< "#define TIMER_HPP\n"
		<< "\n"
		<< "class Timer {\n"
		<< "public:\n"
		<< "\tTimer() = default;\n"
		<< "\tstatic void wait(unsigned long t);\n"
		<< "\tstatic unsigned long milliseconds();\n"
		<< "};\n"
		<< "\n"
		<< "#endif\n"
		<< Qt::flush;

	cpp << "#include \"Timer.hpp\"\n"
		<< "void Timer::wait(unsigned long t) {\n"
		<< "\t//insert function to block the execution\n"
		<< "\t//delay(t);\n"
		<< "}\n"
		<< "unsigned long Timer::milliseconds() {\n"
		<< "\tunsigned long milliseconds = 0;\n"
		<< "\t//insert function to return milliseconds from startup\n"
		<< "\t//milliseconds = millis();\n"
		<< "\treturn milliseconds;\n"
		<< "}\n"
		<< Qt::flush;

	cppFile.close();
	hppFile.close();
}
