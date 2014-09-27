#ifndef JPEG_HEADS_MODEL_H
#define JPEG_HEADS_MODEL_H

#include <QAbstractListModel>

class JpegHeadsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    JpegHeadsModel(QObject *parent =nullptr);
    ~JpegHeadsModel();
    
    QModelIndex index(int row, int column =0, const QModelIndex &parent =QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;        
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); } 
#endif    
    
public slots:
    void setHeads(const QList<int> &list);
    void toggleSelected(int cluster);
    
signals:
    void headSelected(int clusterNo, bool isSelected);    
    
private:
    static QHash<int, QByteArray> roleNames_internal();

    class Private;
    Private *m_d;    
};

#endif // JPEG_HEADS_MODEL_H