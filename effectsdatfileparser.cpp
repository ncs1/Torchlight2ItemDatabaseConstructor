#include "effectsdatfileparser.h"

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QThread>

const QString EffectsDatFileParser::EffectsFileName = "EFFECTSLIST.DAT";

EffectsDatFileParser::EffectsDatFileParser(QString itemDetailsDatabaseFilePath, QObject* parent) :
    QObject(parent)
{
    mEffectsTable = new EffectsTable(itemDetailsDatabaseFilePath);
}

EffectsDatFileParser::~EffectsDatFileParser()
{
    if (mEffectsTable != NULL)
        delete mEffectsTable;
}


void EffectsDatFileParser::ParseEffects()
{
    if (mDatFilesFolder.exists() && mEffectsTable != NULL)
    {
        QFile effectsFile(mDatFilesFolder.absoluteFilePath(EffectsFileName));

        if (effectsFile.exists() && effectsFile.open(QIODevice::ReadOnly))
        {
            QTextStream fileStream(&effectsFile);

            Effect nextEffect;
            mNumEffectsProcessed = 0;

            QString nextLine = fileStream.readLine().trimmed();
            if (mEffectsTable->BeginTransaction())
            {

                while (!fileStream.atEnd())
                {
                    if (nextEffect.rawName.isEmpty() &&
                            nextLine.startsWith("<STRING>EFFECT:"))
                    {
                        nextEffect.rawName = nextLine.mid(15);
                    }
                    else if (nextEffect.name.isEmpty() &&
                            nextLine.startsWith("<STRING>NAME:"))
                    {
                        nextEffect.name = nextLine.mid(13);
                    }
                    else if (nextLine.startsWith("<TRANSLATE>GOODDES"))
                    {
                        if (nextLine.at(18) == 'O')
                        {
                            nextEffect.goodDescriptionOverTime = nextLine.mid(21);
                        }
                        else
                        {
                            nextEffect.goodDescription = nextLine.mid(19);
                        }
                    }
                    else if (nextLine.startsWith("<TRANSLATE>BADDES"))
                    {
                        if (nextLine.at(17) == 'O')
                        {
                            nextEffect.badDescriptionOverTime = nextLine.mid(20);
                        }
                        else
                        {
                            nextEffect.badDescription = nextLine.mid(18);
                        }
                    }
                    else if (nextLine.startsWith("<INTEGER>DISPLAYPRECISION:"))
                    {
                        nextEffect.displayPrecision = nextLine.mid(26).toInt();
                    }
                    else if (nextLine.startsWith("<FLOAT>DISPLAYPRECISIONMAXVAL:"))
                    {
                        nextEffect.displayPrecisionMaxVal = nextLine.mid(30).toFloat();
                    }
                    else if (nextLine.startsWith("[/EFFECT]"))
                    {
    //                    emit effectProcessed(mNumEffectsProcessed);
                        nextEffect.effectNumber = mNumEffectsProcessed++;
                        mEffectsTable->AddEffect(nextEffect);
                        nextEffect.Clear();
                    }

                    nextLine = fileStream.readLine().trimmed();
                    QThread::msleep(0);
                }
                mEffectsTable->CommitTransaction();
            }

            effectsFile.close();
        }
    }
    emit finishedParsingEffects();
}
