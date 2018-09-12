#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>


#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QLocale>
#include <string>

#include "localeLanguageManager/LlmQtLanguage.h"

BOOST_AUTO_TEST_SUITE( LlmQtLanguage_suite )

bool isExceptionOk( std::runtime_error const& ex )
{
    std::string sWhat(ex.what());
    return  sWhat.size()>0;
}



BOOST_AUTO_TEST_CASE(constructor)
{
    LlmQtLanguage l;

    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}

BOOST_AUTO_TEST_CASE(setQAppDefaultLanguage1)
{
    LlmQtLanguage l;
    int argc=0;
    QApplication app(argc, nullptr);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::Belgium));
    // Choosing Belgium, the displayed nativeLanguageName is just "English".
    // Otherwise it can be "American English", "British English"...
    l.setQAppDefaultLanguage();
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::Belgium);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::English);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "BE");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "English (BE)");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "English (BE)");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "en");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}
BOOST_AUTO_TEST_CASE(setQAppDefaultLanguage2)
{
    LlmQtLanguage l;
    int argc=0;
    QApplication app(argc, nullptr);
    QLocale::setDefault(QLocale(QLocale::Basque, QLocale::Spain));
    l.setQAppDefaultLanguage();
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::Spain);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::Basque);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "ES");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "eu");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "Basque (ES)");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "euskara (ES)");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}
BOOST_AUTO_TEST_CASE(set1)
{
    LlmQtLanguage l;
    bool bRet;
    bRet = l.set(QLocale::German);
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::German);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "de");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "German");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "Deutsch");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(set2)
{
    LlmQtLanguage l;
    bool bRet;
    bRet = l.set(QLocale::German, QLocale::Germany);
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::Germany);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::German);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "DE");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "de");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "German (DE)");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "Deutsch (DE)");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(set3)
{
    LlmQtLanguage l;
    bool bRet;
    bRet = l.set(QLocale::English);

    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::English);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "en");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "English");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "English");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(set4)
{
    LlmQtLanguage l;
    bool bRet;
    QString sErrMsg;
   // the language/country couple is not compatible
    bRet = l.set(QLocale::English, QLocale::France, &sErrMsg );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
}
BOOST_AUTO_TEST_CASE(set5)
{
    LlmQtLanguage l;

   // the language/country couple is not compatible
    BOOST_CHECK_EXCEPTION(l.set(QLocale::English, QLocale::France) , std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}


BOOST_AUTO_TEST_CASE(setQmFileProperties1)
{
    LlmQtLanguage l;
    bool bRet;
    bRet = l.setQmFileProperties("_eu.qm");
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::Basque);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "eu");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "Basque");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "euskara");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "_eu.qm");
    BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(setQmFileProperties2)
{
    LlmQtLanguage l;
    bool bRet;
    bRet = l.setQmFileProperties("_zh_CN.qm");
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::China);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::Chinese);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "CN");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "zh");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "Chinese (CN)");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "简体中文 (CN)");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "_zh_CN.qm");
    BOOST_CHECK_EQUAL(bRet, true);
}


BOOST_AUTO_TEST_CASE(setQmFileProperties3)
{
    LlmQtLanguage l;

    BOOST_CHECK_EXCEPTION(l.setQmFileProperties(""),std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}
BOOST_AUTO_TEST_CASE(setQmFileProperties3b)
{
    LlmQtLanguage l;
    bool bRet;
        QString sErrMsg;
    bRet = l.setQmFileProperties("", &sErrMsg );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
}
BOOST_AUTO_TEST_CASE(setQmFileProperties4)
{
    LlmQtLanguage l;
    bool bRet;
    QString sErrMsg;
    bRet = l.setQmFileProperties("zh_CN.qm", &sErrMsg );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
}
BOOST_AUTO_TEST_CASE(setQmFileProperties4b)
{
    LlmQtLanguage l;

    BOOST_CHECK_EXCEPTION(l.setQmFileProperties("en_FR.qm"),std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}
BOOST_AUTO_TEST_CASE(setQmFileProperties5)
{
    LlmQtLanguage l;

    BOOST_CHECK_EXCEPTION(l.setQmFileProperties("_zmh_CN.qm"),std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}
BOOST_AUTO_TEST_CASE(setQmFileProperties6)
{
    LlmQtLanguage l;

    BOOST_CHECK_EXCEPTION(l.setQmFileProperties("_zmh.qm"),std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}
BOOST_AUTO_TEST_CASE(setQmFileProperties6b)
{
    LlmQtLanguage l;
    bool bRet;
    QString sErrMsg;
    bRet = l.setQmFileProperties("_zmh.qm", &sErrMsg);
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
}

BOOST_AUTO_TEST_CASE(setQmFileProperties7)
{
    LlmQtLanguage l;

    BOOST_CHECK_EXCEPTION(l.setQmFileProperties("en.qm"),std::runtime_error, isExceptionOk );
    BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
    BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
    BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
    BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");

}

BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties1)
{
   LlmQtLanguage l;
   bool bRet;
   bRet = l.setLanguageScriptCountryCodeProperties("fr");
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::French);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "fr");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "French");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "français");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
   BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties2)
{
   LlmQtLanguage l;
   bool bRet;
   bRet = l.setLanguageScriptCountryCodeProperties("fr-FR");
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::France);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::French);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "FR");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "fr");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "French (FR)");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "français (FR)");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
   BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties3)
{
   LlmQtLanguage l;
   bool bRet;
   bRet = l.setLanguageScriptCountryCodeProperties("zh-Hans-CN");
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::China);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::Chinese);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "CN");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "zh");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "Chinese (CN)");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "简体中文 (CN)");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
   BOOST_CHECK_EQUAL(bRet, true);
}
BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties_exception1)
{
   LlmQtLanguage l;

   BOOST_CHECK_EXCEPTION(l.setLanguageScriptCountryCodeProperties("-"),std::runtime_error, isExceptionOk );
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}
BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties_exception2)
{
   LlmQtLanguage l;
   BOOST_CHECK_EXCEPTION(l.setLanguageScriptCountryCodeProperties("hh"),std::runtime_error, isExceptionOk );
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}
BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties_exception3)
{
   LlmQtLanguage l;
   BOOST_CHECK_EXCEPTION(l.setLanguageScriptCountryCodeProperties("en-Hans-CN"),std::runtime_error, isExceptionOk );
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
}

