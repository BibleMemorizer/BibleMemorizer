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
    static bool needsInitializing = true;
    if (needsInitializing)
    {
        settings.setPath("biblememorizer.sf.net", "BibleMemorizer",
                QSettings::User);
        needsInitializing = false;
    }
    return settings;
}

void Settings::setPluginDir(const QString& dir)
{
    qSettings().writeEntry("/biblememorizer/plugin/directory",
            QDir::cleanDirPath(dir));
}

QString Settings::getPluginDir()
{
    return qSettings().readEntry("/biblememorizer/plugin/directory", "");
}

void Settings::setPluginFile(const QString& file)
{
    qSettings().writeEntry("/biblememorizer/plugin/file",
            QDir::cleanDirPath(file));
}

QString Settings::getPluginFile()
{
    return qSettings().readEntry("/biblememorizer/plugin/file", "");
}

void Settings::setLastFile(const QString& file)
{
    qSettings().writeEntry("/biblememorizer/session/file",
            QDir::cleanDirPath(file));
}

QString Settings::getLastFile()
{
    return qSettings().readEntry("/biblememorizer/session/file", "");
}

void Settings::setWebBrowser(const QString& browser)
{
    qSettings().writeEntry("/biblememorizer/help/browser",
            QDir::cleanDirPath(browser));
}

QString Settings::getWebBrowser()
{
    return qSettings().readEntry("/biblememorizer/help/browser", "");
}

void Settings::setHelpDir(const QString& dir)
{
    qSettings().writeEntry("/biblememorizer/help/directory",
            QDir::cleanDirPath(dir));
}

QString Settings::getHelpDir()
{
    return qSettings().readEntry("/biblememorizer/help/directory", "");
}

void Settings::setDefaultTranslation(const QString& translation)
{
    qSettings().writeEntry("/biblememorizer/general/default_translation",
            translation);
}

QString Settings::getDefaultTranslation()
{
    return qSettings().readEntry("/biblememorizer/general/default_translation",
            "");
}

void Settings::setOpenLastFile(bool use)
{
    qSettings().writeEntry("/biblememorizer/session/openlast", use);
}

bool Settings::getOpenLastFile()
{
    return qSettings().readBoolEntry("/biblememorizer/session/openlast", true);
}

void Settings::setCountPuncErrors(bool count)
{
    qSettings().writeEntry("/biblememorizer/quiz/puncerror", count);
}

bool Settings::getCountPuncErrors()
{
    return qSettings().readBoolEntry("/biblememorizer/quiz/puncerror", false);
}

void Settings::setCountCapErrors(bool count)
{
    qSettings().writeEntry("/biblememorizer/quiz/caperror", count);
}

bool Settings::getCountCapErrors()
{
    return qSettings().readBoolEntry("/biblememorizer/quiz/caperror", false);
}

void Settings::setCountSpellingErrors(bool count)
{
    qSettings().writeEntry("/biblememorizer/quiz/spellingerror", count);
}

bool Settings::getCountSpellingErrors()
{
    return qSettings().readBoolEntry("/biblememorizer/quiz/spellingerror",
            false);
}

}
