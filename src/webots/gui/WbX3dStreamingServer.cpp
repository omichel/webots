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

#include "WbX3dStreamingServer.hpp"

#include "WbAnimationRecorder.hpp"
#include "WbHttpReply.hpp"
#include "WbNodeOperations.hpp"
#include "WbProtoList.hpp"
#include "WbProtoModel.hpp"
#include "WbRobot.hpp"
#include "WbSupervisorUtilities.hpp"
#include "WbTemplateManager.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtWebSockets/QWebSocket>

WbX3dStreamingServer::WbX3dStreamingServer(bool monitorActivity, bool disableTextStreams, bool ssl, bool controllerEdit) :
  WbStreamingServer(monitorActivity, disableTextStreams, ssl, controllerEdit),
  mX3dWorldGenerationTime(-1.0) {
  connect(WbNodeOperations::instance(), &WbNodeOperations::nodeDeleted, this, &WbX3dStreamingServer::propagateNodeDeletion);
  connect(WbTemplateManager::instance(), &WbTemplateManager::preNodeRegeneration, this,
          &WbX3dStreamingServer::propagateNodeDeletion);
}

WbX3dStreamingServer::~WbX3dStreamingServer() {
  if (WbAnimationRecorder::isInstantiated())
    WbAnimationRecorder::instance()->cleanupFromStreamingServer();
}

void WbX3dStreamingServer::start(int port) {
  if (WbWorld::instance()) {
    try {
      WbAnimationRecorder::instance()->initFromStreamingServer();
    } catch (const QString &e) {
      WbLog::error(tr("Error when initializing the animation recorder: %1").arg(e));
      return;
    }
  }
  WbStreamingServer::start(port);
}

void WbX3dStreamingServer::stop() {
  // Test that the animation recorder is instanciated.
  // Otherwise, the instance() call can wrongly recreate an instance of the
  // animation recorder in the cleanup routines.
  if (WbAnimationRecorder::isInstantiated())
    WbAnimationRecorder::instance()->cleanupFromStreamingServer();
  WbStreamingServer::stop();
}

void WbX3dStreamingServer::create(int port) {
  WbStreamingServer::create(port);
  generateX3dWorld();
}

void WbX3dStreamingServer::sendTcpRequestReply(const QString &requestedUrl, const QString &etag, QTcpSocket *socket) {
  if (!mX3dWorldTextures.contains(requestedUrl))
    WbStreamingServer::sendTcpRequestReply(requestedUrl, etag, socket);
  else
    socket->write(WbHttpReply::forgeFileReply(mX3dWorldTextures[requestedUrl], etag));
}

void WbX3dStreamingServer::processTextMessage(QString message) {
  if (message.startsWith("x3d")) {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    WbLog::info(tr("Streaming server: Client set mode to X3D."));
    mPauseTimeout = message.endsWith(";broadcast") ? -1 : 0;
    if (!WbWorld::instance()->isLoading()) {
      startX3dStreaming(client);
      sendToClients();  // send possible bufferized messages
    }
    // else streaming is started once the world loading is completed
    return;
  } else if (message == "reset") {
    // reset nodes visibility
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    foreach (WbBaseNode *node, WbWorld::instance()->viewpoint()->getInvisibleNodes())
      client->sendTextMessage(QString("visibility:%1:1").arg(node->uniqueId()));
    resetSimulation();
    const QString &state = WbAnimationRecorder::instance()->computeUpdateData(true);
    if (!state.isEmpty()) {
      foreach (QWebSocket *client, mWebSocketClients)
        sendWorldStateToClient(client, state);
    }
    sendToClients("reset finished");
    return;
  } else if (message.startsWith("mjpeg: ")) {
    WbLog::error(tr("Streaming server received unsupported MJPEG message: '%1'. You should run Webots with the "
                    "'--stream=\"mode=mjpeg\"' command line option.")
                   .arg(message));
    return;
  }
  WbStreamingServer::processTextMessage(message);
}

