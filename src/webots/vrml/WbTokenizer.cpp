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

#include "WbTokenizer.hpp"

#include "WbApplicationInfo.hpp"
#include "WbLog.hpp"
#include "WbProtoTemplateEngine.hpp"
#include "WbToken.hpp"

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#include <cassert>

static WbVersion cWorldFileVersion;

const WbVersion &WbTokenizer::worldFileVersion() {
  return cWorldFileVersion;
}

WbTokenizer::WbTokenizer() :
  mFileType(UNKNOWN),
  mFileVersion(WbApplicationInfo::version()),
  mStream(NULL),
  mLine(1),
  mColumn(0),
  mTokenLine(1),
  mTokenColumn(-1),
  mIndex(-1),
  mAtEnd(false),
  mErrorOffset(0) {
}

WbTokenizer::~WbTokenizer() {
  qDeleteAll(mVector);
}

void WbTokenizer::skipToken(const char *expectedWord) {
  if (!hasMoreTokens()) {
    reportError(QObject::tr("Expected '%1' but reached the end of the file").arg(expectedWord), lastToken());
    throw 0;
  }

  WbToken *token = nextToken();

  if (token->word() != expectedWord) {
    reportError(QObject::tr("Expected '%1' but found '%2'").arg(expectedWord).arg(token->word()), token);
    throw 0;
  }
}

const QString &WbTokenizer::nextWord() {
  return nextToken()->word();
}

WbToken *WbTokenizer::lastToken() const {
  assert(mIndex > 0);
  if (mIndex > 0)
    return mVector[mIndex - 1];
  else
    return NULL;
}

const QString &WbTokenizer::lastWord() const {
  static QString emptyWord;
  if (lastToken())
    return lastToken()->word();
  else
    return emptyWord;
}

const QString &WbTokenizer::peekWord() const {
  return peekToken()->word();
}

void WbTokenizer::markTokenStart() {
  mTokenLine = mLine;
  mTokenColumn = mColumn;
}

void WbTokenizer::displayHeaderHelp(QString fileName, QString headerTag) {
  const WbVersion &v = WbApplicationInfo::version();
  WbLog::info(
    QObject::tr("Please modify the first line of '%1' to \"#%2 %3 utf8\".").arg(fileName).arg(headerTag).arg(v.toString(false)),
    false, WbLog::PARSING);
}

bool WbTokenizer::readFileInfo(bool headerRequired, bool displayWarning, QString headerTag, bool isProto) {
  // reset version
  const WbVersion &webotsVersion = WbApplicationInfo::version();
  mFileVersion = webotsVersion;

  // store all the comments into mInfo
  while (true) {
    qint64 savedPos = mStream->pos();
    QString line = readLine();
    if (line.startsWith('#')) {
      line = line.mid(1).trimmed();  // remove '#' and whitespace at the beginning and end
      mInfo.append(line + '\n');
    } else {
      mStream->seek(savedPos);
      mLine--;        // one extra line was read
      mInfo.chop(1);  // remove last '\n'
      break;
    }
  }

  // this step can be removed when Lua support is dropped, but is necessary for two different tokens to cohexist as tokenizer
  // functions like ReadWord need to adapt the tokens to the context.
  if (isProto) {
    bool isLua = true;
    QStringList splittedInfo = mInfo.split('\n');
    for (int i = 0; i < splittedInfo.size(); ++i) {
      if (splittedInfo[i].toLower().startsWith("template language") && splittedInfo[i].toLower().contains("javascript"))
        isLua = false;
    }

    if (isLua) {
      WbProtoTemplateEngine::setOpeningToken(QString("%{"));
      WbProtoTemplateEngine::setClosingToken(QString("}%"));
    } else {
      WbProtoTemplateEngine::setOpeningToken(QString("%<"));
      WbProtoTemplateEngine::setClosingToken(QString(">%"));
    }
  }

  // empty info case
  if (mInfo.isEmpty()) {
    if (headerRequired) {
      WbLog::error(QObject::tr("'%1': error: Missing header.").arg(mFileName), false, WbLog::PARSING);
      displayHeaderHelp(mFileName, headerTag);
      return false;
    } else
      return true;
  }
  // get the first line
  QStringList splittedInfo = mInfo.split('\n');
  QString header = splittedInfo[0];

  // check if the first line is an header

  // matches examples:
  //   "#VRML_SIM R2018a utf8"
  //   "#VRML_SIM V6.0 utf8"
  //   "#VRML V2.0 utf8"
  //   "#VRML_OBJ V7.1.2"
  // notes:
  //   - support new versioning format Webots R2018a
  //   - versions with 3 parts was introduced in Webots 7.2.5
  //   - OBJECT headers without ' utf8' were present before Webots 7.2.5
  bool found = mFileVersion.fromString(header, "^VRML(_...|) V?", "( utf8|)$", 1);
  if (found) {
    if (mFileType == WORLD)
      cWorldFileVersion = mFileVersion;
    // remove the header and trim whitespaces from mInfo
    mInfo.clear();
    for (int i = 1; i < splittedInfo.size(); ++i)
      mInfo.append(splittedInfo[i].trimmed() + '\n');
    mInfo.chop(1);  // remove last '\n'

    // do a forward compatibility test based on the file and webots versions without the maintenance id
    WbVersion forwardCompatiblityFileVersion = mFileVersion;
    forwardCompatiblityFileVersion.setRevision(0);
    WbVersion forwardCompatiblityWebotsVersion = webotsVersion;
    forwardCompatiblityWebotsVersion.setRevision(0);
    const WbVersion r2021b(2021, 1, 0);
    if (forwardCompatiblityFileVersion > forwardCompatiblityWebotsVersion)
      WbLog::warning(QObject::tr("'%1': This file was created by Webots %2 while you are using Webots %3. "
                                 "Forward compatibility may not work.")
                       .arg(mFileName)
                       .arg(mFileVersion.toString())
                       .arg(webotsVersion.toString()),
                     false, WbLog::PARSING);
    else if (forwardCompatiblityFileVersion < r2021b && forwardCompatiblityWebotsVersion >= r2021b)
      WbLog::warning(
        QObject::tr("'%1': This file was created with Webots %2 while you are using Webots %3. "
                    "You may need to adjust urls for textures and meshes, see details in the change log of Webots R2021b.")
          .arg(mFileName)
          .arg(mFileVersion.toString())
          .arg(webotsVersion.toString()),
        false, WbLog::PARSING);

    return true;
  } else {
    if (headerRequired) {
      WbLog::error(QObject::tr("'%1': Invalid header.").arg(mFileName), false, WbLog::PARSING);
      displayHeaderHelp(mFileName, headerTag);
      return false;
    } else {
      if (displayWarning) {
        WbLog::warning(QObject::tr("'%1': Missing header.").arg(mFileName), false, WbLog::PARSING);
        displayHeaderHelp(mFileName, headerTag);
      }
      return true;
    }
  }
}

