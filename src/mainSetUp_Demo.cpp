#include "mainSetUp_Demo.h"
#include "defines_Demo.h"
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>

/**
 * Retrieve the path of the folder containing the qm files (translations).
 * At running time, we look at all the possible app setting folder until founding one called APP_TRANSLATIONS_FOLDER, and having qm files.
 * At building time, the qm files has to be in ./BUILD_TIME_SHARE_DIR/APP_TRANSLATIONS_FOLDER
 * @return Path of the folder if found. Empty string if not.
 */
QString qmFilesFolder()
{
    QString sQmFilesPath;
    QString sErrMsg;

    #if USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS
        sQmFilesPath = QApplication::applicationDirPath();
        sQmFilesPath.append(QString("/") + BUILD_TIME_SHARE_DIR + QString("/") + APP_TRANSLATIONS_FOLDER);
        return sQmFilesPath;

    #else
        // we iterate on the different path of AppDataLocation send by qt
        QDir qmFilesDirectory;
        QStringList shareDirList = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        for (int i = 0 ; i < shareDirList.count() ; ++i)
        {
            qmFilesDirectory.setPath( shareDirList[i] + QString("/") + APP_TRANSLATIONS_FOLDER );

            if (qmFilesDirectory.exists() == true)
            {
                // qm file research
                QStringList langFiles = qmFilesDirectory.entryList(QStringList("*.qm"));
                if (langFiles.count() > 0) // we found the good folder
                {
                    return qmFilesDirectory.absolutePath();

                }
            }
        }
        // if not found:
        return "";

    #endif // USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS
}


/**
 * To be able to run properly, QtLanguageManager needs some configurations,
 * such as qm files, settings object...
 * @param [in] languageManager The QtLanguageManager to be configured.
 * @return true if success
 */
bool configureLanguageManager(LlmQtLocaleLanguageManager & languageManager)
{
    QString sQmFilesPath;
    QString sErrMsg;

    languageManager.setSourceLanguage(QLocale::English);
    // alternative to setSourceLanguage:
    //languageManager.setSourceLanguageFromQAppLocalSettings();

    languageManager.setKeySettingsTranslateLanguage(SETTINGS_KEY_LOCALE_LANGUAGE);
    languageManager.setQmFilesDirectory(qmFilesFolder());

    if (languageManager.analyseAvailableLanguages(&sErrMsg) == false)
    {
        QMessageBox::warning(nullptr, "Warning", sErrMsg + "\nIs the application correctly installed?");
        return false;
    }

    if (languageManager.translateAppToUserSettingsLanguage(&sErrMsg) == false)
    {
        QMessageBox::warning(nullptr, "Warning", sErrMsg);
        return false;
    }

    return true;
}







