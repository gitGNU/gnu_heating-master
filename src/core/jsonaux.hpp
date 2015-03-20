/*
 * jsonaux.hpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#ifndef SRC_CORE_JSONAUX_HPP_
#define SRC_CORE_JSONAUX_HPP_

#include <string>
#include <fstream>
#include "../external/json-parser/json.h"

/* Wrapper to pass a filename instead of a JSON string to the JSON parser */
json_value* parseJsonFile(std::string fileName);


#endif /* SRC_CORE_JSONAUX_HPP_ */
