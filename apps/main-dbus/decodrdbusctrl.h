#ifndef DECODRDBUSCTRL_H
#define DECODRDBUSCTRL_H

#include "org.salvum.DecodrCtrlAdp.h"

class DecodrDbusCtrl : public QObject
{
    Q_OBJECT
    
public:
    DecodrDbusCtrl(QObject *parent =nullptr);
    ~DecodrDbusCtrl();
    
public slots:    
    void sendStart(int clusterNo) 	{ emit start(clusterNo); }
    void sendResume()			{ emit resume(); }
    void sendBaseline(int clusterNo)	{ emit baseline(clusterNo); }
signals:
    void atEndRecv(bool complete, const DecodedClusters &decodedClusters, const Pixmap &pixmap);
    
private slots:
    void atEnd(bool complete, const DecodedClusters &decodedClusters, const Pixmap &pixmap)
					{ emit atEndRecv(complete, decodedClusters, pixmap); }
        
signals:    
    void start(int clusterNo);
    void resume();
    void baseline(int clusterNo);
};

#endif