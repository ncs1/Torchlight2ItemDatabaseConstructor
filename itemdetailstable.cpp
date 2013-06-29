#include "itemdetailstable.h"
#include <QSqlQuery>
#include <QVariant>

const QString ItemDetailsTable::ItemDetailsTableName = "ItemDetailsTable";
const QString ItemDetailsTable::ItemsTableId = "ItemId";
const QString ItemDetailsTable::ItemFilePath = "ItemFilePath";
const QString ItemDetailsTable::Guid = "Guid";
const QString ItemDetailsTable::Name = "Name";
const QString ItemDetailsTable::DisplayName = "DisplayName";
const QString ItemDetailsTable::SetName = "SetName";
const QString ItemDetailsTable::ItemType = "ItemType";
const QString ItemDetailsTable::Level = "Level";
const QString ItemDetailsTable::LevelRequired = "LevelRequired";
const QString ItemDetailsTable::Is2Handed = "Is2Handed";
const QString ItemDetailsTable::DefenseRequired = "DefenseRequired";
const QString ItemDetailsTable::DexterityRequired = "DexterityRequired";
const QString ItemDetailsTable::StrengthRequired = "StrengthRequired";
const QString ItemDetailsTable::MagicRequired = "MagicRequired";
const QString ItemDetailsTable::DamagePhysical = "DamagePhysical";
const QString ItemDetailsTable::DamageElectric = "DamageElectric";
const QString ItemDetailsTable::DamageFire = "DamageFire";
const QString ItemDetailsTable::DamageIce = "DamageIce";
const QString ItemDetailsTable::DamagePoison = "DamagePoison";
const QString ItemDetailsTable::MinimumDamage = "MinimumDamage";
const QString ItemDetailsTable::MaximumDamage = "MaximumDamage";
const QString ItemDetailsTable::DeviationFrom30FPS = "DeviationFrom30FPS";
const QString ItemDetailsTable::Speed = "Speed";
const QString ItemDetailsTable::SpeedDamageMod = "SpeedDamageMod";
const QString ItemDetailsTable::RarityDamageMod = "RarityDamageMod";
const QString ItemDetailsTable::Description = "Description";
const QString ItemDetailsTable::RequiredClass = "RequiredClass";

ItemDetailsTable::ItemDetailsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void ItemDetailsTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + ItemDetailsTableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + ItemDetailsTableName + " (" +
                             ItemsTableId + " INTEGER PRIMARY KEY, " +
                             ItemFilePath + " TEXT, " +
                             Guid + " INTEGER NOT NULL, " +
                             Name + " TEXT, " +
                             DisplayName + " TEXT, " +
                             SetName + " TEXT, " +
                             ItemType + " TEXT, " +
                             Level + " INTEGER, " +
                             LevelRequired + " INTEGER, " +
                             Is2Handed + " INTEGER, " +
                             DefenseRequired + " INTEGER NOT NULL default 0, " +
                             DexterityRequired + " INTEGER NOT NULL default 0, " +
                             StrengthRequired + " INTEGER NOT NULL default 0, " +
                             MagicRequired + " INTEGER NOT NULL default 0, " +
                             DamagePhysical + " INTEGER NOT NULL default 0, " +
                             DamageElectric + " INTEGER NOT NULL default 0, " +
                             DamageFire + " INTEGER NOT NULL default 0, " +
                             DamageIce + " INTEGER NOT NULL default 0, " +
                             DamagePoison + " INTEGER NOT NULL default 0, " +
                             MinimumDamage + " INTEGER NOT NULL default 0, " +
                             MaximumDamage + " INTEGER NOT NULL default 0, " +
                             DeviationFrom30FPS + " REAL NOT NULL default 1, " +
                             Speed + " INTEGER, " +
                             SpeedDamageMod + " INTEGER, " +
                             RarityDamageMod + " INTEGER, " +
                             Description + " TEXT, " +
                             RequiredClass + " TEXT)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE UNIQUE INDEX " + ItemDetailsTableName + Guid + "Index ON " + ItemDetailsTableName +
                             " (" + Guid + ")"))
        {
            PrintSqlError(query.lastError());
        }


        db.close();
    }
}

qint64 ItemDetailsTable::AddItem(const ItemDetails &inItem)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + ItemDetailsTableName + " (" + ItemFilePath + ", " + Guid + ", " + Name + ", " +
                          DisplayName + ", " + SetName + ", " + ItemType + ", " + Level + ", " + LevelRequired + ", " +
                          Is2Handed + ", " + DefenseRequired + ", " + DexterityRequired + ", " + StrengthRequired + ", " +
                          MagicRequired + ", " + DamagePhysical + ", " + DamageElectric + ", " +
                          DamageFire + ", " + DamageIce + ", " + DamagePoison + ", " + MinimumDamage + ", " + MaximumDamage
                          + ", " + DeviationFrom30FPS + ", " + Speed + ", " +
                          SpeedDamageMod + ", " + RarityDamageMod + ", " + Description + ", " + RequiredClass +
                          ") VALUES (" +
                          ":filePath, :guid, :name, :displayName, :setName, :itemType, :level, :levelRequired, " +
                          ":is2Handed, :defenseRequired, :dexterityRequired, :strengthRequired, " +
                          ":magicRequired, :damagePhysical, :damageElectric, :damageFire, " +
                          ":damageIce, :damagePoison, :minimumDamage, :maximumDamage, :deviationFrom30FPS, " +
                          ":speed, :speedDamageMod, :rarityDamageMod, :description, " +
                          ":requiredClass)"))
        {
            query.bindValue(":filePath", inItem.filePath);
            query.bindValue(":guid", inItem.guid);
            query.bindValue(":name", inItem.name);
            query.bindValue(":displayName", inItem.displayName);
            query.bindValue(":setName", inItem.setName);
            query.bindValue(":itemType", inItem.itemType);
            query.bindValue(":level", inItem.level);
            query.bindValue(":levelRequired", inItem.levelRequired);
            query.bindValue(":is2Handed", inItem.is2Handed ? 1 : 0);
            query.bindValue(":defenseRequired", inItem.defenseRequired);
            query.bindValue(":dexterityRequired", inItem.dexterityRequired);
            query.bindValue(":strengthRequired", inItem.strengthRequired);
            query.bindValue(":magicRequired", inItem.magicRequired);
            query.bindValue(":damagePhysical", inItem.damagePhysical);
            query.bindValue(":damageElectric", inItem.damageElectric);
            query.bindValue(":damageFire", inItem.damageFire);
            query.bindValue(":damageIce", inItem.damageIce);
            query.bindValue(":damagePoison", inItem.damagePoison);
            query.bindValue(":minimumDamage", inItem.minimumDamage);
            query.bindValue(":maximumDamage", inItem.maximumDamage);
            query.bindValue(":deviationFrom30FPS", inItem.deviationFrom30FPS);
            query.bindValue(":speed", inItem.speed);
            query.bindValue(":speedDamageMod", inItem.speedDamageMod);
            query.bindValue(":rarityDamageMod", inItem.rarityDamageMod);
            query.bindValue(":description", inItem.description);
            query.bindValue(":requiredClass", inItem.requiredClass);

            if (!query.exec())
            {
                PrintSqlError(query.lastError());
            }
            else
            {
                if (!query.lastInsertId().isNull() && query.lastInsertId().isValid())
                    result = query.lastInsertId().toLongLong();
            }
        }
        else
        {
            PrintSqlError(query.lastError());
        }

        if (!mDoingTransaction)
            db.close();
    }

    return result;
}
