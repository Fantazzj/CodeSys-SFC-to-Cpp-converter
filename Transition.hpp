//
// Created by Fantazz on 26/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_TRANSITION_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_TRANSITION_HPP

#include "QString"
#include "QDebug"

class Transition {
private:
public:
    Transition(QString name);
    QString name;
    void printAll() const;
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_TRANSITION_HPP
