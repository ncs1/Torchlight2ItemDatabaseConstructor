#ifndef SQLITETABLE_H
#define SQLITETABLE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QObject>
#include <QMutex>

class SqliteTable
{
public:
    SqliteTable();

    bool BeginTransaction();
    bool CommitTransaction();

protected:
    QSqlDatabase GetDatabase();
    void PrintSqlError(QSqlError);
    void DatabaseName(QString filePath);

    bool mDoingTransaction;
private:
    QString mDatabaseName;

    QSqlDatabase mCurrentDb;

    static const QString SqlConnectionName;

};

#endif // SQLITETABLE_H
