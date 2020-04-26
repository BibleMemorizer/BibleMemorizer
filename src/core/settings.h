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

#ifndef BMEMCORESETTINGS_H
#define BMEMCORESETTINGS_H

class QSettings;
class QString;

namespace bmemcore
{
class Settings
{
public:

    /** Sets the directory in which plugins are stored.
      * Plugin information files can be in either this directory
      * or any immediate subdirectory.
      * @param dir The plugin directory.
      */
    static void setPluginDir(const QString& dir);
    /** Gets the directory in which the plugins are stored.
      * @return The directory in which to find plugins,
      * or the empty string if unspecified.
      */
    static QString getPluginDir();
    /** Sets the file to use for the plugin.
      * This should be the text information file
      * (typically with a filename ending in ".plugin")
      * rather than the actual library (typically with
      * a filename ending in ".so" or ".dll").
      * The filename should be fully qualified with all
      * directory information.
      * @param file The filename for the plugin information file.
      */
    static void setPluginFile(const QString& file);
    /** Gets the location of the plugin information file.
      * @return The filename of the plugin information file,
      * or the empty string if unspecified.
      */
    static QString getPluginFile();
    /** Sets the last file used by the interface.
      * Used to automatically load the last used file on startup.
      * This may be especially useful for those with only one verse collection.
      * @param file The fully qualified filename of the last file opened.
      */
    static void setLastFile(const QString& file);
    /** Gets the last file used in BibleMemorizer.
      * @return The last-used file,
      * or the empty string if unspecified.
      */
    static QString getLastFile();
    /** Sets the default translation to select for new verses.
      * @param translation The translation to use.
      */
    static void setDefaultTranslation(const QString& translation);
    /** Gets the default translation to select for new verses.
      * @return The default translation to select
      * or the empty string if unspecified.
      */
    static QString getDefaultTranslation();
    /** Sets whether to open the last used file on startup.
      * @param use Whether to open the last used file on startup.
      */
    static void setOpenLastFile(bool use);
    /** Gets whether to open the last used file on startup.
      * @return Whether to open the last used file on startup.
      * Defaults to true.
      */
    static bool getOpenLastFile();
    /** Sets whether punctuation errors count as errors during a quiz.
     *  @param Whether to count minor errors as errors.
     */
    static void setCountPuncErrors(bool count);
    /** Gets whether punctuation errors count as errors during a quiz.
     *  @return Whether to count punctuation errors as errors.
     */
    static bool getCountPuncErrors();
    /** Sets whether capitalization errors count as errors during a quiz.
     *  @param Whether to count punctuation errors as errors.
     */
    static void setCountCapErrors(bool count);
    /** Gets whether capitalization errors count as errors during a quiz.
     *  @return Whether to count punctuation errors as errors.
     */
    static bool getCountCapErrors();
    /** Sets whether spelling errors count as errors during a quiz.
     *  @param Whether to count spelling errors as errors.
     */
    static void setCountSpellingErrors(bool count);
    /** Gets whether spelling errors count as errors during a quiz.
     *  @return Whether to count spelling errors as errors.
     */
    static bool getCountSpellingErrors();
protected:
    /** Gets the QSettings object directly.
      * @return A reference to the QSettings object.
      */
    static QSettings& qSettings();
};
}
#endif
