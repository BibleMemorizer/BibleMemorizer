/*
 * Copyright (c) 2005-2009 Jeremy Erickson
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "core/settings.h"
#include <qstring.h>
#include <qsettings.h>
#include <qdir.h>

namespace bmemcore
{

QSettings& Settings::qSettings()
{
    static QSettings settings;
    return settings;
}

void Settings::setPluginDir(const QString& dir)
{
    qSettings().setValue("/plugin/directory",
            QDir::cleanPath(dir));
}

QString Settings::getPluginDir()
{
    return qSettings().value("/plugin/directory", "").toString();
}

void Settings::setPluginFile(const QString& file)
{
    qSettings().setValue("/plugin/file",
            QDir::cleanPath(file));
}

QString Settings::getPluginFile()
{
    return qSettings().value("/plugin/file", "").toString();
}

void Settings::setLastFile(const QString& file)
{
    qSettings().setValue("/session/file",
            QDir::cleanPath(file));
}

QString Settings::getLastFile()
{
    return qSettings().value("/session/file", "").toString();
}

void Settings::setWebBrowser(const QString& browser)
{
    qSettings().setValue("/help/browser",
            QDir::cleanPath(browser));
}

QString Settings::getWebBrowser()
{
    return qSettings().value("/help/browser", "").toString();
}

void Settings::setHelpDir(const QString& dir)
{
    qSettings().setValue("/help/directory",
            QDir::cleanPath(dir));
}

QString Settings::getHelpDir()
{
    return qSettings().value("/help/directory", "").toString();
}

void Settings::setDefaultTranslation(const QString& translation)
{
    qSettings().setValue("/general/default_translation",
            translation);
}

QString Settings::getDefaultTranslation()
{
    return qSettings().value("/general/default_translation",
            "").toString();
}

void Settings::setOpenLastFile(bool use)
{
    qSettings().setValue("/session/openlast", use);
}

bool Settings::getOpenLastFile()
{
    return qSettings().value("/session/openlast", true).toBool();
}

void Settings::setCountPuncErrors(bool count)
{
    qSettings().setValue("/quiz/puncerror", count);
}

bool Settings::getCountPuncErrors()
{
    return qSettings().value("/quiz/puncerror", false).toBool();
}

void Settings::setCountCapErrors(bool count)
{
    qSettings().setValue("/quiz/caperror", count);
}

bool Settings::getCountCapErrors()
{
    return qSettings().value("/quiz/caperror", false).toBool();
}

void Settings::setCountSpellingErrors(bool count)
{
    qSettings().setValue("/quiz/spellingerror", count);
}

bool Settings::getCountSpellingErrors()
{
    return qSettings().value("/quiz/spellingerror",
            false).toBool();
}

}
