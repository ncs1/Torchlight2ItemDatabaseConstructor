#include "affixdatfileparser.h"

#include <QStringList>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>

AffixDatFileParser::AffixDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent) :
    QObject(parent)
{
    mAffixTable = new AffixTable(itemDetailsDatabaseFilePath);
    mNumFilesProcessed = 0;
}

AffixDatFileParser::~AffixDatFileParser()
{
    if (mAffixTable != NULL)
        delete mAffixTable;
}

void AffixDatFileParser::ParseAffixes()
{
    if (mDatFilesFolder.exists() && mAffixTable != NULL)
    {
        mNumFilesProcessed = 0;
        QDir affixDir = mDatFilesFolder;
        QStringList nameFilters;
        nameFilters << "*.dat";

        affixDir.setNameFilters(nameFilters);

        if (affixDir.cd("AFFIXES/ITEMS"))
        {
            mAffixTable->BeginTransaction();
            ParseAffixFileDir(affixDir);
            mAffixTable->CommitTransaction();
        }
    }
    emit finishedParsingAffixFiles();
}

void AffixDatFileParser::ParseAffixFileDir(QDir directoryToParse)
{
    QFileInfoList folderItemsList = directoryToParse.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs |
                                                                   QDir::Files, QDir::DirsFirst);

    for (qint32 i = 0; i < folderItemsList.count(); ++i)
    {
        QFileInfo fileInfo = folderItemsList.at(i);

        if (fileInfo.isDir())
        {
            QDir childDir(fileInfo.absoluteFilePath());
            ParseAffixFileDir(childDir);
        }
        else if (fileInfo.suffix().toLower() == "dat")
        {
            Affix affix;
            ParseAffixFile(fileInfo.absoluteFilePath(), affix);

            mAffixTable->AddAffix(affix);
            ++mNumFilesProcessed;
        }
    }

}

void AffixDatFileParser::ParseAffixFile(QString filePath, Affix &affix)
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
                affix.name = nextLine.mid(13);
            }
            else if (nextLine.startsWith("<FLOAT>DURATION:"))
            {
                affix.duration = nextLine.mid(16).toDouble();
            }
            else if (nextLine.startsWith("[EFFECT]"))
            {
                QString innerLine = fileStream.readLine().trimmed();

                while (!innerLine.startsWith("[/EFFECT"))
                {
                    if (innerLine.startsWith("<STRING>NAME:"))
                    {
                        affix.effectName = innerLine.mid(13);
                    }
                    else if (innerLine.startsWith("<STRING>DURATION:"))
                    {
                        affix.effectDuration = innerLine.mid(17);
                    }
                    else if (innerLine.startsWith("<STRING>TYPE:"))
                    {
                        affix.effectName = innerLine.mid(13);
                    }
                    else if (innerLine.startsWith("<STRING>DAMAGE_TYPE:"))
                    {
                        affix.effectDamageType = innerLine.mid(20);
                    }
                    else if (innerLine.startsWith("<FLOAT>MIN:"))
                    {
                        affix.effectMin = innerLine.mid(11).toDouble();
                    }
                    else if (innerLine.startsWith("<FLOAT>MAX:"))
                    {
                        affix.effectMax = innerLine.mid(11).toDouble();
                    }

                    innerLine = fileStream.readLine().trimmed();
                }
            }

            nextLine = fileStream.readLine().trimmed();
        }
    }
}
