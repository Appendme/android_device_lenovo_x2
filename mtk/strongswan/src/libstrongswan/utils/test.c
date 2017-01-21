/*
 * Copyright (C) 2013 Tobias Brunner
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "test.h"

#include <library.h>

/**
 * A collection of testable functions
 */
hashtable_t *testable_functions;

/*
 * Described in header.
 */
void testable_function_register(char *name, void *fn)
{
	if (testable_functions)
	{
		bool old = FALSE;
		if (lib->leak_detective)
		{
			old = lib->leak_detective->set_state(lib->leak_detective, FALSE);
		}
		if (fn)
		{
			testable_functions->put(testable_functions, name, fn);
		}
		else
		{
			testable_functions->remove(testable_functions, name);
		}
		if (lib->leak_detective)
		{
			lib->leak_detective->set_state(lib->leak_detective, old);
		}
	}
}
