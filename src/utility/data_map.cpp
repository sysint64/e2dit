/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UTILITY.

 * E2DIT-UTILITY is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT-UTILITY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UTILITY.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#include "utility/data_map.h"

/**
 * Empty Constructor
 */

DataMap::DataMap () {

}

/**
 * Load Data From File
 *
 * @param fileName
 * @param rt type file for read (bin or text)
 */

DataMap::DataMap (const char *fileName, ReadType rt) {

	switch (rt) {

		case ReadType::Bin  : loadFromBin  (fileName); break;
		case ReadType::Text : loadFromText (fileName); break;
		default             : return;

	}

}

/**
 * Load Data from text File
 * @param fileName
 */

void DataMap::loadFromText (const char *fileName) {

	lineno = 1; posno = 1;
	offsetStack.push_back(0);
	this->fileName = fileName;

	/* Check file exist */

	if (!fs::exists (fileName)) {

		/* if not exist, then write error to log */

		Application::getInstance()->log.ewrite ("Load data file error, file doesn't exist : %s", fileName);

		/* And Exit */

		return;

	}

	inFile = fopen (fileName, "r");
	parse();
	fclose (inFile);

}

/**
 * Load Data from bin File
 * @param fileName
 */

void DataMap::loadFromBin (const char *fileName) {
	
	this->fileName = fileName;

}

/**
 * Save Data to Text File
 * @param fileName
 */

void DataMap::saveToText (const char *fileName) {

	FILE *outFile = fopen (fileName, "w+");

	/* Get All Elements */

	for (auto it:element) {

		fputs (it.first.c_str(), outFile); fputs ("\n", outFile);
		DataParams tf = element[it.first];

		/* Get All Functions */

		for (auto itf:tf.params) {

			fputs ("	", outFile); fputs (itf.first.c_str(), outFile);
			fputs (" : ", outFile);	

			/* Get All Values */

			for (auto itv:tf.params[itf.first]) {

				char splitc = ',';
				//char splitc = i < tf.func[itf->first].size()-1 ? ',' : ';';

				if (itv.isStr) {

					fprintf (outFile, "\"%s\"%c ", itv.str.c_str(), splitc);

				} else {

					fprintf (outFile, "%f%c ", itv.num, splitc);

				}

			}

			fputs("\n", outFile);

		}

	}

	fclose (outFile);	

}

/**
 * Save Data to Bin File
 * @param fileName
 */

void DataMap::saveToBin  (const char *fileName) {

	FILE *outFile = fopen (fileName, "w+");

	for (auto byte:bytecode)
		fputc (byte, outFile);

	fclose (outFile);

}

/**
 *
 */

void DataMap::update() {

}
