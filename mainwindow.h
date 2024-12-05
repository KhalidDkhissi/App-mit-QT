#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookroomdialog.h"
#include "checkoutdialog.h"
#include "roomavailabledialog.h"
#include "transactiondialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    BookRoomDialog * ptrRoomBookingDlg;
    CheckOutDialog * ptrCheckOutDlg;
    RoomAvailableDialog * ptrRoomAvailableDlg;
    TransactionDialog * ptrTransaction;
    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToDatabase();

private slots:
    void on_btnRoomBooking_clicked();
    void on_btnRoomCheckout_clicked();
    void on_btnCheckAvailability_clicked();
    void on_bntTransaction_clicked();
};

#endif // MAINWINDOW_H
