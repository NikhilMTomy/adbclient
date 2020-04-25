#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    table_view_directory_listings_model = new DirectoryContentModel();
    ui->table_view_directory_listings->setModel(table_view_directory_listings_model);

    table_view_directory_listings_model->setHeaderData(0, Qt::Horizontal, "Name");
    table_view_directory_listings_model->setHeaderData(1, Qt::Horizontal, "Size");
    ui->table_view_directory_listings->verticalHeader()->hide();
    ui->table_view_directory_listings->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->table_view_directory_listings->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->table_view_directory_listings->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);
    QFont table_header_font = ui->table_view_directory_listings->horizontalHeader()->font();
    table_header_font.setBold(true);
    ui->table_view_directory_listings->horizontalHeader()->setFont(table_header_font);

    ui->table_view_directory_listings->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_view_directory_listings->setFocusPolicy(Qt::NoFocus);

    set_connection_established(false);
    current_directory = "";

    adb_functions = new ADBFunctions;
    adb_functions_thread = new QThread;
    adb_functions->moveToThread(adb_functions_thread);
    adb_functions_thread->start();

    connect( adb_functions, &ADBFunctions::finished_connecting_to_device, this, &MainWindow::adb_functions_finished_connecting_to_device);
    connect( adb_functions, &ADBFunctions::finished_connecting_to_wireless_device, this, &MainWindow::adb_functions_finished_connecting_to_wireless_device);
    connect( adb_functions, &ADBFunctions::finished_getting_directory_content, this, &MainWindow::adb_functions_finished_getting_directory_content);

    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &QThread::quit );
    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &ADBFunctions::deleteLater );
    connect( adb_functions_thread, &QThread::finished, adb_functions_thread, &QThread::deleteLater );

}
MainWindow::~MainWindow() {
    delete ui;
    adb_functions_thread->quit();
    adb_functions_thread->wait();
}

void MainWindow::set_connection_established(const bool &connected) {
    this->connection_established = true;
    ui->button_delete->setEnabled(connected);
    ui->button_download->setEnabled(connected);
    ui->button_upload->setEnabled(connected);
    ui->button_refresh->setEnabled(connected);
    ui->button_add_new_folder->setEnabled(connected);
    ui->combo_box_storage_select->setEnabled(connected);
    ui->table_view_directory_listings->setEnabled(connected);
    if (!connected)
        ui->button_back->setEnabled(false);
}
bool MainWindow::get_connection_established() {
    return this->connection_established;
};

void MainWindow::adb_functions_finished_getting_directory_content(const QList<DataStructures::DirectoryItem> &directory_items) {
    table_view_directory_listings_model->remove_all_directory_items();
    table_view_directory_listings_model->addDirectoryItems(directory_items);
    if (this->current_directory.length() > 1)
        ui->button_back->setEnabled(true);
}
void MainWindow::adb_functions_finished_connecting_to_device(const int &flag) {
    if (flag != -1) {
        //qDebug() << "Successfully connected to device";
        this->ui->combo_box_storage_select->addItems(adb_functions->get_storage_directories());
        set_connection_established(true);
    } else {
        //qDebug() << "Flag : " << flag;
        this->ui->combo_box_storage_select->clear();
        set_connection_established(false);
    }
}
void MainWindow::adb_functions_finished_connecting_to_wireless_device(const int &flag) {
    if (flag == 0) {
        //qDebug() << "Successfully connected to wireless device";
        set_connection_established(true);
        this->ui->combo_box_storage_select->addItems(adb_functions->get_storage_directories());
    } else if (flag == -1) {
        //qDebug() << "Failed to connect to wireless device.";
    } else {
        //qDebug() << "Flag : " << flag;
        this->ui->combo_box_storage_select->clear();
        set_connection_established(false);
    }
}

void MainWindow::on_actionConnect_triggered() {
    ConnectDialog connect_dialog(this);
    connect_dialog.setModal(true);
    connect_dialog.setWindowTitle("Connect to Android device");

    DataStructures::DeviceInfo device_info;

    if (connect_dialog.exec() == QDialog::Accepted) {
        device_info = connect_dialog.selected_device;
        this->adb_functions->set_device_info(device_info);
        //qDebug() << "DeviceInfo";
        //qDebug() << device_info.device_name;
        //qDebug() << device_info.device_serial;
        this->adb_functions->connect_to_device();
    }
}
void MainWindow::on_actionConnect_to_ADB_Wireless_triggered() {
    WirelessConnectDialog wireless_connect_dialog(this);
    wireless_connect_dialog.setModal(true);
    wireless_connect_dialog.setWindowTitle("Connect to ADB Wireless");

    DataStructures::WirelessDeviceInfo wireless_device_info;

    if (wireless_connect_dialog.exec() == QDialog::Accepted) {
        wireless_device_info = wireless_connect_dialog.get_wireless_device_info();
        this->adb_functions->set_wireless_device_info(wireless_device_info);
        this->adb_functions->connect_to_wireless_device();
        adb_functions->get_directory_contents("");
    }
}
void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::on_combo_box_storage_select_currentTextChanged(const QString &text) {
    adb_functions->set_root_directory(text);
    this->current_directory = "/";
    adb_functions->get_directory_contents(this->current_directory);
}

void MainWindow::on_table_view_directory_listings_doubleClicked(const QModelIndex &index) {
    QItemSelectionModel *select = this->ui->table_view_directory_listings->selectionModel();
    if (select->hasSelection()) {
        QModelIndex selectedRow = select->selectedRows()[0];
        DataStructures::DirectoryItem selected_directory_item = this->table_view_directory_listings_model->getItem(selectedRow);
        if (selected_directory_item.get_type() == DataStructures::DirectoryItemType::Directory) {
           // qDebug() << "Directory selected";
            this->current_directory += selected_directory_item.get_name() + QString("/");
            adb_functions->get_directory_contents(this->current_directory);
        } else {
            // Not a directory. Do nothing ???
            //qDebug() << "File selected";
        }
    }
}

void MainWindow::on_button_back_clicked() {
    this->current_directory = this->current_directory.replace(QRegExp("\\w+/$"), "");
    adb_functions->get_directory_contents(this->current_directory);
}

void MainWindow::on_button_add_new_folder_clicked() {

}

void MainWindow::on_button_delete_clicked() {

}

void MainWindow::on_button_upload_clicked() {

}

void MainWindow::on_button_download_clicked() {

}

void MainWindow::on_button_refresh_clicked() {

}
