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

#include "qmpdplasmoid.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <KLocale>
#include <KPushButton>

#include <plasma/svg.h>
#include <plasma/theme.h>
#include <plasma/widgets/lineedit.h>
#include <plasma/widgets/pushbutton.h>
#include <Plasma/ToolButton>
#include <plasma/widgets/label.h>
#include <Plasma/ToolTipManager>
#include <Plasma/Slider>

#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QGraphicsLinearLayout>
#include <QPalette>
#include <QGraphicsSceneWheelEvent>

qmpdplasmoid::qmpdplasmoid(QObject *parent, const QVariantList &args)
        : Plasma::Applet(parent, args),
        m_icon("applications-multimedia")
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setAutoFillBackground(false);
    // this will get us the standard applet background, for free!
//     setBackgroundHints(DefaultBackground);
    //m_svg.setImagePath("widgets/background");
    setHasConfigurationInterface(true);
    //resize(200, 400);

    hostname ="";
    autoconnect = false;
    password ="";
    port =0;
//     setWindowFrameMargins(0,0,0,0);
    setFocusPolicy(Qt::StrongFocus);

}


qmpdplasmoid::~qmpdplasmoid()
{

}

void qmpdplasmoid::init()
{
    mainlayout = new QGraphicsLinearLayout(this);
    mainlayout->setOrientation(Qt::Vertical); //so widgets will be stacked up/down
    buttonlayout = new QGraphicsLinearLayout(mainlayout);
    buttonlayout->setOrientation(Qt::Horizontal);
    viewbuttonlayout = new QGraphicsLinearLayout(mainlayout);
    viewbuttonlayout->setOrientation(Qt::Horizontal);
    //Context view
    netaccess = new NetworkAccess(this);
    contextwidgetproxy = new QGraphicsProxyWidget(this);
    contextview = new Ui_ContextView(NULL,netaccess);
    contextwidgetproxy->setWidget(contextview);

    //Create status labels
    artistlabel = new Plasma::Label(this);
//     artistlabel->setWordWrap(false);
    tracklabel = new Plasma::Label(this);
//     tracklabel->setWordWrap(false);
    albumlabel = new Plasma::Label(this);
/*    albumlabel->setWordWrap(false);*/
    

    //Create buttons
    playbutton = new WheelButton(this);
    stopbutton = new WheelButton(this);
    nextbutton = new WheelButton(this);
    prevbutton = new WheelButton(this);
    volumebutton = new WheelButton(this);
    connectbutton = new WheelButton(this);
    shufflebutton = new Plasma::PushButton(this);
    repeatbutton = new Plasma::PushButton(this);
    playlistbutton = new Plasma::PushButton(this);
    albumbutton = new Plasma::PushButton(this);
    artistbutton = new Plasma::PushButton(this);
    filesbutton  = new Plasma::PushButton(this);
    //Set button icons
    playbutton->setIcon(KIcon("media-playback-start"));
    stopbutton->setIcon(KIcon("media-playback-stop"));
    nextbutton->setIcon(KIcon("media-skip-forward"));
    prevbutton->setIcon(KIcon("media-skip-backward"));
    volumebutton->setIcon(KIcon("audio-volume-high"));
    connectbutton->setIcon(KIcon("network-disconnect"));
    shufflebutton->setIcon(KIcon("media-playlist-shuffle"));
    repeatbutton->setIcon(KIcon("media-playlist-repeat"));
    playbutton->setToolTip(i18n("Play"));
    stopbutton->setToolTip(i18n("Stop"));
    nextbutton->setToolTip(i18n("Next song"));
    prevbutton->setToolTip(i18n("Previous song"));
    connectbutton->setToolTip(i18n("Connect to server"));
    shufflebutton->setToolTip(i18n("Shuffle"));
    repeatbutton->setToolTip(i18n("Repeat"));
    volumebutton->setToolTip(i18n("Click for volume setting"));

    //Set button texts
    playlistbutton->setText(i18n("Playlist"));
    albumbutton->setText(i18n("Albums"));
    artistbutton->setText(i18n("Artists"));
    filesbutton->setText(i18n("Files"));
    tracklabel->setText(i18n("Title:"));

    shufflebutton->setCheckable(true);
    repeatbutton->setCheckable(true);

    //Volume slider creation

    volumeslider = new Plasma::Slider(this);
    volumeslider->setOrientation(Qt::Vertical);
    volumeslider->setMaximum(100);
    volumeslider->setMinimum(0);
    volumeslider->hide();
    volumeslider->setGeometry(QRect(0,0,50,300));

    positionslider = new Plasma::Slider(this);
    positionslider->setOrientation(Qt::Horizontal);
    positionslider->setMaximum(100);
    positionslider->setMinimum(0);
    positionslider->nativeWidget()->setTracking(false);

    buttonlayout->addItem(prevbutton);
    buttonlayout->addItem(nextbutton);
    buttonlayout->addItem(stopbutton);
    buttonlayout->addItem(playbutton);
    buttonlayout->addItem(repeatbutton);
    buttonlayout->addItem(shufflebutton);
    buttonlayout->addItem(connectbutton);
    buttonlayout->addItem(volumebutton);

    viewbuttonlayout->addItem(filesbutton);
    viewbuttonlayout->addItem(artistbutton);
    viewbuttonlayout->addItem(albumbutton);
    viewbuttonlayout->addItem(playlistbutton);


    mainlayout->insertItem(0,contextwidgetproxy);
    mainlayout->insertItem(3,artistlabel);
    mainlayout->insertItem(2,albumlabel);
    mainlayout->insertItem(1,tracklabel);
    mainlayout->insertItem(4,positionslider);
    mainlayout->insertItem(5,viewbuttonlayout);
    mainlayout->insertItem(6,buttonlayout);


    artistlabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    tracklabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    albumlabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    contextwidgetproxy->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    contextview->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    reconnecttimer = new QTimer(this);
    connect(reconnecttimer,SIGNAL(timeout()),this,SLOT(reconnectTry()));
    connect(netaccess,SIGNAL(connectionestablished()),this,SLOT(connectedFromServerHandle()));
    connect(netaccess,SIGNAL(disconnected()),this,SLOT(disconnectedFromServerHandle()));

    netaccess->setUpdateInterval(1000);

    contextwidgetproxy->setAutoFillBackground(false);

    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::transparent);
    p.setColor(QPalette::Shadow,Qt::white);
    p.setColor(QPalette::Light,Qt::white);
    p.setColor(QPalette::Midlight,Qt::white);
    p.setColor(QPalette::Dark,Qt::white);
    p.setColor(QPalette::Mid,Qt::white);
    p.setColor(QPalette::Base,Qt::white);
    p.setColor(QPalette::AlternateBase,Qt::white);


    contextview->setPalette(p);
    contextview->setAutoFillBackground(false);
    contextwidgetproxy->setPalette(p);
    this->setPalette(p);

    connect(netaccess,SIGNAL(statusUpdate(status_struct)),this,SLOT(updateStatusLabels(status_struct)));
    //button actions
    connect(playbutton,SIGNAL(clicked()),netaccess,SLOT(pause()));
    connect(stopbutton,SIGNAL(clicked()),netaccess,SLOT(stop()));
    connect(nextbutton,SIGNAL(clicked()),netaccess,SLOT(next()));
    connect(prevbutton,SIGNAL(clicked()),netaccess,SLOT(previous()));
    //connect(filesbutton,SIGNAL(clicked()),contextview,)
    connect(albumbutton,SIGNAL(clicked()),contextview,SLOT(showAlbums()));
    connect(artistbutton,SIGNAL(clicked()),contextview,SLOT(showArtists()));
    connect(playlistbutton,SIGNAL(clicked()),contextview,SLOT(showCurrentPlaylist()));
    connect(filesbutton,SIGNAL(clicked()),this,SLOT(showFilesClicked()));
    connect(connectbutton,SIGNAL(clicked()),this,SLOT(connectButtonHandle()));
    connect(volumebutton,SIGNAL(clicked()),this,SLOT(toggleVolumeSlider()));
    connect(volumeslider,SIGNAL(valueChanged(int)),this,SLOT(changeServerVolume(int)));
    connect(repeatbutton,SIGNAL(toggled(bool)),netaccess,SLOT(setRepeat(bool)));
    connect(shufflebutton,SIGNAL(toggled(bool)),netaccess,SLOT(setRandom(bool)));
    connect(positionslider,SIGNAL(valueChanged(int)),this,SLOT(updatePlaybackPosition(int)));
    connect(volumebutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
    artist="";
    album="";
    title="";
    playing=false;
    contextview->setEnabled(false);
    configChanged();
}


