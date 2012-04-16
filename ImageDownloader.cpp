#include "ImageDownloader.h"

#include <QPixmap>
#include <QImage>
#include <QNetworkReply>

#include <QDebug>
#include <QFile>

ImageDownloader::
ImageDownloader(QObject * o) :
    QObject(o)
{
    m_working = false;
    connect(& m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(readReply(QNetworkReply*)));
}

void
ImageDownloader::
setUrls(const QStringList & urls)
{
    m_urls = urls;
}

void
ImageDownloader::
start()
{
    m_working = true;
    m_imageCounter = -1;

    getNextImage();
}

void
ImageDownloader::
getNextImage()
{
    if( ! m_working )
        return;

    if( ++m_imageCounter < m_urls.size() )
    {
        QString url = m_urls[m_imageCounter];
        if( url.startsWith("https") )
            url.remove(4, 1);

        m_networkAccessManager.get( QNetworkRequest( QUrl( url ) ) );
    }
}

void
ImageDownloader::
readReply(QNetworkReply * reply)
{
    if( reply->error() == QNetworkReply::NoError )
    {
        QByteArray ba(reply->readAll());
        QPixmap pixmap;
        pixmap.loadFromData( ba );

        emit newPixmap(m_imageCounter, pixmap);
    }
    reply->deleteLater();

    getNextImage();
}
