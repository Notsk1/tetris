#include "mainwindow.hh"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::movePieces);

    ui->retryButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    for(auto piece:moving_){
        delete piece;
    }
    moving_.clear();
    for(auto piece:onBoard_){
        delete piece;
    }
    onBoard_.clear();
    delete scene_;
    delete timer_;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Movement is done through the wasd keys, which is the most used way in modern games

    if(event->key() == Qt::Key_D) {
        if(checkMovement('e')){
            for(QGraphicsRectItem* piece:moving_){
                piece->moveBy(20,0);
            }
        }
        return;
    }
    if(event->key() == Qt::Key_A) {
        if(checkMovement('w')){
            for(QGraphicsRectItem* piece:moving_){
                piece->moveBy(-20,0);
            }
        }
        return;
    }
    if(event->key() == Qt::Key_S) {
        if(checkMovement('s')){
            for(QGraphicsRectItem* piece:moving_){
                piece->moveBy(0,20);
            }
        }
        return;
    }
}

bool MainWindow::checkMovement(char direction)
{
    int x_move = 0;
    int y_move = 0;

    if(direction == 'e'){
        x_move += 20;
    }
    else if(direction == 'w'){
        x_move -= 20;
    }
    else if(direction == 's'){
        y_move += 20;
    }

    for(QGraphicsRectItem* piece:moving_){
        int current_x = piece->x();
        int current_y = piece->y();
        QRectF rect = scene_->sceneRect();

        // This is needed so that the pieces can be made outside the screen,
        // which give little more time for the players to think
        if(current_y<0){
            if(direction == 's'){
                if(checkCollision(current_x+x_move,current_y+y_move)){
                    return false;
                }
            }
            else{
                if(rect.contains(current_x+x_move, 20)){
                }
                else{
                    return false;
                }
            }
        }
        else{
            if(rect.contains(current_x+x_move, current_y+y_move) and not
                    checkCollision(current_x+x_move,current_y+y_move)) {
            }
            else {
                return false;
            }
        }

    }
    return true;
}

bool MainWindow::checkCollision(int x, int y)
{
    for(QGraphicsRectItem* otherPiece:onBoard_){
        QRectF otherPieceRect = otherPiece->rect();
        otherPieceRect.setRect(otherPiece->x(),otherPiece->y(),19,19);
        if(otherPieceRect.contains(x,y)){
            return true;
        }
    }
    return false;
}

void MainWindow::movePieces()
{
    if(moving_.size() == 0){
        makePieces();
        gameScore_ += 1;
        ui->scoreLcdNumber->display(gameScore_);
        // Speeds up the game every 10 score
        if(gameScore_%10 == 0){
            timer_->setInterval(timer_->interval()-100);
        }
    }
    else{
        // Checks if moving down is possible and moves if possible
        if(checkMovement('s')){
            for(QGraphicsRectItem* piece:moving_){
                piece->moveBy(0,20);
            }
        }
        else{
            bool gameOver = false;
            for(QGraphicsRectItem* piece:moving_){
                if(piece->y() < 0){
                    endGame();
                    gameOver=true;
                    break;
                }
                onBoard_.push_back(piece);
            }
            if(not gameOver){
                moving_.clear();
            }
            else{
                ui->textBrowser->setText("Game Over");
                ui->textBrowser->setStyleSheet("background-color:""black;"
                                               "color:""rgb(255, 255, 255);");
            }
        }
    }
}

void MainWindow::makePieces()
{
    // Random integer for choosing random tetros
    int tentamino = distr(randomEng);

    int last = 0;
    int y_coord = -60;

    // Same outline for every tetros
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    for(int value:tetros_.at(tentamino)){
        // x-values is stored so that when same x-values are in a row the y-value changes and
        // that gives the pieces their unigue 2D shape
        if(last==value){
            y_coord += SQUARE_SIDE;
        }
        QGraphicsRectItem* piece = scene_->addRect(0,0,SQUARE_SIDE,
                                                   SQUARE_SIDE,blackPen,
                                                   colors_.at(tentamino));
        piece->setPos(value,y_coord);
        last = value;
        moving_.push_back(piece);
    }
}

void MainWindow::startGame()
{
    timer_->start(1000);
}

void MainWindow::endGame()
{
    pauseGame();
    // Keeping track of the games played and the scores
    gameNumber_ += 1;
    if(nameGiven_){
        ui->playerNameBrowser->append(ui->playerLineEdit->text());
    }
    else{
        QString game = "Game ";
        QString gameNumber = QString::number(gameNumber_);
        game += gameNumber;
        ui->playerNameBrowser->append(game);
    }

    QString gameScore = QString::number(gameScore_);
    gameScore_=0;
    ui->playerScoreBrowser->append(gameScore);

    ui->startButton->setEnabled(true);
    ui->scoreLcdNumber->display(gameScore_);

    // Deleting all existing tetros from the game
    for(auto piece:moving_){
        delete piece;
    }
    moving_.clear();
    for(auto piece:onBoard_){
        delete piece;
    }
    onBoard_.clear();

    ui->retryButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
}

void MainWindow::pauseGame()
{
    timer_->stop();
}

void MainWindow::continueGame()
{
    timer_->start();
}

void MainWindow::on_retryButton_clicked()
{
    endGame();
    if(ui->pauseButton->text() == "Paused"){
        ui->pauseButton->setText("Pause");
        ui->textBrowser->setStyleSheet("background-color: transparent;");
        ui->textBrowser->setText("");
    }
    ui->retryButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
}

void MainWindow::on_startButton_clicked()
{
    startGame();
    ui->startButton->setEnabled(false);
    ui->retryButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->textBrowser->setStyleSheet("background-color: transparent;");
    ui->textBrowser->setText("");
}

void MainWindow::on_pauseButton_clicked()
{
    if(ui->pauseButton->text() == "Paused"){
        continueGame();
        ui->pauseButton->setText("Pause");
        ui->textBrowser->setStyleSheet("background-color: transparent;");
        ui->textBrowser->setText("");
    }
    else{
        ui->pauseButton->setText("Paused");
        pauseGame();
        ui->textBrowser->setStyleSheet("background-color:black;"
                                       "color: rgb(255, 255, 255);");
        ui->textBrowser->setText("Game Paused");
    }
}

void MainWindow::on_playerLineEdit_editingFinished()
{
    if(ui->playerLineEdit->text() != ""){
        nameGiven_ = true;
    }
    else{
        nameGiven_ = false;
    }
}

