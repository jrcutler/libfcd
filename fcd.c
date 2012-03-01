/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif
#include <stdlib.h> /* NULL, malloc, free */
#include "fcd.h"


/*! \brief Implementation of \ref FCD */
struct FCD_impl
{
	/*! \brief Placeholder (to give \ref FCD non-zero size) */
	void *placeholder;
};


FCD * fcd_open(void)
{
	FCD *handle;

	handle = malloc(sizeof(FCD));
	if (NULL != handle)
	{
		/** \todo implement open */
		handle->placeholder = NULL;
	}

	return handle;
}


void fcd_close(FCD *handle)
{
	if (NULL != handle)
	{
		/** \todo implement close */
		free(handle);
	}
}
