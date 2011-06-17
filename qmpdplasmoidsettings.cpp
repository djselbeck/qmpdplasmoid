// This file is generated by kconfig_compiler from qmpdplasmoid.kcfg.
// All changes you do to this file will be lost.

#include "qmpdplasmoidsettings.h"

#include <kglobal.h>
#include <QtCore/QFile>

class QmpdPlasmoidSettingsHelper
{
  public:
    QmpdPlasmoidSettingsHelper() : q(0) {}
    ~QmpdPlasmoidSettingsHelper() { delete q; }
    QmpdPlasmoidSettings *q;
};
K_GLOBAL_STATIC(QmpdPlasmoidSettingsHelper, s_globalQmpdPlasmoidSettings)
QmpdPlasmoidSettings *QmpdPlasmoidSettings::self()
{
  if (!s_globalQmpdPlasmoidSettings->q) {
    new QmpdPlasmoidSettings;
    s_globalQmpdPlasmoidSettings->q->readConfig();
  }

  return s_globalQmpdPlasmoidSettings->q;
}

QmpdPlasmoidSettings::QmpdPlasmoidSettings(  )
  : KConfigSkeleton( QLatin1String( "qmpdplasmoidrc" ) )
{
  Q_ASSERT(!s_globalQmpdPlasmoidSettings->q);
  s_globalQmpdPlasmoidSettings->q = this;
  setCurrentGroup( QLatin1String( "network" ) );

  KConfigSkeleton::ItemString  *itemServerName;
  itemServerName = new KConfigSkeleton::ItemString( currentGroup(), QLatin1String( "ServerName" ), mServerName );
  addItem( itemServerName, QLatin1String( "ServerName" ) );
  KConfigSkeleton::ItemInt  *itemPort;
  itemPort = new KConfigSkeleton::ItemInt( currentGroup(), QLatin1String( "Port" ), mPort, 6600 );
  itemPort->setMinValue(1);
  itemPort->setMaxValue(65536);
  addItem( itemPort, QLatin1String( "Port" ) );
  KConfigSkeleton::ItemPassword  *itemPassword;
  itemPassword = new KConfigSkeleton::ItemPassword( currentGroup(), QLatin1String( "Password" ), mPassword );
  addItem( itemPassword, QLatin1String( "Password" ) );
}

QmpdPlasmoidSettings::~QmpdPlasmoidSettings()
{
  if (!s_globalQmpdPlasmoidSettings.isDestroyed()) {
    s_globalQmpdPlasmoidSettings->q = 0;
  }
}
