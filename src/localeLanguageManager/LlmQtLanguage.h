#ifndef LLMQTLANGUAGE_H
#define LLMQTLANGUAGE_H
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
 * @class LlmQtLanguage LlmQtLanguage.h
 * The class represents a language, with this Qt properties.
 * It shouldn't be used directly, but only through LlmQtLocaleLanguageManager.
 *
 * @author Jon Petrissans
 */

// Includes:
#include <QLocale>
#include <QString>

class LlmQtLanguage
{
public:
    LlmQtLanguage();

    ~LlmQtLanguage();

    // Getters:
    QString languageISOCode() const             {return m_sLanguageISOCode;}/**< Retrieve the languages ISO639-1 code. ex: en, fr... @return Language ISO Code*/
    QString countryISOCode() const              {return m_sCountryISOCode;} /**< Retrieve the country ISO3166-1 code. ex: en, GB... (optional) @return Country ISO Code*/
    QString nativeNameWithCountry() const;
    QString englishNameWithCountry() const;
    QString qmFile() const                      {return m_sQmFile;}         /**< Retrieve the translation file name: xxxx_en.qm */
    QLocale::Language languageQtFlag() const    {return m_languageQtFlag;}  /**< Retrieve the languages Qt flag, from the QLocale::Language enum */
    QLocale::Country countryQtFlag() const      {return m_countryQtFlag;}   /**< Retrieve the country Qt flag, from the QLocale::Country enum (optional) */


    // Setters:
    void setQAppDefaultLanguage();
    bool set(QLocale::Language languageQtFlag, QLocale::Country countryQtFlag = QLocale::AnyCountry, QString *sReturnErrMsg = nullptr);

    bool setQmFileProperties(const QString & sQmFileName, QString *sReturnErrMsg = nullptr);
    bool setLanguageScriptCountryCodeProperties(const QString & sLanguageScriptCountryCode, QString *sReturnErrMsg = nullptr);

    // Methods:
    bool isRealizableBy(const LlmQtLanguage & candidateLanguage);

    static bool readCodesFromLanguageScriptCountryCode(const QString & sLanguageScriptCountryCode,
                                                       QString & sRetLanguageCode, QString & sRetScriptCode, QString & sRetCountryCode,
                                                       QString *sReturnErrMsg = nullptr);

    static bool isValidAssociation(const QString & sLanguageCode, const QString & sCountryCode,
                            QLocale::Language &retLanguageQtFlag , QLocale::Country &retCountryQtFlag , QString & sRetNativeLanguageName);

private:
    // Methods:
    void reset();


    // Members:
    QString m_sLanguageISOCode;   /**< Languages ISO639-1 code. ex: en, fr... */
    QString m_sCountryISOCode;    /**< Country ISO3166-1 code. ex: en, GB... (optional) */
    QString m_sNativeName;        /**< Languages name in its language. ex: British English, Français... */
    QString m_sEnglishName;       /**< Languages name in English. ex: English, French... */
    QString m_sQmFile;            /**< Translation file name: xxxx_en.qm */
    QLocale::Language m_languageQtFlag;   /**< Languages Qt flag, from the QLocale::Language enum */
    QLocale::Country m_countryQtFlag;     /**< Country Qt flag, from the QLocale::Country enum (optional) */
};

#endif // LLMQTLANGUAGE_H
