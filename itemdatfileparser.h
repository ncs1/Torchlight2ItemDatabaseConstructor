#ifndef ITEMDATFILEPARSER_H
#define ITEMDATFILEPARSER_H

#include <QObject>
#include <itemdetailstable.h>
#include <QDir>
#include <QHash>


class ItemDatFileParser : public QObject
{
    Q_OBJECT
public:
    explicit ItemDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent = 0);
    ~ItemDatFileParser();
    
    void DatFilesFolder(QDir inDatFilesFolder) { mDatFilesFolder = inDatFilesFolder; }

    qint32 NumFilesProcessed() { return mNumFilesProcessed; }

signals:
    void finishedParsingItemsFiles();
    
public slots:
    void ParseItems();

private:
    void ParseItemFileDir(QDir directoryToParse);
    void ParseItemFile(QString filePath, ItemDetails& itemDetails);

    ItemDetailsTable* mItemsTable;
    QDir mDatFilesFolder;

    qint32 mNumFilesProcessed;

    enum LineKeys
    {
        BaseFile,
        UnitGuid,
        RequiredClassOpeningTag,
        RequiredClassClosingTag,
        UnitType,
        Name,
        DisplayName,
        Description,
        StrengthRequired,
        DexterityRequired,
        DefenseRequired,
        MagicRequired,
        Speed,
        DamagePhysical,
        DamagePoison,
        DamageFire,
        DamageIce,
        DamageElectric,
        MinDamage,
        MaxDamage,
        DeviationFrom30FPS,
        Level,
        LevelRequired,
        SpeedDmgMod,
        RarityDmgMod,
        Set
    };

    QHash<QString, ItemDatFileParser::LineKeys> mSearchKeys;



    
};

#endif // ITEMDATFILEPARSER_H
