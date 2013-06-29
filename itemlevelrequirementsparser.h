#ifndef ITEMLEVELREQUIREMENTSPARSER_H
#define ITEMLEVELREQUIREMENTSPARSER_H

#include <QObject>
#include <QDir>
#include <QHash>
#include <QFile>
#include <itembaserequirementstable.h>
#include <graphfileparser.h>

class ItemLevelRequirementsParser : public GraphFileParser
{
    Q_OBJECT
public:
    explicit ItemLevelRequirementsParser(QString itemDetailsDatabaseFilePath,
                                         QObject *parent = 0);
    ~ItemLevelRequirementsParser();
    
    void DatFilesFolder(QDir datFilesFolder)
    {
        QDir statsFolder = datFilesFolder;

        if (statsFolder.cd("GRAPHS/STATS"))
            mDatFilesFolder = statsFolder;
    }

    quint32 NumItemRequirementsProcessed() { return mNumItemRequirementsProcessed; }

signals:
    void finishedParsingItemRequirements();
    
public slots:
    void ParseItemBaseRequirements();

private:
    ItemBaseRequirementsTable* mItemBaseRequirementsTable;

    QDir mDatFilesFolder;

    quint32 mNumItemRequirementsProcessed;

//    QHash<qint32, qint32>* ParseLevelRequirementsFile();
//    QHash<qint32, qint32>* ParseLevelRequirementsNormalFile();
//    QHash<qint32, qint32>* ParseLevelRequirementsSocketableFile();
//    QHash<qint32, qint32>* ParseDefenseRequirementsFile();
//    QHash<qint32, qint32>* ParseDexterityRequirementsFile();
//    QHash<qint32, qint32>* ParseMagicRequirementsFile();
//    QHash<qint32, qint32>* ParseStrengthRequirementsFile();
//    QHash<qint32, qint32>* ParseRequirementsFile(QFile& inFile, qint32& highestLevel);


    static const QString LevelRequirementsFileName;
    static const QString LevelRequirementsNormalFileName;
    static const QString LevelRequirementsSocketableFileName;
    static const QString DefenseRequirementsFileName;
    static const QString DexterityRequirementsFileName;
    static const QString MagicRequirementsFileName;
    static const QString StrengthRequirementsFileName;
    
};

#endif // ITEMLEVELREQUIREMENTSPARSER_H
