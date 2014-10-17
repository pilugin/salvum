#include "workspacemodel.h"
#include <QtDebug>
#include <QDir>
#include <QFile>

static const char *s_bitmapFileName = "bitmap.data";
static const char *s_clustersDecodedFileName = "clusters";
static const char *s_imageFileName = "image.png";
static const char *s_blocksFileName = "image.blocks";

struct WorkspaceModel::Private
{
    struct HeadInfo
    {
        int cluster;
        QList<int> clusterList;
        int blocksDecoded;
        int blocksTotal;
        bool previewAvailable;
        QImage preview;
        QString path;
    };
    QList<HeadInfo> items;
    QString wspacePath;
    QString bitmapPath;
    
    // non-datamodel stuff
    QString imageProviderName;
    
    void processClusterDir(const QString &path, int clusterNo)
    {
        qDebug()<<"Process: "<<path<<clusterNo;
        
        HeadInfo hi;
        hi.cluster=clusterNo;
        hi.blocksDecoded = hi.blocksTotal = 0;
        hi.path = path;
        
        // PARSE clustersDecoded
        QFile cdf(path + "/" + s_clustersDecodedFileName);
        if (cdf.open(QFile::ReadOnly)) {
            while (!cdf.atEnd()) {
                QString line = cdf.readLine();
                QStringList range = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                bool ok = false;
                do {
                    if (range.size() == 2) {
                        int from = range[0].toInt(&ok, 16);
                        if (!ok)
                            break;
                        int to = range[1].toInt(&ok, 16);
                        if (!ok)
                            break;
                        for (; from<=to; ++from)
                            hi.clusterList.push_back(from);                                                    
                    
                    } else if (range.size() == 1) {
                        int cluster = range[0].toInt(&ok, 16);
                        if (!ok)
                            break;
                        hi.clusterList.push_back(cluster);
                    } 
                } while (false);
                if (!ok) 
                    qDebug()<<"ERR: WorkspaceModel::Private::processClusterDir. File: "<<cdf.fileName()
                        <<" failed to parse string: "<<line;
                    
            }
        }
        cdf.close();
        
        // CHECK IMAGE
        hi.previewAvailable = QFile::exists(path + "/" + s_imageFileName);
        
        // PARSE blocks decoded/total
        QFile bf(path + "/" + s_blocksFileName);
        if (bf.open(QFile::ReadOnly)) {
            QString line = bf.readAll();
            QStringList blocks = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            bool ok = false;
            do {
                if (blocks.size() == 2) {
                    hi.blocksDecoded = blocks[0].toInt(&ok, 16);
                    if (!ok)
                        break;
                    hi.blocksTotal = blocks[1].toInt(&ok, 16);                    
                }
            } while (false);
            if (!ok)
                qDebug()<<"ERR: WorkspaceModel::Private::processClusterDir. File: "<<bf.fileName()<< " failed to parse: "<<line;
        }
        bf.close();
        
        // END PARSE
        items.push_back(hi);
    }
    
    void loadPreview(int i)
    {
        if (items[i].previewAvailable && items[i].preview.isNull()) {
            items[i].preview.load( items[i].path + "/" + s_imageFileName);
        }
    }
};

WorkspaceModel::WorkspaceModel(QObject *parent)
: QAbstractListModel(parent)
, m_d(new Private)
{
#if QT_VERSION >= 0x050000
    setRoleNames(roleNames_internal());
#endif    
}

WorkspaceModel::~WorkspaceModel()
{
    delete m_d;
}

QString WorkspaceModel::getPathForDecoder(int clusterNo) const
{
    return QString("%1/%2").arg(m_d->wspacePath, QString().sprintf("%08X", clusterNo));
}

int WorkspaceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())   
        return 0;
        
    return m_d->items.size();
}

QModelIndex WorkspaceModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row <0 || row>=m_d->items.size() || column!=0 || parent.isValid())
        return QModelIndex();
        
    return createIndex(row, 0, row);
}

QModelIndex WorkspaceModel::indexByCluster(int clusterNo) const
{
    for (int i=0; i<m_d->items.size(); ++i)
        if (m_d->items[i].cluster == clusterNo)
            return createIndex(i, 0, i);
            
    return QModelIndex();
}

QVariant WorkspaceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    else switch (role) {
    case Role_Cluster:      return m_d->items[index.row()].cluster;    
//    case Role_ClusterList:  return m_d->items[index.row()].clusterList;
    case Role_ClustersDecoded: return m_d->items[index.row()].clusterList.size();
    case Role_Preview: {
        m_d->loadPreview(index.row());
        return QString("image://%1/%2/preview-%3").arg(m_d->imageProviderName).arg(name()).arg(index.row());
    }
    case Role_BlocksDecoded:return m_d->items[index.row()].blocksDecoded;
    case Role_BlocksTotal:  return m_d->items[index.row()].blocksTotal;
    default: 
        return QVariant();
    };
}
    
QString WorkspaceModel::bitmapPath() const
{
   return m_d->bitmapPath;
}
    
QImage WorkspaceModel::get(const QString &image) const
{
    return QImage();
}

void WorkspaceModel::createWspace(const QString &path, const QString &mediaPath)
{
    //create wspace
    
    //load it after
    loadWspace(path);
}

void WorkspaceModel::loadWspace(const QString &path)
{  
    beginResetModel();
    m_d->items.clear();
    QDir wspaceDir(path);
    
    do {
        if (!wspaceDir.exists()) {
            m_d->bitmapPath.clear();
            emit bitmapPathUpdated();
            QString err = QString("Dir doesn't exists. %1").arg(path);
            emit wspaceError(err);
            qDebug()<<"ERR: WorkspaceModel: "<<err;
            break;
        }
        
        if (wspaceDir.exists(s_bitmapFileName)) {
            m_d->bitmapPath = wspaceDir.absoluteFilePath(s_bitmapFileName);
            emit bitmapPathUpdated();
        } else {
            m_d->bitmapPath.clear();
            emit bitmapPathUpdated();
            QString err = QString("Bitmap file doesn't exists. %1/%2").arg(path).arg(s_bitmapFileName);
            qDebug()<<"ERR: WorkspaceModel: "<<err;
            emit wspaceError(err);
            break;            
        }
        
        for (QString clusterDir: wspaceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {            
            if (clusterDir == s_bitmapFileName)
                continue;
            
            bool ok;
            int clusterNo = clusterDir.toInt(&ok, 16);
            if (ok) {
                m_d->processClusterDir(wspaceDir.absoluteFilePath(clusterDir), clusterNo);
            
            } else {
                qDebug()<<"Skip dir "<<clusterDir;
            }
        }
    
    } while (false);
    emit wspaceReady();
    endResetModel();
}

void WorkspaceModel::reloadWspace()
{
    loadWspace(m_d->wspacePath);
}
    
    
QHash<int, QByteArray> WorkspaceModel::roleNames_internal() const
{
    QHash<int, QByteArray> r;
    r.insert(Role_Cluster, "cluster");
    r.insert(Role_ClusterList, "clusterList");
    r.insert(Role_ClustersDecoded, "clustersDecoded");
    r.insert(Role_Preview, "preview");
    r.insert(Role_BlocksDecoded, "blocksDecoded");
    r.insert(Role_BlocksTotal, "blocksTotal");
    return r;
}

void WorkspaceModel::setImageProviderName(const QString &name)
{
    m_d->imageProviderName = name;
    emit dataChanged(index(0), index(m_d->items.size()));
}
