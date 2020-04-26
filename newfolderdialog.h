#ifndef NEWFOLDERDIALOG_H
#define NEWFOLDERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDebug>

namespace Ui {
class NewFolderDialog;
}

class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolderDialog(QWidget *parent = nullptr);
    ~NewFolderDialog();
    QString new_folder_name;

private slots:
    void on_button_box_accepted();
    void on_button_box_rejected();
    void on_line_edit_new_folder_name_textChanged(const QString &arg1);

private:
    Ui::NewFolderDialog *ui;
    bool validate_folder_name(const QString &);
};

#endif // NEWFOLDERDIALOG_H
