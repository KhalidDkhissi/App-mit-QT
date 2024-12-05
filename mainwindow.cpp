#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) ://In Qt, most graphical elements (like windows, dialogs, buttons, etc.) inherit from QWidget, allows the flexibility to pass any type of widget (like QMainWindow, QDialog, or any custom widget) as a parent since they all inherit from QWidget.
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(730,450);
    setWindowTitle("HotelManagement");
    this-> setWindowIcon(QIcon("C:/Users/hp/Desktop/icon"));
    this->setStyleSheet("background-color:lightblue;selection-color: yellow;");
    ui->btnRoomBooking->setStyleSheet("font-size: 18px;color: blue; border:2px solid green;");
    ui->btnRoomCheckout->setStyleSheet("font-size: 18px;color: blue; border:2px solid green;");
    ui->btnCheckAvailability->setStyleSheet("font-size: 17px;color: blue; border:2px solid green;");
    ui->bntTransaction->setStyleSheet("font-size: 18px;color: blue; border:2px solid green;");

    QPixmap pm("C:/Users/hp/Desktop/Hotel");
    ui->imgLabel->setPixmap((pm));
    ui->imgLabel->setScaledContents(true);

    // Establish a single database connection
    connectToDatabase();

    // Initialize dialogs with parent
    ptrRoomBookingDlg = new BookRoomDialog(this);
    ptrCheckOutDlg = new CheckOutDialog(this);
    ptrRoomAvailableDlg = new RoomAvailableDialog(this);
    //ptrCheckOutDlg->findChild<QPushButton*>("checkOutButton")->setStyleSheet("font-size:16px;color:red;border:2px solid;");

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow: Deleting";
    delete ui;
    delete ptrRoomBookingDlg;
    delete ptrCheckOutDlg;
    //delete ptrRoomAvailableDlg;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void MainWindow::connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-345R5RR;DATABASE=hotelmanagement;UID=sa;PWD=dev;");
    //db.setHostName("DESKTOP-345R5RR"); // Your server name
    //db.setUserName("sa");
    //db.setPassword("dev");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Connection Error", db.lastError().text());//Once you have the QSqlError object, you immediately call the text() method on it, which returns the error message as a QString.
    } else {
        qDebug() << "Database connected successfully.";
    }
}

void MainWindow::on_btnRoomBooking_clicked()
{
    qDebug() << metaObject()->className() << ": In Room Booking";
    ptrRoomBookingDlg->show();

    if(ptrRoomBookingDlg->isVisible())
        qDebug() << "Room Booking Dialog is visible";
    else
        qDebug() << "Room Booking Dialog is not visible";
}

void MainWindow::on_btnRoomCheckout_clicked()
{
    qDebug() << this->metaObject()->className() << ": In Room Checkout";
    ptrCheckOutDlg->show();
}
void MainWindow::on_btnCheckAvailability_clicked()
{
    qDebug() <<this->metaObject()->className()<< ": In Check Availability";
    ptrRoomAvailableDlg->updateRoomStatus();
    ptrRoomAvailableDlg->exec();
    //RoomAvailableDialog roomAvailableDlg(this);
    //roomAvailableDlg.exec();
}
void MainWindow::on_bntTransaction_clicked()
{
    ptrTransaction = new TransactionDialog(this);
    ptrTransaction ->show();
    //TransactionDialog transactionDialog(this);
    //transactionDialog.exec();
}
