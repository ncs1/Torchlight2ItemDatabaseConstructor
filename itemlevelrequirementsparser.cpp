#include "itemlevelrequirementsparser.h"

#include <QTextStream>

const QString ItemLevelRequirementsParser::LevelRequirementsFileName = "ITEM_LEVEL_REQUIREMENTS.DAT";
const QString ItemLevelRequirementsParser::LevelRequirementsNormalFileName = "ITEM_LEVEL_REQUIREMENTS_NORMAL.DAT";
const QString ItemLevelRequirementsParser::LevelRequirementsSocketableFileName = "ITEM_LEVEL_REQUIREMENTS_SOCKETABLE.DAT";
const QString ItemLevelRequirementsParser::DefenseRequirementsFileName = "ITEM_DEFENSE_REQUIREMENTS.DAT";
const QString ItemLevelRequirementsParser::DexterityRequirementsFileName = "ITEM_DEXTERITY_REQUIREMENTS.DAT";
const QString ItemLevelRequirementsParser::MagicRequirementsFileName = "ITEM_MAGIC_REQUIREMENTS.DAT";
const QString ItemLevelRequirementsParser::StrengthRequirementsFileName = "ITEM_STRENGTH_REQUIREMENTS.DAT";

ItemLevelRequirementsParser::ItemLevelRequirementsParser(QString itemDetailsDatabaseFilePath,
                                                         QObject *parent) :
    GraphFileParser(parent)
{
    mItemBaseRequirementsTable = new ItemBaseRequirementsTable(itemDetailsDatabaseFilePath);
    mNumItemRequirementsProcessed = 0;
}

ItemLevelRequirementsParser::~ItemLevelRequirementsParser()
{
    if (mItemBaseRequirementsTable != NULL)
        delete mItemBaseRequirementsTable;
}

void ItemLevelRequirementsParser::ParseItemBaseRequirements()
{
    if (mDatFilesFolder.exists() && mItemBaseRequirementsTable != NULL)
    {
        qint32 maxLevel = 0;
        QFile levelRequirementsFile(mDatFilesFolder.absoluteFilePath(LevelRequirementsFileName));
        QHash<qint32, float>* levelRequirements = ParseLevelGraphFile(levelRequirementsFile, maxLevel);

        QFile levelRequirementsNormalFile(mDatFilesFolder.absoluteFilePath(LevelRequirementsNormalFileName));
        QHash<qint32, float>* levelRequirementsNormal = ParseLevelGraphFile(levelRequirementsNormalFile, maxLevel);

        QFile levelRequirementsSocketableFile(mDatFilesFolder.absoluteFilePath(LevelRequirementsSocketableFileName));
        QHash<qint32, float>* levelRequirementsSocketable = ParseLevelGraphFile(levelRequirementsSocketableFile, maxLevel);

        QFile defenseRequirementsFile(mDatFilesFolder.absoluteFilePath(DefenseRequirementsFileName));
        QHash<qint32, float>* defenseRequirements = ParseLevelGraphFile(defenseRequirementsFile, maxLevel);

        QFile dexterityRequirementsFile(mDatFilesFolder.absoluteFilePath(DexterityRequirementsFileName));
        QHash<qint32, float>* dexterityRequirements = ParseLevelGraphFile(dexterityRequirementsFile, maxLevel);

        QFile magicRequirementsFile(mDatFilesFolder.absoluteFilePath(MagicRequirementsFileName));
        QHash<qint32, float>* magicRequirements = ParseLevelGraphFile(magicRequirementsFile, maxLevel);

        QFile strengthRequirementsFile(mDatFilesFolder.absoluteFilePath(StrengthRequirementsFileName));
        QHash<qint32, float>* strengthRequirements = ParseLevelGraphFile(strengthRequirementsFile, maxLevel);

        ItemBaseRequirements itemRequirements;

        if (mItemBaseRequirementsTable->BeginTransaction())
        {

            for (qint32 i = 0; i <= maxLevel; ++i)
            {
                itemRequirements.Clear();

                itemRequirements.level = i;
                itemRequirements.levelRequired = levelRequirements->value(i);
                itemRequirements.levelRequiredNormal = levelRequirementsNormal->value(i);
                itemRequirements.levelRequiredSocketable = levelRequirementsSocketable->value(i);
                itemRequirements.defenseRequired = defenseRequirements->value(i);
                itemRequirements.dexterityRequired = dexterityRequirements->value(i);
                itemRequirements.magicRequired = magicRequirements->value(i);
                itemRequirements.strengthRequired = strengthRequirements->value(i);

                mItemBaseRequirementsTable->AddItemBaseRequirements(itemRequirements);
                mNumItemRequirementsProcessed = i;
            }
            mItemBaseRequirementsTable->CommitTransaction();
        }

        delete levelRequirements;
        delete levelRequirementsNormal;
        delete levelRequirementsSocketable;
        delete defenseRequirements;
        delete dexterityRequirements;
        delete magicRequirements;
        delete strengthRequirements;
    }
    emit finishedParsingItemRequirements();
}