bool WbTokenizer::checkFileHeader() {
  switch (mFileType) {
    case WORLD:
      return readFileInfo(true, true, "VRML_SIM");
    case OBJECT:
      return readFileInfo(true, true, "VRML_OBJ");
    case MODEL:
      return readFileInfo(false, false, "VRML");
    case PROTO:
      return readFileInfo(false, true, "VRML_SIM", true);
    default:
      return true;
  }
}

QString WbTokenizer::readLine() {
  mLine++;
  mColumn = 0;
  return mStream->readLine();
}

QChar WbTokenizer::readChar() {
  if (mStream->atEnd()) {
    if (!mAtEnd) {
      mAtEnd = true;
      return '\n';
    }
    throw 0;
  }

  QChar c;
  *mStream >> c;
  mColumn++;

  if (c == '\n') {
    mLine++;
    mColumn = 0;
  }

  return c;
}

void WbTokenizer::skipWhiteSpace() {
  while (WbToken::isSpace(mChar) || mChar == '#') {
    // skip comments
    if (mChar == '#') {
      mChar = readChar();
      while (mChar != '\n')
        mChar = readChar();
    } else
      mChar = readChar();
  }
}

QString WbTokenizer::readWord() {
  skipWhiteSpace();

  QString word;
  word.append(mChar);
  markTokenStart();

  // handle string literals
  if (mChar == '"') {
    mChar = readChar();
    // we must find the closing double quotes
    while (mChar != '"') {
      if (mChar == '\\') {
        mChar = readChar();
        if (mChar == 'n')  // '\n' is allowed to create new line in SFString
          word.append('\\');
        else if (mChar != '\\' && mChar != '"')  // only allowed to escape double quotes and backslash
          reportError(QObject::tr("Invalid escaped character"), mLine, mColumn);
      }
      if (mChar == '\n') {
        reportError(QObject::tr("Unclosed string literal"), mTokenLine, mTokenColumn);
        mChar = '"';
        break;
      }
      word.append(mChar);
      mChar = readChar();
    }
    word.append(mChar);
    mChar = readChar();
    return word;
  }

  const QString &open = WbProtoTemplateEngine::openingToken();
  const QString &close = WbProtoTemplateEngine::closingToken();
  // tokenize template code but skip comments
  if (mChar == open[0]) {
    int nOpen = open.size();

    for (int i = 1; i < nOpen; ++i) {
      mChar = readChar();
      word.append(mChar);
      if (mChar != open[i]) {
        reportError(QObject::tr("Unexpected template statement opening. Expected='%1', Received='%2'").arg(open[i]).arg(mChar),
                    mTokenLine, mTokenColumn);
        return word;
      }
    }

    // skip comments:
    // * single line comment starting with '--'
    // * multi line comment between '--[[' and '--]]'
    int commentCharIndex = 0;  // count consecutive '-' characters
    bool shortComment = false;
    bool longComment = false;
    QChar stringStart = 0;
    int finalEscapeCharactersCount = 0;
    while (!word.endsWith(close)) {
      mChar = readChar();

      // short or long comment found
      if (stringStart == 0) {  // ignore comment prefix in strings
        if (commentCharIndex == 2) {
          // sequence '--'
          shortComment = !longComment;
          if (longComment && mChar == ']') {
            mChar = readChar();
            if (mChar == ']') {  // sequence '--]]'
              longComment = false;
              mChar = readChar();
            }
          } else if (!longComment && mChar == '[') {
            mChar = readChar();
            if (mChar == '[') {  // sequence '--[['
              longComment = true;
              mChar = readChar();
            }
          }

          if (shortComment) {
            word.remove(-2, 2);  // remove '--'
            if (longComment)
              shortComment = false;
          }
          commentCharIndex = 0;
        }

        if (!shortComment && mChar == '-')
          ++commentCharIndex;
        else
          // count consecutive '-' characters only
          commentCharIndex = 0;
      }

      if (!shortComment && !longComment) {
        if (stringStart == mChar && finalEscapeCharactersCount % 2 == 0)
          stringStart = 0;
        else if (stringStart == 0 && (mChar == "'" || mChar == "\""))
          stringStart = mChar;
        if (mChar == "\\")
          finalEscapeCharactersCount += 1;
        else
          finalEscapeCharactersCount = 0;
        word.append(mChar);
      }

      if (shortComment && mChar == '\n')
        shortComment = false;
    }
  }

  // handle "[]{}"
  if (WbToken::isPunctuation(mChar)) {
    mChar = readChar();
    return word;
  }

  mChar = readChar();

  while (!WbToken::isSpace(mChar) && !WbToken::isPunctuation(mChar) && mChar != '#') {
    word.append(mChar);
    mChar = readChar();
  }

  return word;
}

