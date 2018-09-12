#include "LlmQtLanguage.h"

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLanguage :: Constructors / Destructors
//-----------------------------------------------------------------------------------------------------------------------
LlmQtLanguage::LlmQtLanguage()
    : m_sLanguageISOCode("")
    , m_sCountryISOCode("")
    , m_sNativeName("")
    , m_sEnglishName("")
    , m_sQmFile("")
    , m_languageQtFlag(QLocale::C)
    , m_countryQtFlag(QLocale::AnyCountry)
{

}
LlmQtLanguage::~LlmQtLanguage()
{

}



//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLanguage :: Getters
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Return the native name (i.e. languages name in its language. ex: British English, Français)
 * with the country code in brackets if any.
 * @return Native name of the language
 */
QString LlmQtLanguage::nativeNameWithCountry() const
{
    if (m_sCountryISOCode.size() > 0)
        return m_sNativeName + " (" + m_sCountryISOCode + ")";
    else
        return m_sNativeName;
}

/**
 * Return the English name (i.e. languages name in English. ex: English, French)
 * with the country code in brackets if any.
 * @return English name of the language
 */
QString LlmQtLanguage::englishNameWithCountry() const
{
    if (m_sCountryISOCode.size() > 0)
        return m_sEnglishName + " (" + m_sCountryISOCode + ")";
    else
        return m_sEnglishName;
}

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLanguage :: Setters
//-----------------------------------------------------------------------------------------------------------------------
/**
 * Retrieve all the information of the default language of the current QApplication.
 * The default language can be set like this:
 * ~~~~~~~~~~~~~{.cpp}
 * QLocale::setDefault( QLocale(QLocale::English, QLocale::Belgium) );
 * ~~~~~~~~~~~~~
 */
void LlmQtLanguage::setQAppDefaultLanguage()
{
    // extraction of the local languages data:
    QLocale l; // when a qLocale is constructed without argument, it takes the default QLocal's values
    m_languageQtFlag = l.language();
    m_countryQtFlag = l.country();
    m_sNativeName = l.nativeLanguageName(); // ex: British English, Français..
    m_sEnglishName = QLocale::languageToString(l.language()); // ex: English, French...
    m_sLanguageISOCode = l.name().left(2);
    m_sCountryISOCode = l.name().mid(3);

}

/**
 * Set language's properties
 * @param [in] languageQtFlag Languages Qt flag, from the QLocale::Language enum
 * @param [in] countryQtFlag Country Qt flag, from the QLocale::Country enum
 * @param [out] sReturnErrMsg description of error if any
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if the language/country couple is not compatible
 */

