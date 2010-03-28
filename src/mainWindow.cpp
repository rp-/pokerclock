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

#include "mainWindow.h"

mainWindow::mainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
	//Initialisation des variables
	level = 0;

	//Mise en place de la fenetre des prizes
	frmPrize = new prizeWindow();

	//Mise en place de l'interface
	setupUi ( this );
	connect ( slideTime, SIGNAL ( valueChanged ( int ) ), this, SLOT ( slideTime_valueChanged ( int ) ) );

#ifndef UNIX
	QFont font = lblPlayers->font();
	font.setPointSize ( 25 );
	lblPlayers->setFont ( font );
	lblReBuy->setFont ( font );
	lblAddOn->setFont ( font );

	font = lblTotalTime->font();
	font.setPointSize ( 30 );
	lblTotalTime->setFont ( font );
	lblLevel->setFont ( font );
#endif

	//Mise en place du degrade
	QPalette palette = centralwidget->palette();
	QLinearGradient gradient ( 1,100,1,1000 );
	gradient.setColorAt ( 0, QColor ( 19,14,30 ) );
	gradient.setColorAt ( 1, QColor ( 20,14,90 ) );
	palette.setBrush ( QPalette::Window, QBrush ( gradient ) );
	centralwidget->setPalette ( palette );

	//Valeurs par defaut
	spinPlayersTotal->setMaximum ( MAX_PLAYERS );
	spinPlayers->setMaximum ( MAX_PLAYERS );
	spinPlayersTotal->setValue ( DEFAULT_PLAYER_TOTAL );
	spinPlayers->setValue ( DEFAULT_PLAYER_TOTAL );
	spinPrize->setValue ( DEFAULT_PRIZE );
	spinStack->setValue ( DEFAULT_STACK );
	timeLevels->setTime ( DEFAULT_TIME );
	timeReBuy->setTime ( DEFAULT_TIME_REBUY );
	timeAddOn->setTime ( DEFAULT_TIME_ADDON );
	timeAddOn2->setTime ( DEFAULT_TIME_ADDON2 );

	//Chargement des parametres
	loadConfig();

	//Reglage du tableau de blinds et d'antes
	tableValues->setRowCount ( NUM_LEVELS );
	tableValues->setColumnCount ( 3 );
	tableValues->setColumnWidth ( SMALL_VALUE, DEFAULT_VALUES_COLUMN_WEIGHT );
	tableValues->setColumnWidth ( BIG_VALUE, DEFAULT_VALUES_COLUMN_WEIGHT );
	tableValues->setColumnWidth ( ANTE_VALUE, DEFAULT_VALUES_COLUMN_WEIGHT );
	QTableWidgetItem *newItem = new QTableWidgetItem ( tr ( "Small" ) );
	tableValues->setHorizontalHeaderItem ( SMALL_VALUE, newItem );
	newItem = new QTableWidgetItem ( tr ( "Big" ) );
	tableValues->setHorizontalHeaderItem ( BIG_VALUE, newItem );
	newItem = new QTableWidgetItem ( tr ( "Ante" ) );
	tableValues->setHorizontalHeaderItem ( ANTE_VALUE, newItem );
	
	setDefaultValues();
	
	//Combo prizes
	int	prizes[NUM_PRIZES2] = TAB_DEFAULT_PRIZES;
	for (int i = 0; i < NUM_PRIZES2; i++)
		cmbPrize->addItem(QString::number(prizes[i]), i);
		
	//Reglage du tableau des prizes
	tablePrizes->setRowCount ( NUM_PRIZES );
	tablePrizes->setColumnCount ( 1 );
	tablePrizes->setColumnWidth ( 0, DEFAULT_PRIZES_COLUMN_WEIGHT );
	newItem = new QTableWidgetItem ( tr ( "Prize (%)" ) );
	tablePrizes->setHorizontalHeaderItem ( 0, newItem );
	
	cmbPrize->setCurrentIndex ( DEFAULT_NUM_PRIZES2 - 1 );

	//Timer
	timer = new QTimer ( this );
	timer->setInterval ( DEFAULT_INTERVAL );
	connect ( timer, SIGNAL ( timeout() ), this, SLOT ( timer_timeout() ) );

	//Verification de presence du fichier audio
	if ( !QFile::exists ( SOUND_FILE ) )
	{
		QMessageBox::critical ( this, tr ( NAME_APP ) + tr ( " - Sound" ), tr ( "Sound File is not present !\nSound disabled." ) );
		grpSound->setEnabled ( false );
	}
}

