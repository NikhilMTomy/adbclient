#include "directoryitem.h"

bool DataStructures::DirectoryItem::directory_item_comparison(const DataStructures::DirectoryItem &item_1, const DataStructures::DirectoryItem &item_2) {
    if (item_1.get_type() < item_2.get_type()) {
        return true;
    } else if (item_1.get_type() > item_2.get_type()) {
        return false;
    } else {
        if (item_1.get_name().compare(item_2.get_name(), Qt::CaseInsensitive) < 0) {
            return true;
        } else {
            return false;
        }
    }
}
QList<DataStructures::DirectoryItem> DataStructures::DirectoryItem::sort_directory_items(QList<DataStructures::DirectoryItem> directory_items, bool order) {
    std::sort(directory_items.begin(), directory_items.end(), DataStructures::DirectoryItem::directory_item_comparison);
    return directory_items;
}

DataStructures::DirectoryItem::DirectoryItem() {

}
DataStructures::DirectoryItem::DirectoryItem(const QString &parent_path, const QString &name, const DirectoryItemType &type, const double &size) {
    this->parent_path = parent_path;
    this->name = name;
    this->type = type;
    this->size = size;
}
QString DataStructures::DirectoryItem::get_name() const {
    return this->name;
}
QString DataStructures::DirectoryItem::get_parent_path() const {
    return this->parent_path;
}
double DataStructures::DirectoryItem::get_size() const {
    return this->size;
}
DataStructures::DirectoryItemType DataStructures::DirectoryItem::get_type() const {
    return this->type;
}
