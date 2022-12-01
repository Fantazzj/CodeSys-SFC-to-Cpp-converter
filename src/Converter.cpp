//
// Created by Fantazz on 29/11/2022.
//

#include "Converter.hpp"

Converter::Converter(QXmlStreamReader* xml, QFile* xmlFile /*, QTextStream* cpp, QTextStream* hpp*/) :
    GeneralConverter(xml, xmlFile) {
    _xml = xml;
    _xmlFile = xmlFile;
    //_cpp = cpp;
    //_hpp = hpp;
    //_sfcConverter = new SFCConverter(_xml, _xmlFile, _cpp, _hpp);
}

void Converter::exec() {
    _reachElement("pous");

    do {
        _reachElement("pou");
        _convertPou();
    } while((_xml->name() == QString("body") && _xml->isEndElement()) &&
            !_xml->isEndDocument());
}

/*void Converter::_reachBody() {
    while(_xml->name() != QString("body") && !_xml->isEndDocument()) _xml->readNextStartElement();
}

void Converter::_reachPous() {
    while(_xml->name() != QString("pous") && !_xml->isEndDocument()) _xml->readNextStartElement();
}
*/
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

    VarsConverter varsConverter = VarsConverter(_xml, _xmlFile, cpp, hpp);
    varsConverter.exec();

    _reachElement("body");
    _xml->readNextStartElement();

    if(_xml->name() == QString("SFC")) {
        SFCConverter sfcConverter = SFCConverter(_xml, _xmlFile, cpp, hpp);
        sfcConverter.exec();
    }

    *hpp << "#endif\n"
         << Qt::flush;

    cppFile.close();
    hppFile.close();
}
/*
void Converter::_reachNextPou() {
    while(_xml->name() != QString("pou") && !_xml->isEndDocument()) _xml->readNextStartElement();
}
*/
