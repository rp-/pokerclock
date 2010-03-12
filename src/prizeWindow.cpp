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

#include "prizeWindow.h"

prizeWindow::prizeWindow ( QDialog *parent ) : QDialog ( parent )
{
	//Mise en place de l'interface
	setupUi ( this );
	setWindowFlags ( Qt::WindowStaysOnTopHint );
	setModal ( true );
	
	//Reglage du tableau des prizes
	tablePrizes->setRowCount ( NUM_PRIZES );
	tablePrizes->setColumnCount ( 1 );
	tablePrizes->setColumnWidth ( 0, DEFAULT_PRIZES_COLUMN_WEIGHT );
	QTableWidgetItem *newItem = new QTableWidgetItem ( "Prize" );
	tablePrizes->setHorizontalHeaderItem ( 0, newItem );
}

prizeWindow::~prizeWindow()
{}

void prizeWindow::on_btnOk_clicked()
{
	close();
}
