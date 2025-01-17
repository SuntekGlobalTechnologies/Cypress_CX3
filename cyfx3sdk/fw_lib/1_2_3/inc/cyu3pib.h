/*
 ## Cypress USB 3.0 Platform Header file (cyu3pib.h)
 ## =====================================
 ##
 ##  Copyright Cypress Semiconductor Corporation, 2010-2011,
 ##  All Rights Reserved
 ##  UNPUBLISHED, LICENSED SOFTWARE.
 ##
 ##  CONFIDENTIAL AND PROPRIETARY INFORMATION
 ##  WHICH IS THE PROPERTY OF CYPRESS.
 ##
 ##  Use of this file is governed
 ##  by the license agreement included in the file
 ##
 ##     <install>/license/license.txt
 ##
 ##  where <install> is the Cypress software
 ##  installation root directory path.
 ##
 ## =====================================
*/

#ifndef _INCLUDED_CYU3_PIB_H_
#define _INCLUDED_CYU3_PIB_H_

#include "cyu3os.h"
#include "cyu3types.h"
#include "cyu3system.h"
#include "cyu3externcstart.h"

/** \file cyu3pib.h
    \brief The Processor Interface Block (PIB) on the FX3 device contains the
    GPIF-II controller and the associated DMA sockets and configuration registers.
    The PIB driver in FX3 firmware is responsible for managing PIB bound data
    transfers and interrupts. This file contains the data types and API definitions
    for the general P-port functionality that is independent of the electrical
    protocol implemented by GPIF-II.
 */

/** \brief Clock configuration information for the PIB block.

    **Description**\n
    The clock for the PIB block is derived from the SYS_CLK. The base clock
    and frequency divider values are selected through this structure.

    The default values used by the driver are:\n
    clkDiv = 2, isHalfDiv = CyFalse, isDllEnable = CyFalse, and clkSrc = CY_U3P_SYS_CLK.

    **\see
    *\see CyU3PSysClockSrc_t
    *\see CyU3PPibInit
 */
typedef struct CyU3PPibClock_t
{
    uint16_t clkDiv;            /**< Divider value for the PIB clock. The min value is 2 and max value is 1024. */
    CyBool_t isHalfDiv;         /**< If set to true, adds 0.5 to the frequency divider value selected by clkDiv. */
    CyBool_t isDllEnable;       /**< This enables or disable the PIB DLL control. */
    CyU3PSysClockSrc_t clkSrc;  /**< The clock source to be used. */
} CyU3PPibClock_t;

/** \brief Enumeration of P-port error types.

    **Description**\n
    The P-port interface block (PIB) of the FX3 device can encounter various errors during
    data transfers performed across the GPIF interface. This enumerated type lists the PIB level
    error conditions that are notified to the user application through the CYU3P_PIB_INTR_ERROR
    interrupt callback. The cbArg parameter passed to the callback will indicate the PIB error code
    as well as the GPIF error code.
 
    The CYU3P_GET_PIB_ERROR_TYPE macro can be used to decode the PIB error type from the callback
    argument.

    **\see
    *\see CyU3PPibIntrType
    *\see CyU3PPibRegisterCallback
    *\see CyU3PGpifErrorType
    *\see CYU3P_GET_PIB_ERROR_TYPE
 */
