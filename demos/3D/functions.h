#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#endif

int list_overflow(int index, int list_size)
{
	return index%list_size;
}

int list_negative(int index,int list_size)
{
	while(index<0)
	{
		index=list_size+index;
	}
	return index;
}