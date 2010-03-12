/***************************************************************************
 *   Copyright (C) 2007 by xes                                             *
 *   pinon.pierre@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef POKERCLOCK_H
#define POKERCLOCK_H

//Parametres generaux
#define NAME_APP					"PokerClock"
#define VERSION_APP					"1.0.4"
#define MAX_PLAYERS					10000
#define DEFAULT_INTERVAL				250

//Autres
#define ABOUT_TEXT					tr("Author : PINON Pierre\nVersion : ") + tr(VERSION_APP) + tr("\nE-Mail : pinon.pierre@gmail.com\nWeb Site : http://xes.sytes.net\n\nReport bugs or anythings else in my e-mail !\n\nThanks : http://www.qtfr.org\n")
#define SMALL_VALUE					0
#define BIG_VALUE					1
#define ANTE_VALUE					2

//Fichier de configuration
#ifdef UNIX
	#define FILE_CONF_PATH			".pokerclock/pokerclock.conf"
#endif
#ifdef WIN32
	#define FILE_CONF_PATH			"pokerclock/pokerclock.conf"
#endif
#define CONF_PATH					QDir::toNativeSeparators (QDir::homePath() + "/" + FILE_CONF_PATH)

//Levels et prizes
#define NUM_LEVELS					30
#define TAB_DEFAULT_VALUES			{{5, 10, 0}, {10, 20, 0}, {15, 30, 0}, {25, 50, 0}, {50, 100, 0}, {75, 150, 0}, {100, 200, 0}, {150, 300, 0}, {200, 400, 0}, {300, 600, 0}, {400, 800, 100}, {500, 1000, 100}, {600, 1200, 100}, {800, 1600, 100}, {1000, 2000, 100}, {1500, 3000, 500}, {2000, 4000, 500}, {3000, 6000, 500}, {4000, 8000, 500}, {5000, 10000, 500}, {6000, 12000, 1500}, {8000, 16000, 1500}, {10000, 20000, 1500}, {15000, 30000, 1500}, {20000, 40000, 1500}, {30000, 60000, 10000}, {40000, 80000, 10000}, {50000, 100000, 10000}, {60000, 120000, 10000}, {80000, 160000, 10000}}

#define NUM_PRIZES					20
#define NUM_PRIZES2					5
#define DEFAULT_NUM_PRIZES2			3
#define TAB_DEFAULT_PRIZES			{1, 2, 3, 5, 10}
#define TAB_DEFAULT_PRIZES2			{{100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {70, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {50, 30, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {40, 24, 16, 12, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {30, 20, 12.5, 9, 8, 6.5, 5, 4, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}

//Parametres par defaut
#define DEFAULT_PLAYER_TOTAL			6
#define DEFAULT_PRIZE				5
#define DEFAULT_STACK				1000
#define DEFAULT_VALUES_COLUMN_WEIGHT	70
#define DEFAULT_PRIZES_COLUMN_WEIGHT	140
#define DEFAULT_TIME				QTime ( 0, 15, 0, 0 )
#define DEFAULT_TIME_REBUY			QTime ( 1, 0, 0, 0 )
#define DEFAULT_TIME_ADDON			QTime ( 1, 0, 0, 0 )
#define DEFAULT_TIME_ADDON2			QTime ( 1, 15, 0, 0 )
#define DEFAULT_PAUSE_CHANGE_LEVEL		false

//System son
#define SOUND_FILE					"event.wav"
#define SOUND_FILE_TAG				"%f"
#define SYSTEM_SOUND_NO				0
#define SYSTEM_SOUND_QT				1
#define SYSTEM_SOUND_OTHER			2
#ifdef UNIX
	#define DEFAULT_SYSTEM_SOUND		SYSTEM_SOUND_NO
	#define DEFAULT_SYSTEM_SOUND_OTHER	"aplay -q %f"
#endif
#ifdef WIN32
	#define DEFAULT_SYSTEM_SOUND		SYSTEM_SOUND_QT
	#define DEFAULT_SYSTEM_SOUND_OTHER	""
#endif

#endif
