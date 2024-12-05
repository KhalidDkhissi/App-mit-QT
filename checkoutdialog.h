#ifndef CHECKOUTDIALOG_H
#define CHECKOUTDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class CheckOutDialog;
}

class CheckOutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckOutDialog(QWidget *parent = nullptr);
    ~CheckOutDialog();

private slots:
    void on_checkOutButton_clicked();
    void on_cancelButton_clicked();

private:
    bool isRoomBooked(int roomNumber);
    bool checkOutRoom(int roomNumber);
    Ui::CheckOutDialog *ui;
};

#endif // CHECKOUTDIALOG_H
