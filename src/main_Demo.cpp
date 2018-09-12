#include "defines_Demo.h"
#include "mainSetUp_Demo.h"
#include <QApplication>
#include <QSettings>
//#include <QStandardPaths>
//#include <QMessageBox>
#include <memory>
#include "ui/MainWindow_Demo.h"
#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"



/**
 * QtLanguageManager demonstration
 * @param argc Not Used
 * @param argv Not Used
 * @return 0 if success
 */
int main(int argc, char *argv[])
{

    // Application configuration

    //LlmQtLocaleLanguageManager lm = &LlmQtLocaleLanguageManager::instance();
    QApplication app(argc, argv);

    LlmQtLocaleLanguageManager languageManager;

    app.setApplicationName(QLatin1String(APPLICATION_NAME));
    app.setOrganizationName(QLatin1String(ORGANIZATION_NAME));
    QLocale::setDefault(QLocale(QLocale::English, QLocale::Belgium));
    // Choosing Belgium, the nativeLanguageName is just "English".
    // Otherwise it can be "American English", "British English"...


    QSettings::setDefaultFormat(QSettings::IniFormat );

#if USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS
    QString sUserSettingsDir = QApplication::applicationDirPath();
    sUserSettingsDir.append(QString("/") + BUILD_TIME_USER_SETTINGS);
    QString sAppSettingsDir = QApplication::applicationDirPath();
    sAppSettingsDir.append(QString("/") + BUILD_TIME_SHARE_DIR);

    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, sAppSettingsDir);
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, sAppSettingsDir);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, sUserSettingsDir);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, sUserSettingsDir);
#endif // USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS

    // Creation of the QSettings in which user's language settings will be stored. (optional step)
    std::shared_ptr<QSettings> userSettings = std::make_shared<QSettings>(QSettings::IniFormat, QSettings::UserScope, app.organizationName(), app.applicationName());
    languageManager.setSettings(userSettings);

    //QtLanguageManager needs some configurations:
    if (configureLanguageManager(languageManager) == false)
    {
        // If languageManager's configuration is not a success, we leave the application running anyway
        // The languageManager.isUsable() method indicate that the language settings menu has to be disabled
    }

    MainWindow_Demo mw(&languageManager);
    mw.show();

    return app.exec();
}