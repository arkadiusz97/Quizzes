#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include "Quiz.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_resetButton_clicked();

    void loadQuiz();

    void answerSelected(QString letter);

    void on_answerAButton_clicked();

    void on_answerBButton_clicked();

    void on_answerCButton_clicked();

    void on_answerDButton_clicked();

    void on_nextButton_clicked();

    void on_skippButton_clicked();

    void setControlButtonsEnabled(bool status);

    void updateResult(unsigned good, unsigned wrong, unsigned skipped);

    void questionsAreOver();

    void showAboutWindow();

    void setAnswerButtonsColor(QString letter, QString color);

    void blockAnswerButtonsSignals(bool status);

    void setAnswerButtonsEnabled(bool status);
private:
    Ui::MainWindow *ui;
    Quiz quiz;
    QString quizFile;
    QShortcut *answerAKey, *answerBKey, *answerCKey, *answerDKey, *nextKey, *skippKey, *resetKey, *openKey;
};

#endif // MAINWINDOW_H
