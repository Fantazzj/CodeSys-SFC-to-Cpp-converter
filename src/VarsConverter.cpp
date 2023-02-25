#include "VarsConverter.hpp"

VarsConverter::VarsConverter(QXmlStreamReader* xml, QFile* xmlFile) :
	GeneralConverter(xml, xmlFile) {}

QVector<Variable> VarsConverter::_searchVariables() {
	QVector<Variable> variablesList;
	Variable newVariable;
	VarEl last;

	while(!_isElement("localVars", QXmlStreamReader::EndElement)) {
		_xml->readNextStartElement();

		if(_isElement("variable")) {
			if(last == Type) {
				newVariable.init = "";
				variablesList.append(newVariable);
			}
			newVariable.name = _getAttribute("name");
			last = VariableEl;
		} else if(_isElement("type")) {
			_xml->readNextStartElement();
			newVariable.type = _xml->name().toString();
			last = Type;
		} else if(_isElement("initialValue")) {
			_xml->readNextStartElement();
			newVariable.init = _getAttribute("value");
			variablesList.append(newVariable);
			last = InitialValue;
		}

	}

	if(last == Type) {
		newVariable.init = "";
		variablesList.append(newVariable);
	}

	_backToLine(_startLine);

	return variablesList;
}

QString VarsConverter::publicVars() {
	QString out;
	QVector<Variable> variableList = _searchVariables();
	for(auto& V: variableList) {
		if(V.type == "TIME") _convertTime(&V.init);
		_convertType(&V.type);
		out += "\t" + V.type + " " + V.name;
		if(!V.init.isEmpty()) out += " = " + V.init;
		out += ";\n";
	}
	return out;
}
