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

    QFile cppFile = QFile("..\\" + pouName + ".cpp");
    cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* cpp = new QTextStream(&cppFile);

    QFile hppFile = QFile("..\\" + pouName + ".hpp");
    hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* hpp = new QTextStream(&hppFile);

    *hpp << "#ifndef " << pouName.toUpper() << "\n"
         << Qt::flush;
    *hpp << "#define " << pouName.toUpper() << "\n\n"
         << Qt::flush;
    *cpp << "#include \"" << pouName.toUpper() << ".hpp\"\n\n"
         << Qt::flush;

    VarsConverter varsConverter = VarsConverter(_xml, _xmlFile, cpp, hpp);
    varsConverter.exec();

    _reachElement("body");
    _xml->readNextStartElement();

    if(_xml->name() == QString("SFC")) {
        SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, pouName);
        sfcConverter.exec();

        *hpp << "class " << pouName << " {\n" << Qt::flush;
        *hpp << "public:\n" << Qt::flush;
        //*hpp << varsConverter.pubVars;
        *hpp << "\tvoid autoCycle();\n" << Qt::flush;
        *hpp << "\tvoid writeOutput();\n" << Qt::flush;
        *hpp << "}\n\n" << Qt::flush;
        *cpp << sfcConverter.autoCycle;
    }

    *hpp << "#endif\n"
         << Qt::flush;

    cppFile.close();
    hppFile.close();
}