mainWindow::~mainWindow()
{}

/////////////////////////////////////////////////////////
//                        SLOTS                        //
/////////////////////////////////////////////////////////

void		mainWindow::on_toolMenu_clicked()
{
	dockMenu->setVisible ( !dockMenu->isVisible() );
	if ( dockMenu->isVisible() )
		toolMenu->setIcon ( QIcon ( ":/images/menu1.png" ) );
	else
		toolMenu->setIcon ( QIcon ( ":/images/menu2.png" ) );
}

void		mainWindow::on_toolQuit_clicked()
{
	exit ( 1 );
}

void		mainWindow::on_toolFullScreen_clicked()
{
	if ( isFullScreen() )
		showNormal();
	else
		showFullScreen();
}

void		mainWindow::on_toolAbout_clicked()
{
	QMessageBox::about ( this, tr ( "About " ) + NAME_APP, ABOUT_TEXT );
}

void		mainWindow::on_toolStart_clicked ( )
{
	int lineErr;

	//Verification du tableau des blinds et d'antes
	if ( ( lineErr = valuesNotValid() ) )
	{
		QMessageBox::critical ( this, tr ( NAME_APP ) + tr ( " - Values Table" ), tr ( "Values Table isn't valid !\nLine " ) + QString::number ( lineErr )  + tr ( "." ) );
		return;
	}

	//Verification du tableau des prizes
	if ( !prizesValid() )
	{
		QMessageBox::critical ( this, tr ( NAME_APP ) + tr ( " - Prizes Table" ), tr ( "Prizes Table isn't valid !\nTotal must be 100%" ) );
		return;
	}

	spinPlayers->setMaximum ( spinPlayersTotal->value() );

	//Mise a jour des widgets de l'interface
	toolStart->setEnabled ( false );
	toolPause->setEnabled ( true );
	toolStop->setEnabled ( true );
	lblTimeLevels->setEnabled ( false );
	timeLevels->setEnabled ( false );
	lblSpinPrize->setEnabled ( false );
	spinPrize->setEnabled ( false );
	lblSpinStack->setEnabled ( false );
	spinStack->setEnabled ( false );
	spinPlayers->setEnabled ( true );
	spinPlayersTotal->setEnabled ( false );
	lblChkAnte->setEnabled ( false );
	chkAnte->setEnabled ( false );
	lblChkReBuy->setEnabled ( false );
	chkReBuy->setEnabled ( false );
	lblChkAddOn->setEnabled ( false );
	chkAddOn->setEnabled ( false );

	tabValues->setEnabled ( false );
	tabPrizes->setEnabled ( false );

	slideTime->setEnabled ( true );

	if ( chkReBuy->isChecked() )
	{
		lblTimeReBuy->setEnabled ( false );
		timeReBuy->setEnabled ( false );
		lblReBuy->setText ( "0" );
	}
	if ( chkAddOn->isChecked() )
	{
		lblTimeAddOn->setEnabled ( false );
		timeAddOn->setEnabled ( false );
		lblTimeAddOn2->setEnabled ( false );
		timeAddOn2->setEnabled ( false );
		lblAddOn->setText ( "0" );
	}

	//Mise en route des compteurs
	timeTotalStart = QTime::currentTime ();
	timeLevelStart = QTime::currentTime ();
	change_level ( level );
	timer->start();
	timer_timeout();
}

void		mainWindow::on_toolPause_clicked ( )
{
	if ( toolPause->isChecked() )
	{
		timePause = QTime::currentTime();
		timer->stop();
	}
	else
	{
		timeTotalStart = timeTotalStart.addSecs ( timePause.secsTo ( QTime::currentTime() ) );
		timeLevelStart = timeLevelStart.addSecs ( timePause.secsTo ( QTime::currentTime() ) );
		timer->start();
	}
}

