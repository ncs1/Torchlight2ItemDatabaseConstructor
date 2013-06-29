#include "baseweapondamagetable.h"

#include <QSqlQuery>
#include <QVariant>

const QString BaseWeaponDamageTable::BaseWeaponDamageTableName = "BaseWeaponDamage";
const QString BaseWeaponDamageTable::BaseWeaponDamageId = "BaseWeaponDamageId";
const QString BaseWeaponDamageTable::Level = "Level";
const QString BaseWeaponDamageTable::BaseDamage = "BaseDamage";

BaseWeaponDamageTable::BaseWeaponDamageTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void BaseWeaponDamageTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + BaseWeaponDamageTableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + BaseWeaponDamageTableName + " (" +
                             BaseWeaponDamageId + " INTEGER PRIMARY KEY, " +
                             Level + " INTEGER, " +
                             BaseDamage + " REAL)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE UNIQUE INDEX " + BaseWeaponDamageTableName + Level + "Index ON " +
                             BaseWeaponDamageTableName + " (" + Level + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

qint64 BaseWeaponDamageTable::AddBaseWeaponDamage(BaseWeaponDamage baseDamage)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + BaseWeaponDamageTableName + "(" + Level + ", " + BaseDamage +
                          ") VALUES (:level, :baseDamage)"))
        {
            query.bindValue(":level", baseDamage.level);
            query.bindValue(":baseDamage", baseDamage.baseDamage);

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
