#ifndef QUIZZESEDITORBACKEND_H
#define QUIZZESEDITORBACKEND_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QStandardItemModel>
class QuizzesEditorBackend : public QObject
{
    Q_OBJECT
public:
    explicit QuizzesEditorBackend(QObject *parent = nullptr);
    QJsonArray openQuiz(QString fileName);
    void saveQuiz(const QStandardItemModel &questions);
    void saveQuizAs(const QStandardItemModel &questions, QString newFileName);
signals:

public slots:

private:
    QString currentFileName;
};

#endif // QUIZZESEDITORBACKEND_H