void		mainWindow::on_toolStop_clicked ()
{
	timer->stop();

	//Remplissage du tableau de prizes
	frmPrize->lblInitial->setText ( QString::number ( spinPlayersTotal->value() * spinPrize->value() ) );
	frmPrize->lblReBuy->setText ( QString::number ( spinReBuy->value() * spinPrize->value() ) );
	frmPrize->lblAddOn->setText ( QString::number ( spinAddOn->value() * spinPrize->value() ) );
	frmPrize->lblTotal->setText ( lblTotalPrize->text() );

	for ( int i = 0; i < NUM_PRIZES; i++ )
	{
		QTableWidgetItem *newItem = new QTableWidgetItem ( QString::number ( lblTotalPrize->text().toDouble() * getPrize ( i ) / 100 ) );
		frmPrize->tablePrizes->setItem ( i, 0, newItem );
	}

	//Reinitialise les valeurs des indicateurs
	spinPlayers->setMaximum ( MAX_PLAYERS );
	spinPlayers->setValue ( spinPlayersTotal->value() );

	lblTotalTime->setText ( "00:00:00" );
	lblCurrentBlinds->setText ( "0/0" );
	lblTime->setText ( "00:00" );
	lblLevel->setText ( "0" );
	lblLevel2->setText ( "0" );
	lblCurrentAntes->setText ( "0" );
	slideTime->setValue ( 0 );
	spinReBuy->setValue ( 0 );
	spinAddOn->setValue ( 0 );
	level = 0;

	//Mise a jour des widgets de l'interface
	toolStart->setEnabled ( true );
	toolPause->setEnabled ( false );
	toolPause->setChecked ( false );
	toolStop->setEnabled ( false );
	toolLevelMinus->setEnabled ( false );
	toolLevelPlus->setEnabled ( false );
	lblTimeLevels->setEnabled ( true );
	timeLevels->setEnabled ( true );
	lblSpinPrize->setEnabled ( true );
	spinPrize->setEnabled ( true );
	lblSpinStack->setEnabled ( true );
	spinStack->setEnabled ( true );
	spinPlayers->setEnabled ( false );
	spinPlayersTotal->setEnabled ( true );
	lblChkAnte->setEnabled ( true );
	chkAnte->setEnabled ( true );
	lblChkReBuy->setEnabled ( true );
	chkReBuy->setEnabled ( true );
	lblChkAddOn->setEnabled ( true );
	chkAddOn->setEnabled ( true );

	tabValues->setEnabled ( true );
	tabPrizes->setEnabled ( true );

	slideTime->setEnabled ( false );

	if ( chkReBuy->isChecked() )
	{
		lblTimeReBuy->setEnabled ( true );
		timeReBuy->setEnabled ( true );
		lblSpinReBuy->setEnabled ( false );
		spinReBuy->setEnabled ( false );
	}
	else
		lblReBuy->setText ( tr ( "No" ) );

	if ( chkAddOn->isChecked() )
	{
		lblTimeAddOn->setEnabled ( true );
		timeAddOn->setEnabled ( true );
		lblTimeAddOn2->setEnabled ( true );
		timeAddOn2->setEnabled ( true );
		lblSpinAddOn->setEnabled ( false );
		spinAddOn->setEnabled ( false );
	}
	else
		lblAddOn->setText ( tr ( "No" ) );

	//Afficage du tableau des prizes
	frmPrize->show();
}

void		mainWindow::on_toolLevelMinus_clicked()
{
	if ( level > 0 )
	{
		change_level ( --level, true );
		timeTotalStart = timeTotalStart.addSecs ( timeLevels->time().minute() * 60 );
	}
}

void		mainWindow::on_toolLevelPlus_clicked()
{
	if ( level < NUM_LEVELS - 1 )
	{
		change_level ( ++level, true );
		timeTotalStart = timeTotalStart.addSecs ( - ( timeLevels->time().minute() * 60 ) );
	}
}

