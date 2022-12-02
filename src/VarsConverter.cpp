#include "VarsConverter.hpp"

VarsConverter::VarsConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp) :
    GeneralConverter(xml, xmlFile) {
    _cpp = cpp;
    _hpp = hpp;
}

void VarsConverter::exec() {
    Variable variable;

    while(!_isElement("localVars", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("variable")) {
            variable.name = _getVarName();
        }
        else if(_isElement("type")) {
            _xml->readNextStartElement();
            variable.type = _xml->name().toString().toLower();
            _variables.append(variable);
        }
    }

    _printVariables(_variables);
    *_hpp << "\n" << Qt::flush;

}

void VarsConverter::_printVariables(QVector<Variable> variablesList) {
    for(Variable V: variablesList)
        *_hpp << "\t" << V.type << " " << V.name << ";\n"
              << Qt::flush;
}

QString VarsConverter::_getVarName() {
    return _xml->attributes().value(QString("name")).toString();
}