typedef enum CyU3PPibErrorType
{
    CYU3P_PIB_ERR_NONE = 0,                     /**< No errors detected. */
    CYU3P_PIB_ERR_THR0_DIRECTION,               /**< Bad transfer direction (read on a write socket or vice versa)
                                                     error on one of the Thread 0 sockets. */
    CYU3P_PIB_ERR_THR1_DIRECTION,               /**< Bas transfer direction (read on a write socket or vice versa)
                                                     error on one of the Thread 1 sockets. */
    CYU3P_PIB_ERR_THR2_DIRECTION,               /**< Bad transfer direction (read on a write socket or vice versa)
                                                     error on one of the Thread 2 sockets. */
    CYU3P_PIB_ERR_THR3_DIRECTION,               /**< Bas transfer direction (read on a write socket or vice versa)
                                                     error on one of the Thread 3 sockets. */
    CYU3P_PIB_ERR_THR0_WR_OVERRUN,              /**< Write overrun (write beyond available buffer size) error on one of
                                                     the Thread 0 sockets. */
    CYU3P_PIB_ERR_THR1_WR_OVERRUN,              /**< Write overrun (write beyond available buffer size) error on one of
                                                     the Thread 1 sockets. */
    CYU3P_PIB_ERR_THR2_WR_OVERRUN,              /**< Write overrun (write beyond available buffer size) error on one of
                                                     the Thread 2 sockets. */
    CYU3P_PIB_ERR_THR3_WR_OVERRUN,              /**< Write overrun (write beyond available buffer size) error on one of
                                                     the Thread 3 sockets. */
    CYU3P_PIB_ERR_THR0_RD_UNDERRUN,             /**< Read underrun (read beyond available data size) error on one of
                                                     the Thread 0 sockets. */
    CYU3P_PIB_ERR_THR1_RD_UNDERRUN,             /**< Read underrun (read beyond available data size) error on one of
                                                     the Thread 1 sockets. */
    CYU3P_PIB_ERR_THR2_RD_UNDERRUN,             /**< Read underrun (read beyond available data size) error on one of
                                                     the Thread 2 sockets. */
    CYU3P_PIB_ERR_THR3_RD_UNDERRUN,             /**< Read underrun (read beyond available data size) error on one of
                                                     the Thread 3 sockets. */
    CYU3P_PIB_ERR_THR0_SCK_INACTIVE = 0x12,     /**< One of the Thread 0 sockets became inactive during transfer. */
    CYU3P_PIB_ERR_THR0_ADAP_OVERRUN,            /**< DMA controller overrun on a write to one of the Thread 0 sockets.
                                                     This typically happens if the DMA controller cannot keep up with
                                                     the incoming data rate. */
    CYU3P_PIB_ERR_THR0_ADAP_UNDERRUN,           /**< DMA controller underrun on a read from one of the Thread 0 sockets.
                                                     This is also a result of the DMA controller not being able to
                                                     keep up with the desired interface data rate. */
    CYU3P_PIB_ERR_THR0_RD_FORCE_END,            /**< A DMA read operation from Thread 0 socket was forcibly ended by
                                                     wrapping up the socket. */
    CYU3P_PIB_ERR_THR0_RD_BURST,                /**< A socket switch was forced in the middle of a read burst from a
                                                     Thread 0 socket. */
    CYU3P_PIB_ERR_THR1_SCK_INACTIVE = 0x1A,     /**< One of the Thread 1 sockets became inactive during transfer. */
    CYU3P_PIB_ERR_THR1_ADAP_OVERRUN,            /**< DMA controller overrun on a write to one of the Thread 1 sockets.
                                                     This typically happens if the DMA controller cannot keep up with
                                                     the incoming data rate. */
    CYU3P_PIB_ERR_THR1_ADAP_UNDERRUN,           /**< DMA controller underrun on a read from one of the Thread 1 sockets.
                                                     This is also a result of the DMA controller not being able to
                                                     keep up with the desired interface data rate. */
    CYU3P_PIB_ERR_THR1_RD_FORCE_END,            /**< A DMA read operation from Thread 1 socket was forcibly ended by
                                                     wrapping up the socket. */
    CYU3P_PIB_ERR_THR1_RD_BURST,                /**< A socket switch was forced in the middle of a read burst from a
                                                     Thread 1 socket. */
    CYU3P_PIB_ERR_THR2_SCK_INACTIVE = 0x22,     /**< One of the Thread 2 sockets became inactive during transfer. */
    CYU3P_PIB_ERR_THR2_ADAP_OVERRUN,            /**< DMA controller overrun on a write to one of the Thread 2 sockets.
                                                     This typically happens if the DMA controller cannot keep up with
                                                     the incoming data rate. */
    CYU3P_PIB_ERR_THR2_ADAP_UNDERRUN,           /**< DMA controller underrun on a read from one of the Thread 2 sockets.
                                                     This is also a result of the DMA controller not being able to
                                                     keep up with the desired interface data rate. */
    CYU3P_PIB_ERR_THR2_RD_FORCE_END,            /**< A DMA read operation from Thread 2 socket was forcibly ended by
                                                     wrapping up the socket. */
    CYU3P_PIB_ERR_THR2_RD_BURST,                /**< A socket switch was forced in the middle of a read burst from a
                                                     Thread 2 socket. */
    CYU3P_PIB_ERR_THR3_SCK_INACTIVE = 0x2A,     /**< One of the Thread 3 sockets became inactive during transfer. */
    CYU3P_PIB_ERR_THR3_ADAP_OVERRUN,            /**< DMA controller overrun on a write to one of the Thread 3 sockets.
                                                     This typically happens if the DMA controller cannot keep up with
                                                     the incoming data rate. */
    CYU3P_PIB_ERR_THR3_ADAP_UNDERRUN,           /**< DMA controller underrun on a read from one of the Thread 3 sockets.
                                                     This is also a result of the DMA controller not being able to
                                                     keep up with the desired interface data rate. */
    CYU3P_PIB_ERR_THR3_RD_FORCE_END,            /**< A DMA read operation from Thread 3 socket was forcibly ended by
                                                     wrapping up the socket. */
    CYU3P_PIB_ERR_THR3_RD_BURST                 /**< A socket switch was forced in the middle of a read burst from a
                                                     Thread 3 socket. */
} CyU3PPibErrorType;

