/**
 * Copyright 2016 Albert Vaca <albertvaka@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "clipboardlistener.h"

ClipboardListener::ClipboardListener()
    : clipboard(QGuiApplication::clipboard())
{
#ifdef Q_OS_MAC
    connect(&m_clipboardMonitorTimer, &QTimer::timeout, this, [this](){ updateClipboard(QClipboard::Clipboard); });
    m_clipboardMonitorTimer.start(1000);    // Refresh 1s
#endif
    connect(clipboard, &QClipboard::changed, this, &ClipboardListener::updateClipboard);
}

void ClipboardListener::updateClipboard(QClipboard::Mode mode)
{
    if (mode != QClipboard::Clipboard) {
        return;
    }

    QString content = clipboard->text();

    if (content == m_currentContent) {
        return;
    }
    m_updateTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_currentContent = content;

    Q_EMIT clipboardChanged(content);
}

QString ClipboardListener::currentContent()
{
    return m_currentContent;
}

qint64 ClipboardListener::updateTimestamp(){

    return m_updateTimestamp;
}

void ClipboardListener::setText(const QString& content)
{
    m_updateTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_currentContent = content;
    clipboard->setText(content);
}
