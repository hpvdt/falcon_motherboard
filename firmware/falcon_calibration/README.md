# Falcon Calibration Firmware

This project serves as a utility to prepare Falcon's sensing systems in advance of actual flights, using the same motherboard as the calibration hardware to minimize discrepancies between calibrations and operation. In addition to calibrations, it also is meant to help serve by providing quick and human readable data streams to diagnose various subsystems.

The program works like a command line utility via a USB connection from the motherboard to a host computer, where it presents itself as a USB-CDC (Serial) device. Opening a serial terminal to the motherboard allows one to begin the process.

For assistance at any point in using the calibration firmware, send an `H` to the motherboard over serial and it will respond with the help menu.

>[!IMPORTANT]
> **All determined calibration values *MUST* be recorded _outside_ of the calibration program!** Either by hand or copied to some document, so they can then be hardcoded into the actual Falcon operation firmware. They can be recorded as expressions instead of final results, e.g. `100.0/3654897` as a load scaling factor instead of recording `0.0000274` as a scaling factor you can record  so we know that at 100% load the ADC read 3654897. 
>
> Not using persistent storage on the microcontroller and hardcoding them into firmware instead was an intentional design choice to ensure that we were using known calibration values and would not potentially lose them.

>[!NOTE]
> Messages sent to the motherboard do not need delimiting characters such as `\n` or `\r`. Simply leave a bit of time (a second is enough) between sequential commands to differentiate them. **Commands are case sensitive.**

# Wing Load System

The driving purpose for this firmware initially was to get readings from the wings, and hopefully even test out calibrations of them before commiting them to the firmware. When operating the wing calibration each instruction needs to address a specific node via its ID. As currently installed _(as of 2025-11-11)_ their locations and respective IDs are:

| Location | ID |
|---|---|
| Port (left) outer-most | `1` |
| Port midway | `6` |
| Port inner-most | `4` |
| Center | `7` |
| Starboard inner-most | `8` |
| Starboard midway | `5` |
| Starboard (right) outer-most | `2` |

_Yes we could have installed them in an easier to remember way, but it's too late now._

All of these monitor bending strain at their locations, **only the innermost starboard (`8`) monitors torsional loading of the spar as well**.

## Wing Load Calibration Commands

All wing load calibration commands follow format `CN,P` where:
- `C` is the command character
- `N` is the target node's ID
- `P` is an optional parameter _(if not needed for a command the leading comma may be ommited)_

| Command character | Operation | Parameter Type |
| --- | --- | --- |
| `H` | Print help message _(a basic summary of this `README.md`)_ | No parameter |
| `R` | Toggle load reading printout | No parameter |
| `F` | Flash a node's light P times | Integer |
| `A` | Set node strain zero point as P | Integer |
| `B` | Set node strain scaling factor as P | Real number |
| `C` | Set node strain gain as a power of 2 | Integer, 0 to 7 inclusive |
| `X` | Set node torsion zero point as P | Integer |
| `Y` | Set node torsion scaling factor as P | Real number |
| `Z` | Set node torsion gain as a power of 2 | Integer, 0 to 7 inclusive |

## Wing Load Calibration Procedure

The wing load sensing system calibration is pretty straight forward. Below is the process that should be followed, _assuming the system has been tested to work properly._ It should only require a single loading cycle to complete the two-point calibration.

0. Set gain to 0, zero point to 0, and scale to 1.0. This will provide RAW ADC readings which are needed for the next step. _The calibration firmware should automatically do this for you on boot unless the firmware is changed._
1. Load the wing to approximately 1G
2. Increase gain until we are risk saturating the target ADC
   - Max readings are +/-&nbsp;8&nbsp;388&nbsp;608 so avoid going too close otherwise more loading won't register!
   - I would suggest having our expected loading read about midrange (about 4&nbsp;000&nbsp;000) so we can observe overload by a factor of 2. However this can adjusted based on the actual safety factor of the wing, since there's no point trying to register loads which will destroy the wing anyways.
3. Record the readings under 1G loading at the desired gain
4. Bring the wing to a no load state
5. Record/set the zero point values. Gain should not be changed from earlier steps.
6. Determine and then set the scaling factor based on the difference of the zero and load readings. This is what we need to multiple the zeroed ADC reading to get our desired unit. 
   - _This can be negative_
   - If the ADC for one node reads `-300` counts at `1.0`G load, `100` at zero loading, then the scaling factor for that node should be calculated as `1.0/(-300-100) = 1.0/-400 = -0.0025`
7. Pass all the recorded values to be hardcoded by the electronics team into future firmware reading from the wing load system

If desired the system can be reloaded as desired to see if the calibrations still hold.
