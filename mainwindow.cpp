#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayoutAnswerLabels->
    connect(ui->actionLoadQuiz, SIGNAL(triggered(bool)), this, SLOT(loadQuiz()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutWindow()));
    connect(ui->actionQuizzes_Editor, SIGNAL(triggered(bool)), this, SLOT(openQuizzesEditor()));
    connect(&quiz, &Quiz::answers, this, &MainWindow::updateResult);

    answerAKey = new QShortcut(Qt::Key_A, this);
    answerBKey = new QShortcut(Qt::Key_B, this);
    answerCKey = new QShortcut(Qt::Key_C, this);
    answerDKey = new QShortcut(Qt::Key_D, this);
    nextKey = new QShortcut(Qt::Key_N, this);
    resetKey = new QShortcut(Qt::Key_R, this);
    skippKey = new QShortcut(Qt::Key_S, this);
    openKey = new QShortcut(Qt::CTRL + Qt::Key_O, this);
    connect(openKey, &QShortcut::activated, this, &MainWindow::loadQuiz);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete answerAKey;
    delete answerBKey;
    delete answerCKey;
    delete answerDKey;
    delete nextKey;
    delete skippKey;
    delete resetKey;
    delete openKey;
}

void MainWindow::on_resetButton_clicked()
{
    quiz.reset();
    on_nextButton_clicked();
    setAnswerButtonsEnabled(true);
}

void MainWindow::loadQuiz()
{
    quizFile = QFileDialog::getOpenFileName(this, tr("Open quiz"), "", tr("Quiz Files (*.qz)"));
    if(quizFile.isEmpty())
    {
        return;
    }
    on_resetButton_clicked();
    quiz.loadQuiz(quizFile);
    on_nextButton_clicked();
    ui->resetButton->setEnabled(true);
    setAnswerButtonsEnabled(true);
    connect(resetKey, &QShortcut::activated, this, &MainWindow::on_resetButton_clicked);
}

void MainWindow::answerSelected(QString letter)
{
    QString correctAnswer = quiz.checkAnswer(letter);
    if(correctAnswer == letter)
    {
        ui->statusLabel->setText("GOOD");
    }
    else if(correctAnswer != letter)
    {
        ui->statusLabel->setText("WRONG");
        setAnswerButtonsColor(letter, "red");
    }
    setAnswerButtonsColor(correctAnswer, "green");
    blockAnswerButtonsSignals(true);
    setControlButtonsEnabled(false);
    disconnect(skippKey, &QShortcut::activated, this, &MainWindow::on_skippButton_clicked);
    connect(nextKey, &QShortcut::activated, this, &MainWindow::on_nextButton_clicked);
}
void MainWindow::on_answerAButton_clicked()
{
    answerSelected("A");
}

void MainWindow::on_answerBButton_clicked()
{
    answerSelected("B");
}

void MainWindow::on_answerCButton_clicked()
{
    answerSelected("C");
}

void MainWindow::on_answerDButton_clicked()
{
    answerSelected("D");
}

void MainWindow::on_nextButton_clicked()
{
    setAnswerButtonsColor("A", "");
    setAnswerButtonsColor("B", "");
    setAnswerButtonsColor("C", "");
    setAnswerButtonsColor("D", "");
    blockAnswerButtonsSignals(false);
    setControlButtonsEnabled(true);
    Question currentQuestion;
    try
    {
        currentQuestion = quiz.loadNext();
    }
    catch(int)
    {
        questionsAreOver();
        return;
    }
    if(currentQuestion.image.isEmpty())
    {
        ui->pictureLabel->setText("No image.");
    }
    else
    {
        ui->pictureLabel->setPixmap(QPixmap(quiz.getQuizPath() + "/" + currentQuestion.image));
    }
    setControlButtonsEnabled(true);
    ui->answerAButton->setText(currentQuestion.answerA);
    ui->answerBButton->setText(currentQuestion.answerB);
    if(currentQuestion.answerC.isEmpty())
    {
        ui->CAnswerLabel->setVisible(false);
        ui->answerCButton->setVisible(false);
        disconnect(answerCKey, &QShortcut::activated, this, &MainWindow::on_answerCButton_clicked);
    }
    else
    {
        ui->answerCButton->setText(currentQuestion.answerC);
        ui->answerCButton->setVisible(true);
        ui->CAnswerLabel->setVisible(true);
        connect(answerCKey, &QShortcut::activated, this, &MainWindow::on_answerCButton_clicked);
    }

    if(currentQuestion.answerD.isEmpty())
    {
        ui->DAnswerLabel->setVisible(false);
        ui->answerDButton->setVisible(false);
        disconnect(answerDKey, &QShortcut::activated, this, &MainWindow::on_answerDButton_clicked);
    }
    else
    {
        ui->DAnswerLabel->setVisible(true);
        ui->answerDButton->setText(currentQuestion.answerD);
        ui->answerDButton->setVisible(true);
        connect(answerDKey, &QShortcut::activated, this, &MainWindow::on_answerDButton_clicked);
    }
    ui->questionLabel->setText(currentQuestion.questionContent);
    ui->statusLabel->setText("Waiting for answer...");
    QString newWindowTitle = QString("Quizzes 1.0 - ") +QString(" (") + QString::number(quiz.numberOfUserAnswers()) + QString("/") + QString::number(quiz.getNumberOfAllQuestions()) + QString(")");
    this->setWindowTitle(newWindowTitle);
    connect(skippKey, &QShortcut::activated, this, &MainWindow::on_skippButton_clicked);
    disconnect(nextKey, &QShortcut::activated, this, &MainWindow::on_nextButton_clicked);
    connect(answerAKey, &QShortcut::activated, this, &MainWindow::on_answerAButton_clicked);
    connect(answerBKey, &QShortcut::activated, this, &MainWindow::on_answerBButton_clicked);
}

