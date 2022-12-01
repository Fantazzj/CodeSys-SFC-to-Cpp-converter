//
// Created by Fantazz on 27/11/2022.
//

#ifndef CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
#define CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP

#include "Converter.hpp"
#include "GeneralConverter.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QtGlobal>

enum SFC {
    Step,
    Transition,
    Action,
    Jump,
    Convergence,
    Divergence
};

class SFCConverter : public GeneralConverter {
private:
    //Files IO
    //QFile* _xmlFile;
    //QXmlStreamReader* _xml;
    //QFile* _outFile;
    QTextStream* _cpp;
    QTextStream* _hpp;

    //
    //quint8 _level = 0;
    QString _step;
    QVector<QString> _divStep;
    QVector<QString> _convStep;
    //QVector<quint8> _divLevel;
    SFC _last;

    //Methods
    //void _reachSFC();
    QString _reachCondition();
    //void _indent(quint8 level);
    QString _searchAfterConv();
    QVector<QString> _searchSteps();
    QString _getStepName();
    QString _getJumpStepName();
    //bool _isConvergence(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    //bool _isDivergence(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    //bool _isStep(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    bool _isInitialStep();
    //bool _isJumpStep(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    //bool _isTransition(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    void _printChangeStep(QString step);
    void _printIf(QString step, QString condition);
    void _printEnum(QVector<QString> stepsList);

public:
    SFCConverter(QXmlStreamReader* xml, QFile* xmlFile, QTextStream* cpp, QTextStream* hpp);
    void exec();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_SFCCONVERTER_HPP
