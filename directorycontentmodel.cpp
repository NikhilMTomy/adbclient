#include "directorycontentmodel.h"

DirectoryContentModel::DirectoryContentModel(QObject *parent) :
    QAbstractTableModel(parent) {
    m_grid_header = QStringList() << "Name" << "Size";
}
int DirectoryContentModel::rowCount(const QModelIndex &parent) const {
    return directory_items.length();
}
int DirectoryContentModel::columnCount(const QModelIndex &parent) const {
    return m_grid_header.length();
}
bool DirectoryContentModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count -1);
    for (int i=row; i<row+count; i++)
        directory_items.append(DataStructures::DirectoryItem());
    endInsertRows();
    return true;
}
bool DirectoryContentModel::insertRow(int row, const QModelIndex &parent) {
    insertRows(row, 1, parent);
    return 0;
}
bool DirectoryContentModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row+count-1);
    for (int i=0; i<count; i++)
        directory_items.removeAt(row);
    endRemoveRows();
    return true;
}
bool DirectoryContentModel::removeRow(int row, const QModelIndex &parent) {
    removeRows(row, 1, parent);
    return 0;
}
DataStructures::DirectoryItem DirectoryContentModel::getItem(const QModelIndex &index) const {
    int row = index.row();
    return directory_items[row];
}
QVariant DirectoryContentModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();
    if (!checkIndex(index))
        return QVariant();
    switch(role) {
    case Qt::DisplayRole:
        if (col == 0) {
            // name
            return QString(directory_items[row].get_name());
        } else {
            // size
            QString string_size;
            double size = directory_items[row].get_size();
            int divide_index = 0;
            while (size > 1024) {
                size /= 1024;
                ++divide_index;
            }
            string_size = QString::number(size, 'f', 2);
            switch(divide_index) {
            case 0:
                string_size += QString(" B");
                break;
            case 1:
                string_size += QString(" KB");
                break;
            case 2:
                string_size += QString(" MB");
                break;
            case 3:
                string_size += QString(" GB");
                break;
            }
            return string_size;
        }

    case Qt::FontRole:
        //if (row == 0) {
        //    QFont bold_font;
        //    bold_font.setBold(true);
        //    return bold_font;
        //}
        break;
    case Qt::BackgroundRole:
        //if (row %2 == 0) {
        //    return QBrush(Qt::lightGray);
        //}
        break;
    case Qt::TextAlignmentRole:
        if (col == 0)
            return Qt::AlignLeft + Qt::AlignVCenter;
        else
            return Qt::AlignRight + Qt::AlignVCenter;
    }
    return QVariant();
}
bool DirectoryContentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    int row = index.row();
    int col = index.column();
    DataStructures::DirectoryItem directory_item = qvariant_cast<DataStructures::DirectoryItem>(value);
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;
        if (row == rowCount()) {
            directory_items.append(directory_item);
        }
        directory_items.replace(row, directory_item);
        return true;
    }
    return false;
}
QVariant DirectoryContentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section >=0 && section < m_grid_header.length()) {
            return m_grid_header[section];
        }
    }
    return QVariant();
}
bool DirectoryContentModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section >=0 && section < m_grid_header.length()+1) {
            m_grid_header[section] = value.toString();
            return true;
        }
    }
    return false;
}
bool DirectoryContentModel::addDirectoryItem(const DataStructures::DirectoryItem &directory_item) {
    int row = rowCount(QModelIndex());
    insertRow(row);
    QModelIndex model_index = index(row, 0);
    setData(model_index, directory_item.get_name());
    model_index = index(row, 1);
    setData(model_index, directory_item.get_size());
    return true;
}
bool DirectoryContentModel::addDirectoryItems(const QList<DataStructures::DirectoryItem> &directory_items) {
    int row = rowCount(QModelIndex());
    insertRows(row, directory_items.count());
    QModelIndex model_index;
    for (const DataStructures::DirectoryItem &directory_item : directory_items) {
        model_index = index(row++, 0);
        setData(model_index, QVariant::fromValue<DataStructures::DirectoryItem>(directory_item));
    }
    return true;
}
bool DirectoryContentModel::remove_all_directory_items() {
    removeRows(0, rowCount(), QModelIndex());
    this->directory_items.clear();
    return true;
}
