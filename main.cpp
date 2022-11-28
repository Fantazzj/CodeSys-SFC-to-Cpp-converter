#include "Converter.hpp"

int main(int argc, char* argv[]) {

    /*
     * QFile xmlFile = QFile(argv[1]);
    xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader xml = QXmlStreamReader(&xmlFile);

    //QFile outFile = QFile("out.cpp");
    //outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    //QTextStream out = QTextStream(&outFile);
    QTextStream out = QTextStream(stdout);
     */

    QString outFile = QString(argv[1]);
    outFile.replace("xml", "txt");

    Converter converter = Converter(argv[1], outFile);

    converter.exec();

    return 0;
}