void qmpdplasmoid::updateStatusLabels(status_struct tempstruct)
{
    album = tempstruct.album;
    artist = tempstruct.artist;
    title = tempstruct.title;

    positionslider->blockSignals(true);
    volumeslider->blockSignals(true);
    if (formFactor()!=Plasma::Vertical&&formFactor()!=Plasma::Horizontal) {
        albumlabel->setText(i18n("Album: %1",album));
        artistlabel->setText(i18n("Artist: %1",artist));
	if(length!=0)
	{tracklabel->setText(i18n("Title: %1 (%2)",title,lengthString(tempstruct.length)));
	}
	else {tracklabel->setText(i18n("Title: %1",title));}
        
        positionslider->setMaximum(tempstruct.length);
        positionslider->setValue(tempstruct.currentpositiontime);
        repeatbutton->setChecked(tempstruct.repeat);
        shufflebutton->setChecked(tempstruct.shuffle);
    }
    if (tempstruct.playing==NetworkAccess::PLAYING)
    {
        playbutton->setIcon(KIcon("media-playback-pause"));

    }
    else
    {
        playbutton->setIcon(KIcon("media-playback-start"));

    }
    updateVolumeSlider(tempstruct.volume);
    this->playing = tempstruct.playing;
    this->length = tempstruct.length;
    this->currenttime = tempstruct.currentpositiontime;
    this->playingid = tempstruct.id;
    fileuri = tempstruct.fileuri;
    positionslider->blockSignals(false);
    volumeslider->blockSignals(false);
}



