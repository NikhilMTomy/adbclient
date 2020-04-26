#ifndef DIRECTORYCONTENTMODEL_H
#define DIRECTORYCONTENTMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <QIcon>
#include <QStyle>
#include <QApplication>

#include <directoryitem.h>

class DirectoryContentModel : public QAbstractTableModel {
    Q_OBJECT
public:
    DirectoryContentModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    bool addDirectoryItem(const DataStructures::DirectoryItem &directory_item);
    bool addDirectoryItems(const QList<DataStructures::DirectoryItem> &directory_items);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    //QModelIndex parent(const QModelIndex &parent) const override;
    DataStructures::DirectoryItem getItem(const QModelIndex &index) const;
    bool remove_all_directory_items();
private:
    QList<QList<QString>> m_grid_data;
    QList<DataStructures::DirectoryItem> directory_items;
    QStringList m_grid_header;
    QModelIndex root_index;
};

#endif // DIRECTORYCONTENTMODEL_H
