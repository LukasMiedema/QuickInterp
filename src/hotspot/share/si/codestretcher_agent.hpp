/*
 * codestretcher_agent.hpp
 *
 *  Created on: Oct 24, 2019
 *      Author: lukas
 */

#ifndef SRC_HOTSPOT_SI_CODESTRETCHER_AGENT_HPP_
#define SRC_HOTSPOT_SI_CODESTRETCHER_AGENT_HPP_

#include "jvm.h"

extern "C" JNIEXPORT jint JNICALL Agent_OnLoad_codestretcher(JavaVM *jvm, char *options, void *reserved);


#endif /* SRC_HOTSPOT_SI_CODESTRETCHER_AGENT_HPP_ */
