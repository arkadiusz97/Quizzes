#include "quizzeseditorbackend.h"

QuizzesEditorBackend::QuizzesEditorBackend(QObject *parent) : QObject(parent)
{
    //Nothing to do yet.
}

QJsonArray QuizzesEditorBackend::openQuiz(QString fileName)
{
    QFile file(fileName);
    if(!file.exists())
    {
        throw 1;
    }
    currentFileName = fileName;
    file.open(QIODevice::ReadOnly);
    QByteArray fileContent = file.readAll();
    file.close();
    QJsonArray jsonQuestions = QJsonDocument::fromJson(fileContent).array();
    return jsonQuestions;
}

void QuizzesEditorBackend::saveQuiz(const QStandardItemModel &questions)
{
    QFile file(currentFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QJsonArray toSave;
    for(int i=0; i<questions.rowCount(); ++i)
    {
        QJsonObject oneQuestion;
        oneQuestion["image"] = questions.data(questions.index(i, 0)).toString();
        oneQuestion["question"] = questions.data(questions.index(i, 1)).toString();
        oneQuestion["Valid answer"] = questions.data(questions.index(i, 2)).toString();
        oneQuestion["answer A"] = questions.data(questions.index(i, 3)).toString();
        oneQuestion["answer B"] = questions.data(questions.index(i, 4)).toString();
        oneQuestion["answer C"] = questions.data(questions.index(i, 5)).toString();
        oneQuestion["answer D"] = questions.data(questions.index(i, 6)).toString();
        toSave.append(QJsonValue(QJsonObject(oneQuestion)));
    }
    QJsonDocument document(toSave);
    QByteArray fileContent = document.toJson();
    file.write(fileContent);
    file.close();
}

void QuizzesEditorBackend::saveQuizAs(const QStandardItemModel &questions, QString newFileName)
{
    QString previousFileName = currentFileName;
    currentFileName = newFileName;
    saveQuiz(questions);
    currentFileName = previousFileName;
}
