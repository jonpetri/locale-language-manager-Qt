#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE // LmQt
#include <boost/test/unit_test.hpp>


#include "defines_Demo.h"
#include "mainSetUp_Demo.h"
#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QLocale>
#include <QDir>
#include <QString>

#include <string>
#include <iostream>

#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"
BOOST_AUTO_TEST_SUITE( LlmQtLocaleLanguageManager_suite )


void testLanguageManagerAtResetState(LlmQtLocaleLanguageManager & lm)
{
    BOOST_CHECK_EQUAL(lm.isUsable(), false);
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size(), 0);
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.systemLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}
BOOST_AUTO_TEST_CASE(constructor)
{
    LlmQtLocaleLanguageManager lm;
    testLanguageManagerAtResetState(lm);
}
BOOST_AUTO_TEST_CASE(setSourceLanguage1)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    bRet = lm.setSourceLanguage(QLocale::English);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(lm.isUsable(), false);
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size(), 1);
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true)[0].toStdString(), "English");
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "English");
    BOOST_CHECK_EQUAL(lm.systemLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}

BOOST_AUTO_TEST_CASE(setSourceLanguage2)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    bRet = lm.setSourceLanguage(QLocale::English,QLocale::UnitedStates);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(lm.isUsable(), false);
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size(), 1);
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true)[0].toStdString(), "American English (US)");
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "American English (US)");
    BOOST_CHECK_EQUAL(lm.systemLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}

BOOST_AUTO_TEST_CASE(setSourceLanguage3)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;
    bRet = lm.setSourceLanguage(QLocale::English,QLocale::France, &sErrMsg); // not possible
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    testLanguageManagerAtResetState(lm);
}

BOOST_AUTO_TEST_CASE(setQmFilesDirectory1)
{
    LlmQtLocaleLanguageManager lm;
    lm.setQmFilesDirectory("-");
    testLanguageManagerAtResetState(lm);
}


BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_wrongQmFolder)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;
    lm.setQmFilesDirectory("-");
    bRet = lm.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    testLanguageManagerAtResetState(lm);
}

BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_emptyQmFolder)
{
    int argc=0;
    QApplication app(argc, nullptr);

    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;
    lm.setQmFilesDirectory(QApplication::applicationDirPath()); // existing but without qm file / empty directory
    bRet = lm.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    testLanguageManagerAtResetState(lm);
}
BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_qmFilesBadName)
{
    int argc=0;
    QApplication app(argc, nullptr);
    QDir dataDir(QApplication::applicationDirPath() + "/data/qmFilesBadName");
    BOOST_REQUIRE_MESSAGE(dataDir.exists(), " .qm files have to be in: " + dataDir.absolutePath().toStdString());

    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;
    lm.setQmFilesDirectory(dataDir.absolutePath()); // existing but without qm file / empty directory
    bRet = lm.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    testLanguageManagerAtResetState(lm);
}
BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_realQmFiles)
{
    int argc=0;
    QApplication app(argc, nullptr);
    QDir dataDir(qmFilesFolder());
    BOOST_REQUIRE_MESSAGE(dataDir.exists(), " .qm files have to be in: " + dataDir.absolutePath().toStdString());

    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;
    lm.setQmFilesDirectory(dataDir.absolutePath()); // existing but without qm file / empty directory
    bRet = lm.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sErrMsg.size() , 0);
    BOOST_CHECK_EQUAL(lm.isUsable(), false); // source language missing
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK(lm.languageNameNativeList(true).size() > 0);
    BOOST_CHECK(lm.languageNameNativeList(true)[0].size() > 0 );
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);

    if (lm.isSystemLanguageAvailable())
        BOOST_CHECK(lm.systemLanguageNativeName().size() > 0);
    else
        BOOST_CHECK_EQUAL(lm.systemLanguageNativeName().toStdString(), "");
}

BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_setSourceLanguage1)
{

    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;

    bRet = lm.setSourceLanguage(QLocale::English, QLocale::AnyCountry, &sErrMsg );
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sErrMsg.size() , 0);
    BOOST_CHECK_EQUAL(lm.isUsable(), false); // analyse not done
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size() , 1);
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true)[0].toStdString(), "English");
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "English");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}

BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_setSourceLanguage2)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;

    bRet = lm.setSourceLanguage(QLocale::English, QLocale::UnitedKingdom, &sErrMsg );
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sErrMsg.size() , 0);
    BOOST_CHECK_EQUAL(lm.isUsable(), false); // analyse not done
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size() , 1);
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true)[0].toStdString(), "British English (GB)");
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "British English (GB)");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}
BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_setSourceLanguage_wrong_Lnaguage_country_couple1)
{
    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;

    bRet = lm.setSourceLanguage(QLocale::Basque, QLocale::UnitedKingdom, &sErrMsg );
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    BOOST_CHECK_EQUAL(lm.isUsable(), false); // analyse not done + source language missing
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.languageNameNativeList(true).size() , 0);
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageAvailable(), false);
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
}

BOOST_AUTO_TEST_CASE(analyseAvailableLanguages_setSourceLanguage_wrong_Lnaguage_country_couple2)
{
    int argc=0;
    QApplication app(argc, nullptr);
    QDir dataDir(qmFilesFolder());
    BOOST_REQUIRE(dataDir.exists());

    LlmQtLocaleLanguageManager lm;
    bool bRet;
    QString sErrMsg;



    lm.setQmFilesDirectory(dataDir.absolutePath()); // existing but without qm file / empty directory
    bRet = lm.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, true);

    bRet = lm.setSourceLanguage(QLocale::Basque, QLocale::UnitedKingdom, &sErrMsg );
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    BOOST_CHECK_EQUAL(lm.isUsable(), false); // source language missing
    BOOST_CHECK_EQUAL(lm.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK(lm.languageNameNativeList(true).size() > 0);
    BOOST_CHECK_EQUAL(lm.sourceLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(lm.isSystemLanguageChosenByUser(), false);
    if (lm.isSystemLanguageAvailable())
        BOOST_CHECK(lm.systemLanguageNativeName().size() > 0);
    else
        BOOST_CHECK_EQUAL(lm.systemLanguageNativeName().toStdString(), "");
}


void classicLanguageManagerSetUp(LlmQtLocaleLanguageManager & languageManager, std::shared_ptr<QSettings> &userSettings)
{
    bool bRet;
    QString sErrMsg;

    // Application configuration
    qApp->setApplicationName(APPLICATION_NAME + QString("_TEST"));
    qApp->setOrganizationName(QLatin1String(ORGANIZATION_NAME));
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

    // Creation of the QSettings in which user's language settings will be stored.

     userSettings = std::make_shared<QSettings>(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(),  qApp->applicationName());
     userSettings->setValue(SETTINGS_KEY_LOCALE_LANGUAGE, "");
    languageManager.setSettings(userSettings);


    QString sQmFilesPath;

    bRet = languageManager.setSourceLanguage(QLocale::English);
    BOOST_CHECK_EQUAL(bRet, true);
    languageManager.setKeySettingsTranslateLanguage(SETTINGS_KEY_LOCALE_LANGUAGE);
    languageManager.setQmFilesDirectory(qmFilesFolder());

    bRet = languageManager.analyseAvailableLanguages(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, true);

    BOOST_CHECK_EQUAL( languageManager.currentLanguageNativeName().toStdString(), "");
    BOOST_CHECK_EQUAL(userSettings->value(SETTINGS_KEY_LOCALE_LANGUAGE).toString().toStdString() , "");
}

BOOST_AUTO_TEST_CASE(settings)
{
    bool bRet;
    QString sErrMsg;
    LlmQtLocaleLanguageManager languageManager;
    std::shared_ptr<QSettings> userSettings = nullptr;
    QSettings::setDefaultFormat(QSettings::IniFormat );

    int argc=0;
    QApplication app(argc, nullptr);
    classicLanguageManagerSetUp(languageManager, userSettings);
    std::cout << "INFO: Settings file= " << userSettings->fileName().toStdString() << std::endl;

    bRet = languageManager.translateAppToLanguage("English");
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL( languageManager.currentLanguageNativeName().toStdString() , "English");
    BOOST_CHECK_EQUAL(userSettings->value(SETTINGS_KEY_LOCALE_LANGUAGE).toString().toStdString() , "English");


    // translation to another language, from the settings
    QStringList languageList = languageManager.languageNameNativeList(false);
    BOOST_REQUIRE(languageList.contains("français") );
    userSettings->setValue(SETTINGS_KEY_LOCALE_LANGUAGE, "French");
    bRet =  languageManager.translateAppToUserSettingsLanguage(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sErrMsg.size() , 0);
    BOOST_CHECK_EQUAL( languageManager.currentLanguageNativeName().toStdString() , "français");

    // translation to a non existing language, from the settings
    userSettings->setValue(SETTINGS_KEY_LOCALE_LANGUAGE, "bla");
    bRet =  languageManager.translateAppToUserSettingsLanguage(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
    BOOST_CHECK_EQUAL( languageManager.currentLanguageNativeName().toStdString() , "français");
 }


BOOST_AUTO_TEST_CASE(without_settings)
{
    bool bRet;
    QString sErrMsg;
    LlmQtLocaleLanguageManager languageManager;
    std::shared_ptr<QSettings> userSettings = nullptr;

    int argc=0;
    QApplication app(argc, nullptr);
    classicLanguageManagerSetUp(languageManager, userSettings);

    languageManager.setSettings(nullptr);


    bRet = languageManager.translateAppToLanguage("English");
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL( languageManager.currentLanguageNativeName().toStdString() , "English");
    BOOST_CHECK_EQUAL(userSettings->value(SETTINGS_KEY_LOCALE_LANGUAGE).toString().toStdString() , "");

 }

BOOST_AUTO_TEST_CASE(write_system_in_settings)
{
    bool bRet;
    QString sErrMsg;
    LlmQtLocaleLanguageManager languageManager;
    std::shared_ptr<QSettings> userSettings = nullptr;

    int argc=0;
    QApplication app(argc, nullptr);
    classicLanguageManagerSetUp(languageManager, userSettings);

    if (languageManager.isSystemLanguageAvailable() == false)
    {
        BOOST_WARN_MESSAGE(languageManager.isSystemLanguageAvailable(), "The translation to the system language is not available, cant do this test.");
        return;
    }

    bRet = languageManager.translateAppToSystemLanguage();
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK( languageManager.currentLanguageNativeName().size() > 0);
    BOOST_CHECK_EQUAL(userSettings->value(SETTINGS_KEY_LOCALE_LANGUAGE).toString().toStdString() , "SYSTEM");
    BOOST_CHECK_EQUAL(languageManager.isSystemLanguageChosenByUser(), true);
 }

BOOST_AUTO_TEST_CASE(set_system_from_settings)
{
    bool bRet;
    QString sErrMsg;
    LlmQtLocaleLanguageManager languageManager;
    std::shared_ptr<QSettings> userSettings = nullptr;

    int argc=0;
    QApplication app(argc, nullptr);
    classicLanguageManagerSetUp(languageManager, userSettings);

    if (languageManager.isSystemLanguageAvailable() == false)
    {
        BOOST_WARN_MESSAGE(languageManager.isSystemLanguageAvailable(), "The translation to the system language is not available, cant do this test.");
        return;
    }

    userSettings->setValue(SETTINGS_KEY_LOCALE_LANGUAGE, "SYSTEM");
    bRet = languageManager.translateAppToUserSettingsLanguage(&sErrMsg);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sErrMsg.size() , 0);
    BOOST_CHECK( languageManager.currentLanguageNativeName().size() > 0);
    BOOST_CHECK_EQUAL(languageManager.isSystemLanguageChosenByUser(), true);

    BOOST_CHECK_EQUAL(userSettings->value(SETTINGS_KEY_LOCALE_LANGUAGE).toString().toStdString() , "SYSTEM");
 }



BOOST_AUTO_TEST_SUITE_END()
