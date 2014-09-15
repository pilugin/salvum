#ifndef SELECTED_HEADS_MODEL_H
#define SELECTED_HEADS_MODEL_H

#include <QAbstractListModel>

class WorkspaceModel;

class SelectedHeadsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SelectedHeadsModel(QObject *parent =nullptr);
    ~SelectedHeadsModel();
       
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const { return roleNames_internal(); }
#endif    
    int rowCount(const QModelIndex &parent =QModelIndex()) const;
    QModelIndex index(int row, int column=0, const QModelIndex &parent =QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
       
    void setWorkspaceModel(WorkspaceModel *wspace);        
    
public slots:
    void onHeadSelected(int clusterNo, bool isSelected);
    void onGoodHeadSelected(int clusterNo, bool isSelected);
    void clear();
    
protected:   
    void appendHead(int clusterNo, bool isGood);
    void removeHead(int clusterNo);
 
    enum {
        Role_Cluster,
        Role_IsGood,
        Role_ClustersDecoded,
        Role_BlocksDecoded,
        Role_BlocksTotal
    };
    
    virtual QHash<int, QByteArray> roleNames_internal() const;
    
    class Private;
    Private *m_d;
};

#endif // SELECTED_HEADS_MODEL_H