void qmpdplasmoid::showFilesClicked()
{
    contextview->showFiles("");
}

void qmpdplasmoid::toggleVolumeSlider()
{
    if (volumeslider->isVisible())
    {
        volumeslider->hide();
        CommonDebug("hide volume slider");
    }
    else {
        CommonDebug("show volume slide");
        int x,y;
        x =  volumebutton->geometry().x()+volumebutton->geometry().width()/2-10;
        y = volumebutton->geometry().y() - (this->geometry().height()*50/100);
        CommonDebug("sliderpos:"+QString::number(x).toAscii()+":"+QString::number(y).toAscii());
        volumeslider->setGeometry(QRect(x,y,20,(this->geometry().height()*50/100)));
        volumeslider->show();
        volumeslider->setVisible(true);
        volumeslider->setFocus();
    }
}

void qmpdplasmoid::updateVolumeSlider(quint8 volume)
{
    if (volume>=85)
    {
        volumebutton->setIcon(KIcon("audio-volume-high"));
    }
    else if ((volume<85)&&(volume>=40))
    {
        volumebutton->setIcon(KIcon("audio-volume-medium"));
    }
    else if ((volume<40)&&(volume>0))
    {
        volumebutton->setIcon(KIcon("audio-volume-low"));
    }
    else if (volume<=0)
    {
        volumebutton->setIcon(KIcon("audio-volume-muted"));
    }
    volumebutton->setToolTip(QString::number(volumeslider->value())+"%");
    volumeslider->setValue(volume);
}


void qmpdplasmoid::changeServerVolume(int volume)
{
    if (volume>=85)
    {
        volumebutton->setIcon(KIcon("audio-volume-high"));
    }
    else if ((volume<85)&&(volume>=40))
    {
        volumebutton->setIcon(KIcon("audio-volume-medium"));
    }
    else if ((volume<40)&&(volume>0))
    {
        volumebutton->setIcon(KIcon("audio-volume-low"));
    }
    else if (volume<=0)
    {
        volumebutton->setIcon(KIcon("audio-volume-muted"));
    }
    netaccess->setVolume(volume);
}

