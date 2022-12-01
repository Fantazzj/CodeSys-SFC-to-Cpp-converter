#include "src/Converter.hpp"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>

int main(int argc, char* argv[]) {
    //region Filenames
    QString xmlFileName = QString(argv[1]);
    //QString cppFileName = QString(argv[1]).replace("xml", "cpp");
    //QString hppFileName = QString(argv[1]).replace("xml", "hpp");
    //endregion

    //region FileOpening
    QFile xmlFile = QFile(xmlFileName);
    xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader* xml = new QXmlStreamReader(&xmlFile);

    /*
    QFile cppFile = QFile(cppFileName);
    cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* cpp = new QTextStream(&cppFile);

    QFile hppFile = QFile(hppFileName);
    hppFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream* hpp = new QTextStream(&hppFile);
     */

    //qDebug() << xmlFileName << cppFileName << hppFileName;
    //endregion

    Converter converter = Converter(xml, &xmlFile /*, cpp, hpp*/);
    converter.exec();

    //region FileClosing
    xmlFile.close();
    //cppFile.close();
    //hppFile.close();
    //endregion

    return 0;
}
