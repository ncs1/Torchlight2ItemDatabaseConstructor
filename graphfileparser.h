#ifndef GRAPHFILEPARSER_H
#define GRAPHFILEPARSER_H

#include <QObject>
#include <QFile>
#include <QHash>


class GraphFileParser : public QObject
{
    Q_OBJECT
public:
    explicit GraphFileParser(QObject *parent = 0);
    
signals:
    
public slots:

protected:
    QHash<qint32, float>* ParseLevelGraphFile(QFile& inFile, qint32& highestLevel);
};

#endif // GRAPHFILEPARSER_H
