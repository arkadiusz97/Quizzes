#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QRegularExpression>
#include <QDateTime>
#include <QRandomGenerator>
#include <QFileInfo>
#include "Question.h"

class Quiz : public QObject
{
    Q_OBJECT
public:
    Quiz();
    bool loadQuiz(QString fileName);
    QString checkAnswer(QString letter);
    Question loadNext();
    void skipp();
    void reset();
    int getNumberOfAllQuestions();
    int numberOfUserAnswers();
    QString getQuizPath();
signals:
    void answers(unsigned qood, unsigned wrong, unsigned skipped);
private:
    QVector<Question>questions;
    QRandomGenerator generator;
    Question currentQuestion;
    unsigned good, wrong, skipped;
    int numberOfAllQuestions;
    QString currentFileName, quizPath;
};
