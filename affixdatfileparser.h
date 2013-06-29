#ifndef AFFIXDATFILEPARSER_H
#define AFFIXDATFILEPARSER_H

#include <QObject>
#include <affixtable.h>
#include <QDir>

class AffixDatFileParser : public QObject
{
    Q_OBJECT
public:
    explicit AffixDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent = 0);
    ~AffixDatFileParser();

    void DatFilesFolder(QDir datFilesFolder) { mDatFilesFolder = datFilesFolder; }

    qint32 NumFilesProcessed() { return mNumFilesProcessed; }
    
signals:
    void finishedParsingAffixFiles();
    
public slots:
    void ParseAffixes();

private:
    void ParseAffixFileDir(QDir directoryToParse);
    void ParseAffixFile(QString filePath, Affix& affix);

    AffixTable* mAffixTable;
    QDir mDatFilesFolder;

    qint32 mNumFilesProcessed;
    
};

#endif // AFFIXDATFILEPARSER_H
