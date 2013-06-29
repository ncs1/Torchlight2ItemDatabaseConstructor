#include "skillsdatfileparser.h"

#include <QStringList>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>
#include <QThread>

SkillsDatFileParser::SkillsDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent) :
    QObject(parent)
{
    mSkillsTable = new SkillsTable(itemDetailsDatabaseFilePath);
    mNumFilesProcessed = 0;
}

SkillsDatFileParser::~SkillsDatFileParser()
{
    if (mSkillsTable != NULL)
        delete mSkillsTable;
}


void SkillsDatFileParser::ParseSkills()
{
    if (mDatFilesFolder.exists() && mSkillsTable != NULL)
    {
        mNumFilesProcessed = 0;
        QDir skillsDir = mDatFilesFolder;
        QStringList nameFilters;
        nameFilters << "*.dat";

        skillsDir.setNameFilters(nameFilters);

        if (skillsDir.cd("SKILLS"))
        {
            mSkillsTable->BeginTransaction();
            ParseSkillsFileDir(skillsDir);
            mSkillsTable->CommitTransaction();
        }
    }
    emit finishedParsingSkillsFiles();
}

void SkillsDatFileParser::ParseSkillsFileDir(QDir directoryToParse)
{
    QFileInfoList folderItemsList = directoryToParse.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    for (qint32 i = 0; i < folderItemsList.count(); ++i)
    {
        QFileInfo fileInfo = folderItemsList.at(i);

        if (fileInfo.isDir())
        {
            QDir childDir(fileInfo.absoluteFilePath());
            ParseSkillsFileDir(childDir);
        }
        else if (fileInfo.suffix().toLower() == "dat")
        {
            Skill skill;
            ParseSkillsFile(fileInfo.absoluteFilePath(), skill);

            mSkillsTable->AddSkill(skill);
            ++mNumFilesProcessed;
        }
    }
}

void SkillsDatFileParser::ParseSkillsFile(QString filePath, Skill &skill)
{
    QFile fileToParse(filePath);

    if (fileToParse.open(QIODevice::ReadOnly))
    {
        QTextStream fileStream(&fileToParse);

        QString nextLine = fileStream.readLine().trimmed();

        while (!fileStream.atEnd())
        {
            if (nextLine.startsWith("<STRING>NAME:"))
            {
                skill.name = nextLine.mid(13);
            }
            else if (nextLine.startsWith("<TRANSLATE>DISPLAYNAME:"))
            {
                skill.displayName = nextLine.mid(23);
            }

            nextLine = fileStream.readLine().trimmed();
        }
    }
}