void		mainWindow::on_spinPrize_valueChanged ( )
{
	updateTotalPrize();
}

void		mainWindow::on_spinStack_valueChanged ()
{
	updateAvgStack();
	updateTotalChips();
}

void		mainWindow::on_spinPlayersTotal_valueChanged ( int i )
{
	if ( !toolStop->isEnabled() )
		spinPlayers->setValue ( i );

	updateAvgStack();
	updateTotalChips();
	updateTotalPrize();
	updateLblPlayers();
}

void		mainWindow::on_spinPlayers_valueChanged ()
{
	updateAvgStack();
	updateLblPlayers();
}

void		mainWindow::on_chkReBuy_stateChanged ( int state )
{
	if ( state == Qt::Checked )
	{
		lblTimeReBuy->setEnabled ( true );
		timeReBuy->setEnabled ( true );
		lblReBuy->setText ( "0" );
	}
	if ( state == Qt::Unchecked )
	{
		lblTimeReBuy->setEnabled ( false );
		timeReBuy->setEnabled ( false );
		lblReBuy->setText ( tr ( "No" ) );
	}
}

void		mainWindow::on_chkAddOn_stateChanged ( int state )
{
	if ( state == Qt::Checked )
	{
		lblTimeAddOn->setEnabled ( true );
		timeAddOn->setEnabled ( true );
		lblTimeAddOn2->setEnabled ( true );
		timeAddOn2->setEnabled ( true );
		lblAddOn->setText ( "0" );
	}
	if ( state == Qt::Unchecked )
	{
		lblTimeAddOn->setEnabled ( false );
		lblTimeAddOn2->setEnabled ( false );
		timeAddOn->setEnabled ( false );
		timeAddOn2->setEnabled ( false );
		lblAddOn->setText ( tr ( "No" ) );
	}
}

void		mainWindow::on_spinReBuy_valueChanged()
{
	lblReBuy->setText ( QString::number ( spinReBuy->value() ) );

	updateAvgStack();
	updateTotalChips();
	updateTotalPrize();
}

void		mainWindow::on_spinAddOn_valueChanged()
{
	lblAddOn->setText ( QString::number ( spinAddOn->value() ) );

	updateAvgStack();
	updateTotalChips();
	updateTotalPrize();
}

void		mainWindow::on_timeAddOn_timeChanged ( const QTime & time )
{
	if ( time > timeAddOn2->time() )
		timeAddOn2->setTime ( time );
}

void		mainWindow::on_timeAddOn2_timeChanged ( const QTime & time )
{
	if ( time < timeAddOn->time() )
		timeAddOn->setTime ( time );
}

void		mainWindow::slideTime_valueChanged ( int value )
{
	dialTime->setValue ( value );
}

void		mainWindow::on_slideTime_sliderPressed ()
{
	timer->stop();
}

void		mainWindow::on_slideTime_sliderMoved ( int value )
{
	QTime time;

	QTime	oldTimeLevelStart = timeLevelStart;

	timeLevelStart = QTime::currentTime().addSecs ( - ( timeLevels->time().minute() * 60 + timeLevels->time().second() ) * value / 100 );
	timeTotalStart = timeTotalStart.addMSecs ( oldTimeLevelStart.msecsTo ( timeLevelStart ) );

	timer_timeout();
}

void		mainWindow::on_slideTime_sliderReleased ()
{
	on_slideTime_sliderMoved ( slideTime->value() );
	if ( !toolPause->isChecked() )
		timer->start();
}

void		mainWindow::on_dockMenu_topLevelChanged ( bool topLevel )
{
	QRect rect;

	toolMenu->setVisible ( !topLevel );

	if ( topLevel )
		dockMenu->setWindowTitle ( tr ( "PokerClock Menu" ) );
	else
		dockMenu->setWindowTitle ( tr ( "Menu" ) );

}

void		mainWindow::on_tableValues_itemChanged ( QTableWidgetItem * item )
{
	//Si pas valide, remplace par 0
	if ( item->text().toInt() <= 0 )
		item->setText ( "0" );
}

