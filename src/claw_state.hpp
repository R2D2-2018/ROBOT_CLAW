/**
 * @file
 * @brief     States used to describe the current condition of the uArm Swift Pro claw/gripper.
 * @author    Wiebe van Breukelen, Joost van Lingen
 * @license   See LICENSE
 */

#ifndef CLAW_STATE_HPP
#define CLAW_STATE_HPP

/**
 * @brief Enum class for claw states
 *
 * The various claw states that can be determined in an enum class.
 */
enum class ClawState { STOPPED, MOVING, GRIPPED_OBJECT, CLOSED, UNKNOWN };

#endif
