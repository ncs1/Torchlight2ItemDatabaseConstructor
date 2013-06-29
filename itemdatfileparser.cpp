#include "itemdatfileparser.h"

#include <QStringList>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>
#include <QThread>

ItemDatFileParser::ItemDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent) :
    QObject(parent)
{
    mItemsTable = new ItemDetailsTable(itemDetailsDatabaseFilePath);
    mNumFilesProcessed = 0;

    mSearchKeys["<STRING>BASEFILE"] = BaseFile;
    mSearchKeys["<STRING>UNIT_GUID"] = UnitGuid;
    mSearchKeys["[REQ_CLASS]"] = RequiredClassOpeningTag;
    mSearchKeys["[/REQ_CLASS]"] = RequiredClassClosingTag;
    mSearchKeys["<STRING>UNITTYPE"] = UnitType;
    mSearchKeys["<STRING>NAME"] = Name;
    mSearchKeys["<TRANSLATE>DISPLAYNAME"] = DisplayName;
    mSearchKeys["<TRANSLATE>DESCRIPTION"] = Description;
    mSearchKeys["<INTEGER>STRENGTH_REQUIRED"] = StrengthRequired;
    mSearchKeys["<INTEGER>DEXTERITY_REQUIRED"] = DexterityRequired;
    mSearchKeys["<INTEGER>DEFENSE_REQUIRED"] = DefenseRequired;
    mSearchKeys["<INTEGER>MAGIC_REQUIRED"] = MagicRequired;
    mSearchKeys["<INTEGER>SPEED"] = Speed;
    mSearchKeys["<INTEGER>DAMAGE_PHYSICAL"] = DamagePhysical;
    mSearchKeys["<INTEGER>DAMAGE_POISON"] = DamagePoison;
    mSearchKeys["<INTEGER>DAMAGE_FIRE"] = DamageFire;
    mSearchKeys["<INTEGER>DAMAGE_ICE"] = DamageIce;
    mSearchKeys["<INTEGER>DAMAGE_ELECTRIC"] = DamageElectric;
    mSearchKeys["<INTEGER>MINDAMAGE"] = MinDamage;
    mSearchKeys["<INTEGER>MAXDAMAGE"] = MaxDamage;
    mSearchKeys["<FLOAT>DEVIATION_FROM_30FPS"] = DeviationFrom30FPS;
    mSearchKeys["<INTEGER>LEVEL"] = Level;
    mSearchKeys["<INTEGER>LEVEL_REQUIRED"] = LevelRequired;
    mSearchKeys["<INTEGER>SPEED_DMG_MOD"] = SpeedDmgMod;
    mSearchKeys["<INTEGER>RARITY_DMG_MOD"] = RarityDmgMod;
    mSearchKeys["<STRING>SET"] = Set;
}

ItemDatFileParser::~ItemDatFileParser()
{
    if (mItemsTable != NULL)
        delete mItemsTable;
}

void ItemDatFileParser::ParseItems()
{
    if (mDatFilesFolder.exists() && mItemsTable != NULL)
    {
        mNumFilesProcessed = 0;
        QDir itemsDir = mDatFilesFolder;
        QStringList nameFilters;
        nameFilters << "*.dat";

        itemsDir.setNameFilters(nameFilters);

        if (itemsDir.cd("UNITS/ITEMS"))
        {
            mItemsTable->BeginTransaction();
            ParseItemFileDir(itemsDir);
            mItemsTable->CommitTransaction();
        }
    }
    emit finishedParsingItemsFiles();
}

void ItemDatFileParser::ParseItemFileDir(QDir directoryToParse)
{
    QFileInfoList folderItemsList = directoryToParse.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs |
                                                                   QDir::Files, QDir::DirsFirst);

    for (qint32 i = 0; i < folderItemsList.count(); ++i)
    {
        QFileInfo fileInfo = folderItemsList.at(i);

        if (fileInfo.isDir())
        {
            QDir childDir(fileInfo.absoluteFilePath());
            ParseItemFileDir(childDir);
        }
        else if (fileInfo.suffix().toLower() == "dat")
        {
            ItemDetails itemDetails;

            ParseItemFile(fileInfo.absoluteFilePath(), itemDetails);

            mItemsTable->AddItem(itemDetails);
            ++mNumFilesProcessed;
        }
    }


}

/*
 *  Recursively goes through the items dat files to build
 *  a complete list of an items details.
 */