void		mainWindow::on_btnVerifyValues_clicked()
{
	int lineErr;

	if ( ( lineErr = valuesNotValid() ) )
		QMessageBox::critical ( this, tr ( NAME_APP ) + tr ( " - Values Table" ), tr ( "Values Table isn't valid !\nLine " ) + QString::number ( lineErr ) + tr ( "." ) );
	else
		QMessageBox::information ( this, tr ( NAME_APP ) + tr ( " - Values Table" ), tr ( "Values Table is valid !\n" ) );
}

void		mainWindow::on_btnDefaultValues_clicked()
{
	setDefaultValues();
}

void		mainWindow::on_btnVerifyPrizes_clicked()
{
	if ( !prizesValid() )
		QMessageBox::critical ( this, tr ( NAME_APP ) + tr ( " - Prizes Table" ), tr ( "Prizes Table isn't valid !\nTotal must be 100%" ) );
	else
		QMessageBox::information ( this, tr ( NAME_APP ) + tr ( " - Prizes Table" ), tr ( "Prizes Table is valid !\n" ) );
}

void		mainWindow::on_cmbPrize_currentIndexChanged ()
{
	setDefaultPrizes();
}

void		mainWindow::on_btnDefaultPrizes_clicked()
{
	setDefaultPrizes();
}

void		mainWindow::on_btnTestSound_clicked()
{
	sound();
}

void		mainWindow::on_btnDefaultSettings_clicked()
{
	if ( DEFAULT_SYSTEM_SOUND == SYSTEM_SOUND_NO )
		radioNoSound->setChecked ( true );
	if ( DEFAULT_SYSTEM_SOUND == SYSTEM_SOUND_QT )
		radioQtSound->setChecked ( true );
	if ( DEFAULT_SYSTEM_SOUND == SYSTEM_SOUND_OTHER )
		radioOtherSound->setChecked ( true );
	txtSound->setText ( DEFAULT_SYSTEM_SOUND_OTHER );
	chkPause->setChecked ( DEFAULT_PAUSE_CHANGE_LEVEL );
}

void		mainWindow::on_btnSave_clicked()
{
	saveConfig();
}

void		mainWindow::timer_timeout()
{
	QTime timeTotal;
	QTime timeLevelInc;
	QTime timeLevelDec;

	//Calcul du temps total
	timeTotal = timeTotal.addSecs ( timeTotalStart.secsTo ( QTime::currentTime() ) );

	//Verification d'activation des ReBuy
	if ( chkReBuy->isChecked() )
	{
		if ( timeReBuy->time() <= timeTotal )
		{
			lblSpinReBuy->setEnabled ( false );
			spinReBuy->setEnabled ( false );
		}
		else
		{
			lblSpinReBuy->setEnabled ( true );
			spinReBuy->setEnabled ( true );
		}
	}

	//Verification de d'activation des AddOn
	if ( chkAddOn->isChecked() )
	{
		if ( timeAddOn->time() <= timeTotal && timeTotal <= timeAddOn2->time() )
		{
			lblSpinAddOn->setEnabled ( true );
			spinAddOn->setEnabled ( true );
		}
		else
		{
			lblSpinAddOn->setEnabled ( false );
			spinAddOn->setEnabled ( false );
		}
	}

	//Calcul du temps ecoule pour la blind et ante
	timeLevelInc = timeLevelInc.addSecs ( timeLevelStart.secsTo ( QTime::currentTime() ) );

	//Verification si changement de level
	if ( ( timer->isActive() ) && ( timeLevelInc >= timeLevels->time() ) )
	{
		sound();
		timeLevelStart = timeLevelStart.addSecs ( timeLevels->time().minute() * 60 + timeLevels->time().second() );
		if ( level < NUM_LEVELS - 1 )
			change_level ( ++level );
		return;
	}

	//Calcul du temps restant pour la blind et ante
	timeLevelDec = timeLevelDec.addSecs ( timeLevelInc.secsTo ( timeLevels->time() ) );

	//Remplir les indicateurs
	lblTotalTime->setText ( timeTotal.toString ( "hh:mm:ss" ) );
	lblTime->setText ( timeLevelDec.toString ( "mm:ss" ) );
	slideTime->setValue ( 100 - ( timeLevelDec.minute() * 60 + timeLevelDec.second() ) * 100 / ( timeLevels->time().minute() *60 + timeLevels->time().second() ) );
}

