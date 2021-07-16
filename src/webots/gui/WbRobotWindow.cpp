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

#include "WbRobotWindow.hpp"

#include "WbApplicationInfo.hpp"
#include "WbLog.hpp"
#include "WbProject.hpp"
#include "WbRobot.hpp"
#include "WbRobotWindowTransportLayer.hpp"
#include "WbStandardPaths.hpp"
#include "WbVersion.hpp"
#include "WbWebPage.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QRegularExpression>
#include <QtCore/QTextStream>
#include <QtCore/QUrl>

#ifdef _WIN32
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebView>
#else
#include <QtWebChannel/QWebChannel>
#include <QtWebEngineWidgets/QWebEngineView>
#endif

// Debug code: uncomment to show a web inspector for QtWebKit.
// #include <QtWebKitWidgets/QWebInspector>
// #include <QtWidgets/QDialog>
// #include <QtWidgets/QVBoxLayout>

WbRobotWindow::WbRobotWindow(WbRobot *robot, QWidget *parent) :
  WbDockWidget(parent),
  mRobot(robot),
  mWebView(NULL),
  mResetCount(0),
  mLoaded(false) {
  QString title = "Robot: " + robot->name();
  setWindowTitle(title);
  setTabbedTitle(title);
  setObjectName("HtmlRobotWindow");

  const QString &windowFileName = robot->windowFile("html");
  if (windowFileName.isEmpty()) {
    robot->parsingWarn(tr("No dockable HTML robot window is set in the 'window' field."));
    return;
  }

  mWebView = new QWebView(this);
  setWidget(mWebView);
  setupPage();

#ifndef _WIN32
  connect(mWebView, &QWebView::loadFinished, this, &WbRobotWindow::notifyLoadCompleted);
#endif
  connect(robot, &WbRobot::sendToJavascript, this, &WbRobotWindow::sendToJavascript);
  connect(robot, &WbRobot::controllerChanged, this, &WbRobotWindow::setupPage);
}

WbRobotWindow::~WbRobotWindow() {
  if (mWebView)
    delete mWebView->page();
  delete mWebView;
}

void WbRobotWindow::setupPage() {
  assert(mWebView);
  mLoaded = false;

  if (mWebView->page())
    delete mWebView->page();

  mWebView->setPage(new WbWebPage());

  // Debug code: uncomment to show a web inspector.
  // mWebView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
  // QWebInspector *inspector = new QWebInspector(this);
  // connect(this, &QObject::destroyed, inspector, &QObject::deleteLater);
  // inspector->setPage(mWebView->page());
  // QDialog *dialog = new QDialog(this, Qt::Tool);
  // QVBoxLayout *layout = new QVBoxLayout(dialog);
  // layout->addWidget(inspector);
  // dialog->show();

  const QString &windowFileName = mRobot->windowFile("html");
  if (windowFileName.isEmpty()) {
    mRobot->parsingWarn(tr("No dockable HTML robot window is set in the 'window' field."));
    return;
  }
  QFile htmlFile(windowFileName);
  const QUrl htmlUrl = QUrl::fromLocalFile(htmlFile.fileName());
  if (htmlFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream htmlInput(&htmlFile);

    QString prependToHead = linkTag(WbStandardPaths::resourcesPath() + "web/local/webots.css");
#ifdef __APPLE__
    // Chromium bug on macOS:
    // - warnings like the following one are displayed in the console: "2018-05-08 11:17:37.496
    // QtWebEngineProcess[70885:9694859] Couldn't set selectedTextBackgroundColor from default ()"
    // - reference: https://bugs.chromium.org/p/chromium/issues/detail?id=641509
    prependToHead += "<style>::selection { background: #b2d7fe; }</style>";
#endif
    const QString prependToBody =
#ifndef _WIN32
      scriptTag(WbStandardPaths::resourcesPath() + "web/local/qwebchannel.js") +
#endif
      scriptTag(WbStandardPaths::resourcesPath() + "web/local/webots.js");
    QString content;
    const QRegularExpression script("<script[^>]*src=[\"']([^\"'>]*)[\"']");
    const QRegularExpression link("<link[^>]*href=[\"']([^\"'>]*)[\"']");
    while (!htmlInput.atEnd()) {
      QString line = htmlInput.readLine();
      if (line.contains("<head>"))
        line += '\n' + prependToHead;
      else if (line.contains("<body>"))
        line += '\n' + prependToBody;
      else {
        QRegularExpressionMatch match = script.match(line);
        if (!match.hasMatch())
          match = link.match(line);
        if (match.hasMatch()) {
          const QString oldUrl = match.captured(1);
          const QString newUrl =
            formatUrl(oldUrl) + "?" + QString::number(mRobot->uniqueId()) + QString("_%1").arg(mResetCount);
          line.remove(match.capturedStart(1), oldUrl.length());
          line.insert(match.capturedStart(1), newUrl);
        }
      }
      content += line + '\n';
    }
    mResetCount++;
    mWebView->setHtml(content, htmlUrl);
    htmlFile.close();
  } else
    WbLog::warning(tr("Unable to load %1.").arg(htmlFile.fileName()));

#ifdef _WIN32
  mFrame = mWebView->page()->mainFrame();
  mFrame->addToJavaScriptWindowObject("_webots", this);
#else
  QWebChannel *channel = new QWebChannel(mWebView->page());
  mWebView->page()->setWebChannel(channel);
  mTransportLayer = new WbRobotWindowTransportLayer(mWebView->page());
  channel->registerObject("_webots", mTransportLayer);
  connect(mTransportLayer, &WbRobotWindowTransportLayer::ackReceived, this, &WbRobotWindow::notifyAckReceived);
  connect(mTransportLayer, &WbRobotWindowTransportLayer::javascriptReceived, mRobot, &WbRobot::receiveFromJavascript);
  connect(mTransportLayer, &WbRobotWindowTransportLayer::titleSet, this, &WbRobotWindow::setTitle);
#endif
}

