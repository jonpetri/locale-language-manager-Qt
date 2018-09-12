#ifndef MAINSETUP_H
#define MAINSETUP_H
/*----------------------------------------------------------------*/
/*  PROJECT:  localeLanguageManager for Qt - Demo                 */
/*----------------------------------------------------------------*/

/* functions used by main() (and unit tests as well)
 */

#include <QString>
#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"

QString qmFilesFolder();
bool configureLanguageManager(LlmQtLocaleLanguageManager &languageManager);



#endif // MAINSETUP_H



