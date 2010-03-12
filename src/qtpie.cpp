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

#include "qtpie.h"
#include <QtGui>
#include <QPainter>
#include <QPainterPath>

qtPie::qtPie ( QWidget *parent ) : QWidget ( parent )
{
	setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum );

	proValue = 50;
	proMinimum = 0;
	proMaximum = 100;
}


qtPie::~qtPie()
{}

int qtPie::value()
{
	return ( proValue );
	update ();
}

void qtPie::setValue ( int value )
{
	if ( value > proMaximum )
		value = proMaximum;
	if ( value < proMinimum )
		value = proMinimum;
	proValue = value;
	update();
}

int qtPie::minimum()
{
	return ( proMinimum );
}

void qtPie::setMinimum ( int value )
{
	if ( value > proMaximum )
		value = proMaximum;
	proMinimum = value;
	if ( proMinimum > proValue )
		proValue = proMinimum;
	update();
}

int qtPie::maximum()
{
	return ( proMaximum );
}

void qtPie::setMaximum ( int value )
{
	if ( value < proMinimum )
		value = proMinimum;
	proMaximum = value;
	if ( proMaximum < proValue )
		proValue = proMaximum;
	update();
}

int qtPie::orientation()
{
	return ( 0 );
}

void qtPie::setOrientation ( int value )
{}

void	qtPie::paintEvent ( QPaintEvent *event )
{
	int size = ( width() < height() ? width() : height() );
	int left = ( height() < width() ? ( width() - height() ) /2 : 0 );
	int top = ( width() < height() ? ( height() - width() ) /2 : 0 );

	//Type du trait de la Base
	QPen penBase ( Qt::white, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );
	//Type du trait du Camembert
	QPen penVal ( Qt::NoBrush, 0, Qt::NoPen, Qt::FlatCap, Qt::MiterJoin );
	
	//Font du Camembert
	QLinearGradient gradient ( 1, 1, size, size );
	gradient.setColorAt ( 0, palette().color ( QPalette::Dark ) );
	gradient.setColorAt ( 1, palette().color ( QPalette::Light ) );
	//Font de la Base
	QBrush base = Qt::NoBrush;

	QPainter p ( this );

	//Dessin du Camembert
	p.setPen ( penVal );
	p.setBrush ( QBrush ( gradient ) );
	p.drawPie ( QRectF ( left + MARGE, top + MARGE, size - MARGE * 2, size - MARGE * 2 ), 16 * 90, - 16 * ( ( proValue - proMinimum ) * 360 / ( proMaximum - proMinimum ) ) );

	//Dessin de la Base
	p.setPen ( penBase );
	p.setBrush ( base );
	p.drawEllipse ( left + MARGE, top + MARGE , size - MARGE * 2, size - MARGE * 2 );

	p.end();
}
