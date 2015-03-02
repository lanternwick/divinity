#ifndef _LIST_H
#define _LIST_H

namespace custom
{
	template<class t>
		class list
	{
	private:
		template<class c> 
			class l_node
		{
		public:
			l_node<c>* next;
			l_node<c>* prev;
			c data;
			l_node() { next = NULL; prev = NULL; }
		};
		l_node<t>* p_front;
		l_node<t>* p_end;
		l_node<t>* p_temp;
		bool empty;
		unsigned long mySize;
	public:
		list()	{
			p_front = NULL;
			p_end = NULL;
			p_temp = NULL;
			mySize = 0;}
		~list() {while(!empty) pop_front();}
		void push_front(const t& data)	{
			push_front(&data);	}
		void push_front(const t* data)	{
			l_node<t>* temp = NULL;
			temp = new l_node<t>;
			temp->data = *data;
			if(p_front)		{
				temp->next = p_front;
				p_front->prev = temp;
				p_front = temp;		}
			else		{
				p_front = temp;
				p_end = temp;		}
			empty = false;
			p_temp = p_front;
			++mySize;
			return;	}
		void push_back(const t& data)	{
			push_back(&data);	}
		void push_back(const t* data)	{
			l_node<t>* temp = NULL;
			temp = new l_node<t>;
			temp->data = *data;
			if(p_end)		{
				temp->prev = p_end;
				p_end->next = temp;
				p_end = temp;		}
			else		{
				p_front = temp;
				p_end = temp;		}
			empty = false;
			p_temp = p_front;
			++mySize;
			return;	}
		t peek_front()	{
			if(p_front)
				return p_front->data;
			throw "List is Empty!";	}
		t peek_back()	{
			if(p_end)
				return p_end->data;
			throw "List is Empty!";	}
		void pop_front()	{
			l_node<t>* temp = p_front;
			--mySize;
			if(p_front == p_end)		{
				empty = true;
				delete p_front;
				p_temp = p_end = p_front = NULL;
				return;		}
			if(!temp)
				throw "List is Empty!", ++mySize;
			p_front = p_front->next;
			p_front->prev = NULL;
			delete temp;
			p_temp = p_front; }
		void pop_back()	{
			l_node<t>* temp = p_end;
			--mySize;
			if(p_front == p_end)		{
				empty = true;
				delete p_end;
				p_temp = p_end = p_front = NULL;
				return;		}
			if(!temp)
				throw "List is Empty!", ++mySize;
			p_end = p_end->prev;
			p_end->next = NULL;
			p_temp = p_front;
			
			
			delete temp;	}
		void remove(t* r)
		{
			if(mySize == 1)
			{
				delete p_front;
				p_front = p_end = NULL;
				--mySize;
				return;
			}
			iterator_reset(0);
			for(int i = 0; i < mySize; ++i, (*this)++)
			{
				if((*(*this)) == (*r))
				{
					p_temp->prev->next = p_temp->next;
					p_temp->next->prev = p_temp->prev;
					delete p_temp;
					iterator_reset(0);
					return;
				}
			}
			while((*(*this)) != (*r))
				(*this)++;

			l_node<t>* temp = p_front;
			while((*r)!=(temp->data))
			{
				temp = temp->next;
			}
			//if(temp == NULL)throw "Item not in List!";
			//if(temp->prev)
				temp->prev->next = temp->next;
			//if(temp->next)
				temp->next->prev = temp->prev;
			delete temp;
			--mySize;
		}

		bool is_empty()	{
			return empty;	}
		unsigned long size()	{
			return mySize;	}
		bool operator ++(int)	{
			if(p_temp->next)
				p_temp = p_temp->next;
			else
				return false;
			return true;	}
		bool operator --(int)	{
			if(p_temp)
				p_temp = p_temp->prev;
			else
				return false;
			return true;	}
		t operator *()	{
			if(p_temp)
				return (p_temp->data);
			throw "List is Empty!";	}
		void operator +(const t& rhs)	{
			push_back(rhs);	}
		void operator -()	{
			pop_back();	}
		
		void iterator_reset(const int l)	{
			switch(l)	{
			case 0:	p_temp = p_front; break;
			case 1: p_temp = p_end; break;}  }
	};
	/*template<class t> void operator >>(istream& is, list<t>& l)	{
		t temp;
		is >> temp;
		l.push_back(&temp);	}*/
};

#endif