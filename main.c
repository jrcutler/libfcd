/*! \file
 * \brief FUNcube dongle utility
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif

#include <stdio.h> /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, NULL */
#include "fcd.h" /* FCD, fcd_for_each, fcd_open, fcd_close */


/*! \copydetails fcd_path_fn
 * \brief Output simple diagnostics for each device
 */
int funcube_debug(const char *path, void *context)
{
	int result = 0;
	FCD *fcd;

	/* explicitly ignore context */
	(void) context;

	/* try to open device */
	fcd = fcd_open(path);

	if (NULL != fcd)
	{
		/*! \todo use open \p fcd device */
	}
	else
	{
		result = -1;
	}

	/* close device */
	fcd_close(fcd);

	/* output simple diagnostics for the device */
	printf("[%s] %s\n", path, (result) ? "ERROR" : "OK");

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
