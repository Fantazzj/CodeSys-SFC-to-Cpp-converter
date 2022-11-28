//
// Created by Fantazz on 27/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP

#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

enum SFC {
    Step,
    Trans,
    Action,
    Jump,
    Convergence,
    Divergence
};

class Converter {
private:
    //Files IO
    QFile* _xmlFile;
    QXmlStreamReader* _xml;
    QFile* _outFile;
    QTextStream* _out;

    //
    quint8 _level = 0;
    QString _step;
    QVector<QString> _divStep;
    QVector<QString> _convStep;
    SFC _last;

    //Methods
    void _reachSFC();
    QString _reachCondition();
    void _indent();
    QString _searchConvStep();

public:
    Converter(QString fileIn, QString fileOut);
    ~Converter();
    QTextStream* exec();
};


#endif//CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
