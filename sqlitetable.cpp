#include "sqlitetable.h"

#include <QMessageBox>
#include <QDebug>

const QString SqliteTable::SqlConnectionName = "Torchlight2SharedStashSqliteConnection";


SqliteTable::SqliteTable()
{
    mDoingTransaction = false;
}


QSqlDatabase SqliteTable::GetDatabase()
{
    if (!mCurrentDb.isValid())
    {
        mCurrentDb = QSqlDatabase::database(SqlConnectionName, false);

        if(!mCurrentDb.isValid())
        {
            mCurrentDb = QSqlDatabase::addDatabase("QSQLITE", SqlConnectionName);
        }

        mCurrentDb.setDatabaseName(mDatabaseName);
    }
    return mCurrentDb;
}

bool SqliteTable::BeginTransaction()
{
    bool result = false;

    QSqlDatabase db = GetDatabase();

    if (db.isValid() && db.open())
    {
        result = db.transaction();

        if (!result)
        {
            PrintSqlError(db.lastError());
        }
        else
        {
            mDoingTransaction = true;
        }
    }

    return result;
}

bool SqliteTable::CommitTransaction()
{
    bool result = false;

    if (mDoingTransaction)
    {
        QSqlDatabase db = GetDatabase();

        if (db.isValid() && db.isOpen())
        {
            result = db.commit();

            if (!result)
            {
                PrintSqlError(db.lastError());
            }
            db.close();
        }
    }

    return result;
}

void SqliteTable::DatabaseName(QString filePath)
{
    mDatabaseName = filePath;
}

void SqliteTable::PrintSqlError(QSqlError error)
{
//    QMessageBox::information(NULL, "Sql Error", error.text(), QMessageBox::Ok);
    qDebug() << error.text();
}
