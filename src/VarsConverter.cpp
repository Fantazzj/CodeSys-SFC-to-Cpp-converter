//
// Created by Fantazz on 30/11/2022.
//

#include "VarsConverter.hpp"

VarsConverter::VarsConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp) {
    _xml = xml;
    _xmlFile = xmlFile;
    _cpp = cpp;
    _hpp = hpp;
}

void VarsConverter::exec() {
}
