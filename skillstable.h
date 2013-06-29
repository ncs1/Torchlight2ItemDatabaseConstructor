#ifndef SKILLSTABLE_H
#define SKILLSTABLE_H

#include <sqlitetable.h>

struct Skill;

class SkillsTable : public SqliteTable
{
public:
    SkillsTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddSkill(const Skill& skill);

private:
    static const QString SkillsTableName;
    static const QString SkillId;
    static const QString Name;
    static const QString DisplayName;
};

struct Skill
{
    Skill()
    {
        Clear();
    }

    void Clear()
    {
        skillId = 0;
        name = "";
        displayName = "";
    }

    qint64 skillId;
    QString name;
    QString displayName;
};

#endif // SKILLSTABLE_H