bool LlmQtLanguage::set(QLocale::Language languageQtFlag, QLocale::Country countryQtFlag, QString *sReturnErrMsg)
{
    QLocale l(languageQtFlag, countryQtFlag);
    m_languageQtFlag = l.language();
    m_countryQtFlag = countryQtFlag;
    m_sEnglishName = QLocale::languageToString(l.language()); // ex: English, French...


    if (m_languageQtFlag == QLocale::English && m_countryQtFlag == QLocale::AnyCountry)
        //I consider QLocale behaviour not logical in that case, so here is a correction
        m_sNativeName = "English";
    else
        m_sNativeName = l.nativeLanguageName(); // ex: British English, Français..

    m_sLanguageISOCode = l.name().left(2);
    m_sCountryISOCode = l.name().mid(3);

    if (countryQtFlag == QLocale::AnyCountry)
    {
        m_sCountryISOCode = "";
    }
    else if(countryQtFlag != l.country())
    {
        this->reset();
        // ERROR: file name not correct
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: That language/country couple is not compatible: " + m_sEnglishName.toStdString() + "/" + m_sCountryISOCode.toStdString() ;
        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------
// LlmQtLanguage :: Methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Test if the language (considered as requirement) can be realized/implemented by candidateLanguage.
 * Example: if the language requirement is English / United States, it can be realized by English / Any country.
 * And if the language requirement is English / Any country, it cannot be realized by English /  United States.
 * @param [in] candidateLanguage A candidate to realize/implement the language
 * @return True if candidateLanguage can realize the language.
 */
bool LlmQtLanguage::isRealizableBy(const LlmQtLanguage &candidateLanguage)
{
    if (candidateLanguage.countryQtFlag() != QLocale::AnyCountry)
    {
        if (m_languageQtFlag == candidateLanguage.languageQtFlag() && m_countryQtFlag ==candidateLanguage.countryQtFlag())
            return true;
        else
            return false;
    }
    else
    {
        if (m_languageQtFlag == candidateLanguage.languageQtFlag())
            return true;
        else
            return false;
    }
}




/**
 * Extract language properties from a qm file name.
 * The properties are extract from the language code (ISO639-1) and the country code (ISO3166-1) at the and of the file name.
 * The file name has to follow Qt Linguist's rules: [..]_\<language code\>[_\<region code\>].ts Ex: _zh_CH.ts or _eu.ts
 * (Note that the first underscore is mandatory)
 *
 * In case of error, the method has 2 behaviours:
 * - if sReturnErrMsg is used sReturnErrMsg return error's description, there is no exception throw
 * - if sReturnErrMsg is not used, the method throws an exception
 * @param [in] sQmFileName A .qm file name. The name has to be: [..]_<language code>[_<region code>].ts  Ex: _zh_CH.ts or _eu.ts
 * @param [out] sReturnErrMsg description of error
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if:
 *  - qm file name doesn't follow the qt standard. The name has to be: [..]_\<language code\>[_\<region code\>].ts
 *  - A valid language ISO639-1 code is not found.
 *  - A valid country ISO3166-1 code is not found.
 *  - The language/country couple is not compatible
 */
bool LlmQtLanguage::setQmFileProperties(const QString & sQmFileName, QString *sReturnErrMsg)
{

    int iIndexPoint, iIndexLastUnderScore, iIndexPenultimateUnderScore;
    QString sTestedChars ;
    QLocale testLocale;
    QString sTestName;
    QString sDummyScript;
    bool bValidAssociation = false;


    // get sLanguageCodeISO6391 extracted from filename
    iIndexPoint = sQmFileName.lastIndexOf('.');
    iIndexLastUnderScore = sQmFileName.lastIndexOf('_');

    // The files needs at least this format: "blabla_de.qm", otherwise it doesn't work well with QT linguist
    if (iIndexPoint == -1 || iIndexLastUnderScore == -1 || (iIndexLastUnderScore != iIndexPoint - 3) )
    {
        // ERROR: file name not correct
        std::string  sErrMsg = "LlmQtLocaleLanguageManager: The name of the following .qm file do not follow the standard: " + sQmFileName.toStdString();
        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }

    // But the file can have this format: : "blabla_zh_CH.qm",
    iIndexPenultimateUnderScore = sQmFileName.lastIndexOf('_',iIndexLastUnderScore-1);



    // test of the last chars, in order to know if it's a language code only or language code + country code (... or a wrong value)
    if (iIndexPenultimateUnderScore == iIndexLastUnderScore -3 && iIndexPenultimateUnderScore >= 0) // we have perhaps blabla_zh_CH.qm
    {

        sTestedChars = sQmFileName; // blabla_zh_CH.qm
        sTestedChars.truncate(iIndexPoint); // blabla_zh_CH
        sTestedChars.remove(0, iIndexPenultimateUnderScore + 1); // zh_CH

         if ( LlmQtLanguage::readCodesFromLanguageScriptCountryCode(sTestedChars,
                                m_sLanguageISOCode, sDummyScript, m_sCountryISOCode, sReturnErrMsg ) == false)
            return false;


         bValidAssociation = LlmQtLanguage::isValidAssociation(m_sLanguageISOCode, m_sCountryISOCode, m_languageQtFlag, m_countryQtFlag, m_sNativeName);

    }

    if (bValidAssociation == false) // we have perhaps blabla_zh.qm
    {
        sTestedChars = sQmFileName; // "blabla_de.qm"
        sTestedChars.truncate(iIndexPoint); // "blabla_de"
        sTestedChars.remove(0, iIndexLastUnderScore + 1); // "de"

        if ( LlmQtLanguage::readCodesFromLanguageScriptCountryCode(sTestedChars,
                               m_sLanguageISOCode, sDummyScript, m_sCountryISOCode, sReturnErrMsg ) == false)
           return false;


        bValidAssociation = LlmQtLanguage::isValidAssociation(m_sLanguageISOCode, m_sCountryISOCode, m_languageQtFlag, m_countryQtFlag, m_sNativeName);
    }


    if(bValidAssociation == false)
    {
        // ERROR: file name not correct
        this->reset();
        std::string  sErrMsg = "LlmQtLanguage: The language code of the following .qm file do not follow QT's specifications ([..]_<language code ISO639>[_<region code>].ts): "
                + std::string("or the language/country values are not compatible with each other: ")
                + sQmFileName.toStdString();

        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }

    m_sEnglishName = QLocale::languageToString(m_languageQtFlag);  // ex: English, French...
    m_sQmFile = sQmFileName;  // ex: blabla_en.qm

    return true;
}


/**
 * Set the LlmQtLanguage properties of the specified code, which has the format "language[_script][_country]".
 * That code is typically a system locale code.
 * Language is a lowercase, two-letter, ISO 639 language code (also some three-letter codes),
 * script is a titlecase, four-letter, ISO 15924 script code,
 * country is an uppercase, two-letter, ISO 3166 country code.
 * @param [in] sLanguageScriptCountryCode
 * @param [out] sReturnErrMsg description of error
 * @return True if success
 * @exception runtime_error Exceptions are raised only if sReturnErrMsg is not used.
 * Throws an exception if:
 *  - code doesn't follow the qt standard.
 *  - A valid language ISO639-1 code is not found.
 *  - A valid country ISO3166-1 code is not found.
 *  - The language/country couple is not compatible
 */
bool LlmQtLanguage::setLanguageScriptCountryCodeProperties(const QString & sLanguageScriptCountryCode, QString *sReturnErrMsg)
{


    QString sDummyScript;
    bool bValidAssociation = false;


    if ( LlmQtLanguage::readCodesFromLanguageScriptCountryCode(sLanguageScriptCountryCode,
                           m_sLanguageISOCode, sDummyScript, m_sCountryISOCode, sReturnErrMsg ) == false)
       return false;


    bValidAssociation = LlmQtLanguage::isValidAssociation(m_sLanguageISOCode, m_sCountryISOCode, m_languageQtFlag, m_countryQtFlag, m_sNativeName);


    if(bValidAssociation == false)
    {
        // ERROR: sLanguageScriptCountryCode name not correct
        this->reset();
        std::string  sErrMsg = "LlmQtLanguage: The language code of the language/Script/Country code do not follow ISO639/ISO15924/ISO3166 specifications:"
                + std::string("or the language/country values are not compatible with each other: ")
                + sLanguageScriptCountryCode.toStdString();

        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }

    m_sEnglishName = QLocale::languageToString(m_languageQtFlag);  // ex: English, French...

    return true;
}


/**
 * Reset the object to this original values
 */
void LlmQtLanguage::reset()
{
    m_sLanguageISOCode = "";
    m_sCountryISOCode = "";
    m_sNativeName = "";
    m_sEnglishName = "";
    m_sQmFile = "";
    m_languageQtFlag = QLocale::C;
    m_countryQtFlag = QLocale::AnyCountry;
}

/**
 * Break down a language / script / country code, and return the language, script and country.
 * the code format is "language[_script][_country]".
 * That code is typically a system locale code.
 * @param [in] sLanguageScriptCountryCode
 * @param [out] sRetLanguageCode
 * @param [out] sRetScriptCode
 * @param [out] sRetCountryCode
 * @param [out] sReturnErrMsg
 * @return True if success
 */
bool LlmQtLanguage::readCodesFromLanguageScriptCountryCode(const QString & sLanguageScriptCountryCode,
                                            QString & sRetLanguageCode, QString & sRetScriptCode, QString & sRetCountryCode,
                                            QString *sReturnErrMsg)
{

    int iIndex1stUnderScore = -1, iIndex2ndUnderScore = -1, iIndexOtherUnderScore = -1;
    QString sReworkedCode ;

    // The Language-Script-Country Code can use either _ or - as separator, so we replace all the - by _
    sReworkedCode = sLanguageScriptCountryCode;
    sReworkedCode.replace("-", "_");


    iIndex1stUnderScore = sReworkedCode.indexOf('_');
    if (iIndex1stUnderScore >= 0)
        iIndex2ndUnderScore = sReworkedCode.indexOf('_', iIndex1stUnderScore +1);
    if (iIndex2ndUnderScore >= 0)
        iIndexOtherUnderScore = sReworkedCode.indexOf('_', iIndex2ndUnderScore +1);


    if (iIndexOtherUnderScore != -1 )
    {
        // ERROR: code not correct
        std::string  sErrMsg = "LlmQtLanguage: The Language-Script-Country Code do not follow the standard: " + sLanguageScriptCountryCode.toStdString();
        if (sReturnErrMsg == nullptr)
            throw std::runtime_error(sErrMsg);
        else
            *sReturnErrMsg = sErrMsg.c_str();

        return false;
    }



    if (iIndex2ndUnderScore >= 0) // we have  zh-Hans-CH
    {
        sRetLanguageCode = sReworkedCode.mid(0,iIndex1stUnderScore);
        sRetScriptCode = sReworkedCode.mid(iIndex1stUnderScore + 1, iIndex2ndUnderScore - iIndex1stUnderScore -1);
        sRetCountryCode = sReworkedCode.mid(iIndex2ndUnderScore + 1);

    }
    else if(iIndex1stUnderScore >= 0)// we have  zh-CH
    {
        sRetLanguageCode = sReworkedCode.mid(0,iIndex1stUnderScore);
        sRetScriptCode = "";
        sRetCountryCode = sReworkedCode.mid(iIndex1stUnderScore + 1);

    }
    else// we have  zh
    {
        sRetLanguageCode = sReworkedCode;
        sRetScriptCode = "";
        sRetCountryCode = "";
    }
    return true;
}

/**
 * Check if the couple sLanguageCode / sCountryCode is a valid association.
 * Return the QT flag of teh language and the country along with the native language name
 * @param [in] sLanguageCode  Languages ISO639-1 code. ex: en, fr...
 * @param [in] sCountryCode Country ISO3166-1 code. ex: en, GB.
 * @param [out] retLanguageQtFlag Return of language's Qt flag, from the QLocale::Language enum
 * @param [out] retCountryQtFlag Return of country's Qt flag, from the QLocale::Country enum
 * @param [out] sRetNativeLanguageName Return of language name in its language. ex: British English, Français..
 * @return True if the sLanguageCode / sCountryCode couple is compatible.
 */
bool LlmQtLanguage::isValidAssociation(const QString & sLanguageCode, const QString & sCountryCode,
                        QLocale::Language & retLanguageQtFlag, QLocale::Country & retCountryQtFlag,
                        QString & sRetNativeLanguageName)
{

    bool bValidQLocaleBuilt = false;
    QString sTestLanguageCode, sTestScriptCode, sTestCountryCode;
    QString sDummy;
    QLocale testLocale(sLanguageCode + "_" + sCountryCode);

    retLanguageQtFlag = QLocale::C;       // Initialization
    retCountryQtFlag = QLocale::AnyCountry;
    sRetNativeLanguageName = "";

    // QLocale's behaviour:
    // If the string violates the locale format, or language is not a valid ISO 639 code, the "C" locale is used instead.
    // If country is not present, or is not a valid ISO 3166 code, the most appropriate country is chosen for the specified language.

    // extract of language / country codes according QLocale
    // QLocale do not provide the script name in testLocale.name() (testLocale.name() = zh_CN)
    if (LlmQtLanguage::readCodesFromLanguageScriptCountryCode(testLocale.name(),
             sTestLanguageCode, sTestScriptCode, sTestCountryCode, &sDummy) == false)
        return false;

    // And we compare them to our values
    if (sCountryCode.size() == 0)
        bValidQLocaleBuilt = (sLanguageCode == sTestLanguageCode);
    else
        bValidQLocaleBuilt = (sLanguageCode == sTestLanguageCode && sCountryCode == sTestCountryCode);


    if (bValidQLocaleBuilt == false)
        return false;

    if (sCountryCode.size() == 0)
    {
        retLanguageQtFlag = testLocale.language();
        retCountryQtFlag = QLocale::AnyCountry;
        if (retLanguageQtFlag == QLocale::English && retCountryQtFlag == QLocale::AnyCountry)
            //I consider QLocale behaviour not logical in that case, so here is a correction
            sRetNativeLanguageName = "English";
        else
            sRetNativeLanguageName = testLocale.nativeLanguageName(); // ex: British English, Français..
    }
    else
    {
        retLanguageQtFlag = testLocale.language();
        retCountryQtFlag = testLocale.country();
        sRetNativeLanguageName = testLocale.nativeLanguageName(); // ex: British English, Français..
    }

    return true;
}
