/**
 * @file
 * @brief     States used to describe the current condition of the uArm Swift Pro claw/gripper.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */
#ifndef CLAW_STATE_HPP
#define CLAW_STATE_HPP

enum class ClawState { STOPPED, MOVING, GRIPPED_OBJECT, UNKNOWN };

#endif