void WbRobotWindow::startControllerIfNeeded() {
  if (!mRobot->isControllerStarted())
    mRobot->startController();
  mRobot->updateControllerWindow();
}

void WbRobotWindow::show() {
  startControllerIfNeeded();
  WbDockWidget::show();
}

QString WbRobotWindow::formatUrl(const QString &urlString) {
  static QStringList repoUrls;
  if (repoUrls.isEmpty()) {
    // forge the GitHub URL to the current version of Webots
    const QString webotsVersion = WbApplicationInfo::version().toString().replace(" revision ", "-rev");
    repoUrls << "https://raw.githubusercontent.com/cyberbotics/webots/" + webotsVersion + "/"
             << "https://cdn.jsdelivr.net/gh/cyberbotics/webots@" + webotsVersion + "/";
  }

  QString urlStringExtented(urlString);
  foreach (const QString url, repoUrls) {
    if (urlString.startsWith(url)) {
      // load local file instead of the online version for the current version of Webots
      urlStringExtented.replace(url, WbStandardPaths::webotsHomePath());
      break;
    }
  }
  const QUrl &url(urlStringExtented);
  const QString &pathString = url.toString(QUrl::RemoveQuery);
  const QFileInfo &fileInfo(pathString);
  if (fileInfo.isAbsolute()) {
    if (fileInfo.isReadable())
      return "file:///" + url.toEncoded();
    return "";
  }

  const QFileInfo &windowInfo(mRobot->windowFile());
  const QFileInfo &absoluteFileInfo(windowInfo.path() + "/" + pathString);
  if (absoluteFileInfo.isReadable())
    return url.toEncoded();

  return urlString;
}

QString WbRobotWindow::linkTag(const QString &file) {
  QString href = formatUrl(file);
  if (href.isEmpty())
    return "";
  return "<link rel='stylesheet' type='text/css' href='" + href + "'>";
}

QString WbRobotWindow::scriptTag(const QString &file) {
  QString src = formatUrl(file);
  if (src.isEmpty())
    return "";
  return "<script src='" + src + "'></script>";
}

#ifndef _WIN32
void WbRobotWindow::notifyLoadCompleted() {
  mLoaded = true;
  if (!mWaitingSentMessages.isEmpty()) {
    foreach (const QString &message, mWaitingSentMessages)
      runJavaScript(message);
    mWaitingSentMessages.clear();
  }
}

void WbRobotWindow::notifyAckReceived() {
  mRobot->setWaitingForWindow(false);
}

void WbRobotWindow::runJavaScript(const QString &message) {
  mTransportLayer->requestAck();
  mWebView->page()->runJavaScript("webots.Window.receive('" + message + "', '" + escapeString(robot()->name()) + "')");
}
#endif

QString WbRobotWindow::escapeString(const QString &text) {
  QString escaped(text);
  escaped.replace("\\", "\\\\\\\\");
  escaped.replace("'", "\\'");
  return escaped;
}

void WbRobotWindow::sendToJavascript(const QByteArray &string) {
  const QString &message(escapeString(string));
#ifdef _WIN32
  mFrame->evaluateJavaScript("webots.Window.receive('" + message + "', '" + escapeString(robot()->name()) + "')");
#else
  mRobot->setWaitingForWindow(true);
  if (mLoaded)
    runJavaScript(message);
  else
    // message will be sent once the robot window loading is completed
    mWaitingSentMessages << message;
#endif
}

#ifdef _WIN32
void WbRobotWindow::receiveFromJavascript(const QByteArray &message) {
  mRobot->receiveFromJavascript(message);
}
#endif

void WbRobotWindow::setTitle(const QString &title, const QString &tabbedTitle) {
  setWindowTitle(title);
  if (tabbedTitle.isEmpty())
    setTabbedTitle(title);
  else
    setTabbedTitle(tabbedTitle);
}
