//
// Created by Fantazz on 26/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_STATE_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_STATE_HPP

#include <QString>
#include <QDebug>
#include <Transition.hpp>

class State {
public:
    State(QString name);
    QString name;
    QVector<Transition> transition;
    QVector<State*> nextstate;
    void printAll(State*);
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_STATE_HPP
