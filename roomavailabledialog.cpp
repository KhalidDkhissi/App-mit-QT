#include "roomavailabledialog.h"
#include "ui_roomavailabledialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RoomAvailableDialog::RoomAvailableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomAvailableDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Available rooms");
    ui->groupBox->setStyleSheet("QGroupBox{" "border: 4px solid yellow;" "font-size: 14px;" "}"
                                "QGroupBox::title{""border:2px solid red;" "subcontrol-position:top center;" "color:blue;""}");
    ui->groupBox->setAlignment(Qt::AlignCenter);
    // Map room numbers to their corresponding QLabel in the UI
    roomLabels[101] = ui->labelRoom101;
    roomLabels[102] = ui->labelRoom102;
    roomLabels[103] = ui->labelRoom103;
    roomLabels[104] = ui->labelRoom104;
    roomLabels[105] = ui->labelRoom105;
    roomLabels[201] = ui->labelRoom201;
    roomLabels[202] = ui->labelRoom202;
    roomLabels[203] = ui->labelRoom203;
    roomLabels[204] = ui->labelRoom204;
    roomLabels[205] = ui->labelRoom205;

    // Call the function to update the room status on dialog creation
    updateRoomStatus();
}

RoomAvailableDialog::~RoomAvailableDialog()
{
    delete ui;
}

void RoomAvailableDialog::updateRoomStatus()
{
    // Iterate through each room and update its status
    for (auto it = roomLabels.begin(); it != roomLabels.end(); ++it) {
        int roomNumber = it.key();//Retrieves the key of the current QMap entry. In this case, it retrieves the room number (an int).
        QLabel *roomLabel = it.value();//Retrieves the value associated with the key. Here, it retrieves a pointer to a QLabel.

        if (isRoomBooked(roomNumber)) {
            roomLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
        } else {
            roomLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
        }
    }
}

bool RoomAvailableDialog::isRoomBooked(int roomNumber)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Bookings WHERE RoomNumber = ?");
    query.addBindValue(roomNumber);

    if (!query.exec()) {
        qDebug() << "Error checking room status:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);  // Room is booked if count > 0
    }

    return false;
}
