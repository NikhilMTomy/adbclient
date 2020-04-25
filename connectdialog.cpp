#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog) {
    ui->setupUi(this);
    this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->model_list_view_attached_devices = new QStringListModel();
    ui->list_view_attached_devices->setModel(this->model_list_view_attached_devices);
    adb_functions = new ADBFunctions;
    adb_functions_thread = new QThread;
    adb_functions->moveToThread(adb_functions_thread);

    connect(
        ui->list_view_attached_devices->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(list_view_item_selection_changed())
    );
    connect(
        adb_functions, &ADBFunctions::finished_getting_connected_devices,
        this, &ConnectDialog::finished_getting_connected_devices
    );
    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &QThread::quit );
    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &ADBFunctions::deleteLater);
    connect( adb_functions_thread, &QThread::finished, adb_functions_thread, &QThread::deleteLater );

    adb_functions_thread->start();

    adb_functions->get_connected_devices();
}
ConnectDialog::~ConnectDialog() {
    delete ui;
}

void ConnectDialog::finished_getting_connected_devices(const QList<DataStructures::DeviceInfo> &connected_devices) {
    this->populate_attached_devices(connected_devices);
}
void ConnectDialog::populate_attached_devices(const QList<DataStructures::DeviceInfo> &connected_devices) {
    this->model_list_view_attached_devices->setStringList(QStringList{});
    int i=0;
    for (DataStructures::DeviceInfo device_info : connected_devices) {
        if(this->model_list_view_attached_devices->insertRow(i)) {
            QModelIndex index = this->model_list_view_attached_devices->index(i, 0);
            this->model_list_view_attached_devices->setData(index, device_info.toString());
        }
    }
}

void ConnectDialog::on_button_box_accepted() {
    QList<QModelIndex> selected_indices = ui->list_view_attached_devices->selectionModel()->selectedIndexes();
    if (selected_indices.count() != 1) {
        QMessageBox msg_box;
        msg_box.setWindowTitle("Error");
        msg_box.setText("Select atleast one device.");
        msg_box.exec();
    }

    QModelIndex selected_index = selected_indices[0];
    if (selected_index.isValid() && selected_index.row() < this->model_list_view_attached_devices->stringList().size()) {
        QString selected_device_string = this->model_list_view_attached_devices->data(selected_index).toString();
        QStringList selected_device_string_list = selected_device_string.split("\t");
        this->selected_device.device_serial = selected_device_string_list[0];
        this->selected_device.device_name = selected_device_string_list[1];
    }
}
void ConnectDialog::list_view_item_selection_changed() {
    QList<QModelIndex> selected_indices = ui->list_view_attached_devices->selectionModel()->selectedIndexes();
    if (selected_indices.count() != 0 ) {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
