#include <QString>
#include <QImage>

struct Question
{
    Question(QString xImage, QString xQuestionContent, QString correctAnswer, QString xAnswerA, QString xAnswerB, QString xAnswerC, QString xAnswerD);
    Question();
    QString image, questionContent, correctAnswer, answerA, answerB, answerC, answerD;
};
