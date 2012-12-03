#pragma once

/*
 * Set to on if you want USART2 (on EXT1 connector)
 * to be used for Trace facility.
 */
#define USE_USART2_TRACE 				1

/*
 * Set to one if NrfTransceiver should trace verbose.
 */
#define NRFTRANSCEIVER_TRACE_VERBOSE	1


/*
 * Uncomment to enable the led blink XF test.
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
//#define LEDBLINK_TESTER

/*
 * Uncomment to enable the paint test on the display.
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
//#define PAINT_TESTER

/*
 * Uncomment to enable the FSM test (Finite state machine).
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
//#define FSM_TESTER

/*
 * Uncomment to enable the local Tic Tac Toe test.
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
#define LOCALTTT_TESTER

/*
 * Uncomment to enable the PHY test.
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
//#define PHY_TESTER

/*
 * Uncomment to enable the Air3T application.
 *
 * Note that only one of the test/application compiler switches here can be active at the same moment.
 */
//#define AIR3T_APPLICATION


/*
 * Set AIR3T_APPLICATION_NODENAME and AIR3T_APPLICATION_NODEID (ID must be exactly 12 alpha-numerical characters)
 * In order to specify the name of the node and the wireless unique MAC address.
 */
#define AIR3T_APPLICATION_NODENAME			"Node A"
#define AIR3T_APPLICATION_NODEID			"000000000001"
