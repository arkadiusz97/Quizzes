#include "Quiz.h"

Quiz::Quiz() : good(0), wrong(0), skipped(0), numberOfAllQuestions(0)
{
    generator.seed(QDateTime::currentMSecsSinceEpoch());
}

bool Quiz::loadQuiz(QString fileName)
{
    questions.clear();
    QRegularExpression re("(.*);(.*);(.*);(.*);(.*);(.*);(.*)");
    QFile file(fileName);
    if(!file.exists())
    {
        return 1;
    }
    file.open(QIODevice::ReadOnly);
    QFileInfo info(file);
    quizPath = info.absolutePath();
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        auto match = re.match(line);
        if(match.hasMatch())
        {
            questions.push_back(Question(match.captured(1), match.captured(2), match.captured(3).toUpper(), match.captured(4), match.captured(5), match.captured(6), match.captured(7)));
        }
    }
    file.close();
    currentFileName = fileName;
    numberOfAllQuestions = questions.size();
    return 0;
}

Question Quiz::loadNext()
{
    if(questions.isEmpty())
    {
        throw 1;
    }
    quint32 questionIndex = generator.bounded(0, questions.size());
    currentQuestion = questions[questionIndex];
    questions.erase(questions.begin() + questionIndex);
    return currentQuestion;
}

QString Quiz::checkAnswer(QString letter)
{
    if(currentQuestion.correctAnswer == letter)
    {
        emit answers(++good, wrong, skipped);
    }
    else
    {
        emit answers(good, ++wrong, skipped);
    }
    return currentQuestion.correctAnswer;
}

void Quiz::skipp()
{
    emit answers(good, wrong, ++skipped);
    if(questions.isEmpty())
    {
        throw 1;
    }
}

void Quiz::reset()
{
    loadQuiz(currentFileName);
    good = wrong = skipped = 0;
    emit answers(good, wrong, skipped);
}

int Quiz::getNumberOfAllQuestions()
{
    return numberOfAllQuestions;
}

int Quiz::numberOfUserAnswers()
{
    return numberOfAllQuestions - questions.size();
}

QString Quiz::getQuizPath()
{
    return quizPath;
}
