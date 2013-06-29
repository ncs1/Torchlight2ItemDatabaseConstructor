#include "effectstable.h"

#include <QSqlQuery>
#include <QVariant>

const QString EffectsTable::EffectsTableName = "EffectsTable";
const QString EffectsTable::EffectId = "EffectId";
const QString EffectsTable::EffectNumber = "EffectNumber";
const QString EffectsTable::EffectRawName = "Effect";
const QString EffectsTable::Name = "Name";
const QString EffectsTable::GoodDescription = "GoodDescription";
const QString EffectsTable::GoodDescriptionOverTime = "GoodDescriptionOverTime";
const QString EffectsTable::BadDescription = "BadDescription";
const QString EffectsTable::BadDescriptionOverTime = "BadDescriptionOverTime";
const QString EffectsTable::DisplayPrecision = "DisplayPrecision";
const QString EffectsTable::DisplayPrecisionMaxVal = "DisplayPrecisionMaxVal";

EffectsTable::EffectsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void EffectsTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + EffectsTableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + EffectsTableName + " (" +
                             EffectId + " INTEGER PRIMARY KEY, " +
                             EffectNumber + " INTEGER, " +
                             EffectRawName + " TEXT, " +
                             Name + " TEXT, " +
                             GoodDescription + " TEXT, " +
                             GoodDescriptionOverTime + " TEXT, " +
                             BadDescription + " TEXT, " +
                             BadDescriptionOverTime + " TEXT, " +
                             DisplayPrecision + " INTEGER, " +
                             DisplayPrecisionMaxVal + " REAL)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE UNIQUE INDEX " + EffectsTableName + EffectNumber + "Index ON " + EffectsTableName +
                             " (" + EffectNumber + ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX " + EffectsTableName + EffectRawName + "Index ON " + EffectsTableName +
                             "(" + EffectRawName + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

qint64 EffectsTable::AddEffect(const Effect& inEffect)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + EffectsTableName + "(" + EffectNumber + ", " + EffectRawName + ", " + Name + ", " +
                          GoodDescription + ", " + GoodDescriptionOverTime + ", " + BadDescription +
                          ", " + BadDescriptionOverTime + ", " + DisplayPrecision + ", " +
                          DisplayPrecisionMaxVal +
                          ") VALUES (:effectNumber, :rawName, :name, " +
                          ":goodDescription, :goodDescriptionOverTime, :badDescription, " +
                          ":badDescriptionOverTime, :displayPrecision, :displayPrecisionMaxVal)"))
        {
            query.bindValue(":effectNumber", inEffect.effectNumber);
            query.bindValue(":rawName", inEffect.rawName);
            query.bindValue(":name", inEffect.name);
            query.bindValue(":goodDescription", inEffect.goodDescription);
            query.bindValue(":goodDescriptionOverTime", inEffect.goodDescriptionOverTime);
            query.bindValue(":badDescription", inEffect.badDescription);
            query.bindValue(":badDescriptionOverTime", inEffect.badDescriptionOverTime);
            query.bindValue(":displayPrecision", inEffect.displayPrecision);
            query.bindValue(":displayPrecisionMaxVal", inEffect.displayPrecisionMaxVal);

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
