#include "LlmQtLocaleLanguageManager.h"

#include <QApplication>
#include <QTextStream>

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: Constructor / Destructor
//-----------------------------------------------------------------------------------------------------------------------


LlmQtLocaleLanguageManager::LlmQtLocaleLanguageManager()
    : m_qmFilesDirectory("")
    , m_translateLanguages()
    , m_translator()
    , m_sCurrentLanguage()
    , m_sourceLanguage()
    , m_systemLanguage()
    , m_bSystemLanguageAvailable {false}
    , m_bSystemLanguageChoosenByUser {false}
    , m_settings {nullptr}
    , m_sKeySettingsTranslateLanguage(DEFAULT_KEY_LOCALE_LANGUAGE)
{

}

LlmQtLocaleLanguageManager::~LlmQtLocaleLanguageManager()
{

}


//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: Analyse methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Analyse which are the available languages/translations for the application.
 * The analysis is done according the .qm files found under m_qmFilesDirectory.
 * LlmQtLocaleLanguageManager::setQmFilesDirectory() has to be called before.
 * If .qm file is added/removed during running time, this method update the analysis.
 *
 * Also, it look to the system languages, and if our app is translatable to a system language, that language is stored in m_systemLanguage.
 *
 * In case of error, the method has 2 behaviours:
 * - if sReturnErrMsg is used sReturnErrMsg return error's description, there is no exception throw.
 * - if sReturnErrMsg is not used, the method throws an exception.
 *
 * @param [out] sReturnErrMsg Description of error
 * @return True if analysis is a success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if:
 *  - qmFilesDirectory doesn't exist.
 *  - No qm file found.
 *  - LlmQtLocaleLanguageManager::languagePropertiesOfQmFile() throws an exception.
 */
bool LlmQtLocaleLanguageManager::analyseAvailableLanguages(QString *sReturnErrMsg)
{
    bool bRet = true;

    if (m_qmFilesDirectory.exists() == false)
    {
        // ERROR: m_qmFilesDirectory does not exist
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: The translation file (.qm) search folder doesn't exist.\n";
        sErrMsg += "Folder: " + QDir::toNativeSeparators( m_qmFilesDirectory.absolutePath() ).toStdString();

        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }

    // Search of qm files
    QStringList langFiles = m_qmFilesDirectory.entryList(QStringList("*.qm"));
    if (langFiles.count() == 0)
    {
        // ERROR: No qm file found
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: There are no translations available, any .qm file found.\n";
        sErrMsg += "Search Folder: " + QDir::toNativeSeparators( m_qmFilesDirectory.absolutePath() ).toStdString();

        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }


    //Filling of the list m_languages:
    m_translateLanguages.clear();

    for (int i = 0 ; i < langFiles.size() ; ++i)
    {
        LlmQtLanguage languageProp;

        // get language extracted from filename
        if (languageProp.setQmFileProperties(langFiles[i], sReturnErrMsg) )
        {
            m_translateLanguages.append(languageProp);
        }
        else
        {
            bRet = false;
        }

    }


    // checking if our app is translatable to one of the system languages
    QStringList systemLanguages = QLocale::system().uiLanguages();
    LlmQtLanguage systemLanguage;
    QString sDummy;

    m_bSystemLanguageAvailable = false; // initialization

    for (int iSys = 0 ; iSys < systemLanguages.count() ; ++iSys)
    {

        if (systemLanguage.setLanguageScriptCountryCodeProperties(systemLanguages[iSys],&sDummy) )
        {
            // System language can be the source language
            if (systemLanguage.isRealizableBy(m_sourceLanguage)  )
            {
                m_bSystemLanguageAvailable = true;
                m_systemLanguage = m_sourceLanguage;
            }

            // or System language can be a translated language
            for (int iTrans = 0 ; iTrans < m_translateLanguages.count() ; ++iTrans)
            {
                //if ( sSystemLanguageISOCode == m_translateLanguages[iTrans].sLanguageISOCode)
                if (systemLanguage.isRealizableBy( m_translateLanguages[iTrans]) )
                {
                    m_bSystemLanguageAvailable = true;
                    m_systemLanguage = m_translateLanguages[iTrans];
                }
            }
        }
    }

    return bRet;
}

/**
 * Set qm files directory.
 * @param sQmFilesDirectory Full path
 */

void LlmQtLocaleLanguageManager::setQmFilesDirectory(const QString & sQmFilesDirectory)
{
    m_qmFilesDirectory.setPath(sQmFilesDirectory);
}

/**
 * Indicate if the class is usable. = qm files available + source language known.
 * @return True if LlmQtLocaleLanguageManager is usable
 */
