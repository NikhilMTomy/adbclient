#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QMessageBox>
#include <QPushButton>
#include <QItemSelection>
#include <QThread>

#include <adbfunctions.h>
#include <deviceinfo.h>

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();
    DataStructures::DeviceInfo selected_device;

private slots:
    void on_button_box_accepted();
    void list_view_item_selection_changed();
    void finished_getting_connected_devices(const QList<DataStructures::DeviceInfo> &connected_devices);

private:
    Ui::ConnectDialog *ui;
    QStringListModel *model_list_view_attached_devices;
    void populate_attached_devices(const QList<DataStructures::DeviceInfo> &connected_devices);
    ADBFunctions *adb_functions;
    QThread *adb_functions_thread;
};

#endif // CONNECTDIALOG_H
