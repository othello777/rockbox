/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 *   Copyright (C) 2010 by Dominik Riebeling
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef TTSCARBON_H
#define TTSCARBON_H

#include <QtCore>
#include "ttsbase.h"

#include <Carbon/Carbon.h>
#include <inttypes.h>

class TTSCarbon : public TTSBase
{
    Q_OBJECT
    //! Enum to identify the settings
    enum ConfigValuesCarbon
    {
        ConfigVoice,
        ConfigSpeed,
        ConfigPitch
    };
    public:
        TTSCarbon(QObject *parent = NULL);

        //! Child class should generate a clip
        TTSStatus voice(const QString& text, const QString& wavfile, QString* errStr);
        //! Child class should do startup
        bool start(QString *errStr);
        //! child class should stop
        bool stop() ;
        QString voiceVendor(void) { return QString(); }

        // configuration
        //! Child class should return true, when configuration is good
        bool configOk();
         //! Child class should generate and insertSetting(..) its settings
        void generateSettings();
        //! Child class should commit the Settings to permanent storage
        void saveSettings();

        Capabilities capabilities();

    private:
        SpeechChannel m_channel;
        CFStringBuiltInEncodings m_voiceScript;

        unsigned long be2u32(unsigned char* buf);
        unsigned long be2u16(unsigned char* buf);
        unsigned char* u32tobuf(unsigned char* buf, uint32_t val);
        unsigned char* u16tobuf(unsigned char* buf, uint16_t val);
        unsigned int extended2int(unsigned char* buf);
        int convertAiffToWav(const char* aiff, const char* wav);

};

#endif // TTSCARBON_H

