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

    Converter converter = Converter(argv[1], "out.txt");

    converter.exec();

    return 0;
}
