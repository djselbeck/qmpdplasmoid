// This file is generated by kconfig_compiler from qmpdplasmoid.kcfg.
// All changes you do to this file will be lost.
#ifndef QMPDPLASMOIDSETTINGS_H
#define QMPDPLASMOIDSETTINGS_H

#include <kconfigskeleton.h>
#include <kdebug.h>

class QmpdPlasmoidSettings : public KConfigSkeleton
{
  public:

    static QmpdPlasmoidSettings *self();
    ~QmpdPlasmoidSettings();

    /**
      Set Defines the sample server.
    */
    static
    void setServerName( const QString & v )
    {
      if (!self()->isImmutable( QString::fromLatin1( "ServerName" ) ))
        self()->mServerName = v;
    }

    /**
      Get Defines the sample server.
    */
    static
    QString serverName()
    {
      return self()->mServerName;
    }

    /**
      Set Defines the server port
    */
    static
    void setPort( int v )
    {
      if (v < 1)
      {
        kDebug() << "setPort: value " << v << " is less than the minimum value of 1";
        v = 1;
      }

      if (v > 65536)
      {
        kDebug() << "setPort: value " << v << " is greater than the maximum value of 65536";
        v = 65536;
      }

      if (!self()->isImmutable( QString::fromLatin1( "Port" ) ))
        self()->mPort = v;
    }

    /**
      Get Defines the server port
    */
    static
    int port()
    {
      return self()->mPort;
    }

    /**
      Set Server password.
    */
    static
    void setPassword( const QString & v )
    {
      if (!self()->isImmutable( QString::fromLatin1( "Password" ) ))
        self()->mPassword = v;
    }

    /**
      Get Server password.
    */
    static
    QString password()
    {
      return self()->mPassword;
    }

  protected:
    QmpdPlasmoidSettings();
    friend class QmpdPlasmoidSettingsHelper;


    // network
    QString mServerName;
    int mPort;
    QString mPassword;

  private:
};

#endif
