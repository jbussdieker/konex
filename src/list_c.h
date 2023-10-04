/***************************************************************************
 *   Copyright (C) 2005 by ecco   *
 *   ecco@ubertu   *
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
#ifndef LIST_C_H
#define LIST_C_H

/**
@author ecco
*/
template <class TYPE>
class item_c{
	public:
		TYPE item;
		int index;
		item_c *nextitem;
		item_c *previtem;

		item_c()
		{
			nextitem = 0;
			previtem = 0;
			index = 0;
		}
};

template <class TYPE>
class list_c{
public:
	item_c<TYPE> *firstitem;
	item_c<TYPE> *lastitem;

	list_c()
	{
		firstitem = 0;
		lastitem = 0;
	}
	
	void moveback(item_c<TYPE> *item)
	{
		item_c<TYPE> *pitem;
		item_c<TYPE> *nitem;
		
		pitem = item->previtem;
		nitem = item->nextitem;

		// Already at the back
		if (nitem == 0)
			return;
		else
		{
			item->previtem = lastitem;
			item->nextitem = 0;
			lastitem->nextitem = item;
			lastitem = item;
			nitem->previtem = pitem;
		}

		if (pitem == 0)
			firstitem = nitem;
		else
			pitem->nextitem = nitem;
	}

	void movefront(item_c<TYPE> *item)
	{
		item_c<TYPE> *pitem;
		item_c<TYPE> *nitem;
		
		pitem = item->previtem;
		nitem = item->nextitem;

		// Already at the front
		if (pitem == 0)
			return;
		else
		{
			item->nextitem = firstitem;
			item->previtem = 0;
			firstitem->previtem = item;
			firstitem = item;
			pitem->nextitem = nitem;
		}
	
		if (nitem == 0)
			lastitem = pitem;
		else
			nitem->previtem = pitem;
	}

	void moveup(item_c<TYPE> *item)
	{
		item_c<TYPE> *pitem;
		item_c<TYPE> *nitem;

		if (item->previtem != 0)
		{
			pitem = item->previtem;
			nitem = item->nextitem;

			if (pitem->previtem == 0)
				firstitem = item;
			else
				pitem->previtem->nextitem = item;

			item->previtem = pitem->previtem;
			pitem->nextitem = item->nextitem;
			

			pitem->previtem = item;

			if (nitem != 0)
				nitem->previtem = pitem;
			else
				lastitem = pitem;

			item->nextitem = pitem;
		}
	}

	void movedown(item_c<TYPE> *item)
	{
		item_c<TYPE> *pitem;
		item_c<TYPE> *nitem;

		if (item->nextitem != 0)
		{
			pitem = item->previtem;
			nitem = item->nextitem;

			if (nitem->nextitem == 0)
				lastitem = item;
			else
				nitem->nextitem->previtem = item;

			item->nextitem = nitem->nextitem;
			nitem->previtem = item->previtem;
			

			nitem->nextitem = item;

			if (pitem != 0)
				pitem->nextitem = nitem;
			else
				firstitem = nitem;

			item->previtem = nitem;
		}
	}

	item_c<TYPE> *insert(TYPE item)
	{
		item_c<TYPE> *newitem;
		newitem = new item_c<TYPE>;
		newitem->nextitem = 0;
		newitem->previtem = lastitem;
		newitem->item = item;

		if (lastitem == 0)
			firstitem = newitem;
		else
			lastitem->nextitem = newitem;

		lastitem = newitem;

		return newitem;
	}

	int count()
	{
		int c = 0;
		item_c<TYPE> *p = firstitem;

		while (p != 0)
		{
			c++;
			p = p->nextitem;	
		}
		return c;
	}
};

#endif