void ItemDatFileParser::ParseItemFile(QString filePath, ItemDetails &itemDetails)
{
    QFile fileToParse(filePath);

    if (fileToParse.open(QIODevice::ReadOnly))
    {
        //first we need to see if this file references any other files
        QTextStream fileStream(&fileToParse);

        QString nextLine = fileStream.readLine().trimmed();
        QStringList keyValuePairs = nextLine.split(":");

        while (!fileStream.atEnd())
        {
            if (keyValuePairs.count() > 1)
            {
                QString key = keyValuePairs.at(0);
    //            QString value = keyValuePairs.at(1);

                if (mSearchKeys.contains(key))
                {
    //            ItemDatFileParser::LineKeys result = mSearchKeys[key];
                    if (keyValuePairs.count() > 1 && mSearchKeys[key] == BaseFile)
                    {
                        QString parentFilePath = keyValuePairs.at(1);
                        if (parentFilePath.startsWith("media\\", Qt::CaseInsensitive) ||
                                parentFilePath.startsWith("media/", Qt::CaseInsensitive))
                            parentFilePath = parentFilePath.remove(0, 5);

                        QFileInfo parentFile(mDatFilesFolder.absolutePath() + parentFilePath);

                        if (parentFile.exists())
                            ParseItemFile(parentFile.absoluteFilePath(), itemDetails);
                        break;
                    }
    //            if (nextLine.startsWith("<STRING>BASEFILE:"))
    //            {
    //                QString parentFilePath = nextLine.mid(17);
    //                if (parentFilePath.startsWith("media\\", Qt::CaseInsensitive) ||
    //                        parentFilePath.startsWith("media/", Qt::CaseInsensitive))
    //                    parentFilePath = parentFilePath.remove(0, 5);

    //                QFileInfo parentFile(mDatFilesFolder.absolutePath() + parentFilePath);

    //                if (parentFile.exists())
    //                    ParseItemFile(parentFile.absoluteFilePath(), itemDetails);
    //                break;
    //            }
                }
            }
            nextLine = fileStream.readLine().trimmed();
            keyValuePairs = nextLine.split(":");

        }
        fileStream.seek(0);

        nextLine = fileStream.readLine().trimmed();
        keyValuePairs = nextLine.split(":");
        itemDetails.filePath = filePath;

        while (!fileStream.atEnd())
        {

            if (keyValuePairs.count() > 1)
            {
                if (mSearchKeys.contains(keyValuePairs.at(0)))
                {
                    switch (mSearchKeys[keyValuePairs.at(0)])
                    {
                        case UnitGuid:
                        {
                            itemDetails.guid = keyValuePairs.at(1).toLongLong();
                            break;
                        }

                        case UnitType:
                        {
                            QString value = keyValuePairs.at(1);
                            itemDetails.is2Handed |= value.contains("2H");
                            itemDetails.itemType = value.replace("1H", "").replace("2H", "");
                            break;
                        }

                        case Name:
                        {
                            itemDetails.name = keyValuePairs.at(1);
                            break;
                        }

                        case DisplayName:
                        {
                            itemDetails.displayName = keyValuePairs.at(1);
                            break;
                        }

                        case Description:
                        {
                            itemDetails.description = keyValuePairs.at(1);
                            break;
                        }

                        case StrengthRequired:
                        {
                            itemDetails.strengthRequired = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DexterityRequired:
                        {
                            itemDetails.dexterityRequired = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DefenseRequired:
                        {
                            itemDetails.defenseRequired = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case MagicRequired:
                        {
                            itemDetails.magicRequired = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case Speed:
                        {
                            itemDetails.speed = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DamagePhysical:
                        {
                            itemDetails.damagePhysical = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DamagePoison:
                        {
                            itemDetails.damagePoison = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DamageFire:
                        {
                            itemDetails.damageFire = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DamageIce:
                        {
                            itemDetails.damageIce = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DamageElectric:
                        {
                            itemDetails.damageElectric = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case MinDamage:
                        {
                            itemDetails.minimumDamage = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case MaxDamage:
                        {
                            itemDetails.maximumDamage = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case DeviationFrom30FPS:
                        {
                            itemDetails.deviationFrom30FPS = keyValuePairs.at(1).toDouble();
                            break;
                        }

                        case Level:
                        {
                            itemDetails.level = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case LevelRequired:
                        {
                            itemDetails.levelRequired = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case SpeedDmgMod:
                        {
                            itemDetails.speedDamageMod = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case RarityDmgMod:
                        {
                            itemDetails.rarityDamageMod = keyValuePairs.at(1).toInt();
                            break;
                        }

                        case Set:
                        {
                            itemDetails.setName = keyValuePairs.at(1);
                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                }
//                if (nextLine.startsWith("<STRING>UNIT_GUID:"))
//                {
//                    itemDetails.guid = nextLine.mid(18).toLongLong();
//                }
//                else if (nextLine.startsWith("<STRING>UNITTYPE:"))
//                {
//                    itemDetails.is2Handed |= nextLine.contains("2H");
//                    itemDetails.itemType = nextLine.replace("1H", "").replace("2H", "").mid(17);
//                }
//                else if (nextLine.startsWith("<STRING>NAME:"))
//                {
//                    itemDetails.name = nextLine.mid(13);
//                }
//                else if (nextLine.startsWith("<TRANSLATE>DISPLAYNAME:"))
//                {
//                    itemDetails.displayName = nextLine.mid(23);
//                }
//                else if (nextLine.startsWith("<TRANSLATE>DESCRIPTION:"))
//                {
//                    itemDetails.description = nextLine.mid(23);
//                }
//                else if (nextLine.startsWith("<INTEGER>STRENGTH_REQUIRED:"))
//                {
//                    itemDetails.strengthRequired = nextLine.mid(27).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DEXTERITY_REQUIRED:"))
//                {
//                    itemDetails.dexterityRequired = nextLine.mid(28).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DEFENSE_REQUIRED:"))
//                {
//                    itemDetails.defenseRequired = nextLine.mid(26).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>MAGIC_REQUIRED:"))
//                {
//                    itemDetails.magicRequired = nextLine.mid(24).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>SPEED:"))
//                {
//                    itemDetails.speed = nextLine.mid(15).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DAMAGE_PHYSICAL:"))
//                {
//                    itemDetails.damagePhysical = nextLine.mid(25).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DAMAGE_POISON:"))
//                {
//                    itemDetails.damagePoison = nextLine.mid(23).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DAMAGE_FIRE:"))
//                {
//                    itemDetails.damageFire = nextLine.mid(21).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DAMAGE_ICE:"))
//                {
//                    itemDetails.damageIce = nextLine.mid(20).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>DAMAGE_ELECTRIC:"))
//                {
//                    itemDetails.damageElectric = nextLine.mid(25).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>MINDAMAGE:"))
//                {
//                    itemDetails.minimumDamage = nextLine.mid(19).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>MAXDAMAGE:"))
//                {
//                    itemDetails.maximumDamage = nextLine.mid(19).toInt();
//                }
//                else if (nextLine.startsWith("<FLOAT>DEVIATION_FROM_30FPS:"))
//                {
//                    itemDetails.deviationFrom30FPS = nextLine.mid(28).toDouble();
//                }
//                else if (nextLine.startsWith("<INTEGER>LEVEL:"))
//                {
//                    itemDetails.level = nextLine.mid(15).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>LEVEL_REQUIRED:"))
//                {
//                    itemDetails.levelRequired = nextLine.mid(24).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>SPEED_DMG_MOD:"))
//                {
//                    itemDetails.speedDamageMod = nextLine.mid(23).toInt();
//                }
//                else if (nextLine.startsWith("<INTEGER>RARITY_DMG_MOD:"))
//                {
//                    itemDetails.rarityDamageMod = nextLine.mid(24).toInt();
//                }
//                else if (nextLine.startsWith("<STRING>SET:"))
//                {
//                    itemDetails.setName = nextLine.mid(12);
//                }
            }
            else
            {
                if (mSearchKeys.contains(nextLine))
                {
                    if (mSearchKeys[nextLine] == RequiredClassOpeningTag)
                    {
                        QString innerLine = fileStream.readLine().trimmed();
                        keyValuePairs = innerLine.split(":");

                        bool isClosingTag = mSearchKeys.contains(innerLine) &&
                                mSearchKeys[innerLine] == RequiredClassClosingTag;

                        while (!isClosingTag)
                        {
                            if (keyValuePairs.count() > 0 && mSearchKeys.contains(keyValuePairs.at(0)))
                            {
                                switch (mSearchKeys[keyValuePairs.at(0)])
                                {
                                    case UnitType:
                                    {
                                        itemDetails.requiredClass = keyValuePairs.at(1);
                                        break;
                                    }

                                    default:
                                    {
                                        break;
                                    }
                                }
                            }

                            innerLine = fileStream.readLine().trimmed();
                            keyValuePairs = innerLine.split(":");

                            isClosingTag = mSearchKeys.contains(innerLine) &&
                                    mSearchKeys[innerLine] == RequiredClassClosingTag;
                        }
                    }
                }
//                if (nextLine.startsWith("[REQ_CLASS]"))
//                {
//                    QString innerLine = fileStream.readLine().trimmed();

//                    while (!innerLine.startsWith("[/REQ_CLASS"))
//                    {
//                        if (innerLine.startsWith("<STRING>UNITTYPE:"))
//                        {
//                            itemDetails.requiredClass = innerLine.mid(17);
//                        }
//                        innerLine = fileStream.readLine().trimmed();
//                    }
//                }
            }

            nextLine = fileStream.readLine().trimmed();
            keyValuePairs = nextLine.split(":");
        }
    }

}