/////////////////////////////////////////////////////////////
//                        FONCTIONS                        //
/////////////////////////////////////////////////////////////

int		mainWindow::getDefaultValue ( int level, int type )
{
	int values[NUM_LEVELS + 1][3] = TAB_DEFAULT_VALUES;

	if ( ( level < 0 ) || ( level >= NUM_LEVELS ) || ( type < 0 ) || ( type > 2 ) )
		return ( -1 );

	return ( values[level][type] );
}

int		mainWindow::getValue ( int level, int type )
{
	QTableWidgetItem *item;

	if ( ( level < 0 ) || ( level >= NUM_LEVELS ) || ( type < 0 ) || ( type > 2 ) )
		return ( -1 );
	item = tableValues->item ( level, type );
	return ( item->text().toInt() );
}

double		mainWindow::getDefaultPrize ( int num, int type )
{
	double prizes[NUM_PRIZES2][NUM_PRIZES] = TAB_DEFAULT_PRIZES2;

	if ( ( num < 0 ) || ( level >= NUM_PRIZES ) )
		return ( -1 );
	return ( prizes[type][num] );
}

double		mainWindow::getPrize ( int num )
{
	QTableWidgetItem *item;

	if ( ( num < 0 ) || ( num >= NUM_PRIZES ) )
		return ( -1 );
	item = tablePrizes->item ( num, 0 );
	return ( item->text().toDouble() );
}

void		mainWindow::setDefaultValues()
{
	//Remplissage du tableau de blinds et d'antes
	for ( int i = 0; i < NUM_LEVELS; i++ )
	{
		QTableWidgetItem *newItem = new QTableWidgetItem ( QString::number ( getDefaultValue ( i, SMALL_VALUE ) ) );
		tableValues->setItem ( i, SMALL_VALUE, newItem );
		newItem = new QTableWidgetItem ( QString::number ( getDefaultValue ( i, BIG_VALUE ) ) );
		tableValues->setItem ( i, BIG_VALUE, newItem );
		newItem = new QTableWidgetItem ( QString::number ( getDefaultValue ( i, ANTE_VALUE ) ) );
		tableValues->setItem ( i, ANTE_VALUE, newItem );
	}
}

void		mainWindow::setDefaultPrizes()
{
	//Remplissage du tableau de prizes
	for ( int i = 0; i < NUM_PRIZES; i++ )
	{
		QTableWidgetItem *newItem = new QTableWidgetItem ( QString::number ( getDefaultPrize ( i, cmbPrize->itemData (cmbPrize->currentIndex()).toInt() ) ) );
		tablePrizes->setItem ( i, 0, newItem );
	}
}

int		mainWindow::valuesNotValid()
{
	int small = 0;
	int big = 0;
	int ante = 0;

	for ( int i = 0; i < NUM_LEVELS; i++ )
	{
		if ( getValue ( i, SMALL_VALUE ) < small )
			return ( i + 1 );
		if ( getValue ( i, BIG_VALUE ) < big )
			return ( i + 1 );
		if ( getValue ( i, ANTE_VALUE ) < ante )
			return ( i + 1 );
		small = getValue ( i, SMALL_VALUE );
		big = getValue ( i, BIG_VALUE );
		ante = getValue ( i, ANTE_VALUE );
		if ( small > big )
			return ( i + 1 );
	}
	return ( 0 );
}

bool		mainWindow::prizesValid()
{
	double total = 0;

	for ( int i = 0; i < NUM_PRIZES; i++ )
		total += getPrize ( i );
	if ( total == 100 )
		return ( true );
	else
		return ( false );
}

