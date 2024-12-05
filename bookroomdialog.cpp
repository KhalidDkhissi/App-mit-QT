#include "bookroomdialog.h"
#include "ui_bookroomdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

BookRoomDialog::BookRoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookRoomDialog),
    checkInDate(QDate()),
    checkOutDate(QDate())
{
    ui->setupUi(this);//is a method to initialize the user interface elements defined in the .ui file and adds it to the BookRoomDialog widget.
    setFixedSize(730, 750);
    QDate currentDate = QDate::currentDate();

    // Set the minimum date to today to prevent booking in the past
    ui->calendarWidget->setMinimumDate(currentDate);
    connect (ui->calendarWidget, &QCalendarWidget::selectionChanged, this, [this]() {
        onCalendarDateSelected(ui->calendarWidget->selectedDate());
    });

    this->setWindowTitle("Book A Room");
    ui->roomComboBox->setStyleSheet("font-size: 16px;color: blue; border:2px solid green;");

    // Populate the combo box with room numbers (101-105)
    ui->roomComboBox->addItems({"101", "102", "103", "104", "105", "201", "202", "203", "204", "205"});
    ui->cardNumberLabel->setVisible(false);
    ui->cardNumberLineEdit->setVisible(false);
    connect(ui->paymentMethodComboBox, &QComboBox::currentTextChanged, this, &BookRoomDialog::onPaymentMethodChanged);

}

BookRoomDialog::~BookRoomDialog()
{
    delete ui;
}

void BookRoomDialog::onPaymentMethodChanged(const QString &paymentMethod) {
    // Show the card number fields only if "Credit Card" or "Debit Card" is selected
    bool showCardFields = (paymentMethod == "Credit Card" || paymentMethod == "Debit Card");

    ui->cardNumberLabel->setVisible(showCardFields);
    ui->cardNumberLineEdit->setVisible(showCardFields);
}

int BookRoomDialog::getCustomerID(const QString &name, const QString &phoneNumber) {
    QSqlQuery query;
    query.prepare("SELECT CustomerID FROM Customers WHERE Name = ? AND PhoneNumber = ?");
    query.addBindValue(name);
    query.addBindValue(phoneNumber);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();  // Return CustomerID if found
    }

    return -1;  // Return -1 if customer doesn't exist
}

// Insert new customer into Customers table
bool BookRoomDialog::addNewCustomer(const QString &name, const QString &phoneNumber) {
    QSqlQuery query;
    query.prepare("INSERT INTO Customers (Name, PhoneNumber, TotalBookings) VALUES (?, ?, 1)");
    query.addBindValue(name);
    query.addBindValue(phoneNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return false;
    }
    return true;
}

// Update TotalBookings in Customers table
bool BookRoomDialog::updateCustomerBookings(int customerID) {
    QSqlQuery query;
    query.prepare("UPDATE Customers SET TotalBookings = TotalBookings + 1 WHERE CustomerID = ?");
    query.addBindValue(customerID);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return false;
    }
    return true;
}

// Check how many bookings a customer has made (for discount logic)
int BookRoomDialog::getTotalBookings(int customerID) {
    QSqlQuery query;
    query.prepare("SELECT TotalBookings FROM Customers WHERE CustomerID = ?");
    query.addBindValue(customerID);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();  // Return total bookings
    }

    return 0;  // Default if not found
}

// Insert a booking using RoomNumber, CustomerID, and Price
bool BookRoomDialog::insertBooking(int roomNumber, const QString &name, const QString &phoneNumber, double price, const QString paymentStatus) {
    // First, check if the customer already exists
    int customerID = getCustomerID(name, phoneNumber);

    // If the customer doesn't exist, add them to the Customers table
    if (customerID == -1) {
        if (!addNewCustomer(name, phoneNumber)) {
            return false;  // Return false if adding a customer failed
        }
        customerID = getCustomerID(name, phoneNumber);  // Retrieve the newly added CustomerID
    } else {
        // Existing customer, update their TotalBookings
        updateCustomerBookings(customerID);
    }

    // Get total bookings for this customer and apply discounts if applicable
    int totalBookings = getTotalBookings(customerID);
    if (totalBookings == 2) {
        price = static_cast<double>(price * 0.85);  // 15% discount for second booking
        QMessageBox::information(this, "Discount Applied", "You've received a 15% discount for being a returning customer!");
    } else if (totalBookings == 3) {
        QMessageBox::information(this, "Special Offer", "Als Stammkunde haben Sie ein kostenloses Frühstück und Mittagessen erhalten!");
    }else if (totalBookings == 4) {
        price = 0;
        QMessageBox::information(this, "Ganz besonderes Angebot", "Herzlichen Glückwunsch, Sie haben eine Gratisnacht bekommen !");
    }
    // Insert the booking into the Bookings table
    QSqlQuery query;
    query.prepare("INSERT INTO Bookings (RoomNumber, CustomerID, Price, CheckInDate, CheckOutDate, PaymentStatus) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(roomNumber);
    query.addBindValue(customerID);  // Link the booking to the CustomerID
    query.addBindValue(price);
    query.addBindValue(checkInDate);
    query.addBindValue(checkOutDate);
    query.addBindValue(paymentStatus);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return false;
    }
    if (totalBookings == 5) {
        resetTotalBookings(customerID);  // Reset the booking count for the customer
    }

    return true;
}

