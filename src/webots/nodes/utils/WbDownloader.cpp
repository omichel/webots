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

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <QtNetwork/QNetworkDiskCache>

#include <iostream>

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
  if (mNetworkReply != NULL)
    mNetworkReply->deleteLater();
}

QIODevice *WbDownloader::device() const {
  return dynamic_cast<QIODevice *>(mNetworkReply);
}

void WbDownloader::download(const QUrl &url) {
  mUrl = url;

  if (gUrlCache.contains(mUrl) && !mIsBackground &&
      (mUrl.toString().endsWith(".png", Qt::CaseInsensitive) || url.toString().endsWith(".jpg", Qt::CaseInsensitive))) {
    if (!(mOffline == true && mCopy == false)) {
      mCopy = true;
      QNetworkReply *reply = gUrlCache[mUrl];
      if (reply) {
        if (reply->isFinished())
          finished();
        else
          connect(reply, &QNetworkReply::finished, this, &WbDownloader::finished, Qt::UniqueConnection);
      } else
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
  }

  gComplete++;
  if (gComplete == gCount) {
    gDownloading = false;
    gDisplayPopUp = false;
    gUrlCache.clear();
    emit WbApplication::instance()->deleteWorldLoadingProgressDialog();
  } else if (gDisplayPopUp)
    emit WbApplication::instance()->setWorldLoadingProgress(progress());

  mFinished = true;
  emit complete();
  if (!mCopy) {
    QVariant isFromCache = mNetworkReply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
    // source is network
    if (!(isFromCache.isValid() && isFromCache.canConvert(QMetaType::Bool) && isFromCache.convert(QMetaType::Bool))) {
      // increase expiration date to one day
      QNetworkCacheMetaData metaData = WbNetwork::instance()->networkAccessManager()->cache()->metaData(mUrl);
      metaData.setExpirationDate(QDateTime::currentDateTimeUtc().addDays(1));
      WbNetwork::instance()->networkAccessManager()->cache()->updateMetaData(metaData);
    }
  }
  QNetworkCacheMetaData test = WbNetwork::instance()->networkAccessManager()->cache()->metaData(mUrl);
  std::cout << test.expirationDate().toString().toStdString() << '\n';
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