/** \brief Enumeration of GPIF error types.

    **Description**\n
    In addition to the PIB level errors, there can be cases where GPIF state machine specific errors
    occur during GPIF operation.  The cbArg parameter passed to the callback in the case of a
    CYU3P_PIB_INTR_ERROR event is composed of a PIB error code as well as a GPIF error code.
    This enumerated type lists the various GPIF specific error types that are defined for the FX3
    device.
 
    The CYU3P_GET_GPIF_ERROR_TYPE macro can be used to get the GPIF error code from the cbArg parameter.

    **\see
    *\see CyU3PPibErrorType
    *\see CYU3P_GET_GPIF_ERROR_TYPE
 */
typedef enum CyU3PGpifErrorType
{
    CYU3P_GPIF_ERR_NONE             = 0,        /**< No GPIF state machine errors. */
    CYU3P_GPIF_ERR_INADDR_OVERWRITE = 0x0400,   /**< Content of INGRESS_ADDR register is overwritten before read. */
    CYU3P_GPIF_ERR_EGADDR_INVALID   = 0x0800,   /**< Attempt to read EGRESS_ADDR register before it is written to. */
    CYU3P_GPIF_ERR_DATA_READ_ERR    = 0x0C00,   /**< Read from DMA data thread which is not ready. */
    CYU3P_GPIF_ERR_DATA_WRITE_ERR   = 0x1000,   /**< Write to DMA data thread which is not ready. */
    CYU3P_GPIF_ERR_ADDR_READ_ERR    = 0x1400,   /**< Read from DMA address thread which is not ready. */
    CYU3P_GPIF_ERR_ADDR_WRITE_ERR   = 0x1800,   /**< Write to DMA address thread which is not ready. */
    CYU3P_GPIF_ERR_INVALID_STATE    = 0x2000    /**< GPIF state machine has reached an invalid state. */
} CyU3PGpifErrorType;

/** \def CYU3P_GET_PIB_ERROR_TYPE
    \brief Get the PIB error code from the CYU3P_PIB_INTR_ERROR callback argument.

    **Description**\n
    This macro is used to get the PIB error code part from the cbArg parameter passed to the
    PIB callback as part of a CYU3P_PIB_INTR_ERROR event.

    **\see
    *\see CyU3PPibErrorType
 */
#define CYU3P_GET_PIB_ERROR_TYPE(param) ((CyU3PPibErrorType)((param) & 0x3F))

/** \def CYU3P_GET_GPIF_ERROR_TYPE
    \brief Get the GPIF error code from the CYU3P_PIB_INTR_ERROR callback argument.

    **Description**\n
    This macro is used to get the GPIF error code part from the cbArg parameter passed to the
    PIB callback as part of a CYU3P_PIB_INTR_ERROR event.

    **\see
    *\see CyU3PGpifErrorType
 */
