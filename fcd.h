/*! \file
 * \brief FUNcube dongle interface definition
 */
#ifndef FCD_H
# define FCD_H

# ifdef __cplusplus
extern "C"
{
# endif


/*
 * Defines
 */

/*! FUNcube dongle USB vendor ID */
#define FCD_USB_VID 0x04d8
/*! FUNcube dongle USB product ID */
#define FCD_USB_PID 0xfb56


/*
 * Types
 */

/* Forward declaration of opaque FUNcube dongle structure */
struct FCD_impl;
/*! \brief Opaque FUNcube dongle handle */
typedef struct FCD_impl FCD;

/*!
 * \brief FUNcube dongle path handler function
 * \param[in]     path    path to a FUNcube dongle
 * \param[in,out] context user context pointer
 * \retval 0     success
 * \retval non-0 failure
 */
typedef int (fcd_path_fn)(const char *path, void *context);


/*
 * Functions
 */

/*!
 * \brief Call a user-provided path handler for all FUNcube dongles present
 * \param         fn      handler function
 * \param[in,out] context context pointer
 * \retval 0     success
 * \retval non-0 failure
 * \note Terminates on the first call to \p fn that fails.
 */
int fcd_for_each(fcd_path_fn *fn, void *context);

/*!
 * \brief Open a FUNcube dongle device
 * \param[in] path USB path uniquely identifying device (or \c NULL for any)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 */
FCD * fcd_open(const char *path);

/*!
 * \brief Close a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 * \post \p dev is no longer valid
 */
void fcd_close(FCD *dev);

/*!
 * \brief Query a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \param[out]    str output buffer
 * \param         len length of output buffer
 * \retval NULL     error (\p str has not changed)
 * \retval non-NULL success (\p str)
 */
char * fcd_query(FCD *dev, char *str, int len);


/*!
 * \brief Reset to bootloader
 */
void fcd_reset_bootloader(void);
/*!
 * \brief Reset to application
 */
void fcd_reset_application(void);


# ifdef __cplusplus
}
# endif

#endif /* FCD_H */
