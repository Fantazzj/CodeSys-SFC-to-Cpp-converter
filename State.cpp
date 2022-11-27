//
// Created by Fantazz on 26/11/2022.
//

#include "State.hpp"
State::State(QString name) {
    this->name = name;
}

void State::printAll(State* call) {
    qDebug() << name;
    for(Transition T : transition) T.printAll();
    for(State* S : nextstate) {
        if(call != this) S->printAll(this);
        else break;
    }
}