void MainWindow::on_skippButton_clicked()
{
    try
    {
        quiz.skipp();
    }
    catch(int)
    {
        questionsAreOver();
        return;
    }
    on_nextButton_clicked();
}

void MainWindow::updateResult(unsigned good, unsigned wrong, unsigned skipped)
{
    QString percentGood = QString::number(good / ((double)good + (double)wrong + (double)skipped) * 100, 'f', 2);
    if(percentGood == "nan")
        percentGood = "0";

    QString percentWrong = QString::number(wrong / ((double)good + (double)wrong + (double)skipped) * 100, 'f', 2);
    if(percentWrong == "nan")
        percentWrong = "0";

    QString percentSkipped = QString::number(skipped / ((double)good + (double)wrong + (double)skipped) * 100, 'f', 2);
    if(percentSkipped == "nan")
        percentSkipped = "0";

    ui->correctAnswersLabel->setText("Correct answers: " + QString::number(good) + " (" + percentGood + " %)");
    ui->wrongAnswersLabel->setText("Wrong answers: " + QString::number(wrong) + " (" + percentWrong + " %)");
    ui->skippedAnswersLabel->setText("Skipped answers: " + QString::number(skipped) + " (" + percentSkipped + " %)");
}

void MainWindow::setControlButtonsEnabled(bool status)
{

    ui->nextButton->setEnabled(!status);
    ui->skippButton->setEnabled(status);
}

void MainWindow::questionsAreOver()
{
    ui->statusLabel->setText("Questions are over.");
    setControlButtonsEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->questionLabel->setText("question");
    ui->answerAButton->setText("A");
    ui->answerBButton->setText("B");
    ui->answerCButton->setText("C");
    ui->answerDButton->setText("D");
    ui->answerCButton->setVisible(true);
    ui->answerDButton->setVisible(true);
    setAnswerButtonsEnabled(false);
    this->setWindowTitle("Quizzes 1.0");
    ui->pictureLabel->setText("No image.");
    blockAnswerButtonsSignals(true);
    disconnect(skippKey, &QShortcut::activated, this, &MainWindow::on_skippButton_clicked);
    disconnect(nextKey, &QShortcut::activated, this, &MainWindow::on_nextButton_clicked);
    ui->CAnswerLabel->setVisible(true);
    ui->DAnswerLabel->setVisible(true);
}

void MainWindow::showAboutWindow()
{
    QMessageBox::information(this, "About", "Quizzes 1.0. Written by Arkadiusz97.");
}

void MainWindow::setAnswerButtonsColor(QString letter, QString color)
{
    if(letter == "A")
        ui->answerAButton->setStyleSheet("background-color: "+color+";");
    else if(letter == "B")
        ui->answerBButton->setStyleSheet("background-color: "+color+";");
    else if(letter == "C")
        ui->answerCButton->setStyleSheet("background-color: "+color+";");
    else if(letter == "D")
        ui->answerDButton->setStyleSheet("background-color: "+color+";");
}

void MainWindow::blockAnswerButtonsSignals(bool status)
{
    ui->answerAButton->blockSignals(status);
    ui->answerBButton->blockSignals(status);
    ui->answerCButton->blockSignals(status);
    ui->answerDButton->blockSignals(status);
    if(status)
    {
        disconnect(answerAKey, &QShortcut::activated, this, &MainWindow::on_answerAButton_clicked);
        disconnect(answerBKey, &QShortcut::activated, this, &MainWindow::on_answerBButton_clicked);
        disconnect(answerCKey, &QShortcut::activated, this, &MainWindow::on_answerCButton_clicked);
        disconnect(answerDKey, &QShortcut::activated, this, &MainWindow::on_answerDButton_clicked);
    }
}

void MainWindow::setAnswerButtonsEnabled(bool status)
{
    ui->answerAButton->setEnabled(status);
    ui->answerBButton->setEnabled(status);
    ui->answerCButton->setEnabled(status);
    ui->answerDButton->setEnabled(status);
}

void MainWindow::openQuizzesEditor()
{
    editor.show();
}
