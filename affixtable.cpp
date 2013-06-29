#include "affixtable.h"

#include <QSqlQuery>
#include <QVariant>

const QString AffixTable::AffixTableName = "AffixTable";
const QString AffixTable::AffixTableId = "Id";
const QString AffixTable::Name = "Name";
const QString AffixTable::Duration = "Duration";
const QString AffixTable::EffectName = "EffectName";
const QString AffixTable::EffectDuration = "EffectDuration";
const QString AffixTable::EffectDamageType = "EffectDamageType";
const QString AffixTable::EffectMin = "EffectMin";
const QString AffixTable::EffectMax = "EffectMax";

AffixTable::AffixTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void AffixTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + AffixTableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + AffixTableName + "(" +
                             AffixTableId + " INTEGER PRIMARY KEY, " +
                             Name + " TEXT, " +
                             Duration + " REAL, " +
                             EffectName + " TEXT, " +
                             EffectDuration + " TEXT, " +
                             EffectDamageType + " TEXT, " +
                             EffectMin + " REAL, " +
                             EffectMax + " REAL)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE UNIQUE INDEX " + AffixTableName + Name + "Index ON " + AffixTableName +
                             " (" + Name + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

qint64 AffixTable::AddAffix(const Affix& affix)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + AffixTableName + " (" + Name + ", " + Duration + ", " + EffectName +
                          ", " + EffectDuration + ", " + EffectDamageType + ", " + EffectMin + ", " + EffectMax +
                          ") VALUES (" +
                          ":name, :duration, :effectName, :effectDuration, :effectDamageType, :effectMin, :effectMax)"))
        {
            query.bindValue(":name", affix.name);
            query.bindValue(":duration", affix.duration);
            query.bindValue(":effectName", affix.effectName);
            query.bindValue(":effectDuration", affix.effectDuration);
            query.bindValue(":effectDamageType", affix.effectDamageType);
            query.bindValue(":effectMin", affix.effectMin);
            query.bindValue(":effectMax", affix.effectMax);

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

        if (!mDoingTransaction)
            db.close();
    }

    return result;
}