#define CYU3P_GET_GPIF_ERROR_TYPE(param) ((CyU3PGpifErrorType)((param) & 0x7C00))

/** \brief Enumeration of P-port interrupt event types.

    **Description**\n
    The P-port interface block (PIB) of the FX3 device has some interrupt sources that are
    unconnected with the GPIF hardware or state machine. These interrupts indicate events
    such as error conditions, mailbox message reception and Interface Config register updates.
    This enumerated type lists the various interrupt events that are valid for the PIB
    interrupt.

    **\see
    *\see CyU3PPibIntrCb_t
    *\see CyU3PPibRegisterCallback
 */
typedef enum CyU3PPibIntrType
{
    CYU3P_PIB_INTR_DLL_UPDATE = 1,      /**< Indicates that a PIB DLL lock or unlock event has occured. The cbArg
                                             parameter indicates whether the DLL is currently locked (non-zero) or
                                             unlocked (zero). */
    CYU3P_PIB_INTR_PPCONFIG = 2,        /**< Indicates that the PIB config register has been written to through
                                             the GPIF interface. The value written into the PP_CONFIG register is
                                             passed as the cbArg parameter. This value typically has the following
                                             format:\n
                                             Bit  15     : User defined.\n
                                             Bit  14     : DRQ polarity. 0 = active low, 1 = active high.\n
                                             Bit  13     : User defined.\n
                                             Bit  12     : DRQ override. 0 = Normal. 1 = Force DRQ on.\n
                                             Bit  11     : INT polarity. 0 = active low. 1 = active high.\n
                                             Bit  10     : INT value. Specifies INT state when override is on.\n
                                             Bit   9     : INT override. 0 = Normal. 1 = Overridden.\n
                                             Bits  8 - 7 : User defined.\n
                                             Bit   7     : User defined.\n
                                             Bit   6     : CFGMODE. Must be 1.\n
                                             Bits  5 - 4 : User defined.\n
                                             Bits  3 - 0 : User defined. Usually set to log2(burst size).\n
                                         */
    CYU3P_PIB_INTR_ERROR = 4            /**< Indicates that an error condition has been encountered by the PIB/GPIF
                                             block. The type of error encountered is passed through the cbArg parameter.
                                             See CyU3PPibErrorType for the possible error types. */
} CyU3PPibIntrType;

/** \brief Type of callback function that will be called on receiving a generic
    P-port interrupt.

    **Description**\n
    The P-port interface block (PIB) of the FX3 device has some interrupt sources that are
    unconnected with the GPIF hardware or state machine. This function is used to register
    a callback function that will be invoked when one of these interrupts is triggered.

    **\see
    *\see CyU3PPibRegisterCallback
 */
typedef void (*CyU3PPibIntrCb_t) (
        CyU3PPibIntrType cbType,        /**< Type of interrupt that caused this callback. */
        uint16_t         cbArg          /**< 16-bit integer argument associated with the interrupt. */
        );

/** \cond PIB_INTERNAL
 */

/**************************************************************************
 **************************** Global Variables ****************************
 **************************************************************************/

extern CyU3PThread glPibThread;                         /* PIB thread */
extern CyU3PEvent  glPibEvt;                            /* Event flag for the Pib Module */

/** \endcond
 */

/**************************************************************************
 *************************** Function prototypes **************************
 **************************************************************************/

/** \brief Initialize the P-port interface block.

    **Description**\n
    This function powers up the P-port interface block. This needs to be the first P-port
    related function call and should be called before any GPIF related calls are made.

    **Return value**\n
    * CY_U3P_SUCCESS              - If the operation is successful.\n
    * CY_U3P_ERROR_BAD_ARGUMENT   - If an incorrect/invalid clock configuration is passed.\n
    * CY_U3P_ERROR_NULL_POINTER   - If the argument pibClock is a NULL.

    **\see
    *\see CyU3PPibDeInit
    *\see CyU3PPibClock_t
 */
extern CyU3PReturnStatus_t
CyU3PPibInit (
        CyBool_t doInit,                /**< Whether to initialize the PIB block. Should generally be set to CyTrue. */
        CyU3PPibClock_t *pibClock       /**< PIB clock configuration. */
        );

