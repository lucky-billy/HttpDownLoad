#include "downloadmanager.h"
#include <QFile>
#include <QUrl>

DownLoadManager::DownLoadManager(QObject *parent) : QObject(parent)
{

}

void DownLoadManager::startDownLoad()
{
    // 下载路径
    QUrl url;
//    url.setUrl("http://34679y496m.oicp.vip/download/version.json");
    url.setUrl("http://34679y496m.oicp.vip/download/Sirius-Pro 4.0_beta_5_23.rar");
    request.setUrl(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

    // 通过发送数据，返回值保存在reply指针里
    accessManager = new QNetworkAccessManager(this);
    reply = accessManager->get(request);

    connect(accessManager, &QNetworkAccessManager::finished, [](QNetworkReply *r){
        if ( r->error() == QNetworkReply::NoError )
        {
            QVariant statusCode = r->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int status = statusCode.toInt();

            // 获取响应的信息，状态码为200表示正常
            if ( status == 200 ) {
                qDebug() << "Download Success !";
            } else {
                qDebug() << "Download Failed ! - Error status: " << status;
            }
        } else {
            // 处理错误
            QString strError = r->errorString();
            qDebug() << "Error: " << strError;
        }
    });

    connect(reply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal){
        // 更新下载进度信息
        qDebug() << QString::number(bytesReceived) + "/" +  QString::number(bytesTotal);
    });

    connect(reply, &QNetworkReply::finished, [&](){
        // 下载回来的文件存放路径
//        QString fileName = "D:/version.json";
        QString fileName = "D:/Sirius-Pro 4.0_beta_5_23.rar";

        // 写入文件
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        file.write(reply->readAll());

        // 下载完成
        file.flush();
        file.close();

        reply->deleteLater();
        reply = nullptr;
    });
}
