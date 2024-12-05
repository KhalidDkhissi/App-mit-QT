#ifndef BOOKROOMDIALOG_H
#define BOOKROOMDIALOG_H

#include <QDialog>
#include <QSqlDatabase> // Include for database support
#include <QDate>
namespace Ui {
class BookRoomDialog;
}

class BookRoomDialog : public QDialog
{
    Q_OBJECT //Q_OBJECT is macro required for signals and slots to function

public:
    explicit BookRoomDialog(QWidget *parent = nullptr);//keyword used to prevent implicit conversions or unintended type conversions when constructing objects.
    ~BookRoomDialog();

private slots:
    void on_submitButton_clicked();
    void on_cancelButton_clicked();
    void onCalendarDateSelected(const QDate &date);

private:
    Ui::BookRoomDialog *ui;
    void onPaymentMethodChanged(const QString &paymentMethod);
    QDate checkInDate;
    QDate checkOutDate;
    int getCustomerID(const QString &name, const QString &phoneNumber);
    bool addNewCustomer(const QString &name, const QString &phoneNumber);
    bool updateCustomerBookings(int customerID);
    int getTotalBookings(int customerID);
    bool insertBooking(int roomNumber, const QString &name, const QString &phoneNumber, double price, const QString paymentStatus);
    bool isRoomAvailable(int roomNumber);
    void resetTotalBookings(int customerID);
    void updateCheckInCheckOutLabels();
};

#endif // BOOKROOMDIALOG_H
