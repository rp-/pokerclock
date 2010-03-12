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

#ifndef QTPIE_H
#define QTPIE_H

#include <QWidget>

#define MARGE	10

class qtPie: public QWidget
{
		Q_OBJECT
		Q_PROPERTY ( int value READ value WRITE setValue )
		Q_PROPERTY ( int minimum READ minimum WRITE setMinimum )
		Q_PROPERTY ( int maximum READ maximum WRITE setMaximum )
		Q_PROPERTY ( int orientation READ orientation WRITE setOrientation )
	public:
		qtPie ( QWidget *parent = 0 );
		~qtPie();
		int value();
		void setValue(const int value);
		int minimum();
		void setMinimum(const int value);
		int maximum();
		void setMaximum(const int value);
		int orientation();
		void setOrientation(int value);
		
	protected:
		void	paintEvent(QPaintEvent *event);
		
	private:
		int proValue;
		int proMinimum;
		int proMaximum;
};

#endif
