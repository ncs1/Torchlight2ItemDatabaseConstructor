#include "itembaserequirementstable.h"

#include <QSqlQuery>
#include <QVariant>

const QString ItemBaseRequirementsTable::TableName = "ItemBaseRequirements";
const QString ItemBaseRequirementsTable::ItemBaseRequirementsId = "itemBaseRequirementsId";
const QString ItemBaseRequirementsTable::Level = "level";
const QString ItemBaseRequirementsTable::LevelRequired = "levelRequired";
const QString ItemBaseRequirementsTable::LevelRequiredNormal = "levelRequiredNormal";
const QString ItemBaseRequirementsTable::LevelRequiredSocketable = "levelRequiredSocketable";
const QString ItemBaseRequirementsTable::DefenseRequired = "defenseRequired";
const QString ItemBaseRequirementsTable::DexterityRequired = "dexterityRequired";
const QString ItemBaseRequirementsTable::MagicRequired = "magicRequired";
const QString ItemBaseRequirementsTable::StrengthRequired = "strengthRequired";

ItemBaseRequirementsTable::ItemBaseRequirementsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void ItemBaseRequirementsTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + TableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + TableName + " (" +
                             ItemBaseRequirementsId + " INTEGER PRIMARY KEY, " +
                             Level + " INTEGER, " +
                             LevelRequired + " INTEGER, " +
                             LevelRequiredNormal + " INTEGER, " +
                             LevelRequiredSocketable + " INTEGER, " +
                             DefenseRequired + " INTEGER, " +
                             DexterityRequired + " INTEGER, " +
                             MagicRequired + " INTEGER, " +
                             StrengthRequired + " INTEGER)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE UNIQUE INDEX " + TableName + Level + "Index ON " + TableName +
                             "(" + Level + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

qint64 ItemBaseRequirementsTable::AddItemBaseRequirements(ItemBaseRequirements itemRequirements)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + TableName + "(" + Level + ", " + LevelRequired + ", " +
                          LevelRequiredNormal + ", " + LevelRequiredSocketable + ", " +
                          DefenseRequired + ", " + DexterityRequired + ", " + MagicRequired + ", " +
                          StrengthRequired + ") VALUES (" +
                          ":level, :levelRequired, :levelRequiredNormal, :levelRequiredSocketable, " +
                          ":defenseRequired, :dexterityRequired, :magicRequired, :strengthRequired)"))
        {
            query.bindValue(":level", itemRequirements.level);
            query.bindValue(":levelRequired", itemRequirements.levelRequired);
            query.bindValue(":levelRequiredNormal", itemRequirements.levelRequiredNormal);
            query.bindValue(":levelRequiredSocketable", itemRequirements.levelRequiredSocketable);
            query.bindValue(":defenseRequired", itemRequirements.defenseRequired);
            query.bindValue(":dexterityRequired", itemRequirements.dexterityRequired);
            query.bindValue(":magicRequired", itemRequirements.magicRequired);
            query.bindValue(":strengthRequired", itemRequirements.strengthRequired);

            if (query.exec())
            {
                if (!query.lastInsertId().isNull() && query.lastInsertId().isValid())
                    result = query.lastInsertId().toLongLong();
            }
            else
            {
                PrintSqlError(query.lastError());
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