bool LlmQtLocaleLanguageManager::isUsable()
{
    bool bQmFilesAvailable, bSourceLanguageKnown;


    bQmFilesAvailable = m_translateLanguages.count() > 0;

    bSourceLanguageKnown =  (
                                m_sourceLanguage.englishNameWithCountry().size() > 0 &&
                                m_sourceLanguage.nativeNameWithCountry().size() > 0 &&
                                m_sourceLanguage.languageQtFlag() != QLocale::C &&
                                m_sourceLanguage.languageISOCode().size() > 0
                            );

    return bQmFilesAvailable && bSourceLanguageKnown;
}



//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: Translate methods
//-----------------------------------------------------------------------------------------------------------------------


/**
 * Return the name of the current language, in native form
 * @return Name of the current language, in native form
 */
QString LlmQtLocaleLanguageManager::currentLanguageNativeName()
{
    return m_sCurrentLanguage.nativeNameWithCountry();
}



/**
 * List of the available languages, in native form.
 * The list include the source language, and a SYSTEM flag if asked (and if the system language's translation is available).
 * If a language has a country specification, the language name will be: "language name (Country code)".
 * @param [in] bIntegrateSystemLanguageFlag Indicate if a SYSTEM flag is needed in the returned list.
 * @return List of the available languages.
 */
QStringList LlmQtLocaleLanguageManager::languageNameNativeList(bool bIntegrateSystemLanguageFlag)
{
    QStringList languagesList;
    languagesList.clear();

    if (m_sourceLanguage.languageQtFlag() != QLocale::C)
        languagesList.append(m_sourceLanguage.nativeNameWithCountry());

    for (int i = 0 ; i < m_translateLanguages.count() ; ++i)
    {
        languagesList.append(m_translateLanguages[i].nativeNameWithCountry());
    }

    languagesList.sort();

    if (m_bSystemLanguageAvailable == true && bIntegrateSystemLanguageFlag == true)
    {
        // add of a SYSTEM Flag in the list: "SYSTEM (English)"
        languagesList.insert(0, QString("%1 (%2)").arg(SYSTEM_FLAG).arg(m_systemLanguage.nativeNameWithCountry()));
    }
    return languagesList;
}

/**
 * Translate the application to the the chosen language.
 * The default QLocale is also modified according the new language.
 * The language name can be either in it's Native form or in it's English form (Français or French), it doesn't matter.
 * It can contain the country code in brackets. (as given by LlmQtLocaleLanguageManager::languageNameNativeList()).
 *
 * If a QSetting has been set, (thanks to LlmQtLocaleLanguageManager::setSettings() ), saves the changes in user's settings.
 *
 * In case of error, the method has 2 behaviours:
 * - if sReturnErrMsg is used sReturnErrMsg return error's description, there is no exception throw
 * - if sReturnErrMsg is not used, the method throws an exception
 *
 * @param [in] sLanguageNativeOrEnglishNameWithCountry The wanted language name in this Native form or in this English form (Français or French).
 * @param [out] sReturnErrMsg description of error
 * @return True is success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if:
 *  - System language is asked and not available. (Use LlmQtLocaleLanguageManager::isSystemLanguageAvailable() to have that information).
 *  - Qm file load error.
 *  - Qm file of the asked language doesn't exist.
 */
