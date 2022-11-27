//
// Created by Fantazz on 26/11/2022.
//

#include "Transition.hpp"

Transition::Transition(QString name) {
    this->name = name;
}

void Transition::printAll() const {
    qDebug() << name;
}