void BookRoomDialog::resetTotalBookings(int customerID) {
    QSqlQuery query;
    query.prepare("UPDATE Customers SET TotalBookings = 1 WHERE CustomerID = ?");  // Reset to 1
    query.addBindValue(customerID);
    query.exec();
}


// Check if a room is available for booking
bool BookRoomDialog::isRoomAvailable(int roomNumber) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Bookings WHERE RoomNumber = ?");
    query.addBindValue(roomNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count == 0);  // If count is 0, the room is available
    }

    return false;
}

void BookRoomDialog::onCalendarDateSelected(const QDate &date) {
    // Logic for setting check-in and check-out dates
    if (checkInDate.isNull()) {
        checkInDate = date;//checkInDate = date; takes the value of the date parameter (which is the date the user selected)
        ui->checkInLabel->setText("Check-In: " + checkInDate.toString("yyyy-MM-dd"));
    }
    // If a check-in date is set but no check-out date is set, check if the new date is valid for check-out
    else if (checkOutDate.isNull() && date > checkInDate) {
        checkOutDate = date;
        ui->checkOutLabel->setText("Check-Out: " + checkOutDate.toString("yyyy-MM-dd"));
    }
    // If both dates are set and the user selects another date, reset the check-in and check-out dates
    else {
        checkInDate = date;
        checkOutDate = QDate(); // Reset the check-out date
        ui->checkInLabel->setText("Check-In: " + checkInDate.toString("yyyy-MM-dd"));
        ui->checkOutLabel->setText("Check-Out: Not selected");
    }
    //updateCheckInCheckOutLabels(); // Update labels to show selected dates
}

void BookRoomDialog::updateCheckInCheckOutLabels() {
    // Update labels to show selected dates
    //QDate selectedDate = ui->calendarWidget->selectedDate();
    //ui->checkInLabel->setText("Check-In: " + checkInDate.toString("yyyy-MM-dd"));
    //ui->checkOutLabel->setText("Check-Out: " + checkOutDate.toString("yyyy-MM-dd"));
}

// Handle the submit button click
void BookRoomDialog::on_submitButton_clicked() {
    // Retrieve input values from the UI
    int roomNumber = ui->roomComboBox->currentText().toInt();
    QString name = ui->nameLineEdit->text();
    QString phoneNumber = ui->phoneLineEdit->text();
    double price = ui->priceLineEdit->text().toDouble();
    QString paymentStatus = ui->paymentStatusComboBox->currentText();

    if (name.isEmpty() || phoneNumber.isEmpty() || checkInDate.isNull() || checkOutDate.isNull()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (checkInDate < QDate::currentDate()) {
        QMessageBox::warning(this, "Invalid Date", "Check-in date cannot be in the past.");
        return;
    }

    // Check if the room is available
    if (!isRoomAvailable(roomNumber)) {
        QMessageBox::warning(this, "Room Unavailable", "This room is already booked.");
        return;
    }
    int numDays = checkInDate.daysTo(checkOutDate);
    if (numDays <= 0) {
        QMessageBox::warning(this, "Date Error", "Check-out date must be after check-in date.");
        return;
    }

    price = price * numDays;

    // Insert the booking with customer info and possible discounts
    if (insertBooking(roomNumber, name, phoneNumber, price, paymentStatus)) {
        QMessageBox::information(this, "Success", "Room booked successfully.");
        this->accept();  // Close the dialog
    } else {
        QMessageBox::critical(this, "Error", "Failed to book the room.");
    }
}
void BookRoomDialog::on_cancelButton_clicked()
{
    this->reject(); // Close the dialog without saving
}
//[this]:lambda function captures the this pointer,giving it access to the member functions and variables of the class(onCalendarDateSelected)
//selectedDate: this method retrieves the currently selected date from the QCalendarWidget and Returns a QDate object,
//while selectionChanged is used to detect when the user changes their selection and does not return a value
//currentTextChanged in QComboBox, which provides the new text directly,
//selectionChanged simply informs that a change happened without providing the new date.
