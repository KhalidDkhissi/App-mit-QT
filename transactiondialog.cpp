#include "transactiondialog.h"
#include "ui_transactiondialog.h"
#include <QSqlError>
#include <QMessageBox>
//#include<QVBoxLayout>

TransactionDialog::TransactionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDialog),
    model(new QSqlQueryModel(this))//is Qt class that provides an interface to an SQL database table,means an editable data model for a single table.
{
    ui->setupUi(this);
    ui->tableView->setStyleSheet("QTableView {" "background-color: white;" "color:black;" "font-size:14px;" "selection-color: red;" "}"
                                 "QHeaderView::section {" "background-color: white;" "color:blue;" "font-size:16px" "}");

    this->setWindowTitle("Booking Transaction");
    // Connect to the database
    //db = QSqlDatabase::database(); // Use the Singleton database connection
    //if (!db.isOpen()) {
    //    QMessageBox::critical(this, "Database Connection Error", "Unable to open database.");
    //    return;
    //}

    setupTableModel();
    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(ui->tableView, 0, Qt::AlignCenter);
    //setLayout(layout);
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
    delete model;
}

void TransactionDialog::setupTableModel()
{
    // Create a custom SQL query to join the Bookings and Customers tables
    QString queryStr = R"(
    SELECT B.BookingID, B.RoomNumber, C.Name, B.Price, B.BookingDate , B.CheckInDate, B.CheckOutDate, C.TotalBookings, B.PaymentStatus,
               CASE
                   WHEN C.TotalBookings = 2 THEN '15% Discount'
                   WHEN C.TotalBookings = 3 THEN 'Free Breakfast & Lunch'
                   WHEN C.TotalBookings >= 4 THEN 'Free Night'
                   ELSE 'No Discount'
               END AS Discount
        FROM Bookings B
        JOIN Customers C ON B.CustomerID = C.CustomerID
    )";

    // Create a QSqlQueryModel for custom queries
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // Execute the query and set the results to the model
    model->setQuery(queryStr);

    // Check for errors in the query
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Database Error", model->lastError().text());
        return;
    }


    // Set the columns to be displayed
    //model->setHeaderData(0, Qt::Horizontal, tr("BookingID"));  //Assuming BookingID is column 0

    model->setHeaderData(0, Qt::Horizontal, "BookingID");
    model->setHeaderData(1, Qt::Horizontal, tr("Room Number"));
    model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    model->setHeaderData(3, Qt::Horizontal, tr("Price"));
    model->setHeaderData(4, Qt::Horizontal, tr("BookingDate"));
    model->setHeaderData(5, Qt::Horizontal, tr("Check-In Date"));  // New Check-In Date column
    model->setHeaderData(6, Qt::Horizontal, tr("Check-Out Date")); // New Check-Out Date column
    model->setHeaderData(7, Qt::Horizontal, tr("Total Bookings"));
    model->setHeaderData(8, Qt::Horizontal, tr("Payment Status"));
    model->setHeaderData(9, Qt::Horizontal, tr("Discount"));

    //This line links the QTableView (which is part of the UI) with the QSqlTableModel.It tells the view to use the data provided by the model for display
    //setModel: This is a method of the QTableView class. It is used to set the data model that the view will use to display data
    //
    ui->tableView->setModel(model);

    // Optional: Resize columns to fit the content
    ui->tableView->resizeColumnsToContents();
}
