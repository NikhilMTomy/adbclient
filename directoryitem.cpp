#include "directoryitem.h"

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