bool LlmQtLocaleLanguageManager::translateAppToLanguage(const QString & sLanguageNativeOrEnglishNameWithCountry, QString * sReturnErrMsg )
{
    QString sLanguageNameCopy = sLanguageNativeOrEnglishNameWithCountry;
    bool bSystemLanguageChosenByUser = false;

    // in case of SYSTEM is selected, we replace by the actual language name
    if (sLanguageNameCopy.contains(SYSTEM_FLAG) )
    {
        if (m_bSystemLanguageAvailable == true)
        {
            sLanguageNameCopy = m_systemLanguage.nativeNameWithCountry();
            bSystemLanguageChosenByUser = true;
        }
        else
        {
            // ERROR: incorrect use of the class LlmQtLocaleLanguageManager, System language is not available
            std::string  sErrMsg = "LlmQtLocaleLanguageManager: incorrect use of the class LlmQtLocaleLanguageManager, System language is not available";
            if (sReturnErrMsg == nullptr)
                throw std::runtime_error(sErrMsg);
            else
                *sReturnErrMsg = sErrMsg.c_str();

            return false;
        }
    }

    // to come back to the app's source language, we just need to remove the translator
    if (sLanguageNameCopy == m_sourceLanguage.nativeNameWithCountry() || sLanguageNameCopy == m_sourceLanguage.englishNameWithCountry() )
    {
        QLocale::setDefault(QLocale(m_sourceLanguage.languageQtFlag(), m_sourceLanguage.countryQtFlag()));

        // remove the old translator
        qApp->removeTranslator(&m_translator);

        this->saveTranslationDone(m_sourceLanguage, bSystemLanguageChosenByUser);
        return true;
    }
    else
    {

        // search of the language among m_languages
        for (int i = 0 ; i < m_translateLanguages.count() ; ++i)
        {
            if (sLanguageNameCopy == m_translateLanguages[i].nativeNameWithCountry() || sLanguageNameCopy == m_translateLanguages[i].englishNameWithCountry())
            {
                QLocale::setDefault(QLocale(m_translateLanguages[i].languageQtFlag(), m_translateLanguages[i].countryQtFlag()));

                // remove the old translator
                qApp->removeTranslator(&m_translator);

                // load the new translator
                QFileInfo qmFile(m_qmFilesDirectory, m_translateLanguages[i].qmFile());
                if( m_translator.load(qmFile.absoluteFilePath()) )
                {
                    qApp->installTranslator(&m_translator);
                    this->saveTranslationDone(m_translateLanguages[i], bSystemLanguageChosenByUser);

                }
                else
                {
                    // ERROR: m_translator.load() error
                    std::string  sErrMsg = "LlmQtLocaleLanguageManager/QTranslator.load(): .qm file load error: " + m_translateLanguages[i].qmFile().toStdString();
                    if (sReturnErrMsg == nullptr)
                        throw std::runtime_error(sErrMsg);
                    else
                        *sReturnErrMsg = sErrMsg.c_str();

                    return false;
                }

                return true;
            }
        }
    }
    // ERROR "The translation file (.qm) of the language %1 is not available" , sLanguageNameCopy
    std::string  sErrMsg = "The translation file (.qm) of the language "
                            + sLanguageNameCopy.toStdString() + " is not available";
    if (sReturnErrMsg == nullptr)
        throw std::runtime_error(sErrMsg);
    else
        *sReturnErrMsg = sErrMsg.c_str();

    return false;

}

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: SourceLanguage methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * The native name of the interface (dialog panels) language used in sources.
 * @return the native name of the source/primary interface language.
 */
QString LlmQtLocaleLanguageManager::sourceLanguageNativeName()
{
    return this->m_sourceLanguage.nativeNameWithCountry();
}


/**
 * Guess source's interface language, from the default QLocal's values.
 * The method is an alternative to LlmQtLocaleLanguageManager::setSourceLanguage(), but prefer use LlmQtLocaleLanguageManager::setSourceLanguage() instead of that one.
 */
void LlmQtLocaleLanguageManager::setSourceLanguageFromQAppLocalSettings()
{
    m_sourceLanguage.setQAppDefaultLanguage();
}

/**
 * Set the source's/primary languages informations.
 * The method is an alternative to LlmQtLocaleLanguageManager::setSourceLanguageFromQAppLocalSettings(), and is a better solution.
 * This method is more "precise", and has to be privileged. It permit to set countryQtFlag to QLocale::AnyCountry.
 * @param [in] languageQtFlag Languages Qt flag, from the QLocale::Language enum
 * @param [in] countryQtFlag Country Qt flag, from the QLocale::Country enum
 * @param [out] sReturnErrMsg description of error
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if the language/country couple is not compatible.
 */

bool LlmQtLocaleLanguageManager::setSourceLanguage(QLocale::Language languageQtFlag,  QLocale::Country countryQtFlag, QString *sReturnErrMsg)
{
    return m_sourceLanguage.set(languageQtFlag, countryQtFlag, sReturnErrMsg);
}



//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: System methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Return the name of the available system language name, in this native form (available among qm files).
 * An analysis need to be done before calling this method, with LlmQtLocaleLanguageManager::analyseAvailableLanguages().
 * @return The name of the available system language name, in this native form
 */
QString LlmQtLocaleLanguageManager::systemLanguageNativeName()
{
    return m_systemLanguage.nativeNameWithCountry();
}

/**
 * Indicate if one of the system languages is available among the qm files.
 * An analysis need to be done before calling this method, with LlmQtLocaleLanguageManager::analyseAvailableLanguages().
 * @return True if one of the system languages is available.
 */
bool LlmQtLocaleLanguageManager::isSystemLanguageAvailable()
{
    return m_bSystemLanguageAvailable;
}

/**
 *Indicate if the Language choice is "System language"
 * @return True if the Language choice is "System language"
 */
bool LlmQtLocaleLanguageManager::isSystemLanguageChosenByUser()
{
    return m_bSystemLanguageChoosenByUser;
}



