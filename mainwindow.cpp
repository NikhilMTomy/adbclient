#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    table_view_directory_listings_model = new DirectoryContentModel();
    ui->table_view_directory_listings->setModel(table_view_directory_listings_model);

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
    connect( adb_functions, &ADBFunctions::finished_creating_directory, this, &MainWindow::adb_functions_finished_creating_directory);
    connect( adb_functions, &ADBFunctions::finished_deleting_directory_items, this, &MainWindow::adb_functions_finished_deleting_directory_items);

    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &QThread::quit );
    connect( adb_functions, &ADBFunctions::finished, adb_functions_thread, &ADBFunctions::deleteLater );
    connect( adb_functions_thread, &QThread::finished, adb_functions_thread, &QThread::deleteLater );

}
MainWindow::~MainWindow() {
    delete ui;
    adb_functions_thread->quit();
    adb_functions_thread->wait();
}

void MainWindow::populate_table_view() {
    disable_ui();
    adb_functions->get_directory_contents(this->current_directory);
}
void MainWindow::enable_ui() {
    ui->button_delete->setEnabled(true);
    ui->button_download->setEnabled(true);
    ui->button_upload->setEnabled(true);
    ui->button_refresh->setEnabled(true);
    ui->button_add_new_folder->setEnabled(true);
    ui->combo_box_storage_select->setEnabled(true);
    ui->table_view_directory_listings->setEnabled(true);
    if (this->current_directory.length() > 1)
        ui->button_back->setEnabled(true);
}
void MainWindow::disable_ui() {
    ui->button_delete->setEnabled(false);
    ui->button_download->setEnabled(false);
    ui->button_upload->setEnabled(false);
    ui->button_refresh->setEnabled(false);
    ui->button_add_new_folder->setEnabled(false);
    ui->combo_box_storage_select->setEnabled(false);
    ui->table_view_directory_listings->setEnabled(false);
    ui->button_back->setEnabled(false);
}
void MainWindow::set_connection_established(const bool &connected) {
    this->connection_established = true;
    if (connected) {
        enable_ui();
    } else {
        disable_ui();
    }
}
bool MainWindow::get_connection_established() {
    return this->connection_established;
}

void MainWindow::adb_functions_finished_deleting_directory_items(const int &status) {
    QMessageBox msg_box(this);
    msg_box.setModal(true);
    if (status == -1) {
        msg_box.setWindowTitle("Error");
        msg_box.setText("Error");
        msg_box.setInformativeText("Failed to perform delete operation.");
    } else {
        msg_box.setWindowTitle("Success");
        msg_box.setText("Success");
        msg_box.setInformativeText("Successfully performed delete operation.");
    }
    msg_box.exec();
    this->populate_table_view();
}
void MainWindow::adb_functions_finished_creating_directory(const int &status) {
    this->populate_table_view();
}
void MainWindow::adb_functions_finished_getting_directory_content(const QList<DataStructures::DirectoryItem> &directory_items) {
    table_view_directory_listings_model->remove_all_directory_items();
    table_view_directory_listings_model->addDirectoryItems(directory_items);
    enable_ui();
}
void MainWindow::adb_functions_finished_connecting_to_device(const int &flag) {
    enable_ui();
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
    enable_ui();
    if (flag == 0) {
        qDebug() << "Successfully connected to wireless device";
        set_connection_established(true);
        this->ui->combo_box_storage_select->addItems(adb_functions->get_storage_directories());
    } else if (flag == -1) {
        qDebug() << "Failed to connect to wireless device.";
    } else {
        qDebug() << "Flag : " << flag;
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
        disable_ui();
        this->adb_functions->set_device_info(device_info);
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
        disable_ui();
        this->adb_functions->set_wireless_device_info(wireless_device_info);
        this->adb_functions->connect_to_wireless_device();
    }
}
void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::on_combo_box_storage_select_currentTextChanged(const QString &text) {
    qDebug() << "Combo box selection changed";
    adb_functions->set_root_directory(text);
    this->current_directory = "/";
    this->populate_table_view();
}

void MainWindow::on_table_view_directory_listings_doubleClicked(const QModelIndex &index) {
    QItemSelectionModel *selection_model = this->ui->table_view_directory_listings->selectionModel();
    if (selection_model->hasSelection()) {
        QModelIndex selected_row = selection_model->selectedRows()[0];
        DataStructures::DirectoryItem selected_directory_item = this->table_view_directory_listings_model->getItem(selected_row);
        if (selected_directory_item.get_type() == DataStructures::DirectoryItemType::Directory) {
            this->current_directory += selected_directory_item.get_name() + QString("/");
            this->populate_table_view();
        } else {
            // Not a directory. Do nothing ???
        }
    }
}

void MainWindow::on_button_back_clicked() {
    this->current_directory = this->current_directory.replace(QRegExp("\\w+/$"), "");
    this->populate_table_view();
}

void MainWindow::on_button_add_new_folder_clicked() {
    NewFolderDialog new_folder_dialog(this);
    new_folder_dialog.setModal(true);
    new_folder_dialog.setWindowTitle("Folder name");

    if (new_folder_dialog.exec() == QDialog::Accepted) {
        QString directory_name = new_folder_dialog.new_folder_name;
        qDebug() << directory_name;
        adb_functions->create_directory(directory_name, this->current_directory);
    }
}

void MainWindow::on_button_delete_clicked() {
    QMessageBox msg_box(this);
    msg_box.setWindowTitle("Delete items");
    msg_box.setText("Delete items");
    QItemSelectionModel *selection_model = this->ui->table_view_directory_listings->selectionModel();
    QList<DataStructures::DirectoryItem> directory_items_to_delete;
    QList<QModelIndex> selected_rows = selection_model->selectedRows();
    if (selected_rows.length() > 0) {
        msg_box.setInformativeText("Are you sure you want to delete the selected files/directories?");
        msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg_box.setDefaultButton(QMessageBox::No);
        if (msg_box.exec() == QMessageBox::Yes) {
            for (const QModelIndex &selected_row: selected_rows) {
                directory_items_to_delete.append(this->table_view_directory_listings_model->getItem(selected_row));
            }
            disable_ui();
            adb_functions->delete_directory_items(directory_items_to_delete);
        }
    } else {
        msg_box.setInformativeText("Please select atleast one file or directory");
        msg_box.exec();
    }
}

void MainWindow::on_button_upload_clicked() {

}

void MainWindow::on_button_download_clicked() {

}

void MainWindow::on_button_refresh_clicked() {
    this->populate_table_view();
}
