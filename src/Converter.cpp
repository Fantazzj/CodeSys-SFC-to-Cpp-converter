#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile, QDir outDir) :
	GeneralConverter(xml, xmlFile) {
	_outDir = outDir;
}

void Converter::exec() {
	_createTimerClass();

	_reachElement("pous");

	do {
		_reachElement("pou");
		_convertPou();
	} while((_xml->name() == QString("body") && _xml->isEndElement()) &&
			!_xml->isEndDocument());
}

void Converter::_convertPou() {
	QString pouName = _getAttribute("name");

	QFile cppFile = QFile(_outDir.dirName() + QString("/") + pouName + QString(".cpp"));
	cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream cpp = QTextStream(&cppFile);

	QFile hppFile = QFile(_outDir.dirName() + QString("/") + pouName + QString(".hpp"));
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

	VarsConverter varsConverter = VarsConverter(_xml, _xmlFile);
	QString publicVars = varsConverter.publicVars();

	_reachElement("body");
	_xml->readNextStartElement();

	if(_xml->name() == QString("SFC")) {
		SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, pouName);

		hpp << sfcConverter.enumStates()
			<< "class " << pouName << " {\n"
			<< "public:\n"
			<< "\t" << pouName << "();\n"
			<< sfcConverter.autoCycleDec()
			<< sfcConverter.outputAnalysisDec()
			<< publicVars
			<< "\tStep newStep;\n"
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
		<< "\tstatic void wait(unsigned long long t);\n"
		<< "\tstatic unsigned long long milliseconds();\n"
		<< "};\n"
		<< "\n"
		<< "#endif\n"
		<< Qt::flush;

	cpp << "#include \"Timer.hpp\"\n"
		<< "void Timer::wait(unsigned long long t) {\n"
		<< "\t//insert function to block the execution\n"
		<< "}\n"
		<< "unsigned long long Timer::milliseconds() {\n"
		<< "\t//insert function to return milliseconds\n"
		<< "\treturn 0;\n"
		<< "}\n"
		<< Qt::flush;

	cppFile.close();
	hppFile.close();
}
