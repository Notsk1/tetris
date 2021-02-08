/* Mainwindow:
 *
 * Desciption:
 * All the tetris games functionality are in the mainwindow files. More information
 * about the mainwindow files can be found in the intructions.txt file.
 *
 * Author:
 * Name: Petteri Nuotiomaa
 * Student id: 275350
 * Username: nuotioma
 * Email: petteri.nuotiomaa@tuni.fi
 *
 * Notes:
 *
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QDebug>
#include <random>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;

    // Method that chechks if movement is allowed in given direction
    bool checkMovement(char direction);

    // Method that checks collision between items on the board and the one moving
    bool checkCollision(int x, int y);

    // Method that is called every second after the game starts. Moves the piece down
    void movePieces();

    // Method that makes the pieces that move on the screen
    void makePieces();

    // Method that is called when startButton is clicked.
    void startGame();

    // Following methods work the same way as startGame but for their relative buttons:

    // This method is also called when Tetris game is lost(piece stops outside the board)
    void endGame();

    void pauseGame();

    void continueGame();

private slots:
    void on_retryButton_clicked();

    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_playerLineEdit_editingFinished();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // Simple score which shows the ammount of tetroids used in a single game
    int gameScore_=0;

    // Number which is put behind game scores to distinct games from each other
    int gameNumber_ = 0;

    // Is used for making the pieces move
    QTimer* timer_;

    // These vectors keep track of the pieces on the board
    std::vector<QGraphicsRectItem*> onBoard_;
    std::vector<QGraphicsRectItem*> moving_;

    // A map of all the possible tetromions as a key and their constructor box
    // locations as a value
    std::map<int, std::vector<int>> tetros_ = {{0,{40,60,80,100}},
                                               {1,{80,80,80,60}},
                                               {2,{60,60,60,80}},
                                               {3,{60,60,80,80}},
                                               {4,{100,80,80,60}},
                                               {5,{80,80,100,60}},
                                               {6,{60,80,80,100}}};

    // Map containing the color for each tetros shape
    std::map<int,QBrush> colors_ = {{0,Qt::red},{1,Qt::green},{2,Qt::blue}
                                  ,{3,Qt::yellow},{4,Qt::cyan},{5,Qt::magenta},
                                  {6,Qt::gray}};

    // Boolean value that determines what is printed as the name after the game.
    bool nameGiven_ = false;
};

#endif // MAINWINDOW_HH