/**
 * Translate the application to an available system language. (Available among the qm files)
 * Before using that method, make sure a system language translation is available thanks LlmQtLocaleLanguageManager::isSystemLanguageAvailable().
 * If a QSetting has been set, (thanks to LlmQtLocaleLanguageManager::setSettings() ), saves the changes in user's settings.
 *
 * In case of error, the method has 2 behaviours:
 * - if sReturnErrMsg is used sReturnErrMsg return error's description, there is no exception throw.
 * - if sReturnErrMsg is not used, the method throws an exception.
 * @param [out] sReturnErrMsg description of error
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if :
 *  - The translation to the system language is not available.
 * System language availability can be checked with LlmQtLocaleLanguageManager::isSystemLanguageChosen()
 *  - LlmQtLocaleLanguageManager::translateAppToLanguage throws an exception
 */
bool LlmQtLocaleLanguageManager::translateAppToSystemLanguage(QString *sReturnErrMsg)
{
    bool bRet = false;
    if (m_bSystemLanguageAvailable)
    {
        bRet= translateAppToLanguage(SYSTEM_FLAG, sReturnErrMsg);
    }
    else
    {
        // ERROR: The translation to the system language is not available
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: The translation to the system language is not available";
        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        bRet = false;
    }
    return bRet;
}

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: Settings methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Set a user QSettings object to the language manager class (Use of that feature is optional).
 * If set, that QSettings object will be used to store the chosen languages.
 * If not set, the language changes are just not saved, without any warning.
 * @param [in] settings Settings object to be used
*/
void LlmQtLocaleLanguageManager::setSettings(const std::shared_ptr<QSettings> &settings)
{
    this->m_settings = settings;
}

/**
 * Modify the key to access "Locale-language" value in user's settings.
 * If not set, the default key is "Locale/Language"
 * @param [in] sKey The new key
 */
void LlmQtLocaleLanguageManager::setKeySettingsTranslateLanguage(const QString & sKey)
{
    m_sKeySettingsTranslateLanguage = sKey;
}



/**
 * Translate the application to the language indicated in the settings m_settings, under the language key.
 * The key is indicated thanks to LlmQtLocaleLanguageManager::setKeySettingsTranslateLanguage().
 * A QSetting has to be set first, thanks to LlmQtLocaleLanguageManager::setSettings().
 *
 * In case of error, the method has 2 behaviours:
 * - if sReturnErrMsg is used sReturnErrMsg return error's description, there is no exception throw
 * - if sReturnErrMsg is not used, the method throws an exception
 * @param [out] sReturnErrMsg description of error
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if:
 * - LlmQtLocaleLanguageManager::translateAppToLanguage() throws an exception.
 * - QSetting has not been set.
 */
bool LlmQtLocaleLanguageManager::translateAppToUserSettingsLanguage(QString *sReturnErrMsg)
{
    bool bRet, bSystemLanguageChoosen_Memo;
    if (m_settings == nullptr)
    {
        // ERROR: QSetting has not been set
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: QSetting has not been set before translateAppToUserSettingsLanguage()";
        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }


    QString sSettingsLanguage = m_settings->value(m_sKeySettingsTranslateLanguage).toString();

    // if no configuration value, we leave the source language
    if (sSettingsLanguage.length() == 0)
        return true;

    if (sSettingsLanguage == SYSTEM_FLAG)
    {
        bSystemLanguageChoosen_Memo = true;
        sSettingsLanguage = m_systemLanguage.englishNameWithCountry();
    }
    else
    {
        bSystemLanguageChoosen_Memo = false;
    }

    bRet = this->translateAppToLanguage(sSettingsLanguage, sReturnErrMsg);
    m_bSystemLanguageChoosenByUser = bSystemLanguageChoosen_Memo;

    if (m_bSystemLanguageChoosenByUser == true)
        m_settings->setValue(m_sKeySettingsTranslateLanguage, SYSTEM_FLAG);

    return bRet;
}


//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLocaleLanguageManager :: Private methods
//-----------------------------------------------------------------------------------------------------------------------




/**
 * When a qTranslator object is set to qApp, this method is called to save the changes within the class.
 * If a QSetting has been set, (thanks to LlmQtLocaleLanguageManager::setSettings() ), saves the changes in user's settings.
 * @param [in] translationLanguageProperties New languages properties.
 */
void LlmQtLocaleLanguageManager::saveTranslationDone(const LlmQtLanguage &translationLanguage, bool bSystemLanguageChosenByUser)
{
    m_bSystemLanguageChoosenByUser = bSystemLanguageChosenByUser;
    m_sCurrentLanguage = translationLanguage;

    if (m_settings != nullptr)
    {
        if (bSystemLanguageChosenByUser == false)
            m_settings->setValue(m_sKeySettingsTranslateLanguage, translationLanguage.englishNameWithCountry() ); //English name is written in order to have only Unicode chars

        else
            m_settings->setValue(m_sKeySettingsTranslateLanguage, SYSTEM_FLAG);
    }
}