int WbTokenizer::tokenize(const QString &fileName) {
  mFileName = fileName;
  mFileType = fileTypeFromFileName(fileName);
  mIndex = 0;

  QFile file(mFileName);
  if (!file.open(QIODevice::ReadOnly)) {
    WbLog::error(QObject::tr("Could not open file: '%1'.").arg(mFileName), false, WbLog::PARSING);
    return 1;
  }

  mStream = new QTextStream(&file);
  if (mStream->atEnd()) {
    WbLog::error(QObject::tr("File is empty: '%1'.").arg(mFileName), false, WbLog::PARSING);
    return 1;
  }

  // check .wbt or .wbo header
  if (!checkFileHeader())
    return 1;

  int errors = 0;
  try {
    mChar = readChar();
    while (true) {
      QString word = readWord();
      WbToken *token = new WbToken(word, mTokenLine, mTokenColumn);
      mVector.append(token);
      if (!token->isValid()) {
        reportError(QObject::tr("Invalid token \"%1\"").arg(token->word()), token);
        errors++;
      }
    }
  } catch (...) {
    // reached end of file
  }

  // add EOF token for parser
  mVector.append(new WbToken(mTokenLine, mTokenColumn));

  delete mStream;

  return errors;
}

int WbTokenizer::tokenizeString(const QString &string) {
  mIndex = 0;

  mStream = new QTextStream(string.toUtf8());
  if (mStream->atEnd()) {
    WbLog::error(QObject::tr("File is empty: '%1'.").arg(mFileName), false, WbLog::PARSING);
    return 1;
  }

  int errors = 0;
  try {
    mChar = readChar();
    while (true) {
      QString word = readWord();
      WbToken *token = new WbToken(word, mTokenLine, mTokenColumn);
      mVector.append(token);
      if (!token->isValid()) {
        reportError(QObject::tr("Invalid token \"%1\"").arg(token->word()), token);
        errors++;
      }
    }
  } catch (...) {
    // reached end of file
  }

  // add EOF token for parser
  mVector.append(new WbToken(mTokenLine, mTokenColumn));

  delete mStream;

  return errors;
}

const QStringList WbTokenizer::tags() const {
  const QStringList lines = mInfo.split("\n");
  foreach (QString line, lines) {
    line.replace(" ", "");
    if (line.startsWith("tags:")) {
      line.remove("tags:");
      return line.split(",");
    }
  }
  return QStringList();
}

