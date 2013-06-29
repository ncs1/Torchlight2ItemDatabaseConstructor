#include "baseweapondamageparser.h"

const QString BaseWeaponDamageParser::BaseWeaponDamageFileName = "BASE_WEAPON_DAMAGE.DAT";

BaseWeaponDamageParser::BaseWeaponDamageParser(QString databaseFilePath, QObject *parent) :
    GraphFileParser(parent)
{
    mBaseWeaponDamageTable = new BaseWeaponDamageTable(databaseFilePath);
    mNumBaseWeaponDamageProcessed = 0;
}

BaseWeaponDamageParser::~BaseWeaponDamageParser()
{
    if (mBaseWeaponDamageTable != NULL)
        delete mBaseWeaponDamageTable;
}

void BaseWeaponDamageParser::ParseBaseWeaponDamage()
{
    if (mDatFilesFolder.exists() && mBaseWeaponDamageTable != NULL)
    {
        mNumBaseWeaponDamageProcessed = 0;

        qint32 maxLevel = 0;
        QFile baseWeaponDamageFile(mDatFilesFolder.absoluteFilePath(BaseWeaponDamageFileName));
        QHash<qint32, float>* baseWeaponDamage = ParseLevelGraphFile(baseWeaponDamageFile, maxLevel);

        if (mBaseWeaponDamageTable->BeginTransaction())
        {
            BaseWeaponDamage baseDamage;

            for (qint32 i = 0; i <= maxLevel; ++i)
            {
                baseDamage.Clear();

                baseDamage.level = i;
                baseDamage.baseDamage = baseWeaponDamage->value(i);

                mBaseWeaponDamageTable->AddBaseWeaponDamage(baseDamage);
                mNumBaseWeaponDamageProcessed = i;
            }
            mBaseWeaponDamageTable->CommitTransaction();
        }
        delete baseWeaponDamage;

    }
    emit finishedParsingBaseWeaponDamage();
}
