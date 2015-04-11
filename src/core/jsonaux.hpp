/*
 * jsonaux.hpp
 *
 *  Copyright (C) 2015, David Rabel
 *  All rights reserved.
 *
 *  This file is part of Heating Master.
 *
 *    Heating Master is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Heating Master is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Heating Master. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_CORE_JSONAUX_HPP_
#define SRC_CORE_JSONAUX_HPP_

#include <string>
#include <fstream>
#include "../external/json-parser/json.h"

/* Wrapper to pass a filename instead of a JSON string to the JSON parser */
json_value* parseJsonFile(std::string fileName);


#endif /* SRC_CORE_JSONAUX_HPP_ */