const QString WbTokenizer::templateLanguage() const {
  const QStringList lines = mInfo.split("\n");
  foreach (QString line, lines) {
    if (line.startsWith("template language:") && line.toLower().contains("javascript")) {
      return QString("javascript");
    }
  }
  return QString("lua");
}

const QString WbTokenizer::license() const {
  const QStringList lines = mInfo.split("\n");
  foreach (QString line, lines) {
    if (line.startsWith("license:")) {
      line.remove("license:");
      return line;
    }
  }
  return QString();
}

const QString WbTokenizer::licenseUrl() const {
  const QStringList lines = mInfo.split("\n");
  foreach (QString line, lines) {
    if (line.startsWith("license url:")) {
      line.remove("license url:");
      return line;
    }
  }
  return QString();
}

const QString WbTokenizer::documentationUrl() const {
  const QStringList lines = mInfo.split("\n");
  foreach (QString line, lines) {
    if (line.startsWith("documentation url:")) {
      line.remove("documentation url:");
      return line.trimmed();
    }
  }
  return QString();
}

void WbTokenizer::reportError(const QString &message, int line, int column) const {
  QString prefix = mErrorPrefix.isEmpty() ? mFileName : mErrorPrefix;
  if (prefix.isEmpty())
    WbLog::error(QObject::tr("%1.").arg(message), false, WbLog::PARSING);
  else
    WbLog::error(QObject::tr("'%1':%2:%3: error: %4.").arg(prefix).arg(line + mErrorOffset).arg(column).arg(message), false,
                 WbLog::PARSING);
}

void WbTokenizer::reportError(const QString &message, const WbToken *token) const {
  if (!token)
    token = mVector[mIndex - 1];

  reportError(message, token->line(), token->column());
}

void WbTokenizer::reportFileError(const QString &message) const {
  QString prefix = mErrorPrefix.isEmpty() ? mFileName : mErrorPrefix;
  WbLog::error(QObject::tr("'%1': error: %2.").arg(prefix, message), false, WbLog::PARSING);
}

WbTokenizer::FileType WbTokenizer::fileTypeFromFileName(const QString &fileName) {
  if (fileName.endsWith(".wbt"))
    return WORLD;
  else if (fileName.endsWith(".proto"))
    return PROTO;
  else if (fileName.endsWith(".wbo"))
    return OBJECT;
  else if (fileName.endsWith(".wrl"))
    return MODEL;
  else
    return UNKNOWN;
}

void WbTokenizer::skipNode(bool deleteTokens) {
  int startPos = mIndex;
  if (deleteTokens && peekWord() == "{")
    // delete node name
    --startPos;

  // move to next "{"
  while (hasMoreTokens() && nextWord() != "{") {
  }

  if (lastToken()->isEof()) {
    ungetToken();
    return;
  }

  // count the same number of opening and closing braces
  int counter = 1;
  while (counter > 0 && hasMoreTokens()) {
    const QString &word = nextWord();
    if (word == "{")
      counter++;
    else if (word == "}")
      counter--;
  }

  if (deleteTokens) {
    int count = mIndex - startPos;
    mVector.remove(startPos, count);
    mIndex = startPos;
  }
}

void WbTokenizer::skipField(bool deleteTokens) {
  if (!hasMoreTokens()) {
    reportError(QObject::tr("End of file reached while a token is expected"), lastToken());
    throw 0;
  }

  // skip node
  if (peekWord() == "USE" || peekWord() == "IS") {
    if (deleteTokens) {
      --mIndex;
      mVector.remove(mIndex, 3);
    } else {
      nextToken();
      nextToken();
    }
    return;
  }

  // skip node
  if (peekToken()->isIdentifier() || peekWord() == "DEF") {
    skipNode(deleteTokens);
    // remove field name
    --mIndex;
    mVector.remove(mIndex);
    return;
  }

  // skip unknown multiple value
  if (peekWord() == "[") {
    int startPos = mIndex - 1;
    nextToken();
    int counter = 1;
    do {
      const QString &word = nextWord();
      if (word == "[")
        counter++;
      else if (word == "]")
        counter--;
    } while (counter > 0);

    if (deleteTokens) {
      mVector.remove(startPos, mIndex - startPos);
      mIndex = startPos;
    }

    return;
  }

  // skip unknown single value
  int startPos = mIndex - 1;
  while (peekToken()->isNumeric() || peekToken()->isString() || peekToken()->isBoolean())
    nextToken();

  if (deleteTokens) {
    mVector.remove(startPos, mIndex - startPos);
    mIndex = startPos;
  }
}