void qmpdplasmoid::createConfigurationInterface(KConfigDialog* parent)
{
    CommonDebug("createConfigurationInterface called");
    QWidget *widget = new QWidget();
    configwidget.setupUi(widget);
    configwidget.hostname->setText(hostname);
    configwidget.password->setText(password);
    configwidget.port->setValue(port);
    configwidget.autoconnect->setChecked(autoconnect);
    parent->addPage(widget, i18n("Network"), "network-server");
    QWidget *viewwidget = new QWidget();
    viewconfigwidget.setupUi(viewwidget);
    viewconfigwidget.doubleclick->setChecked(contextview->doubleClickToSelect());
    parent->addPage(viewwidget,i18n("View"), "applications-graphics");


//     connect(ui.showTimezoneStringCheckBox, SIGNAL(stateChanged(int)), parent, SLOT(settingsModified()));
    connect(configwidget.password,SIGNAL(editingFinished()),parent, SLOT(settingsModified()));
    connect(configwidget.hostname,SIGNAL(editingFinished()),parent, SLOT(settingsModified()));
    connect(configwidget.port,SIGNAL(valueChanged(int)),parent, SLOT(settingsModified()));
    connect(parent,SIGNAL(applyClicked()),this,SLOT(onConfigAccepted()));
    connect(parent,SIGNAL(okClicked()),this,SLOT(onConfigAccepted()));

}

void qmpdplasmoid::configChanged()
{
    qDebug("configChanged called");
    KConfigGroup config = this->config();
    QString hhostname ="" ;
    hhostname = config.readEntry("hostname",QString());
    QString hpassword = "";
    hpassword = config.readEntry("password",QString());
    CommonDebug(hhostname+":"+hpassword);
    if (hhostname!="") {
        CommonDebug("Hostname loaded");
        hostname = hhostname;
    }
    else {
        hostname ="localhost";
    }
    if (hpassword!="") {
        password = hpassword;
    }
    QString portstring ="" ;
    portstring = config.readEntry("port",QString());
    if (portstring!="") {
        port = portstring.toUInt();
    }
    else {
        port = 6600;
    };
    autoconnect = config.readEntry("autoconnect",false);
    contextview->setDoubleClickNeeded(config.readEntry("doubleclick",false));
    netaccess->disconnect();
    if (reconnecttimer->isActive())
    {
        reconnecttimer->stop();
    }
    disconnectedFromServerHandle();
//     if (hostname!=""&&autoconnect)
//     {
//         netaccess->connectToHost(hostname,port);
//         if (password!="")
//         {
//             netaccess->authenticate(password);
//         }
//     }
    if (autoconnect&&hostname!="")
    {
        reconnecttimer->start(30000);
    }


}

void qmpdplasmoid::onConfigAccepted()
{
    qDebug("Write config \n");
    KConfigGroup config = this->config();
    config.writeEntry("password",configwidget.password->text());
    config.writeEntry("hostname",configwidget.hostname->text());
    config.writeEntry("port",QString::number(configwidget.port->value()));
    config.writeEntry("autoconnect",configwidget.autoconnect->isChecked());
    config.writeEntry("doubleclick",viewconfigwidget.doubleclick->isChecked());
    hostname = configwidget.hostname->text();
    password = configwidget.password->text();
    port = configwidget.port->value();
    autoconnect = configwidget.autoconnect->isChecked();
    contextview->setDoubleClickNeeded(viewconfigwidget.doubleclick->isChecked());
    Q_EMIT configNeedsSaving();
}

void qmpdplasmoid::reconnectTry()
{
    this->setBusy(true);
    if (hostname!="")
    {
        netaccess->disconnect();
        netaccess->connectToHost(hostname,port);
        if (password!="")
        {
            netaccess->authenticate(password);
        }
    }
    this->setBusy(false);

}

