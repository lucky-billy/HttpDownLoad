#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

class QReplyTimeout : public QObject
{
    Q_OBJECT
public:
    QReplyTimeout(QNetworkReply *reply, const int time) : QObject(reply) {
        if ( reply && reply->isRunning() ) {
            // 启动单次定时器
            QTimer::singleShot(time, this, [&](){
                // 处理超时
                QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
                if ( reply->isRunning() ) {
                    emit replyTimeOut();
                }
            });
        }
    }

signals:
    void replyTimeOut();  // 超时信号
};

class DownLoadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownLoadManager(QObject *parent = nullptr);

    void startDownLoad();

    // 独立的 get 请求
    QString get(QString url, int timeOutValue = 3000)
    {
        // 请求url
        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = manager->get(request);
        QReplyTimeout *pTimeout = new QReplyTimeout(reply, timeOutValue);
        QEventLoop eventLoop;

        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &eventLoop, SLOT(quit()));

        // 超时处理
        QObject::connect(pTimeout, SIGNAL(replyTimeOut()), &eventLoop, SLOT(quit()));
        QObject::connect(pTimeout, &QReplyTimeout::replyTimeOut, [=]() {
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
            qDebug() << str + ": timeout !";
        });

        // 启动事件循环
        eventLoop.exec();

        // 事件循环已结束，文件已下载完成
        QByteArray returnValue;
        if ( reply->error() ) {
            returnValue = "";
            qDebug() << "reply error !";
        } else {
            // 获取返回信息
            returnValue = reply->readAll();
        }

        manager->deleteLater();
        reply->abort();
        reply->deleteLater();
        pTimeout->deleteLater();

        return QString(returnValue);
    }

    // 独立的 post 请求
    QString post(QString url, QString data, int timeOutValue = 3000)
    {
        // 请求url
        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 注意中文乱码问题
        QByteArray append = data.toUtf8();
        QNetworkReply *reply = manager->post(request, append);
        QReplyTimeout *pTimeout = new QReplyTimeout(reply, timeOutValue);
        QEventLoop eventLoop;

        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &eventLoop, SLOT(quit()));

        // 超时处理
        QObject::connect(pTimeout, SIGNAL(replyTimeOut()), &eventLoop, SLOT(quit()));
        QObject::connect(pTimeout, &QReplyTimeout::replyTimeOut, [=]() {
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
            qDebug() << str + ": timeout !";
            qDebug() << data;
        });

        // 启动事件循环
        eventLoop.exec(QEventLoop::WaitForMoreEvents);

        // 事件循环已结束，文件已下载完成
        QByteArray returnValue;
        if ( reply->error() ) {
            returnValue = "";
            qDebug() << "reply error !";
        } else {
            // 获取返回信息
            returnValue = reply->readAll();
        }

        manager->deleteLater();
        reply->abort();
        reply->deleteLater();
        pTimeout->deleteLater();

        return QString(returnValue);
    }

private:
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
};

#endif // DOWNLOADMANAGER_H
