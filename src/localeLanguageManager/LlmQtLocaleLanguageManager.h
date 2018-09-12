#ifndef LLMQTLOCALELANGUAGEMANAGER_H
#define LLMQTLOCALELANGUAGEMANAGER_H

/*
 * MIT License
 *
 * Copyright (c) 2018 Jon Petrissans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/*----------------------------------------------------------------*/
/*  PROJECT:  localeLanguageManager for Qt                        */
/*----------------------------------------------------------------*/

/**
 * @class LlmQtLocaleLanguageManager LlmQtLocaleLanguageManager.h
 * LlmQtLocaleLanguageManager is an utilitarian class that simplifies language choice management in a multilingual Qt application.
 * It provide methods to deal with .qm files, app's active language, system language, language switch, choice storing in user settings...
 * It handle all the possible error during .qm files analysis.
 * Dialog's straight translation at running time is possible.
 *
 * How to use explanations and use examples are in main folder's README.md .
 *
 * @author Jon Petrissans
 */


// Includes:
#include <memory>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QString>
#include <QDir>
#include "LlmQtLanguage.h"

class LlmQtLocaleLanguageManager
{
public:
    LlmQtLocaleLanguageManager();
    ~LlmQtLocaleLanguageManager();
    LlmQtLocaleLanguageManager & operator=(const LlmQtLocaleLanguageManager&) = delete;
    LlmQtLocaleLanguageManager(const LlmQtLocaleLanguageManager&) = delete;

    // Analyse methods:
    bool analyseAvailableLanguages(QString * sReturnErrMsg = nullptr);
    void setQmFilesDirectory(const QString & sQmFilesDirectory);
    bool isUsable();

    // Translate methods:
    QString currentLanguageNativeName();
    QStringList languageNameNativeList(bool bIntegrateSystemLanguageFlag);
    bool translateAppToLanguage(const QString & sLanguageNativeOrEnglishNameWithCountry, QString *sReturnErrMsg = nullptr);

    // SourceLanguage methods:
    QString sourceLanguageNativeName();
    bool setSourceLanguage(QLocale::Language languageQtFlag, QLocale::Country countryQtFlag = QLocale::AnyCountry, QString *sReturnErrMsg = nullptr);
    void setSourceLanguageFromQAppLocalSettings();

    // System language methods:
    QString systemLanguageNativeName();
    bool isSystemLanguageAvailable();
    bool isSystemLanguageChosenByUser();
    bool translateAppToSystemLanguage(QString *sReturnErrMsg = nullptr);

    // Settings methods:
    void setSettings(const std::shared_ptr<QSettings> & settings);
    void setKeySettingsTranslateLanguage(const QString & sKey);
    bool translateAppToUserSettingsLanguage(QString *sReturnErrMsg = nullptr);

private:
    // Methods:
    void saveTranslationDone(const LlmQtLanguage & translationLanguage, bool bSystemLanguageChosenByUser);

    // Members:
    QDir m_qmFilesDirectory;        // directory containing the qm files
    QList<LlmQtLanguage> m_translateLanguages;    // list of the available languages
    QTranslator m_translator;       // contains the active translator of this application

    LlmQtLanguage m_sCurrentLanguage; // the currently loaded language
    LlmQtLanguage m_sourceLanguage;   // store properties of source language of the app

    LlmQtLanguage m_systemLanguage;   // store properties of 1st available system language of the app (of 1st system language if no compatibility)
    bool m_bSystemLanguageAvailable;    // indicate if our app is translatable to a system language
    bool m_bSystemLanguageChoosenByUser;      // indicate if our app is translated to a system language

    std::shared_ptr<QSettings> m_settings;                     // QSettings in which the translation language will be stored
    QString m_sKeySettingsTranslateLanguage;   // Translation language store key


    const char SYSTEM_FLAG[9] = "SYSTEM";     // "TranslateLanguage" setting value is the system language is chosen for translation
    const char DEFAULT_KEY_LOCALE_LANGUAGE[26] = "Locale/Language"; // Translation language store default key <lk>

};

#endif // LLMQTLOCALELANGUAGEMANAGER_H
