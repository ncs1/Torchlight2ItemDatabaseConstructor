#include "skillstable.h"

#include <QSqlQuery>
#include <QVariant>

const QString SkillsTable::SkillsTableName = "SkillsTable";
const QString SkillsTable::SkillId = "SkillId";
const QString SkillsTable::Name = "Name";
const QString SkillsTable::DisplayName = "DisplayName";

SkillsTable::SkillsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void SkillsTable::DatabasePath(QString databaseName)
{
    DatabaseName(databaseName);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("DROP TABLE IF EXISTS " + SkillsTableName))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE TABLE " + SkillsTableName + " (" +
                             SkillId + " INTEGER PRIMARY KEY, " +
                             Name + " TEXT, " +
                             DisplayName + " TEXT)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX " + SkillsTableName + Name + "Index ON " + SkillsTableName +
                             "(" + Name + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

qint64 SkillsTable::AddSkill(const Skill &skill)
{
    qint64 result = -1;

    QSqlDatabase db = GetDatabase();

    if (db.isOpen() || db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO " + SkillsTableName + "(" + Name + ", " + DisplayName +
                          ") VALUES (:name, :displayName)"))
        {
            query.bindValue(":name", skill.name);
            query.bindValue(":displayName", skill.displayName);

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
