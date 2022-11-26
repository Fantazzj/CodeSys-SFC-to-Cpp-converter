#include <QXmlStreamReader>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QXmlStreamReader xml = QXmlStreamReader(argv[1]);

    qDebug() << argv[1];

    return 0;
}
