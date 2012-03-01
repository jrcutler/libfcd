/*! \file
 * \brief FUNcube dongle utility
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif

#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, NULL */
#include "fcd.h" /* FCD, fcd_open, fcd_close */


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
	FCD *fcd;

	/* explicitly ignore arguments */
	(void) argc;
	(void) argv;

	/* get a handle to any device */
	fcd = fcd_open(NULL);

	/*! \todo use open \p fcd handle */
	if (NULL == fcd)
	{
		result = EXIT_FAILURE;
	}

	/* close handle */
	fcd_close(fcd);

	return result;
}
