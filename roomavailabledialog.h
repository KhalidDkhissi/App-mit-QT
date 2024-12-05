#ifndef ROOMAVAILABLEDIALOG_H
#define ROOMAVAILABLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class RoomAvailableDialog;
}

class RoomAvailableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomAvailableDialog(QWidget *parent = nullptr);
    ~RoomAvailableDialog();

    void updateRoomStatus();  // Function to update room status

private:
    Ui::RoomAvailableDialog *ui;
    QMap<int, QLabel*> roomLabels;  // Map room numbers to their corresponding QLabel, QMap is a Qt container class that stores key-value pairs
    bool isRoomBooked(int roomNumber);  // Function to check if a room is booked
};

#endif // ROOMAVAILABLEDIALOG_H
