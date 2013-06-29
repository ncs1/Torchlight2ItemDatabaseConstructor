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

        while (!fileStream.atEnd())
        {
            if (nextLine.startsWith("<STRING>BASEFILE:"))
            {
                QString parentFilePath = nextLine.mid(17);
                if (parentFilePath.startsWith("media\\", Qt::CaseInsensitive) ||
                        parentFilePath.startsWith("media/", Qt::CaseInsensitive))
                    parentFilePath = parentFilePath.remove(0, 5);

                QFileInfo parentFile(mDatFilesFolder.absolutePath() + parentFilePath);

                if (parentFile.exists())
                    ParseItemFile(parentFile.absoluteFilePath(), itemDetails);
                break;
            }

            nextLine = fileStream.readLine().trimmed();
        }
        fileStream.seek(0);

        nextLine = fileStream.readLine().trimmed();
        itemDetails.filePath = filePath;

        while (!fileStream.atEnd())
        {

            if (nextLine.startsWith("<STRING>UNIT_GUID:"))
            {
                itemDetails.guid = nextLine.mid(18).toLongLong();
            }
            else if (nextLine.startsWith("[REQ_CLASS]"))
            {
                QString innerLine = fileStream.readLine().trimmed();

                while (!innerLine.startsWith("[/REQ_CLASS"))
                {
                    if (innerLine.startsWith("<STRING>UNITTYPE:"))
                    {
                        itemDetails.requiredClass = innerLine.mid(17);
                    }
                    innerLine = fileStream.readLine().trimmed();
                }
            }
            else if (nextLine.startsWith("<STRING>UNITTYPE:"))
            {
                itemDetails.is2Handed |= nextLine.contains("2H");
                itemDetails.itemType = nextLine.replace("1H", "").replace("2H", "").mid(17);
            }
            else if (nextLine.startsWith("<STRING>NAME:"))
            {
                itemDetails.name = nextLine.mid(13);
            }
            else if (nextLine.startsWith("<TRANSLATE>DISPLAYNAME:"))
            {
                itemDetails.displayName = nextLine.mid(23);
            }
            else if (nextLine.startsWith("<TRANSLATE>DESCRIPTION:"))
            {
                itemDetails.description = nextLine.mid(23);
            }
            else if (nextLine.startsWith("<INTEGER>STRENGTH_REQUIRED:"))
            {
                itemDetails.strengthRequired = nextLine.mid(27).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DEXTERITY_REQUIRED:"))
            {
                itemDetails.dexterityRequired = nextLine.mid(28).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DEFENSE_REQUIRED:"))
            {
                itemDetails.defenseRequired = nextLine.mid(26).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>MAGIC_REQUIRED:"))
            {
                itemDetails.magicRequired = nextLine.mid(24).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>SPEED:"))
            {
                itemDetails.speed = nextLine.mid(15).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DAMAGE_PHYSICAL:"))
            {
                itemDetails.damagePhysical = nextLine.mid(25).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DAMAGE_POISON:"))
            {
                itemDetails.damagePoison = nextLine.mid(23).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DAMAGE_FIRE:"))
            {
                itemDetails.damageFire = nextLine.mid(21).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DAMAGE_ICE:"))
            {
                itemDetails.damageIce = nextLine.mid(20).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>DAMAGE_ELECTRIC:"))
            {
                itemDetails.damageElectric = nextLine.mid(25).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>MINDAMAGE:"))
            {
                itemDetails.minimumDamage = nextLine.mid(19).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>MAXDAMAGE:"))
            {
                itemDetails.maximumDamage = nextLine.mid(19).toInt();
            }
            else if (nextLine.startsWith("<FLOAT>DEVIATION_FROM_30FPS:"))
            {
                itemDetails.deviationFrom30FPS = nextLine.mid(28).toDouble();
            }
            else if (nextLine.startsWith("<INTEGER>LEVEL:"))
            {
                itemDetails.level = nextLine.mid(15).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>LEVEL_REQUIRED:"))
            {
                itemDetails.levelRequired = nextLine.mid(24).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>SPEED_DMG_MOD:"))
            {
                itemDetails.speedDamageMod = nextLine.mid(23).toInt();
            }
            else if (nextLine.startsWith("<INTEGER>RARITY_DMG_MOD:"))
            {
                itemDetails.rarityDamageMod = nextLine.mid(24).toInt();
            }
            else if (nextLine.startsWith("<STRING>SET:"))
            {
                itemDetails.setName = nextLine.mid(12);
            }

            nextLine = fileStream.readLine().trimmed();
        }
    }

}
