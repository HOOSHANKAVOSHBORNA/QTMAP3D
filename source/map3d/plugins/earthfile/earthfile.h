#ifndef EARTHFILE_H
#define EARTHFILE_H

#include "plugininterface.h"

class EarthFile : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "earthfile.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit EarthFile(QWidget *parent = nullptr);
//    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
//    void onFileItemClicked(const QString &name, const QString &category) override;
    bool setup() override;
private slots:
    void onImportItemClick();
    void onExportItemClick();
};

#endif // EARTHFILE_H
