#ifndef SKILLSDATFILEPARSER_H
#define SKILLSDATFILEPARSER_H

#include <QObject>
#include <skillstable.h>
#include <QDir>

class SkillsDatFileParser : public QObject
{
    Q_OBJECT
public:
    explicit SkillsDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent = 0);
    ~SkillsDatFileParser();

    void DatFilesFolder(QDir inDatFilesFolder) { mDatFilesFolder = inDatFilesFolder; }

    qint32 NumFilesProcessed() { return mNumFilesProcessed; }
    
signals:
    void finishedParsingSkillsFiles();
    
public slots:
    void ParseSkills();

private:
    void ParseSkillsFileDir(QDir directoryToParse);
    void ParseSkillsFile(QString filePath, Skill& skill);

    SkillsTable* mSkillsTable;
    QDir mDatFilesFolder;

    qint32 mNumFilesProcessed;
    
};

#endif // SKILLSDATFILEPARSER_H
