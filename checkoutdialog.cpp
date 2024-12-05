#include "checkoutdialog.h"
#include "ui_checkoutdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

CheckOutDialog::CheckOutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckOutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Check-Out");
    ui->checkOutButton->setStyleSheet("font-size: 16px;color: blue; border:2px solid green;");
    ui->cancelButton->setStyleSheet("font-size: 16px;color: blue; border:2px solid green;");
    ui->roomComboBox->setStyleSheet("font-size: 16px;color: blue; border:2px solid green;");
    ui->roomComboBox->addItems({"101", "102", "103", "104", "105", "201", "202", "203", "204", "205"});
}

CheckOutDialog::~CheckOutDialog()
{
    delete ui;
}

bool CheckOutDialog::isRoomBooked(int roomNumber) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Bookings WHERE RoomNumber = ?");
    query.addBindValue(roomNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);  // Room is booked if count > 0
    }
    return false;
}
bool CheckOutDialog::checkOutRoom(int roomNumber) {
    QSqlQuery query; // Using the singleton instance
    query.prepare("DELETE FROM Bookings WHERE RoomNumber = ?");
    query.addBindValue(roomNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return false;
    }

    return true;
}

void CheckOutDialog::on_checkOutButton_clicked()
{
    int roomNumber = ui->roomComboBox->currentText().toInt();

    if (!isRoomBooked(roomNumber)) {
        QMessageBox::information(this, "Check-Out Status", "The room is already available.");
        return;
    }

    if (checkOutRoom(roomNumber)) {
        QMessageBox::information(this, "Check-Out Status", "Room has been checked out! Say thank you to Customer.");
        this->accept(); // Close the dialog
    }
}

void CheckOutDialog::on_cancelButton_clicked()
{
    this->reject(); // Close the dialog without making changes
}
