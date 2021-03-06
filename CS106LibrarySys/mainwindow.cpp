#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "logindialog.h"

#include "createaccountdialog.h"

#include "addbookdialog.h"

#include "editbookdialog.h"

#include "admindialog.h"

#include "book.h"

#include "passworddialog.h"

#include <QFile>

#include <QString>

MainWindow::MainWindow(QWidget * parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);
    ui -> loginLb -> setText("Not Logged In");
    ui -> signOutBtn -> hide();
    QPixmap image("C:/Users/hunte/OneDrive/Desktop/CS106LibrarySys/logo.png");
    ui -> logoImg -> setPixmap(image);
    ui -> logoImg -> setScaledContents(true);
    ui -> logoImg -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui -> showPasswordBtn -> hide();
    //Load books
    QFile inputFile("books.txt");
    inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in ( & inputFile);
    ui -> listWidget_books -> clear();
    while (! in .atEnd()) {
        QString line = in .readLine();
        QStringList info = line.split(",");
        //handle list of products ui
        int archived = info.at(0).split(" ")[0].toInt();
        //handle vector
        if (archived != -1) {
            qDebug() << archived;
            QString name = info.at(1);
            QString genre = info.at(2);
            ui -> listWidget_books -> addItem(info.at(1));
            book * newBook = new book(archived, name, genre, info.at(3), info.at(4), info.at(5), info.at(6), info.at(7));
            bookList.push_back(newBook);
            if (!archived /* && (info.at(6) == "")*/ ) {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(false);
            } else {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(true);
            }
        } //end while
        in .flush();
        inputFile.close();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
//Login btn
void MainWindow::on_loginBtn_clicked() {
    loginDialog dialog;
    dialog.setModal(true);
    connect( & dialog, & loginDialog::sendLogin, this, & MainWindow::receiveLogin);
    dialog.exec();
    //This func leads to receiveLogin if login is sucessful
}
//create account btn
void MainWindow::on_createAccountBtn_clicked() {
    createAccountDialog dialog;
    dialog.setModal(true);
    connect( & dialog, & createAccountDialog::sendLogin, this, & MainWindow::receiveLogin);
    dialog.exec();
    //This func leads to receiveLogin if login is sucessful see below
}
//go to admin
void MainWindow::on_pushButton_clicked() {
    adminDialog dialog;
    dialog.setModal(true);
    dialog.exec();
}
void MainWindow::runAdmin() {
    adminDialog dialog;
    dialog.setModal(true);
    dialog.exec();
}
void MainWindow::on_showPasswordBtn_clicked() {
    passwordDialog dialog;
    dialog.setModal(true);
    connect( & dialog, & passwordDialog::openAdminPass, this, & MainWindow::runAdmin);
    dialog.exec();
}
//addBook (admin use only)
void MainWindow::on_addBookAdmin_clicked() {
    book * newBook = nullptr;
    addBookDialog dialog(newBook, nullptr);
    dialog.setModal(true);
    dialog.exec();
    if (newBook != nullptr) {
        bookList.push_back(newBook);
        ui -> listWidget_books -> addItem(newBook -> getName());
        QFile userFile("books.txt");
        userFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream out( & userFile);
        //Edit file new line
        out << "0,";
        out << newBook -> getName() << ",";
        out << newBook -> getGenre() << ",";
        out << newBook -> getImageFilePath() << ",";
        out << newBook -> getWords() << ",";
        out << newBook -> getAuthor() << ",";
        out << newBook -> getAdded_date() << ",";
        out << newBook -> getChecked_by() << ",";
        out << newBook -> getReturn_date() << Qt::endl;
    }
}

void MainWindow::on_pushButton_8_clicked() {
    if (username.isEmpty()) {
        loginDialog dialog;
        dialog.setModal(true);
        connect( & dialog, & loginDialog::sendLogin, this, & MainWindow::receiveLogin);
        dialog.exec();
    } else {
        if (ui -> comboBox_2 -> isHidden()) {
            ui -> pushButton_8 -> setText("Borrow Book");
            ui -> comboBox_2 -> show();
            ui -> returnInLb -> setText("Borrow for : ");
            int index = ui -> listWidget_books -> currentIndex().row();
            this -> ui -> listWidget_books -> item(index) -> setForeground(Qt::black);

        } else {
            ui -> pushButton_8 -> setText("Return Book");
            ui -> comboBox_2 -> hide();
            ui -> returnInLb -> setText("Return within 14 days.");
            int index = ui -> listWidget_books -> currentIndex().row();
            this -> ui -> listWidget_books -> item(index) -> setForeground(Qt::darkGray);
        }
    }
}

void MainWindow::on_save_clicked() {
    qDebug() << "redunt";
}

void MainWindow::on_listWidget_books_itemPressed(QListWidgetItem * item) {
    //change to edit mode if admin
    int index = item -> listWidget() -> currentRow();
    if (index != -1) {
        book * currentItem = bookList.at(index);
        if (currentItem != nullptr) {
            ui -> titleLabel -> setText(currentItem -> getName());
            ui -> authLabel -> setText(currentItem -> getGenre());
            ui -> bookContentPlain -> setPlainText(currentItem -> getWords());
            ui -> authLabel -> setText(currentItem -> getAuthor());
            ui -> addeddateLb -> setText(currentItem -> getAdded_date());
            ui -> tagLb -> setText(currentItem -> getGenre());
            QPixmap pixmap(currentItem -> getImageFilePath());
            qDebug() << currentItem -> getImageFilePath();
            ui -> bookPreview -> setPixmap(pixmap);
            ui -> bookPreview -> setScaledContents(true);
            if (item -> foreground() == Qt::darkGray) {
                ui -> pushButton_8 -> setText("Return Book");
                ui -> comboBox_2 -> hide();
                ui -> returnInLb -> setText("Return within 14 days.");
                int index = ui -> listWidget_books -> currentIndex().row();
                this -> ui -> listWidget_books -> item(index) -> setForeground(Qt::darkGray);
            } else {
                ui -> pushButton_8 -> setText("Borrow Book");
                ui -> comboBox_2 -> show();
                ui -> returnInLb -> setText("Borrow for : ");
                this -> ui -> listWidget_books -> item(index) -> setForeground(Qt::black);
            }
        } //end inner if
    } //end if
}
void MainWindow::on_listWidget_books_itemDoubleClicked(QListWidgetItem * item) {
    //change to edit mode if admin
    int index = item -> listWidget() -> currentRow();
    if (index != -1) {
        book * currentItem = bookList.at(index);
        if (currentItem != nullptr) {
            ui -> titleLabel -> setText(currentItem -> getName());
            ui -> authLabel -> setText(currentItem -> getGenre());
            ui -> bookContentPlain -> setPlainText(currentItem -> getWords());
            ui -> authLabel -> setText(currentItem -> getAuthor());
            ui -> addeddateLb -> setText(currentItem -> getAdded_date());
            ui -> tagLb -> setText(currentItem -> getGenre());
            QPixmap pixmap(currentItem -> getImageFilePath());
            qDebug() << currentItem -> getImageFilePath();
            ui -> bookPreview -> setPixmap(pixmap);
            ui -> bookPreview -> setScaledContents(true);
        } //end inner if
    } //end if
}
void MainWindow::receiveLogin(const QString & user) {
    //login sucess.
    //qDebug() << username << " has logged in (Verifed login).";
    ui -> loginLb -> setText("Logged In as: " + user);
    username = user;
    ui -> showPasswordBtn -> show();
    ui -> createAccountBtn -> hide();
    ui -> loginBtn -> hide();
    ui -> signOutBtn -> show();

}
void MainWindow::on_signOutBtn_clicked() {
    // Sign out btn clicked
    ui -> signOutBtn -> hide();
    ui -> createAccountBtn -> show();
    ui -> loginBtn -> show();
    ui -> showPasswordBtn -> hide();
    username = "";
    ui -> loginLb -> setText("Not Signed in.");
    ui -> pushButton_8 -> setText("Borrow Book");
    ui -> comboBox_2 -> show();
    ui -> returnInLb -> setText("Borrow for : ");
}

//MAKE CASE INSENTIVE!!!
//MAKE CASE INSENTIVE!!!
void MainWindow::on_searchLn_textChanged(const QString & text) {
    int listWidgetSize = ui -> listWidget_books -> count();
    for (int k1 = 0; k1 < listWidgetSize; k1++) {
        if (ui -> listWidget_books -> item(k1) -> text().startsWith(text)) {
            ui -> listWidget_books -> item(k1) -> setHidden(false);
        } else {
            ui -> listWidget_books -> item(k1) -> setHidden(true);
        }
    }
}
//refresh
void MainWindow::on_pushButton_7_clicked() {
    QFile inputFile("books.txt");
    inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in ( & inputFile);
    ui -> listWidget_books -> clear();
    bookList.clear();
    while (! in .atEnd()) {
        QString line = in .readLine();
        QStringList info = line.split(",");
        //handle list of products ui
        int archived = info.at(0).split(" ")[0].toInt();
        //handle vector
        if (archived != -1) {
            QString name = info.at(1);
            QString genre = info.at(2);
            ui -> listWidget_books -> addItem(info.at(1));
            book * newBook = new book(archived, name, genre, info.at(3), info.at(4), info.at(5), info.at(6), info.at(7));
            qDebug() << archived << name << genre << info.at(3) << info.at(4) << info.at(5) << info.at(6) << info.at(7);
            bookList.push_back(newBook);
            if (!archived) {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(false);
            } else {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(true);
            }
        }
    }
    in.flush();
    inputFile.close();
}
void MainWindow::on_pushButton_2_clicked() {
    QFile inputFile("books.txt");
    inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in ( & inputFile);
    ui -> listWidget_books -> clear();
    bookList.clear();
    while (!in.atEnd()) {
        QString line = in .readLine();
        QStringList info = line.split(",");
        //handle list of products ui
        int archived = info.at(0).split(" ")[0].toInt();
        //handle vector
        if (archived != -1) {
            qDebug() << archived;
            QString name = info.at(1);
            QString genre = info.at(2);
            ui -> listWidget_books -> addItem(info.at(1));
            book * newBook = new book(archived, name, genre, info.at(3), info.at(4), info.at(5), info.at(6), info.at(7));
            bookList.push_back(newBook);
            if (!archived) {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(false);
            } else {
                this -> ui -> listWidget_books -> item(this -> ui -> listWidget_books -> count() - 1) -> setHidden(true);
            }
        }
    } //end while
    in .flush();
    inputFile.close();
}



