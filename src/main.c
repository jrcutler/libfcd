/*! \file
 * \brief FUNcube dongle utility
 * \author Justin R. Cutler
 */
/*
 * Copyright (C) 2012 Justin R. Cutler
 *
 * libfcd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libfcd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfcd.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h> /* printf, fprintf, stderr */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, NULL */
#include "fcd.h" /* FCD, fcd_for_each, fcd_open, fcd_close, fcd_query */


/*! \copydetails fcd_path_callback
 * \brief Output simple diagnostics for each device
 */
int funcube_debug(const char *path, void *context)
{
	int result = 0;
	FCD *fcd;
	char query[64];

	/* explicitly ignore context */
	(void) context;

	/* try to open device */
	fcd = fcd_open(path);

	if (NULL != fcd)
	{
		if (fcd_query(fcd, query, sizeof(query)))
		{
			unsigned int start, end;
			/* output simple diagnostics for the device */
			printf("[%s]\t%s\n", path, query);
			if (!fcd_bl_get_address_range(fcd, &start, &end))
			{
				unsigned char block[48];
				printf("\tRange: 0x%08x - 0x%08x\n", start, end);
				fcd_bl_set_address(fcd, start);
				if (!fcd_bl_read_block(fcd, block))
				{
					unsigned int index;
					for (index = 0; index < sizeof(block); ++index)
					{
						char delim = ((index & 0xf) == 0xf) ? '\n' : ' ';
						printf("%02x%c", block[index], delim);
					}
				}
			}
		}
		else
		{
			fprintf(stderr, "Query failed for [%s]\n", path);
			result = -1;
		}
		/* close device */
		fcd_close(fcd);
	}
	else
	{
		fprintf(stderr, "Could not open [%s]\n", path);
		result = -1;
	}

	return result;
}


/*!
 * \brief FUNcube dongle utility entry point
 * \param argc number of command line arguments
 * \param argv command line arguments
 * \retval EXIT_SUCCESS success
 * \retval EXIT_FAILURE failure
 */
int main(int argc, char **argv)
{
	int result = EXIT_SUCCESS;

	/* explicitly ignore arguments */
	(void) argc;
	(void) argv;

	if (fcd_for_each(funcube_debug, NULL))
	{
		result = EXIT_FAILURE;
	}

	return result;
}
