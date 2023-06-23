#include "./include/downloadAssetIndex.hpp"
#include "./assistance/path.hpp"

DownloadAssetIndex::DownloadAssetIndex(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager()) {}

DownloadAssetIndex::~DownloadAssetIndex() { delete m_manager; }

void DownloadAssetIndex::downloadAssetIndex(const QString &versionGame) {
  const QString path = Path::launcherPath() + "/../" +
                       "/MineLaunch/backend/launcher/minecraft/versions/";
  QDir dir(path);
  QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  if (dirs.contains(versionGame)) {
    QFile file;
    file.setFileName(path + versionGame + "/version.json");

    if (!file.open(QIODevice::ReadOnly)) {
      qDebug() << "Failed open file for read [downloadClient]"
               << file.errorString();
      return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj = doc.object();
    QJsonValue assetIndexValue =
        obj.value("assetIndex").toObject().value("url");

    QString httpHashVersion = assetIndexValue.toString().split("/").last();

    QNetworkRequest request(assetIndexValue.toString());
    QNetworkReply *reply = m_manager->get(request);
    getReply(reply);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::downloadProgress, this,
                     [=](qint64 bytesReceived, qint64 bytesTotal) {
                       int progress = (bytesTotal > 0)
                                          ? (bytesReceived * 100 / bytesTotal)
                                          : 0;
                       emit progressChanged(progress);

                       if (progress == 0) {
                         emit onFinished();
                       }
                     });

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
      emit errorDownloadAssetIndex("Error download asset index :" +
                                   reply->errorString());
      return;
    }

    const QString savePath = Path::launcherPath() + "/../" +
        "/MineLaunch/backend/launcher/minecraft/assets/indexes/";

    QDir saveDir(savePath);
    if (!saveDir.exists()) {
      qDebug() << "Directory where you wanted saved doesn't exists "
                  "[downloadAssetIndex]";
      return;
    }

    QFile saveFile(savePath + httpHashVersion);
    if (!saveFile.open(QIODevice::WriteOnly)) {
      qDebug() << "Failed to open file for write [downloadAssetIndex]: "
               << file.errorString();
      return;
    }

    saveFile.write(reply->readAll());
    saveFile.close();
    reply->deleteLater();
  }
}

void DownloadAssetIndex::stopIsDownloadingAssetIndex() {

  auto reply = getReply();
  reply->abort();
}



QNetworkReply *DownloadAssetIndex::getReply(QNetworkReply *reply) noexcept {
  return reply;
}
