/* Tetris game
 *
 * Description:
 * This program is my take on the classic videogame called tetris. There is a simple UI
 * with only the essential buttons. All the functionality and the UI are in the
 * mainwindow.hh/cpp/ui files. Detailed descrition about the program and the user manual
 * can be found in the intructions.txt file. This file only starts the mainwindow.
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
#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
