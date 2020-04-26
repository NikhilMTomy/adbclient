#include "newfolderdialog.h"
#include "ui_newfolderdialog.h"

NewFolderDialog::NewFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolderDialog) {
    ui->setupUi(this);
    ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
}

bool NewFolderDialog::validate_folder_name(const QString &name) {
    QRegExp valid_chars = QRegExp("^(\\d*|\\w*)-?(\\d*|\\w*)$");
    if (name.isNull() || name.isEmpty()) {
        return false;
    }
    if (!name.contains(valid_chars)) {
        return false;
    }
    return true;
}

NewFolderDialog::~NewFolderDialog() {
    delete ui;
}

void NewFolderDialog::on_button_box_accepted() {
    this->new_folder_name = this->ui->line_edit_new_folder_name->text();
}

void NewFolderDialog::on_button_box_rejected() {

}

void NewFolderDialog::on_line_edit_new_folder_name_textChanged(const QString &name) {
    if (validate_folder_name(name)) {
        ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
