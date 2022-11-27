#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QVector>
#include "State.hpp"
#include "Transition.hpp"

int main(int argc, char* argv[]) {
    QFile xmlFile = QFile(argv[1]);
    xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader xml = QXmlStreamReader(&xmlFile);

    QVector<State*> States;

    qDebug() << argv[1] << Qt::endl;

    while(xml.name() != QString("SFC")) xml.readNextStartElement();

    while(!(xml.name() == QString("SFC") && xml.tokenType() == QXmlStreamReader::EndElement)) {
        xml.readNext();

        if(xml.name() == QString("step") && xml.tokenType() == QXmlStreamReader::StartElement) {
            QString name = xml.attributes().value(QString("name")).toString();

            States.append(new State(name));

            if(States.length() > 1)
                States.at(States.length()-2)->nextstate.append(States.last());
        }

        if(xml.name() == QString("inVariable") && xml.tokenType() == QXmlStreamReader::StartElement) {
            do xml.readNext();
            while(xml.name() != QString("expression"));
            while(xml.tokenType() != QXmlStreamReader::Characters) xml.readNext();

            QString name = xml.text().toString();

            States.last()->transition.append(Transition(name));

        }

        if(xml.name() == QString("jumpStep") && xml.tokenType() == QXmlStreamReader::StartElement) {
            QString target = xml.attributes().value(QString("targetName")).toString();

            for(State* S: States) {
                if(S->name == target) {
                    States.last()->nextstate.append(S);
                    break;
                }
            }
        }
    }

    for(State* S: States) {
        S->printAll(S);
        qDebug() << "------------------";
    }

    return 0;
}
