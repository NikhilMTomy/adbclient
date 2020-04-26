#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include <QString>
#include <QList>
#include <QtAlgorithms>
#include <QMetaType>

namespace DataStructures {
    enum DirectoryItemType {Directory, File};
    class DirectoryItem
    {
    public:
        static const int SORT_ASCENDING = 0;
        static const int SORT_DESCENDING = 1;
        DirectoryItem();
        DirectoryItem(const QString &parent_path, const QString &name, const DataStructures::DirectoryItemType &type, const double &size);
        QString get_name() const;
        QString get_parent_path() const;
        double get_size() const;
        DataStructures::DirectoryItemType get_type() const;

        static QList<DirectoryItem> sort_directory_items(QList<DirectoryItem> directory_items, bool order=SORT_ASCENDING);
        static bool directory_item_comparison(const DirectoryItem &item_1, const DirectoryItem &item_2);

    private:
        DirectoryItemType type;
        QString name;
        QString parent_path;
        double size;
    };
}

Q_DECLARE_METATYPE(DataStructures::DirectoryItem);

#endif // DIRECTORYITEM_H
