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


/*
 * Functions
 */

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


# ifdef __cplusplus
}
# endif

#endif /* FCD_H */
