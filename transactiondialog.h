#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class TransactionDialog;
}

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    ~TransactionDialog();

private:
    Ui::TransactionDialog *ui;
    QSqlQueryModel *model; // Use QSqlTableModel to interface with the SQL database
    void setupTableModel();
};

#endif // TRANSACTIONDIALOG_H