/** \brief De-initialize the P-port interface block.

    **Description**\n
    This function disables and powers off the P-port interface block.

    **Return value**\n
    * CY_U3P_SUCCESS           - If the operation is successful.\n
    * CY_U3P_ERROR_NOT_STARTED - If the block has not been initialized.

    **\see
    *\see CyU3PPibInit
 */
extern CyU3PReturnStatus_t
CyU3PPibDeInit (
        void);

/** \brief Set the IO drive strength for the Pport.

    **Description**\n
    The function sets the IO Drive strength for the P-port signals. The default IO
    drive strength is set to CY_U3P_DS_THREE_QUARTER_STRENGTH.

    **Return value**\n
    * CY_U3P_SUCCESS            - If the operation is successful.\n
    * CY_U3P_ERROR_BAD_ARGUMENT - If the Drive strength requested is invalid.

    **\see
    *\see CyU3PDriveStrengthState_t
 */
extern CyU3PReturnStatus_t
CyU3PSetPportDriveStrength (
            CyU3PDriveStrengthState_t pportDriveStrength        /**< Desired drive strength */
            );

/** \brief Register a callback function for notification of PIB interrupts.

    **Description**\n
    This function registers a callback function that will be called for notification of PIB interrupts
    and also selects the PIB interrupt sources of interest.

    **Return value**\n
    * None

    **\see
    *\see CyU3PPibIntrCb_t
    *\see CyU3PPibIntrType
 */
extern void
CyU3PPibRegisterCallback (
        CyU3PPibIntrCb_t cbFunc,        /**< Callback function pointer being registered. */
        uint32_t         intMask        /**< Bitmask representing the various interrupts callbacks to be enabled.
                                             This value is derived by ORing the various callback types of
                                             interest from the CyU3PPibIntrType enumeration. */
        );

/** \brief Set the priority level for PIB/GPIF interrupts in the FX3 system.

    **Description**\n
    By default, PIB and GPIF interrupts in the FX3 system have low priority and can be pre-empted by
    other interrupts such as the USB interrupt. This API is provided to switch to a higher priority
    non pre-emptable version of the interrupt handler; or back to the default setting.

    **Note**\n
    It is not advisable to set the PIB interrupt as high priority if the CYU3P_PIB_INTR_ERROR callback
    type is enabled.

    **Return value**\n
    * CY_U3P_SUCCESS if the interrupt priority was updated as specified.\n
    * CY_U3P_ERROR_INVALID_CALLER if this function is called from interrupt context.\n
 */
extern CyU3PReturnStatus_t
CyU3PPibSetInterruptPriority (
        CyBool_t isHigh                 /**< Whether the PIB (GPIF) interrupt is to be assigned high priority. */
        );

/** \brief Select the sources that trigger an FX3 interrupt to external processor.

    **Description**\n
    The FX3 device is capable of generating interrupts to the external processor connected on
    the GPIF port for various reasons. This function is used to select the interrupt sources
    allowed to interrupt the external processor.

    **Return value**\n
    * None
 */
extern void
CyU3PPibSelectIntSources (
        CyBool_t pibSockEn,             /**< Whether PIB socket DMA ready status should trigger an interrupt. */
        CyBool_t gpifIntEn,             /**< Whether GPIF state machine can trigger an interrupt. */
        CyBool_t pibErrEn,              /**< Whether an error condition in the PIB/GPIF should trigger an
                                             interrupt. The type of error can be detected by reading the
                                             PP_ERROR register. */
        CyBool_t mboxIntEn,             /**< Whether a mailbox message ready for reading should trigger an
                                             interrupt. The PP_RD_MAILBOX registers should be read to fetch
                                             the message indicated by this interrupt. */
        CyBool_t wakeupEn               /**< Whether a FX3 wakeup from sleep mode should trigger an interrupt.
                                             This interrupt source will not be triggered by the FX3 device
                                             using the current firmware library. */
        );

#include "cyu3externcend.h"

#endif /* _INCLUDED_CYU3_PIB_H_ */

/*[]*/
