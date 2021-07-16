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

#ifndef WB_DOWNLOADER_HPP
#define WB_DOWNLOADER_HPP

#include <QtCore/QObject>
#include <QtCore/QUrl>

class QNetworkReply;
class QIODevice;

class WbDownloader : public QObject {
  Q_OBJECT
public:
  explicit WbDownloader(QObject *parent = NULL);
  ~WbDownloader();
  void download(const QUrl &url);
  const QUrl &url() const { return mUrl; }
  QIODevice *device() const;
  bool isCopy() const { return mCopy; }
  bool hasFinished() const { return mFinished; }
  void setIsBackground(bool isBackground) { mIsBackground = isBackground; }
  const QString &error() const { return mError; }
  static int progress();
  static void reset();

signals:
  void complete();
  void progress(float progress);

private:
  QUrl mUrl;
  QNetworkReply *mNetworkReply;
  bool mFinished;
  QString mError;
  bool mOffline;
  bool mCopy;
  bool mIsBackground;

private slots:
  void finished();
  static void displayPopUp();
};

#endif
