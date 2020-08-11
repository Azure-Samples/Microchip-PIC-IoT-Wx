/*
    \file   banner.h

    \brief  banner text.

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#ifndef BANNER_H
#define BANNER_H

#if defined(__AVR__)
#define BANNER "\33[34m"\
"                                                       .   \r\n"\
"        ;;;;;;.  ;;;;;;          ;;;;;;  `;;;;;;;;;:  `.,  \r\n"\
"       `'''''';  `''''',        .''''',  ;'''''''''''`.`,  \r\n"\
"       :'''''''   ''''''        ;'''';   ''''''''''''' `   \r\n"\
"       '''''''':  ,'''''`      `''''':  :''''''''''''':    \r\n"\
"      `'''''''''   ''''';      :'''''   ''''':  `''''';    \r\n"\
"      ;'''''''''.  ;'''''      ''''';  .'''''`   :'''''    \r\n"\
"      ''''':'''';  `''''':    .'''''`  ;'''';    ,'''''    \r\n"\
"     :''''; '''''`  ;'''';    ''''';  `''''',    ;'''';    \r\n"\
"     '''''` ;'''';  .'''''`  `'''''.  :'''''    ;''''':    \r\n"\
"    `'''''  .'''''   ''''';  ;'''''   '''''';;''''''''     \r\n"\
"    ;'''';   ''''':  :'''''  ''''',  .'''''''''''''''      \r\n"\
"    '''''`   :'''''   '''''.,'''''   '''''''''''''';       \r\n"\
"   ,'''''    `'''''.  ;'''';;'''';   '''''''''''';,        \r\n"\
"   ;''''';;;;;''''';  `''''''''''`  ;''''',:'''';          \r\n"\
"  `'''''''''''''''''`  ''''''''';   ''''';  '''''          \r\n"\
"  ;''''''''''''''''':  ,'''''''',  :'''''`  ''''':         \r\n"\
"  '''''''''''''''''''   ''''''''   ''''';   ;'''';         \r\n"\
" .''''':,,,,,,,:'''''.  ;'''''';  .''''',   .'''''         \r\n"\
" ;'''''         ''''';  `''''''   ;'''''    `''''':        \r\n"\
" ''''';         ,'''''`  ;'''';  `''''';     ''''';        \r\n"\
",'''''`          ''''';  `''''`  ;'''''`     ;'''''`       \r\n"\
"                          ''''                             \r\n"\
"                          ,'',                             \r\n"\
"                           ''                              \r\n"\
"                           ;:                              \r\n"\
"                           `                               \r\n"\
"\33[0m"\
"                 ooooo           ooooooooooooo\r\n"\
"                 `888'           8'   888   `8\r\n"\
"                  888   .ooooo.       888\r\n"\
"                  888  d88' `88b      888\r\n"\
"                  888  888   888      888\r\n"\
"                  888  888   888      888\r\n"\
"                 o888o `Y8bod8P'     o888o\r\n"
#else
#define BANNER "\33[31m"\
"PPPPPPPPPPPPPPPPP   IIIIIIIIII      CCCCCCCCCCCCC\r\n"\
"P::::::::::::::::P  I::::::::I   CCC::::::::::::C\r\n"\
"P::::::PPPPPP:::::P I::::::::I CC:::::::::::::::C\r\n"\
"PP:::::P     P:::::PII::::::IIC:::::CCCCCCCC::::C\r\n"\
"  P::::P     P:::::P  I::::I C:::::C       CCCCCC\r\n"\
"  P::::P     P:::::P  I::::IC:::::C              \r\n"\
"  P::::PPPPPP:::::P   I::::IC:::::C              \r\n"\
"  P:::::::::::::PP    I::::IC:::::C              \r\n"\
"  P::::PPPPPPPPP      I::::IC:::::C              \r\n"\
"  P::::P              I::::IC:::::C              \r\n"\
"  P::::P              I::::IC:::::C              \r\n"\
"  P::::P              I::::I C:::::C       CCCCCC\r\n"\
"PP::::::PP          II::::::IIC:::::CCCCCCCC::::C\r\n"\
"P::::::::P          I::::::::I CC:::::::::::::::C\r\n"\
"P::::::::P          I::::::::I   CCC::::::::::::C\r\n"\
"PPPPPPPPPP          IIIIIIIIII      CCCCCCCCCCCCC\r\n"\
"\33[0m\r\n"\
" IIIIIIIIII              TTTTTTTTTTTTTTTTTTTTTTT\r\n"\
" I::::::::I              T:::::::::::::::::::::T\r\n"\
" I::::::::I              T:::::::::::::::::::::T\r\n"\
" II::::::II              T:::::TT:::::::TT:::::T\r\n"\
"   I::::I     oooooooooooTTTTTT  T:::::T  TTTTTT\r\n"\
"   I::::I   oo:::::::::::oo      T:::::T        \r\n"\
"   I::::I  o:::::::::::::::o     T:::::T        \r\n"\
"   I::::I  o:::::ooooo:::::o     T:::::T        \r\n"\
"   I::::I  o::::o     o::::o     T:::::T        \r\n"\
"   I::::I  o::::o     o::::o     T:::::T        \r\n"\
"   I::::I  o::::o     o::::o     T:::::T        \r\n"\
"   I::::I  o::::o     o::::o     T:::::T        \r\n"\
" II::::::IIo:::::ooooo:::::o   TT:::::::TT      \r\n"\
" I::::::::Io:::::::::::::::o   T:::::::::T      \r\n"\
" I::::::::I oo:::::::::::oo    T:::::::::T      \r\n"\
" IIIIIIIIII   ooooooooooo      TTTTTTTTTTT      \r\n"

#endif

#endif //BANNER_H