BOOST_AUTO_TEST_CASE(setLanguageScriptCountryCodeProperties_exception4)
{
   LlmQtLanguage l;
   bool bRet;
   QString sErrMsg;
   bRet = l.setLanguageScriptCountryCodeProperties("en-Hans-CN", &sErrMsg);
   BOOST_CHECK_EQUAL(l.countryQtFlag(), QLocale::AnyCountry);
   BOOST_CHECK_EQUAL(l.languageQtFlag(), QLocale::C);
   BOOST_CHECK_EQUAL(l.countryISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.englishNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.nativeNameWithCountry().toStdString(), "");
   BOOST_CHECK_EQUAL(l.languageISOCode().toStdString(), "");
   BOOST_CHECK_EQUAL(l.qmFile().toStdString(), "");
   BOOST_CHECK_EQUAL(bRet, false);
   BOOST_CHECK(sErrMsg.size() > 0);
}



BOOST_AUTO_TEST_CASE(isRealizableBy1)
{
    LlmQtLanguage l, lOther;
    BOOST_CHECK(l.set(QLocale::French, QLocale::Belgium));
    BOOST_CHECK(lOther.set(QLocale::French));
    BOOST_CHECK(l.isRealizableBy(lOther) == true);
}
BOOST_AUTO_TEST_CASE(isRealizableBy2)
{
    LlmQtLanguage l, lOther;
    BOOST_CHECK(l.set(QLocale::French));
    BOOST_CHECK(lOther.set(QLocale::French, QLocale::Belgium));
    BOOST_CHECK(l.isRealizableBy(lOther) == false);
}

BOOST_AUTO_TEST_CASE(isRealizableBy3)
{
    LlmQtLanguage l, lOther;
    BOOST_CHECK(l.set(QLocale::Chinese, QLocale::Taiwan));
    BOOST_CHECK(lOther.set(QLocale::Chinese, QLocale::China));
    BOOST_CHECK(l.isRealizableBy(lOther) == false);
}
BOOST_AUTO_TEST_CASE(isRealizableBy4)
{
    LlmQtLanguage l, lOther;
    BOOST_CHECK(l.set(QLocale::Chinese));
    BOOST_CHECK(lOther.set(QLocale::Chinese));
    BOOST_CHECK(l.isRealizableBy(lOther) == true);
}

BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode1)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;
    bool bRet;
    bRet = LlmQtLanguage::readCodesFromLanguageScriptCountryCode("en", sLanguageCode, sScriptCode, sCountryCode, &sErrorMessage);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "en");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sErrorMessage.toStdString(), "");
}


BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode2)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;
    bool bRet;
    bRet = LlmQtLanguage::readCodesFromLanguageScriptCountryCode("zh_CN", sLanguageCode, sScriptCode, sCountryCode, &sErrorMessage);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "zh");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "CN");
    BOOST_CHECK_EQUAL(sErrorMessage.toStdString(), "");
}

BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode3)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;
    bool bRet;
    bRet = LlmQtLanguage::readCodesFromLanguageScriptCountryCode("zh-Hans-CN", sLanguageCode, sScriptCode, sCountryCode, &sErrorMessage);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "zh");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "Hans");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "CN");
    BOOST_CHECK_EQUAL(sErrorMessage.toStdString(), "");
}
BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode_false1)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;
    bool bRet;
    QString sErrMsg;

      bRet = LlmQtLanguage::readCodesFromLanguageScriptCountryCode(
                "zh-Hans-CN_i", sLanguageCode, sScriptCode, sCountryCode, &sErrMsg);

    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "");
    BOOST_CHECK_EQUAL(bRet, false);
    BOOST_CHECK(sErrMsg.size() > 0);
}

BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode4)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;
    bool bRet;
    QString sErrMsg;
      bRet = LlmQtLanguage::readCodesFromLanguageScriptCountryCode(
                  "fr-US", sLanguageCode, sScriptCode, sCountryCode, &sErrMsg);

    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "fr");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "US");
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK(sErrMsg.size() == 0);
}

BOOST_AUTO_TEST_CASE(readCodesFromLanguageScriptCountryCode_exception1)
{
    QString sLanguageCode, sScriptCode, sCountryCode, sErrorMessage;

    BOOST_CHECK_EXCEPTION(
      LlmQtLanguage::readCodesFromLanguageScriptCountryCode("zh-Hans-CN_i", sLanguageCode, sScriptCode, sCountryCode),
            std::runtime_error, isExceptionOk);
    BOOST_CHECK_EQUAL(sLanguageCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sScriptCode.toStdString(), "");
    BOOST_CHECK_EQUAL(sCountryCode.toStdString(), "");
}



BOOST_AUTO_TEST_CASE(isValidAssociation1)
{
    QString sNativeName;
    QLocale::Country c;
    QLocale::Language l;
    bool bRet;
    bRet = LlmQtLanguage::isValidAssociation("en", "", l, c, sNativeName);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sNativeName.toStdString(), "English");
    BOOST_CHECK_EQUAL(l, QLocale::English);
    BOOST_CHECK_EQUAL(c, QLocale::AnyCountry);
}

BOOST_AUTO_TEST_CASE(isValidAssociation2)
{
    QString sNativeName;
    QLocale::Country c;
    QLocale::Language l;
    bool bRet;
    bRet = LlmQtLanguage::isValidAssociation("zh", "CN", l, c, sNativeName);
    BOOST_CHECK_EQUAL(bRet, true);
    BOOST_CHECK_EQUAL(sNativeName.toStdString(), "简体中文");
    BOOST_CHECK_EQUAL(l, QLocale::Chinese);
    BOOST_CHECK_EQUAL(c, QLocale::China);
}



BOOST_AUTO_TEST_CASE(isValidAssociation_false1)
{
    QString sNativeName;
    QLocale::Country c;
    QLocale::Language l;
    bool bRet;
    QString sErrMsg;
      bRet = LlmQtLanguage::isValidAssociation(
                  "zh", "US", l, c, sNativeName);

      BOOST_CHECK_EQUAL(sNativeName.toStdString(), "");
      BOOST_CHECK_EQUAL(l, QLocale::C);
      BOOST_CHECK_EQUAL(c, QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(bRet, false);

}
BOOST_AUTO_TEST_CASE(isValidAssociation_false2)
{
    QString sNativeName;
    QLocale::Country c;
    QLocale::Language l;
    bool bRet;
    QString sErrMsg;
      bRet = LlmQtLanguage::isValidAssociation(
                  "-", "US", l, c, sNativeName);

      BOOST_CHECK_EQUAL(sNativeName.toStdString(), "");
      BOOST_CHECK_EQUAL(l, QLocale::C);
      BOOST_CHECK_EQUAL(c, QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(bRet, false);

}

BOOST_AUTO_TEST_CASE(isValidAssociation_false3)
{
    QString sNativeName;
    QLocale::Country c;
    QLocale::Language l;
    bool bRet;
    QString sErrMsg;
      bRet = LlmQtLanguage::isValidAssociation(
                  "-", "", l, c, sNativeName);

      BOOST_CHECK_EQUAL(sNativeName.toStdString(), "");
      BOOST_CHECK_EQUAL(l, QLocale::C);
      BOOST_CHECK_EQUAL(c, QLocale::AnyCountry);
    BOOST_CHECK_EQUAL(bRet, false);

}




BOOST_AUTO_TEST_CASE(setSystemLanguage)
{
    int argc=0;
    QApplication app(argc, nullptr);
    LlmQtLanguage lSys, lTranslate;
    QStringList systemLanguages = QLocale::system().uiLanguages();
    QString sErrMsg;

    for (int iSys = 0 ; iSys < systemLanguages.count() ; ++iSys)
    {
        BOOST_CHECK( lSys.setLanguageScriptCountryCodeProperties(systemLanguages[iSys], &sErrMsg) );
        BOOST_CHECK_EQUAL(sErrMsg.toStdString(), "");

        if(lSys.languageQtFlag() == QLocale::English)
        {
            BOOST_CHECK( lTranslate.set( QLocale::English ));
            BOOST_CHECK_EQUAL( lSys.isRealizableBy(lTranslate), true);
        }
        else if(lSys.languageQtFlag() == QLocale::French)
        {
            BOOST_CHECK(lTranslate.set(QLocale::French));
            BOOST_CHECK_EQUAL( lSys.isRealizableBy(lTranslate), true);
        }

    }
}

BOOST_AUTO_TEST_SUITE_END()
