/*
 * command_line.h
 *
 *  Created on: Oct 26, 2025
 *      Author: savo
 */

#ifndef INC_COMMAND_LINE_H_
#define INC_COMMAND_LINE_H_

enum CLICommandType {
	CLI_UNKNOWN 	= 0,
	CLI_HELP		= 'H',
	CLI_FLASH 		= 'F',
	CLI_STR_ZERO 	= 'A',
	CLI_STR_SCALE 	= 'B',
	CLI_STR_GAIN 	= 'C',
	CLI_TOR_ZERO 	= 'X',
	CLI_TOR_SCALE 	= 'Y',
	CLI_TOR_GAIN 	= 'Z',
	CLI_READINGS 	= 'R',
};

#endif /* INC_COMMAND_LINE_H_ */