void qmpdplasmoid::disconnectedFromServerHandle()
{
    if (autoconnect) {
        reconnecttimer->start(30000);
    }
    connectbutton->setIcon(KIcon("network-disconnect"));
    artistlabel->setText(i18n("Artist:  "));
    albumlabel->setText(i18n("Album:  "));
    tracklabel->setText(i18n("Title:  "));
    connectbutton->setToolTip("Connect to server");
    contextview->showCurrentPlaylist();
    contextview->setEnabled(false);
}

void qmpdplasmoid::connectedFromServerHandle()
{
    connectbutton->setIcon(KIcon("network-connect"));
    if (reconnecttimer->isActive())
    {
        reconnecttimer->stop();
    }
    connectbutton->setToolTip("Disconnect from server");
    contextview->setEnabled(true);
}

void qmpdplasmoid::connectButtonHandle()
{
    if (netaccess->connected())
    {
        netaccess->disconnect();
        disconnectedFromServerHandle();


    }
    else {
        reconnectTry();
    }
}

void qmpdplasmoid::constraintsEvent(Plasma::Constraints constraints)
{
    //TODO MAJOR CLEANUP SORRY
    Plasma::Applet::constraintsEvent(constraints);
    const Plasma::FormFactor form(formFactor());
    //RELAYOUT ALL
    //END
    CommonDebug("constraintsevent");
    if ( constraints.testFlag(Plasma::FormFactorConstraint) )
    {

        contextwidgetproxy->hide();
//         contextview->hide();
        volumeslider->hide();
        //sliderwidgetproxy->hide();
        artistbutton->hide();
        filesbutton->hide();
        albumbutton->hide();
        playlistbutton->hide();
        artistlabel->hide();
        albumlabel->hide();
        tracklabel->hide();
        shufflebutton->hide();
        repeatbutton->hide();
        volumebutton->hide();
        playbutton->hide();
        prevbutton->hide();
        nextbutton->hide();
        stopbutton->hide();
        connectbutton->hide();
        positionslider->hide();
        repeatbutton->setMaximumSize(0,0);
        shufflebutton->setMaximumSize(0,0);
        volumebutton->setMaximumSize(0,0);
        artistbutton->setMaximumSize(0,0);
        albumbutton->setMaximumSize(0,0);
        filesbutton->setMaximumSize(0,0);
        playlistbutton->setMaximumSize(0,0);
        artistlabel->setMaximumSize(0,0);
        albumlabel->setMaximumSize(0,0);
        tracklabel->setMaximumSize(0,0);
// 	contextview->setMaximumSize(0,0);
        contextwidgetproxy->setMaximumSize(0,0);
        positionslider->setMaximumSize(0,0);


        mainlayout->setOrientation(Qt::Vertical); //so widgets will be stacked up/down
        if ( form == Plasma::Vertical )
        {

            CommonDebug("Vertical layout requested");
            setMinimumSize(-1,-1);
            setMaximumSize(-1,-1);

            mainlayout->setOrientation(Qt::Vertical); //so widgets will be stacked up/down
            buttonlayout->setOrientation(Qt::Vertical);
            viewbuttonlayout->setOrientation(Qt::Horizontal);

            playbutton->setMinimumSize(0,0);
            connectbutton->setMinimumSize(0,0);
            stopbutton->setMinimumSize(0,0);
            prevbutton->setMinimumSize(0,0);
            nextbutton->setMinimumSize(0,0);
            playbutton->setMaximumSize(10000,128);
            connectbutton->setMaximumSize(10000,128);
            stopbutton->setMaximumSize(10000,128);
            prevbutton->setMaximumSize(10000,128);
            nextbutton->setMaximumSize(10000,128);
            playbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
            stopbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
            nextbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
            prevbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
            connectbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
            setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
            mainlayout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
            buttonlayout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
            mainlayout->setSpacing(0);
            this->setContentsMargins(0,0,0,0);
            mainlayout->setContentsMargins(1,1,1,1);
            buttonlayout->setSpacing(0);
            playbutton->show();
            prevbutton->show();
            nextbutton->show();
            stopbutton->show();
            connectbutton->show();
            connect(volumebutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(playbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(stopbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(connectbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(nextbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(prevbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            Plasma::ToolTipManager::self()->registerWidget(this);


        }
        else if (form == Plasma::Horizontal)
        {
            CommonDebug("Horizontal layout requested");
            setMinimumSize(-1,-1);
            setMaximumSize(-1,-1);
            mainlayout->setOrientation(Qt::Vertical); //so widgets will be stacked up/down
            buttonlayout->setOrientation(Qt::Horizontal);
            viewbuttonlayout->setOrientation(Qt::Horizontal);

            playbutton->setMinimumSize(0,0);
            connectbutton->setMinimumSize(0,0);
            stopbutton->setMinimumSize(0,0);
            prevbutton->setMinimumSize(0,0);
            nextbutton->setMinimumSize(0,0);
            playbutton->setMaximumSize(128,10000);
            connectbutton->setMaximumSize(128,10000);
            stopbutton->setMaximumSize(128,10000);
            prevbutton->setMaximumSize(128,10000);
            nextbutton->setMaximumSize(128,10000);
            playbutton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
            stopbutton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
            nextbutton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
            prevbutton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
            connectbutton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);

            setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);
            mainlayout->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);
            buttonlayout->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);
            mainlayout->setSpacing(0);
            this->setContentsMargins(0,0,0,0);
            mainlayout->setContentsMargins(1,1,1,1);

            buttonlayout->setSpacing(0);
            playbutton->show();
            prevbutton->show();
            nextbutton->show();
            stopbutton->show();
            connectbutton->show();
            playbutton->setProperty("flat",true);
//             mainlayout->updateGeometry();
//             buttonlayout->updateGeometry();
//             viewbuttonlayout->updateGeometry();
//  	    contextwidgetproxy->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
            connect(volumebutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(playbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(stopbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(connectbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(nextbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            connect(prevbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            Plasma::ToolTipManager::self()->registerWidget(this);

        }
        else {
            setMaximumHeight(-1);
            setMinimumHeight(-1);
            setMaximumWidth(-1);
            setMinimumWidth(-1);
            repeatbutton->setMaximumSize(-1,-1);
            shufflebutton->setMaximumSize(-1,-1);
            volumebutton->setMaximumSize(-1,-1);
            artistbutton->setMaximumSize(-1,-1);
            albumbutton->setMaximumSize(-1,-1);
            filesbutton->setMaximumSize(-1,-1);
            playlistbutton->setMaximumSize(-1,-1);
            artistlabel->setMaximumSize(-1,-1);
            albumlabel->setMaximumSize(-1,-1);
            tracklabel->setMaximumSize(-1,-1);
            connectbutton->setMaximumSize(-1,-1);
            contextwidgetproxy->setMaximumSize(-1,-1);
            positionslider->setMaximumSize(-1,-1);
// 	contextview->setMaximumSize(-1,-1);
            /*	contextview->resize(300,300);*/
// 	contextview->setMinimumSize(-1,-1);
            CommonDebug("Normal Layout");
            mainlayout->setOrientation(Qt::Vertical); //so widgets will be stacked up/down
            buttonlayout->setOrientation(Qt::Horizontal);
            viewbuttonlayout->setOrientation(Qt::Horizontal);

            playbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            stopbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            nextbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            prevbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            connectbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            volumebutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            shufflebutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            repeatbutton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

            mainlayout->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            mainlayout->setContentsMargins(6,6,6,6);
//             contextview->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            contextwidgetproxy->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            viewbuttonlayout->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            buttonlayout->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

            qDebug("Buttons added\n");
            contextview->show();

            contextwidgetproxy->show();

            playbutton->show();
            nextbutton->show();
            prevbutton->show();
            stopbutton->show();
            connectbutton->show();
            volumebutton->show();
            shufflebutton->show();
            repeatbutton->show();
            artistbutton->show();
            albumbutton->show();
            playlistbutton->show();
            filesbutton->show();
            positionslider->show();
            albumlabel->show();
            artistlabel->show();
            tracklabel->show();



            mainlayout->setSpacing(0);
            buttonlayout->setSpacing(0);
            viewbuttonlayout->setSpacing(0);
            playbutton->setMinimumSize(24,24);
            connectbutton->setMinimumSize(24,24);
            stopbutton->setMinimumSize(24,24);
            repeatbutton->setMinimumSize(24,24);
            shufflebutton->setMinimumSize(24,24);
            prevbutton->setMinimumSize(24,24);
            nextbutton->setMinimumSize(24,24);
            disconnect(playbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            disconnect(stopbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            disconnect(connectbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            disconnect(nextbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            disconnect(prevbutton,SIGNAL(wheelmove(int)),this,SLOT(mouseWheelMove(int)));
            Plasma::ToolTipManager::self()->unregisterWidget(this);
        }
        mainlayout->invalidate();
        buttonlayout->invalidate();
        viewbuttonlayout->invalidate();
    }

    if (form == Plasma::Horizontal)
    {
        setMaximumWidth(5*buttonlayout->geometry().height()+((buttonlayout->geometry().height())*100)/23);
    }
    else if (form == Plasma::Vertical)
    {
        setMaximumHeight(5*buttonlayout->geometry().width()+((buttonlayout->geometry().width())*100)/20);
    }
    CommonDebug("plasmoidsize:"+QString::number(size().width())+":"+QString::number(size().height()));
    CommonDebug("mainlayoutsize:"+QString::number(mainlayout->geometry().width())+":"+QString::number(mainlayout->geometry().height()));
    CommonDebug("buttonlayoutsize:"+QString::number(buttonlayout->geometry().width())+":"+QString::number(buttonlayout->geometry().height()));
    QGraphicsLayoutItem::updateGeometry();
}

void qmpdplasmoid::toolTipAboutToShow()
{
    Plasma::ToolTipContent toolTip;
    QString subtext;
    if(title!="")
    {   subtext = i18n("%1 (%2/%3) <b> on</b>\n %4<b> from</b>\n %5",title,lengthString(currenttime),lengthString(length),album,artist); }
    else {
      subtext = i18n("%1 (%2/%3)",fileuri,lengthString(currenttime),lengthString(length));
    }
    if (playing==NetworkAccess::PLAYING) {
        toolTip.setMainText(i18n("Currently playing:"));
        toolTip.setSubText(subtext);
    }
    else if (playing==NetworkAccess::STOP){
        toolTip.setMainText(i18n("Currently stopped"));
    }
    else if (playing==NetworkAccess::PAUSE){
        toolTip.setMainText(i18n("Currently paused:"));
	toolTip.setSubText(subtext);
    }
    if (!netaccess->connected())
    {
        toolTip.setSubText("");
        toolTip.setMainText(i18n("Currently not connected to server"));
    }
    toolTip.setImage(KIcon("applications-multimedia"));
    Plasma::ToolTipManager::self()->setContent(this, toolTip);

}
void qmpdplasmoid::toolTipHidden()
{
    Plasma::ToolTipManager::self()->clearContent(this);

}

QString qmpdplasmoid::lengthString(quint32 length)
{
    QString temp;
    int hours=0,min=0,sec=0;
    hours = length/3600;
    if (hours>0)
    {
        min=(length-(3600*hours))/60;
    }
    else {
        min=length/60;
    }
    sec = length-hours*3600-min*60;
    if (hours==0)
    {
        temp=QString::number(min)+":"+(sec<10?"0":"")+QString::number(sec);
    }
    else
    {
        temp=QString::number(hours)+":"+QString::number(min)+":"+QString::number(sec);
    }
    return temp;
}

void qmpdplasmoid::updatePlaybackPosition(int pos)
{
    if (playing)
    {
        CommonDebug("Seek to: "+QString::number(pos)+ " on track:"+QString::number(playingid));
        netaccess->seekPosition(playingid,pos);
    }
}

void qmpdplasmoid::mouseWheelMove(int delta)
{
    CommonDebug("Wheel event delta:"+ QString::number(delta));
    if (delta>=120)
    {
        netaccess->setVolume(volumeslider->value()+3);
        updateVolumeSlider(volumeslider->value()+3);
    }
    else if (delta<=-120)
    {
        netaccess->setVolume(volumeslider->value()-3);
        updateVolumeSlider(volumeslider->value()-3);
    }
}



#include "qmpdplasmoid.moc"

