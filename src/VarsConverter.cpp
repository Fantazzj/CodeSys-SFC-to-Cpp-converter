#include "VarsConverter.hpp"

VarsConverter::VarsConverter(QXmlStreamReader* xml, QFile* xmlFile) :
    GeneralConverter(xml, xmlFile) {}

void VarsConverter::exec() {
    Variable variable;

    while(!_isElement("localVars", QXmlStreamReader::EndElement)) {
        _xml->readNext();

        if(_isElement("variable")) {
            variable.name = _getVarName();
        } else if(_isElement("type")) {
            _xml->readNextStartElement();
            variable.type = _xml->name().toString().toLower();
            _variables.append(variable);
        }
    }

    _printVariables(_variables);
}

void VarsConverter::_printVariables(QVector<Variable> variablesList) {
    for(Variable V: variablesList)
        pubVars += QString("\t") + V.type + QString(" ") + V.name + QString(";\n");
}

QString VarsConverter::_getVarName() {
    return _xml->attributes().value(QString("name")).toString();
}
