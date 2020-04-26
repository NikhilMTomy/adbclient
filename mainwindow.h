#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include <adbfunctions.h>
#include <connectdialog.h>
#include <wirelessconnectdialog.h>
#include <directoryitem.h>
#include <directorycontentmodel.h>
#include <newfolderdialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_connection_established(const bool &);
    bool get_connection_established();

private:
    Ui::MainWindow *ui;
    ADBFunctions *adb_functions;
    QThread *adb_functions_thread;

    QString current_directory;
    bool connection_established;
    DirectoryContentModel *table_view_directory_listings_model;
    void populate_table_view();
    void disable_ui();
    void enable_ui();

private slots:
    void adb_functions_finished_creating_directory(const int &status);
    void adb_functions_finished_connecting_to_device(const int &flag);
    void adb_functions_finished_connecting_to_wireless_device(const int &flag);
    void adb_functions_finished_getting_directory_content(const QList<DataStructures::DirectoryItem> &directory_items);
    void adb_functions_finished_deleting_directory_items(const int &flag);

    void on_actionConnect_triggered();
    void on_actionExit_triggered();
    void on_actionConnect_to_ADB_Wireless_triggered();
    void on_combo_box_storage_select_currentTextChanged(const QString &text);
    void on_table_view_directory_listings_doubleClicked(const QModelIndex &index);
    void on_button_back_clicked();
    void on_button_add_new_folder_clicked();
    void on_button_delete_clicked();
    void on_button_upload_clicked();
    void on_button_download_clicked();
    void on_button_refresh_clicked();
};
#endif // MAINWINDOW_H
