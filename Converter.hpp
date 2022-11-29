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
    QVector<quint8> _divLevel;
    SFC _last;

    //Methods
    void _reachSFC();
    QString _reachCondition();
    void _indent(quint8 level);
    QString _searchAfterConv();
    QString _getStepName();
    QString _getJumpStepName();
    bool _isConvergence(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    bool _isDivergence(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    bool _isStep(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    bool _isJumpStep(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    bool _isTransition(QXmlStreamReader::TokenType tokenType = QXmlStreamReader::StartElement);
    void _printChangeStep(QString step);
    void _printIf(QString step, QString condition);
public:
    Converter(QString fileIn, QString fileOut);
    ~Converter();
    void exec();
};

#endif//CODESYS_SFC_TO_CPP_CONVERTER_CONVERTER_HPP
