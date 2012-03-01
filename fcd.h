/*! \file
 * \brief FUNcube dongle interface definition
 */
#ifndef FCD_H
# define FCD_H

# ifdef __cplusplus
extern "C"
{
# endif

/* Forward declaration of opaque FUNcube dongle structure */
struct FCD_impl;
/*! \brief Opaque FUNcube dongle handle */
typedef struct FCD_impl FCD;

/*!
 * \brief Open a FUNcube dongle handle
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 */
FCD * fcd_open(void);

/*!
 * \brief Close a FUNcube dongle handle
 * \param[in,out] handle open \ref FCD (or NULL)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 * \post \p handle is no longer valid
 */
void fcd_close(FCD *handle);


# ifdef __cplusplus
}
# endif

#endif /* FCD_H */
