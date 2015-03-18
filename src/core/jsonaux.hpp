/*
 * jsonaux.hpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
 */

#ifndef SRC_CORE_JSONAUX_HPP_
#define SRC_CORE_JSONAUX_HPP_

#include <string>
#include <fstream>
#include "../external/json.h"

using namespace std;

json_value* parseJsonFile(string fileName);


#endif /* SRC_CORE_JSONAUX_HPP_ */
