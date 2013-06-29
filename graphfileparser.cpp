#include "graphfileparser.h"

#include <QTextStream>

GraphFileParser::GraphFileParser(QObject *parent) :
    QObject(parent)
{
}


QHash<qint32, float>* GraphFileParser::ParseLevelGraphFile(QFile& inFile, qint32& highestLevel)
{
    ///TODO: create a parent class for this function, since base weapon damage parses the exact same way
    QHash<qint32, float>* result = new QHash<qint32, float>();
    if (inFile.exists())
    {
        if (inFile.open(QIODevice::ReadOnly))
        {
            QTextStream fileStream(&inFile);
            QString nextLine = fileStream.readLine().trimmed();

            while (!fileStream.atEnd())
            {
                if (nextLine.startsWith("[POINT]"))
                {
                    qint32 x = 0;
                    float y = 0;
                    nextLine = fileStream.readLine().trimmed();
                    while (!fileStream.atEnd() && !nextLine.startsWith("[/POINT]"))
                    {
                        if (nextLine.startsWith("<FLOAT>X:"))
                        {
                            x = nextLine.mid(9).toInt();
                        }
                        else if (nextLine.startsWith("<FLOAT>Y:"))
                        {
                            y = nextLine.mid(9).toFloat();
                        }
                        nextLine = fileStream.readLine().trimmed();
                    }
                    if (x > highestLevel)
                        highestLevel = x;
                    result->insert(x, y);
                }
                nextLine = fileStream.readLine().trimmed();
            }

            inFile.close();
        }
    }
    return result;
}
