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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainWindow.h"
#include "pokerclock.h"
#include <QtGui>
#include "prizeWindow.h"

class mainWindow: public QMainWindow, public Ui::mainWindow
{
		Q_OBJECT
	public:
		mainWindow ( QWidget *parent = 0 );
		~mainWindow();
		
	
	private slots:
		void		on_toolMenu_clicked();
		void		on_toolQuit_clicked();
		void		on_toolFullScreen_clicked();
		void		on_toolAbout_clicked();
		void		on_toolStart_clicked();
		void		on_toolPause_clicked();
		void		on_toolStop_clicked();
		void		on_toolLevelMinus_clicked();
		void		on_toolLevelPlus_clicked();
				
		void		on_spinPrize_valueChanged();
		void		on_spinStack_valueChanged();
		void		on_spinPlayersTotal_valueChanged ( int i );
		void		on_spinPlayers_valueChanged ();
		void		on_chkReBuy_stateChanged( int state );
		void		on_chkAddOn_stateChanged ( int state );
		void		on_spinReBuy_valueChanged();
		void		on_spinAddOn_valueChanged();
		void		on_timeAddOn_timeChanged ( const QTime & time );
		void		on_timeAddOn2_timeChanged ( const QTime & time );
		
		void		slideTime_valueChanged ( int value );
		void		on_slideTime_sliderPressed();
		void		on_slideTime_sliderMoved ( int value );
		void		on_slideTime_sliderReleased();
		
		void		on_dockMenu_topLevelChanged ( bool topLevel );
		
		void		on_tableValues_itemChanged ( QTableWidgetItem * item );
		void		on_btnVerifyValues_clicked();
		void		on_btnDefaultValues_clicked();
		void		on_btnVerifyPrizes_clicked();
		void		on_cmbPrize_currentIndexChanged ();
		void		on_btnDefaultPrizes_clicked();
		void		on_btnTestSound_clicked();
		void		on_btnDefaultSettings_clicked();
		void		on_btnSave_clicked();		
		
		void		timer_timeout();

	private:
		int		getDefaultValue ( int level, int type );
		int		getValue ( int level, int type );
		double	getDefaultPrize ( int num, int type);
		double	getPrize ( int num );
		void		setDefaultValues();
		void		setDefaultPrizes();
		int		valuesNotValid();
		bool		prizesValid();
		void		updateAvgStack();
		void		updateTotalChips();
		void		updateTotalPrize();
		void		updateLblPlayers();
		void		change_level ( int level, bool noPause=false );
		void		sound();
		void		loadConfig();
		void		saveConfig();

		int		level;
		QTimer	*timer;
		QTime	timeTotalStart;
		QTime	timeLevelStart;
		QTime	timePause;
		prizeWindow *frmPrize;
};

#endif
