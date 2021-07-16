// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "WbDownloader.hpp"

#include "WbApplication.hpp"
#include "WbNetwork.hpp"
#include "WbSimulationState.hpp"

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <QtNetwork/QNetworkDiskCache>

static int gCount = 0;
static int gComplete = 0;
static bool gDownloading = false;
static QTimer *gTimer = NULL;
static bool gDisplayPopUp = false;
static QMap<QUrl, QNetworkReply *> gUrlCache;

int WbDownloader::progress() {
  return gCount == 0 ? 100 : 100 * gComplete / gCount;
}

void WbDownloader::reset() {
  gCount = 0;
  gComplete = 0;
}

WbDownloader::WbDownloader(QObject *parent) :
  QObject(parent),
  mNetworkReply(NULL),
  mFinished(false),
  mOffline(false),
  mCopy(false),
  mIsBackground(false) {
  gCount++;
}

WbDownloader::~WbDownloader() {
  if (mNetworkReply != NULL) {
    mNetworkReply->deleteLater();
    if (gUrlCache.contains(mUrl))
      gUrlCache.remove(mUrl);
  }

  if (!mFinished)
    gCount--;
}

QIODevice *WbDownloader::device() const {
  return dynamic_cast<QIODevice *>(mNetworkReply);
}

void WbDownloader::download(const QUrl &url) {
  WbSimulationState::instance()->pauseSimulation();

  mUrl = url;

  if (gUrlCache.contains(mUrl) && !mIsBackground &&
      (mUrl.toString().endsWith(".png", Qt::CaseInsensitive) || url.toString().endsWith(".jpg", Qt::CaseInsensitive))) {
    if (!(mOffline == true && mCopy == false)) {
      mCopy = true;
      QNetworkReply *reply = gUrlCache[mUrl];
      if (reply && !reply->isFinished())
        connect(reply, &QNetworkReply::finished, this, &WbDownloader::finished, Qt::UniqueConnection);
      else
        finished();
      return;
    }
  }

  if (!gDownloading) {
    gDownloading = true;
    gTimer = new QTimer(0);
    connect(gTimer, &QTimer::timeout, &WbDownloader::displayPopUp);
    gTimer->setInterval(1000);
    gTimer->setSingleShot(true);
    gTimer->start();
  }
  QNetworkRequest request;
  request.setUrl(url);
  mFinished = false;
  if (mOffline)
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysCache);
  else
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

  mNetworkReply = WbNetwork::instance()->networkAccessManager()->get(request);
  connect(mNetworkReply, &QNetworkReply::finished, this, &WbDownloader::finished, Qt::UniqueConnection);
  connect(WbApplication::instance(), &WbApplication::worldLoadingWasCanceled, mNetworkReply, &QNetworkReply::abort);

  gUrlCache.insert(url, mNetworkReply);
}

void WbDownloader::finished() {
  if (!mCopy) {
    assert(mNetworkReply);
    if (mNetworkReply->error())
      mError = tr("Cannot download %1: %2").arg(mUrl.toString()).arg(mNetworkReply->errorString());
    disconnect(mNetworkReply, &QNetworkReply::finished, this, &WbDownloader::finished);
    if (!mError.isEmpty() && !mOffline) {
      mError = QString();
      mOffline = true;
      download(mUrl);
      return;
    }

    QNetworkCacheMetaData metaData = WbNetwork::instance()->networkAccessManager()->cache()->metaData(mUrl);
    // We need to replace the expiration date only the first time the asset is downloaded and when it has been refreshed by the
    // cache. The QNetworkRequest::SourceIsFromCacheAttribute attribute present in the reply is not enough because the image is
    // still loaded from the cache when the expiration date is just refreshed.
    // The hack we use to detect if the expiration date of an asset is the one set by webots or is the automatic one works as
    // follows: The automatic expiration date is an <http-date> and thus is in UTC format. The expiration date set by webots is
    // in local format. So we just need to check the format of the expiration date to know if it needs to be updated or not.
    if (metaData.expirationDate().toUTC().toString() == metaData.expirationDate().toString()) {
      // increase expiration date to one day
      metaData.setExpirationDate(QDateTime::currentDateTime().addDays(1));
      WbNetwork::instance()->networkAccessManager()->cache()->updateMetaData(metaData);
    }
  }

  gComplete++;
  mFinished = true;
  emit complete();

  if (gComplete == gCount) {
    gDownloading = false;
    gDisplayPopUp = false;
    gUrlCache.clear();
    emit WbApplication::instance()->deleteWorldLoadingProgressDialog();
    WbSimulationState::instance()->resumeSimulation();
  } else if (gDisplayPopUp)
    emit WbApplication::instance()->setWorldLoadingProgress(progress());
}

void WbDownloader::displayPopUp() {
  if (gDownloading) {
    WbApplication::instance()->setWorldLoadingStatus(tr("Downloading assets"));
    gDisplayPopUp = true;
  }

  if (gTimer) {
    delete gTimer;
    gTimer = NULL;
  }
}