void WbX3dStreamingServer::startX3dStreaming(QWebSocket *client) {
  try {
    if (WbWorld::instance()->isModified() || mX3dWorldGenerationTime != WbSimulationState::instance()->time())
      generateX3dWorld();
    sendWorldToClient(client);
    // send the current simulation state to the newly connected client
    const QString &stateMessage = simulationStateString();
    if (!stateMessage.isEmpty())
      client->sendTextMessage(stateMessage);
  } catch (const QString &e) {
    WbLog::error(tr("Streaming server: Cannot send world date to client [%1] because: %2.").arg(clientToId(client)).arg(e));
  }
}

void WbX3dStreamingServer::sendUpdatePackageToClients() {
  sendActivityPulse();

  if (mWebSocketClients.size() > 0) {
    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (mLastUpdateTime < 0.0 || currentTime - mLastUpdateTime >= 1000.0 / WbWorld::instance()->worldInfo()->fps()) {
      const QString &state = WbAnimationRecorder::instance()->computeUpdateData(false);
      if (!state.isEmpty()) {
        foreach (QWebSocket *client, mWebSocketClients) {
          sendWorldStateToClient(client, state);
          pauseClientIfNeeded(client);
        }
      }
      mLastUpdateTime = currentTime;
    }
  }
}

bool WbX3dStreamingServer::prepareWorld() {
  try {
    generateX3dWorld();
    foreach (QWebSocket *client, mWebSocketClients)
      sendWorldToClient(client);
    WbAnimationRecorder::instance()->initFromStreamingServer();
  } catch (const QString &e) {
    WbLog::error(tr("Error when reloading world: %1.").arg(e));
    destroy();
    return false;
  }

  return true;
}

void WbX3dStreamingServer::deleteWorld() {
  if (!isActive())
    return;
  WbAnimationRecorder::instance()->cleanupFromStreamingServer();
  WbStreamingServer::deleteWorld();
}

void WbX3dStreamingServer::propagateNodeAddition(WbNode *node) {
  if (!isActive() || WbWorld::instance() == NULL)
    return;

  if (node->isProtoParameterNode()) {
    // PROTO parameter nodes are not exported to X3D or transmitted to webots.min.js
    foreach (WbNode *nodeInstance, node->protoParameterNodeInstances())
      propagateNodeAddition(nodeInstance);
    return;
  }

  WbStreamingServer::propagateNodeAddition(node);

  const WbBaseNode *baseNode = static_cast<WbBaseNode *>(node);
  if (baseNode && baseNode->isInBoundingObject())
    return;

  if (!mWebSocketClients.isEmpty()) {
    QString nodeString;
    WbVrmlWriter writer(&nodeString, node->modelName() + ".x3d");
    node->write(writer);

    mX3dWorldTextures.insert(writer.texturesList());

    foreach (QWebSocket *client, mWebSocketClients)
      // add root <nodes> element to handle correctly multiple root elements like in case of PBRAppearance node.
      client->sendTextMessage(QString("node:%1:<nodes>%2</nodes>").arg(node->parentNode()->uniqueId()).arg(nodeString));
  }
}

void WbX3dStreamingServer::propagateNodeDeletion(WbNode *node) {
  if (!isActive() || WbWorld::instance() == NULL)
    return;

  foreach (QWebSocket *client, mWebSocketClients)
    client->sendTextMessage(QString("delete:%1").arg(node->uniqueId()));
}

void WbX3dStreamingServer::generateX3dWorld() {
  const WbWorld *world = WbWorld::instance();
  if (!world)
    return;

  QString worldString;
  WbVrmlWriter writer(&worldString, QFileInfo(world->fileName()).baseName() + ".x3d");
  world->write(writer);
  mX3dWorld = worldString;
  mX3dWorldTextures = writer.texturesList();
  mX3dWorldGenerationTime = WbSimulationState::instance()->time();
  mLastUpdateTime = -1.0;
}

void WbX3dStreamingServer::sendWorldToClient(QWebSocket *client) {
  const qint64 ret = client->sendTextMessage(QString("model:") + mX3dWorld);
  if (ret < mX3dWorld.size())
    throw tr("Cannot sent the entire world");

  const QString &state = WbAnimationRecorder::instance()->computeUpdateData(true);
  if (!state.isEmpty())
    sendWorldStateToClient(client, state);

  WbStreamingServer::sendWorldToClient(client);
}

void WbX3dStreamingServer::sendWorldStateToClient(QWebSocket *client, const QString &state) const {
  client->sendTextMessage(QString("application/json:") + state);
}
