#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include <QString>
#include <QMetaType>

namespace DataStructures {
    enum DirectoryItemType {File, Directory};
    class DirectoryItem
    {
    public:
        DirectoryItem();
        DirectoryItem(const QString &parent_path, const QString &name, const DataStructures::DirectoryItemType &type, const double &size);
        QString get_name() const;
        QString get_parent_path() const;
        double get_size() const;
        DataStructures::DirectoryItemType get_type() const;
    private:
        DirectoryItemType type;
        QString name;
        QString parent_path;
        double size;
    };
}

Q_DECLARE_METATYPE(DataStructures::DirectoryItem);

#endif // DIRECTORYITEM_H
