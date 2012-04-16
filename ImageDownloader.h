#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QPixmap>

#include <QNetworkAccessManager>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QObject *parent = 0);

signals:
    void newPixmap(int,const QPixmap &);
    
public slots:
    void setUrls(const QStringList & urls);
    void stop() { m_working = false; }

    void start();

private slots:
    void readReply(QNetworkReply*);

private:
    void getNextImage();
    
private:
    bool m_working;
    QStringList m_urls;

    int m_imageCounter;
    QNetworkAccessManager m_networkAccessManager;
};

#endif // IMAGEDOWNLOADER_H