void		mainWindow::updateAvgStack()
{
	lblAvgStack->setText ( QString::number ( ( ( spinPlayersTotal->value() + spinReBuy->value() + spinAddOn->value() ) * spinStack->value() ) / spinPlayers->value() ) );
}

void		mainWindow::updateTotalChips()
{
	lblTotalChips->setText ( QString::number ( ( ( spinPlayersTotal->value() + spinReBuy->value() + spinAddOn->value() ) * spinStack->value() ) ) );
}

void		mainWindow::updateTotalPrize()
{
	lblTotalPrize->setText ( QString::number ( ( ( spinPlayersTotal->value() + spinReBuy->value() + spinAddOn->value() ) * spinPrize->value() ) ) );
}

void		mainWindow::updateLblPlayers()
{
	lblPlayers->setText ( QString::number ( spinPlayers->value() ) + "/" + QString::number ( spinPlayersTotal->value() ) );
}

void		mainWindow::change_level ( int level, bool noPause )
{
	if ( chkPause->isChecked() && level != 0 && !noPause )
	{
		toolPause->setChecked ( true );
		on_toolPause_clicked ( );
	}

	//Grise les boutons si necessaire
	if ( level == 0 )
		toolLevelMinus->setEnabled ( false );
	else
		toolLevelMinus->setEnabled ( true );
	if ( level >= NUM_LEVELS - 1 )
		toolLevelPlus->setEnabled ( false );
	else
		toolLevelPlus->setEnabled ( true );

	lblLevel->setText ( QString::number ( level + 1 ) );
	lblLevel2->setText ( lblLevel->text() );
	lblCurrentBlinds->setText ( QString::number ( getValue ( level, SMALL_VALUE ) ) + "/" + QString::number ( getValue ( level, BIG_VALUE ) ) );
	if ( chkAnte->isChecked() )
		lblCurrentAntes->setText ( QString::number ( getValue ( level, ANTE_VALUE ) ) );
}

void		mainWindow::sound()
{
	if ( !grpSound->isEnabled() )
		return;
	if ( radioQtSound->isChecked() )
	{
		QSound bells ( SOUND_FILE );
		if ( bells.isAvailable () )
			bells.play();
		else
			QMessageBox::critical ( this, tr ( NAME_APP ), tr ( "Error : Qt System Sound" ) );
	}

	if ( radioOtherSound->isChecked() )
	{
		QString command = txtSound->text();
		command.replace ( SOUND_FILE_TAG, SOUND_FILE );
		QProcess::startDetached ( command );
	}
}

void		mainWindow::loadConfig()
{
	QSettings       settings ( CONF_PATH, QSettings::IniFormat );

	if ( settings.value ( "systemSound", DEFAULT_SYSTEM_SOUND ).toInt() == DEFAULT_SYSTEM_SOUND )
		radioNoSound->setChecked ( true );
	if ( settings.value ( "systemSound", DEFAULT_SYSTEM_SOUND ).toInt() == SYSTEM_SOUND_QT )
		radioQtSound->setChecked ( true );
	if ( settings.value ( "systemSound", DEFAULT_SYSTEM_SOUND ).toInt() == SYSTEM_SOUND_OTHER )
		radioOtherSound->setChecked ( true );
	txtSound->setText ( settings.value ( "systemSoundOther", DEFAULT_SYSTEM_SOUND_OTHER ).toString() );
	chkPause->setChecked ( settings.value ( "pauseLevelChange", DEFAULT_PAUSE_CHANGE_LEVEL ).toBool() );
}

void		mainWindow::saveConfig()
{
	QSettings       settings ( CONF_PATH, QSettings::IniFormat );

	if ( radioNoSound->isChecked() )
		settings.setValue ( "systemSound", SYSTEM_SOUND_NO );
	if ( radioQtSound->isChecked() )
		settings.setValue ( "systemSound", SYSTEM_SOUND_QT );
	if ( radioOtherSound->isChecked() )
		settings.setValue ( "systemSound", SYSTEM_SOUND_OTHER );
	settings.setValue ( "systemSoundOther", txtSound->text() );
	settings.setValue ( "pauseLevelChange", chkPause->isChecked() );
}
