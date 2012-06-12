
#include "../include/critical.h"
#include "../include/kasm.h"

static int _critical_level = 0;

void
critical_enter()
{
	_cli();
	_critical_level++;
	
}

void
critical_leave()
{
	if (_critical_level > 0)
	{
		_critical_level--;
		if (_critical_level == 0)
			_sti();
	}
}
