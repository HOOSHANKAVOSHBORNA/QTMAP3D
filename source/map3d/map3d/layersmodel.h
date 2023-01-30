
#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <QList>
#include <osgEarth/Layer>

class LayersModel : public QAbstractListModel
{
    Q_OBJECT
public:
	LayersModel(QObject *parent = nullptr);

    enum {
        LayerIndexRole = Qt::UserRole + 300,
        LayerEnabledRole = Qt::UserRole + 301
    };

public:
    void updateLayers(osgEarth::Map *map);


public slots:
    void toggleLayerEnabled(int layerIndex);

public:
	int rowCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int,QByteArray> roleNames() const override;

private:
    QList<osgEarth::Layer*> mLayersList;
};

#endif // LAYERSMODEL_H
