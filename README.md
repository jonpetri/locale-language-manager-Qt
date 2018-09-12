**Table of contents**
- [Why this repository](#p1)
- [Class's features](#p2)
- [Code documentation](#p3)
- [How to use it](#p4)
- [Use example](#p5)
- [About dialog panels instant translation at running time](#p6)
- [About Cmake + Qt5LinguistTools](#p7)

![Demo's screenshot](/data/images/screenshot_LlmQtLocaleLanguageManager_demo.png)


<br><a name="p1"></a> 
# Why this repository

--------
The Qt framework provide a fantastic module and tool chain to easily implement a multilingual user interface. Anyway when I first used them to implement a multilingual application, I felt that some aspects of my code where a bit messy :<br> 
 \- The language list is obtaind from qm file name analysis... <br> 
 \- No way to retrieve the QTranslator object from qApp once set. (If there is actually a way to do that, let me know)

So I decided to do a simple class in charge of everything about  languages / translations  in my apps. That is : deal with .qm files, language switch, language choice storing in user settings, system language...

Here is that class: LlmQtLocaleLanguageManager. The class is here contained in a simple Cmake project which is also a demonstration of use. The demo has a minimalist QMainWindow and two different language setting panels. The demo contains unit tests and a Doxygen doc generation, and is compilable with Windows and Linux.
You can use / copy everything in here, especially if you face some issues with QTranslator, QLocale and qm files. Everything here is under MIT license.

I'm looking forward to hearing any comment and improvement of my code, so do not hesitate to do so: <jon.petrissans[at]protonmail.com>.


<br><a name="p2"></a> 
# Class's features

--------
LlmQtLocaleLanguageManager is an utilitarian class that simplifies language choice management in a multilingual Qt application. Features are:

|Qm files |
|:---|
|- Qm files analyse, with clear error message in case of issue. Extraction of all the necessary informations. (The Qm file is Qt's app translation source file)  <br> - In case of incorrect language / country association, or incorrect language code, an error is raised (unlike QLocale behaviour which continues running without saying anything).|

|System language|
|:---|
|- System languages analyse, and confrontation to application's translations.|

|App language choice and switch|
|:---|
|- Provide the list of available languages. Languages are called in native form (English, Français, 简体中文...), with associated country if any.  <br> - App language switch: Translator installation in qApp and set of the default QLocale.  <br> - Instant translation at running time is possible.|

|Settings saving and loading|
|:---|
|- Save the chosen language in a QSettings (ini/conf file or registry)  (If feature enabled)  <br> - At start up, load setting's language in qApp.  (If feature enabled)|



<br><a name="p3"></a>
# Code documentation

--------
Class's Doxygen documentation is [here](/doc/LlmQtLocaleLanguageManager.html), under [/doc/](/doc/).



<br><a name="p4"></a>
# How to use the class

--------
> **Prerequisites :** C++11 (for a smart pointer), Qt5 framework, Qt5LinguistTools

1. Copy the 4 files in [/src/localeLanguageManager/](/src/localeLanguageManager/) somewhere in your project. (LlmQtLocaleLanguageManager.h  LlmQtLocaleLanguageManager.cpp  LlmQtLanguage.h  LlmQtLanguage.cpp)
<br><br>
2. Include LlmQtLocaleLanguageManager.h where you want to use it.
<br><br>
3. I suggest to create a single instance of the class in the main() function, and to send it to the sub-objects using it.
The LlmQtLocaleLanguageManager object has to be hold "alive" during all the application run time,
because it contains a pointer to the active QTranslator.
<br><br>
4. At application start up, the LlmQtLocaleLanguageManager object needs some configurations:
 - A QSettings object in which language choice is stored (optional)
 - Application's source (primary) interface language.
 - .qm files location.
<br><br>
5. Then, the object can give many information:
 - It indicates if it's well configured, if it has locale's translations (to enable or not the language settings menu).
 - A list of available languages/translations for the application.
 - System's language.
 - Currently chosen language.
<br><br>
6. The Object provides different methods to dynamically switch to a language at running time.
 - Translate to user's setting's language.
 - Translate to another language.
 - Translate to system language.

	Optionally, the object is in charge of storing choices in user's settings.


<br><a name="p5"></a>
# Use example

--------

### Set up in the main:

~~~~~~~~~~~~~{.cpp}
LlmQtLocaleLanguageManager languageManager;
languageManager.setSettings(userSettings);
languageManager.setKeySettingsTranslateLanguage(SETTINGS_KEY_LOCALE_LANGUAGE);

languageManager.setSourceLanguage(QLocale::English);
// alternative to setSourceLanguage:
//languageManager.setSourceLanguageFromQAppLocalSettings();

sQmFilesPath = .....;
languageManager.setQmFilesDirectory(sQmFilesPath);

if (languageManager.analyseAvailableLanguages(&sErrMsg) == false)
{ ... }

if (languageManager.translateAppToUserSettingsLanguage(&sErrMsg) == false)
{ ... }
~~~~~~~~~~~~~

### In the main window:

~~~~~~~~~~~~~{.cpp}
// Enable access to language menu if m_languageManager is usable.
// Hence the language setting panel is accessible only if we have qm files !!
menu->setEnabled(m_languageManager->isUsable());
~~~~~~~~~~~~~

### Use in language panel:

~~~~~~~~~~~~~{.cpp}
QStringList languagesList = m_languageManager->languageNameNativeList(false);
...
if ( m_languageManager->translateAppToLanguage(sLanguageChosen, &sErrMsg) == false)
{ ... }
...

if (m_languageManager->translateAppToSystemLanguage(&sErrMsg) == false)
{ ... }
~~~~~~~~~~~~~
 

<br><a name="p6"></a>
# About dialog panels instant translation at running time

--------
To achieve a straight translation when the .translateAppToLanguage() method is called,
Each QDialog of the application has to override the .changeEvent() method, as in the following code.
When a new translator is set up, a QEvent::LanguageChange is send to the dialogs.
~~~~~~~~~~~~~{.cpp}
     // Override of QWidget::changeEvent, to implement the dynamic translation of the ui
     // param [in] event UI's state change
     void MainWindow::changeEvent(QEvent* event)
     {
        if (event->type() == QEvent::LanguageChange)
        {
            // retranslate qtDesigner form
            ui->retranslateUi(this);
             // retranslate other widgets which weren't added in designer, if needed
            this->updateWidgets();
        }
       // call of base class implementation
       QWidget::changeEvent(event);
     }
~~~~~~~~~~~~~


<br><a name="p7"></a>
# About Cmake + Qt5LinguistTools

--------
That point was not easy to set up, so it may help some people.

You can find an example of integration of Qt's linguistic tools (lupdate and lrelease) into CMake's build. Thanks to Cmake's QT5_CREATE_TRANSLATION() command, each build updates the translation files (.ts) and generates the translation binaries (.qm).  The translation files are updated according to added and deleted translations in the sources. The [CMakeLists.txt](/data/locale/CMakeLists.txt) file managing all the above is in [/data/locale/](/data/locale/). 

In addition, you will notice that I found a way to add new .ts file through Cmake's cache.
