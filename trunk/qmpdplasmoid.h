/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

// Here we avoid loading the header multiple times
#ifndef QMPDPLASMOID_HEADER
#define QMPDPLASMOID_HEADER

#include <KIcon>
#include <KConfigDialog>
// We need the Plasma Applet headers
#include <Plasma/Applet>
#include <Plasma/Svg>

#include <QGraphicsLinearLayout>
#include <QSlider>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>

#include "networkaccess.h"
#include "ui_contextview.h"
#include "qmpdplasmoidsettings.h"
#include "ui_qmpdplasmoidconfig.h"
#include "ui_qmpdplasmoidconfigview.h"
#include "wheelbutton.h"

class QSizeF;


namespace Plasma {
class LineEdit;
class PushButton;
class ToolButton;
class Label;
class ToolTipContent;
class Slider;
}


// Define our plasma Applet
class qmpdplasmoid : public Plasma::Applet
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    qmpdplasmoid(QObject *parent, const QVariantList &args);
    ~qmpdplasmoid();

    // The paintInterface procedure paints the applet to the desktop
    /*void paintInterface(QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            const QRect& contentsRect);*/
    void init();

protected:
    void createConfigurationInterface(KConfigDialog *parent);
    void constraintsEvent(Plasma::Constraints constraints);


private:
    Plasma::Svg m_svg;
    KIcon m_icon;
    //Status labels
    Plasma::Label *artistlabel;
    Plasma::Label *albumlabel;
    Plasma::Label *tracklabel;

    //Control buttons
    WheelButton *playbutton;
    WheelButton *stopbutton;
    WheelButton *nextbutton;
    WheelButton *prevbutton;
    WheelButton *connectbutton;
    WheelButton *volumebutton;
    Plasma::PushButton *playlistbutton;
    Plasma::PushButton *albumbutton;
    Plasma::PushButton *artistbutton;
    Plasma::PushButton *filesbutton;
    Plasma::PushButton *shufflebutton;
    Plasma::PushButton *repeatbutton;
    Plasma::Slider *volumeslider;
    Plasma::Slider *positionslider;
    QGraphicsLinearLayout *mainlayout;
    QGraphicsLinearLayout *buttonlayout;
    QGraphicsLinearLayout *viewbuttonlayout;
    NetworkAccess *netaccess;
    QGraphicsProxyWidget *contextwidgetproxy;
    Ui_ContextView *contextview;
    QmpdPlasmoidSettings *plasmoidsettings;
    KConfigDialog *configdialog;
    Ui::qmpdplasmoidConfig configwidget;
    Ui::qmpdplasmoidConfigView viewconfigwidget;
    QTimer *reconnecttimer;
    QString hostname;
    QString password;
    quint16 port;
    bool autoconnect;
    QString artist;
    QString album;
    QString title;
    QString fileuri;
    quint8 playing;
    quint32 length;
    quint32 currenttime;
    quint32 playingid;



    // methods
    QString lengthString(quint32 length);


protected slots:
    void updateStatusLabels(status_struct);
    void showFilesClicked();
    void toggleVolumeSlider();
    void updateVolumeSlider(quint8 volume);
    void changeServerVolume(int volume);
    void reconnectTry();
    void disconnectedFromServerHandle();
    void connectedFromServerHandle();
    void connectButtonHandle();
    void updatePlaybackPosition(int pos);
    void mouseWheelMove(int);

public slots:
    void toolTipAboutToShow();
    void toolTipHidden();

private Q_SLOTS:
    void configChanged();
    void onConfigAccepted();


};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(qmpdplasmoid, qmpdplasmoid)
#endif
