#pragma once
#include "SensorDataBasicClasses.h"
#include "SensorData.h"

// Specialization of "date class" for BKLab sensor data: using inheritance
class BKLabDate : public basicSensorDate {
protected:
	;
public:
	virtual bool setFromString(const string& dateInputString, bool allowOverwrite = true)
	{// in string example: "2004-03-31" 
		string element; bool result;
		unsigned short int in_year; unsigned char in_month, in_day;
		element = dateInputString.substr(0, 4);
		in_year = stoul(element);
		element = dateInputString.substr(5, 2);
		in_month = (unsigned char)stoul(element);
		element = dateInputString.substr(8, 2);
		in_day = (unsigned char)stoul(element);
		result = setDate(in_year, in_month, in_day, allowOverwrite);
		return result;
	}
};

// Specialization of "time class" for BKLab sensor data: using inheritance
class BKLabTime : public basicSensorTime {
protected:
	;
public:
	virtual bool setFromString(const string& timeInputString, bool allowOverwrite = true)
	{// in string example: "03:38:15.757551" 
		string element; bool result;
		unsigned char in_hour, in_minute; float in_second;
		element = timeInputString.substr(0, 2);
		in_hour = (unsigned char)stoul(element);
		element = timeInputString.substr(3, 2);
		in_minute = (unsigned char)stoul(element);
		element = timeInputString.substr(6, 10);// if too long, will be terminated at the end of the string
		in_second = stof(element);
		result = setTime(in_hour, in_minute, in_second, allowOverwrite);
		return result;
	}
};

// Specialization of templated "date&time class" for BKLab sensor data: using Template specialization
///---------------- ///
// the BKLab Date-time : combination of the BKL-Date and BKL-Time
typedef sensorDateAndTime <BKLabDate, BKLabTime> BKLabDateAndTime;
///---------------- ///
template <>
bool BKLabDateAndTime::setFromString(const string& dateAndTimeInputString, bool allowOverwrite)
{	// in string example: "2004-03-31 03:38:15.757551 " 
	string dateInputString, timeInputString;
	bool result_date, result_time;
	int length = dateAndTimeInputString.length();
	if (length < 20)
		return false;
	dateInputString = dateAndTimeInputString.substr(0, 11);
	timeInputString = dateAndTimeInputString.substr(11, length - 11);
	result_date = date.setFromString(dateInputString, allowOverwrite);
	result_time = time.setFromString(timeInputString, allowOverwrite);
	return (result_date && result_time);
}
///////////////////////////////////////////////////////////////////////////////////////

// // Specialization of templated "date&time Interval class" for BKLab sensor data
typedef senDateTimeInterval <BKLabDateAndTime> BKLabInterval;


class BKLabSensorFullReading : public basicSensorMultipleScalarReading {
protected:
	static const int FULL_READING_SIZE = 4;
	static string shared_readingName_array[FULL_READING_SIZE];
	static string shared_readingUnit_array[FULL_READING_SIZE];
	static bool shared_nameInitialized_array[FULL_READING_SIZE];
	static bool shared_unitInitialized_array[FULL_READING_SIZE];
	// variable 0 is temparature (C); var 1 is Humidity (%); var 2 is light (?); var 3 is voltage (V)
	virtual bool blankInitialization()
	{
		//allocate memory (only once)
		if (!memIsAllocated)
			memAllocation();
		if (memIsAllocated)
		{
			// flags for values are all reset
			for (int i = 0; i < max_variables; i++)
				valueInitialized_array[i] = false;
			// names and units are shared by all instances via the static arrays (initialized only once)
		}
		return memIsAllocated;
	}
	virtual bool memAllocation()
	{
		bool result = false;
		if (!memIsAllocated && max_variables > 0)
		{
			value_array = (float*)calloc(max_variables, sizeof(float));
			if (value_array == NULL)
				return false;
			//
			valueInitialized_array = (bool*)calloc(max_variables, sizeof(bool));
			if (valueInitialized_array == NULL)
				return false;

			// the following are shared by all instances
			nameInitialized_array = shared_nameInitialized_array;
			if (nameInitialized_array == NULL)
				return false;
			//
			unitInitialized_array = shared_unitInitialized_array;
			if (unitInitialized_array == NULL)
				return false;
			//
			readingName_array = shared_readingName_array;
			if (readingName_array == NULL)
				return false;
			//
			readingUnit_array = shared_readingUnit_array;
			if (readingUnit_array == NULL)
				return false;
			// finally set memory flag
			memIsAllocated = true;
			result = true;
		}
		return result;
	}
	virtual void memDeallocation()
	{
		if (memIsAllocated)
		{
			free(value_array);
			free(valueInitialized_array);
			max_variables = 0;
			memIsAllocated = false;
		}
	}
	virtual bool checkConsistencyOfInput(float in_temp, float in_hum, float in_light, float in_volt)const
	{
		// test: skip any consitency check and allow the "weird" entries
		return true;

		// empirical, for the time being...
		if (in_hum < 0)
			return false;
		if (in_temp > 30)
			//return false;
			if (in_light < 0)
				return false;
		//if (in_volt < 2.300)// 2.37??? 2.31
		//	return false;
		//if ( (in_volt < 2.37) && (in_temp > 28) )// test
		//	return false;
		return true;
	}
public:
	BKLabSensorFullReading()
	{
		memIsAllocated = false;  max_variables = FULL_READING_SIZE;
		setnumOfVariables(max_variables);
	}
	~BKLabSensorFullReading() { memDeallocation(); }
	BKLabSensorFullReading(int numOfVariablesMeasured)
	{
		memIsAllocated = false;  max_variables = FULL_READING_SIZE;
		setnumOfVariables(max_variables);
	}
	// copy constructor
	BKLabSensorFullReading(const BKLabSensorFullReading& other)
	{
		memIsAllocated = false;  max_variables = 0;
		copyFrom(other, true);
	}
	BKLabSensorFullReading(const basicSensorMultipleScalarReading& other)
	{
		memIsAllocated = false;  max_variables = 0;
		copyFrom(other, true);
	}
	// copy operators
	virtual void copyFrom(const BKLabSensorFullReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString; int tot_val;
				int other_tot_val = other.getMaxVariables();
				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(other_tot_val);

				tot_val = getMaxVariables();

				for (int varnum = 0; varnum < tot_val; varnum++)
				{
					if (other.getVal(varnum, otherVal))
						setVal(varnum, otherVal, allowOverwrite);
				}
			}
		}
	}
	BKLabSensorFullReading& operator = (const BKLabSensorFullReading& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	virtual void copyFrom(const basicSensorMultipleScalarReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString, thisString; int tot_val = 0;
				int other_tot_val = other.getMaxVariables();
				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(other_tot_val);

				int this_tot_val = getMaxVariables();

				// scan all fields of the other object
				for (int varnum = 0; varnum < other_tot_val; varnum++)
				{
					// if the reading is initialized and the name is set
					if (other.getVal(varnum, otherVal) && other.getName(varnum, otherString))
					{
						// compare name (and unit) with all supported readings: if both match, set the value
						for (int this_varnum = 0; this_varnum < this_tot_val; this_varnum++)
						{	// compare name
							getName(this_varnum, thisString);
							if (otherString == thisString)
								if (other.getUnit(varnum, otherString))
								{	// compare unit: if this (too) matches, set the value
									getUnit(this_varnum, thisString);
									if (otherString == thisString)
									{
										setVal(this_varnum, otherVal, allowOverwrite);
										tot_val++;
										if (tot_val == this_tot_val)
											return;
										break;
									}
								}
						}
					}
				}
			}
		}
	}
	BKLabSensorFullReading& operator = (const basicSensorMultipleScalarReading& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	// setters
	virtual bool setName(int varnum, string inName, bool allowOverwrite = false)
	{
		cout << "Error: Attempt to overwrite Reading Name" << endl;
		return false;
	}
	virtual bool setUnit(int varnum, string inUnit, bool allowOverwrite = false)
	{
		cout << "Error: Attempt to overwrite Reading Unit" << endl;
		return false;
	}
	//
	virtual bool setnumOfVariables(int numOfVariablesMeasured)
	{
		return basicSensorMultipleScalarReading::setnumOfVariables(FULL_READING_SIZE);
	}
	//
	virtual void printClassInfo() const
	{
		string to_print;
		cout << "BKLabSensorFullReading; memory allocated to support " << getMaxVariables() << " variables" << endl;
		for (int varnum = 0; varnum < getMaxVariables(); varnum++)
		{
			getName(varnum, to_print);
			cout << "Var " << varnum << " is " << to_print;
			getUnit(varnum, to_print);
			cout << " (" << to_print << ")" << endl;
		}
	}

	// getters/setters - specific of this class
	// getters - specific of this class
	virtual bool getTemp(float& outVal) const { return getVal(0, outVal); }
	virtual bool getTemp(float& outVal, string& outString) const { return getVal(0, outVal, outString); }
	virtual bool getHum(float& outVal) const { return getVal(1, outVal); }
	virtual bool getHum(float& outVal, string& outString) const { return getVal(1, outVal, outString); }
	virtual bool getLight(float& outVal) const { return getVal(2, outVal); }
	virtual bool getLight(float& outVal, string& outString) const { return getVal(2, outVal, outString); }
	virtual bool getVolt(float& outVal) const { return getVal(3, outVal); }
	virtual bool getVolt(float& outVal, string& outString) const { return getVal(3, outVal, outString); }
	// setters - specific of this class
	virtual bool setTemp(float in_val, bool allowOverwrite = false) { return setVal(0, in_val, allowOverwrite); }
	virtual bool setHum(float in_val, bool allowOverwrite = false) { return setVal(1, in_val, allowOverwrite); }
	virtual bool setLight(float in_val, bool allowOverwrite = false) { return setVal(2, in_val, allowOverwrite); }
	virtual bool setVolt(float in_val, bool allowOverwrite = false) { return setVal(3, in_val, allowOverwrite); }
	//
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& readingsInputString, bool allowOverwrite = true)
	{	// temperature humidity light voltage : sample input string (from line)
		// 19.9884 37.0933 45.08 2.69964
		string element;
		bool result_temp, result_hum, result_light, result_volt;
		size_t len, match_pos, start_pos = 0;
		float in_temp, in_hum, in_light, in_volt;

		// first check on basic length;
		len = readingsInputString.length();
		if (len < 15)
			return false;
		//
		match_pos = readingsInputString.find(' ', start_pos);
		len = match_pos - start_pos + 1;
		if (len < 3)
			return false;
		element = readingsInputString.substr(start_pos, len);
		in_temp = stof(element);
		//
		start_pos = match_pos + 1;
		match_pos = readingsInputString.find(' ', start_pos);
		len = match_pos - start_pos + 1;
		if (len < 3)
			return false;
		element = readingsInputString.substr(start_pos, len);
		in_hum = stof(element);
		//
		start_pos = match_pos + 1;
		match_pos = readingsInputString.find(' ', start_pos);
		len = match_pos - start_pos + 1;
		if (len < 3)
			return false;
		element = readingsInputString.substr(start_pos, len);
		in_light = stof(element);
		//
		start_pos = match_pos + 1;
		len = readingsInputString.length() - start_pos + 1;
		if (len < 3)
			return false;
		element = readingsInputString.substr(start_pos, len);
		in_volt = stof(element);


		// perform a check to see if these are all valid entries from a "valid line".
		if (checkConsistencyOfInput(in_temp, in_hum, in_light, in_volt))
		{
			result_temp = setTemp(in_temp, allowOverwrite);
			result_hum = setHum(in_hum, allowOverwrite);
			result_light = setLight(in_light, allowOverwrite);
			result_volt = setVolt(in_volt, allowOverwrite);
			return (result_temp && result_hum && result_light && result_volt);
		}
		else
			return false;
	}
};

// init the shared variables (won't be allowed to change) ///////////////////////////
string BKLabSensorFullReading::shared_readingName_array[FULL_READING_SIZE] = { "Temperature" ,  "Humidity" , "Light", "Voltage" };
string BKLabSensorFullReading::shared_readingUnit_array[FULL_READING_SIZE] = { "C" ,  "%" , "LUX", "V" };
bool BKLabSensorFullReading::shared_nameInitialized_array[FULL_READING_SIZE] = { true ,  true , true, true };
bool BKLabSensorFullReading::shared_unitInitialized_array[FULL_READING_SIZE] = { true ,  true , true, true };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//// Inherited class for Temperature Reading only
class BKLabSensorTemperatureReading : public BKLabSensorFullReading {
protected:
	// inherits all from parent: only access variable 0, i.e. temparature (C); 
	// blankInitialization(): OK from parent
	// memAllocation(): OK from parent [temp is "pos 0" in nameInitialized_array and unitInitialized_array]
	// memDeallocation(): OK from parent
	//
	// Needed to sidestep the parent-default mem allocation
	bool memReallocation()
	{
		// First deallocate the memory;
		memDeallocation();
		// Then allocate with the "child" values
		max_variables = 1;
		return setnumOfVariables(max_variables);
	}
	// specific of Temp only
	bool checkConsistencyOfInput(float in_temp)const
	{
		// test: skip any consitency check and allow the "weird" entries
		return true;

		// empirical, for the time being...		
		if (in_temp > 30)
			return false;
		return true;
	};
	// adapt parent function.
	virtual bool checkConsistencyOfInput(float in_temp, float in_hum, float in_light, float in_volt) const
	{
		return checkConsistencyOfInput(in_temp);
	}
public:
	BKLabSensorTemperatureReading()
	{
		// At this stage the parent constructor has already allocated memory etc.
		// Parent constructor call to virtualized setnumOfVariables(max_variables) 
		// can't refer to the "child version" of that function 
		// (virtual func table may not be set till the end of the constructor).		
		memReallocation();
	}
	BKLabSensorTemperatureReading(int numOfVariablesMeasured)
	{
		// input values do not matter: just one variable allowed
		memReallocation();
	}
	// copy constructor
	BKLabSensorTemperatureReading(const BKLabSensorTemperatureReading& other)
	{
		memReallocation();
		copyFrom(other, true);
	}
	BKLabSensorTemperatureReading(const basicSensorMultipleScalarReading& other)
	{
		memIsAllocated = false;  max_variables = 0;
		copyFrom(other, true);
	}
	// copy operators
	//  copyFrom(const BKLabSensorTemperatureReading&,... : should work with parent function	
	//  BKLabSensorFullReading& operator = ... : should work with parent function
	virtual void copyFrom(const basicSensorMultipleScalarReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString, thisString; int tot_val = 0;
				int other_tot_val = other.getMaxVariables();
				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(other_tot_val);

				int this_tot_val = getMaxVariables();

				// scan all fields of the other object
				for (int varnum = 0; varnum < other_tot_val; varnum++)
				{
					// if the reading is initialized and the name is set
					if (other.getVal(varnum, otherVal) && other.getName(varnum, otherString))
					{
						// compare name (and unit) with all supported readings: if both match, set the value
						for (int this_varnum = 0; this_varnum < this_tot_val; this_varnum++)
						{	// compare name
							if (!getName(this_varnum, thisString))
								continue;
							if (otherString == thisString)
								if (other.getUnit(varnum, otherString))
								{	// compare unit: if this (too) matches, set the value
									if (!getUnit(this_varnum, thisString))
										continue;
									if (otherString == thisString)
									{
										setVal(this_varnum, otherVal, allowOverwrite);
										tot_val++;
										if (tot_val == this_tot_val)
											return;
										break;
									}
								}
						}
					}
				}
			}
		}
	}
	//  BKLabSensorFullReading& operator = ... : should work with parent function [can only set 1 variable]
	//
	virtual void copyFrom(const BKLabSensorFullReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString; int tot_val;

				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(1);

				if (other.getVal(0, otherVal))
					setTemp(otherVal, allowOverwrite);
			}
		}
	}
	//
	// setters
	//  "setName", "setUnit", same as the parent class (will not allow to modify anything)
	//
	// The number of variables is fixed to 1.	
	virtual bool setnumOfVariables(int numOfVariablesMeasured)
	{
		return basicSensorMultipleScalarReading::setnumOfVariables(1);
	}
	//
	virtual void printClassInfo() const
	{
		cout << "BKLabSensorTemperatureReading, based on";
		BKLabSensorFullReading::printClassInfo();
	}
	//
	// getters/setters - specific of this class: "suppress" the functions for the unsupported readings	
	virtual bool getHum(float& outVal) const { return false; }
	virtual bool getHum(float& outVal, string& outString) const { return false; }
	virtual bool getLight(float& outVal) const { return false; }
	virtual bool getLight(float& outVal, string& outString) const { return false; }
	virtual bool getVolt(float& outVal) const { return false; }
	virtual bool getVolt(float& outVal, string& outString) const { return false; }
	virtual bool setHum(float in_val, bool allowOverwrite = false) { return false; }
	virtual bool setLight(float in_val, bool allowOverwrite = false) { return false; }
	virtual bool setVolt(float in_val, bool allowOverwrite = false) { return false; }
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& readingsInputString, bool allowOverwrite = true)
	{	// temperature : sample input string (from line)
		// 19.9884 37						
		float in_temp; string element; size_t len, match_pos, start_pos = 0;

		// FV_edit: 30 June 2022; support to (also) read from the "Full reading" file
		// check if the reading includes more than just the temperature;
		// if so, stop at the first blank.
		len = readingsInputString.length();
		if (len < 3)
			return false;

		match_pos = readingsInputString.find(' ', start_pos);

		if ((match_pos < 3) || (match_pos >= len))
			in_temp = stof(readingsInputString);// no "blank" found: this is just Temp
		else
		{
			// this is a "full reading" string;
			len = match_pos - start_pos + 1;
			if (len < 3)
				return false;
			element = readingsInputString.substr(start_pos, len);
			in_temp = stof(element);
		}

		//
		// perform a check to see if it is a valid entry from a "valid line".
		if (checkConsistencyOfInput(in_temp))
			return setTemp(in_temp, allowOverwrite);
		else
			return false;
	}
};
////



// Specialization of templated "Sensor data point" for BKLab sensor data: using Template specialization /////
// the BKLab SensorDataEntry : combination of the BKL-Date_Time, Readings and info

// FV_edit: 04 July 2022 - derive from base class (empty shell): virtualize reset();
class BKLabReadingInfo : public SensorOrReadingInfo {
protected:
	unsigned short epoch;
	unsigned short totAveragedSamples;
	void blank_init()
	{
		epoch = 0;
		totAveragedSamples = 0;
	}
public:
	BKLabReadingInfo() { blank_init(); }
	BKLabReadingInfo(const BKLabReadingInfo& other)
	{
		copyFrom(other);
	}
	void copyFrom(const BKLabReadingInfo& other)
	{
		if (this != &other)
		{
			epoch = other.getEpoch();
			totAveragedSamples = other.getTotAveragedSamples();
		}
	}
	void setEpoch(unsigned short val) { epoch = val; }
	// more needed, to set/update the totAveraged... (?!) 
	unsigned short getEpoch() const { return epoch; }
	unsigned short getTotAveragedSamples() const { return totAveragedSamples; }
	BKLabReadingInfo& operator = (const BKLabReadingInfo& other)
	{
		this->copyFrom(other);
		return*this;
	}
	virtual void reset() { blank_init(); }
};

///---------------- ///
// sensor data entry complying with the BKLab data format
typedef SensorDataEntry <BKLabDateAndTime, BKLabSensorFullReading, BKLabReadingInfo, basicSensorID> BKLabSensorDataEntry;
template <>
bool BKLabSensorDataEntry::setFromString(const string& fullLineInputString, bool allowOverwrite)
{	// date time epoch moteid temperature humidity light voltage - in string example: 
	// "2004-02-28 00:59:16.02785 3 1 19.9884 37.0933 45.08 2.69964" 
	string dateAndTimeInputString, readingsInputString, epoch_str, moteID_str;
	bool result_date_time, result_readings, result_nodeid;
	int length = fullLineInputString.length();
	int start_pos, match_pos, len; unsigned char epoc, moteID;

	// find substring with date and time - then fill date and time
	start_pos = 13;// skip the blank between date and time
	match_pos = fullLineInputString.find(' ', start_pos);
	start_pos = 0;
	len = match_pos - start_pos + 1;
	if (len < 21)
		return false;
	dateAndTimeInputString = fullLineInputString.substr(start_pos, len);
	result_date_time = dateNtime.setFromString(dateAndTimeInputString, allowOverwrite);

	// find epoch
	start_pos = match_pos + 1;
	match_pos = fullLineInputString.find(' ', start_pos);
	len = match_pos - start_pos + 1;
	if (len < 2)
		return false;
	epoch_str = fullLineInputString.substr(start_pos, len);
	epoc = (unsigned char)stoul(epoch_str);
	info.setEpoch(epoc);

	// find moteid
	start_pos = match_pos + 1;
	match_pos = fullLineInputString.find(' ', start_pos);
	len = match_pos - start_pos + 1;
	if (len < 2)
		return false;
	moteID_str = fullLineInputString.substr(start_pos, len);
	moteID = (unsigned char)stoul(moteID_str);
	result_nodeid = nodeID.setnodeIDnum((unsigned int)moteID, allowOverwrite);

	// finish with substring with readings - then fill readings
	start_pos = match_pos + 1; 	len = length - 1;
	if (len < 15)
		return false;
	readingsInputString = fullLineInputString.substr(start_pos, len);
	result_readings = reading.setFromString(readingsInputString, allowOverwrite);
	//
	return (result_date_time && result_readings && result_nodeid);
}
template <>
string BKLabSensorDataEntry::convert2string(bool longform, bool includeNodeID) const
{	// date time epoch moteid temperature humidity light voltage - in string example: 
	// "2004-02-28 00:59:16.02785 3 1 19.9884 37.0933 45.08 2.69964" 
	string str_date_and_time, str_reading, str_epoc, str_nodeId, str_all;
	if (dateNtime.getInit())
	{
		str_date_and_time = dateNtime.convert2string(longform);
		str_all += str_date_and_time;
		//str_all += ' ';
	}
	str_epoc = to_string(info.getEpoch());
	str_all += str_epoc;
	str_all += ' ';
	//
	if (includeNodeID)
	{
		str_nodeId = nodeID.convert2string(longform);
		str_all += str_nodeId;
		str_all += ' ';
	}
	//
	if (reading.getInit())
	{
		str_reading = reading.convert2string(longform);
		str_all += str_reading;
	}
	return str_all;
}
///---------------- ///


///++++++++++++++++++ ///
// "simplified" sensor data entry (same as BKLab data, but readings only include "temperature")
// For this specilized object, 3 out of 4 templated components are the same as in BKLabSensorDataEntry; 
// the 4th is a "child" of the corresponding component in BKLabSensorDataEntry.
// Therefore the following specialized functions as the same as for BKLabSensorDataEntry
// (not sure if there is a way to avoid this duplication; leave it as is for now).
typedef SensorDataEntry <BKLabDateAndTime, BKLabSensorTemperatureReading, BKLabReadingInfo, basicSensorID> BKLabTemperatureSensorDataEntry;
template <>
bool BKLabTemperatureSensorDataEntry::setFromString(const string& fullLineInputString, bool allowOverwrite)
{	// date time epoch moteid temperature  - in string example: 
	// "2004-02-28 00:59:16.02785 3 1 19.9884" 
	string dateAndTimeInputString, readingsInputString, epoch_str, moteID_str;
	bool result_date_time, result_readings, result_nodeid;
	int length = fullLineInputString.length();
	int start_pos, match_pos, len; unsigned char epoc, moteID;

	// find substring with date and time - then fill date and time
	start_pos = 13;// skip the blank between date and time
	match_pos = fullLineInputString.find(' ', start_pos);
	start_pos = 0;
	len = match_pos - start_pos + 1;
	if (len < 21)
		return false;
	dateAndTimeInputString = fullLineInputString.substr(start_pos, len);
	result_date_time = dateNtime.setFromString(dateAndTimeInputString, allowOverwrite);

	// find epoch
	start_pos = match_pos + 1;
	match_pos = fullLineInputString.find(' ', start_pos);
	len = match_pos - start_pos + 1;
	if (len < 2)
		return false;
	epoch_str = fullLineInputString.substr(start_pos, len);
	epoc = (unsigned char)stoul(epoch_str);
	info.setEpoch(epoc);

	// find moteid
	start_pos = match_pos + 1;
	match_pos = fullLineInputString.find(' ', start_pos);
	len = match_pos - start_pos + 1;
	if (len < 2)
		return false;
	moteID_str = fullLineInputString.substr(start_pos, len);
	moteID = (unsigned char)stoul(moteID_str);
	result_nodeid = nodeID.setnodeIDnum((unsigned int)moteID, allowOverwrite);

	// finish with substring with readings - then fill readings
	start_pos = match_pos + 1; 	len = length - 1;
	if (len < 15)
		return false;
	readingsInputString = fullLineInputString.substr(start_pos, len);
	result_readings = reading.setFromString(readingsInputString, allowOverwrite);
	//
	return (result_date_time && result_readings && result_nodeid);
}
template <>
string BKLabTemperatureSensorDataEntry::convert2string(bool longform, bool includeNodeID) const
{	// date time epoch moteid temperature humidity light voltage - in string example: 
	// "2004-02-28 00:59:16.02785 3 1 19.9884 37.0933 45.08 2.69964" 
	string str_date_and_time, str_reading, str_epoc, str_nodeId, str_all;
	if (dateNtime.getInit())
	{
		str_date_and_time = dateNtime.convert2string(longform);
		str_all += str_date_and_time;
		//str_all += ' ';
	}
	str_epoc = to_string(info.getEpoch());
	str_all += str_epoc;
	str_all += ' ';
	//
	if (includeNodeID)
	{
		str_nodeId = nodeID.convert2string(longform);
		str_all += str_nodeId;
		str_all += ' ';
	}
	//
	if (reading.getInit())
	{
		str_reading = reading.convert2string(longform);
		str_all += str_reading;
	}
	return str_all;
}
///++++++++++++++++++ ///

template <>
bool crossCopySourceToDest(const BKLabSensorDataEntry& source, BKLabTemperatureSensorDataEntry& destination)
{
	bool result = false;
	bool allowOverwrite = true;
	//cout << "Source: " << source.convert2string(false) << endl;
	//cout << "Dest: " << destination.convert2string(false) << endl;
	if (source.getInit())
	{
		destination.nodeID.copyFrom(source.nodeID, allowOverwrite);
		destination.info.copyFrom(source.info);
		destination.dateNtime.copyFrom(source.dateNtime, allowOverwrite);
		destination.reading.copyFrom(source.reading, allowOverwrite);
		result = true;
	}
	return result;
}

//Knox addition to convert BK into a generic that can be handled by gPlotter
template <class sns_DATA_ENTRY_type1, class sns_DATA_ENTRY_type2>
bool crossCopyDataEntrySourceToDest(const sns_DATA_ENTRY_type1& source, sns_DATA_ENTRY_type2& destination)
{
	bool result = false;
	bool allowOverwrite = true;
	if (source.getInit())
	{
		destination.nodeID.copyFrom(source.nodeID, allowOverwrite);
		destination.dateNtime.date.copyFrom(source.dateNtime.date, allowOverwrite);
		destination.dateNtime.time.copyFrom(source.dateNtime.time, allowOverwrite);
		destination.reading.copyFrom(source.reading, allowOverwrite);
		//info not needed or able to be coppied for plotting in the basic entry
		//destination.info.copyFrom(source.info);
		result = true;
	}
	return result;
}



// Specialization of templated "Sensor data list" for BKLab sensor data: using Template specialization
// the BKLab SensorDataEntry : combination of the BKL-Date_Time, Readings and info (and uchar for notde ID: max 255 nodes)
typedef CS_SensorDataList <BKLabSensorDataEntry> BKLabSensorDataList;

template <class sns_DATA_LIST_type1, class sns_DATA_LIST_type2, class entry_type1, class entry_type2>
bool crossCopyDataListSourceToDest(sns_DATA_LIST_type1& source, sns_DATA_LIST_type2& destination)
{
	bool result = false;

	if (source.isListSet() && !destination.isListSet())
	{
		int size = source.getListSize();
		unsigned int ID = source.getlistNodeID();
		destination.setUpList(ID);
		entry_type1 SourceTemp;
		entry_type2 DestTemp;
		for (int i = 0; i < size; i++)
		{
			source.getFromList(i, SourceTemp, false);
			crossCopyDataEntrySourceToDest<entry_type1, entry_type2>(SourceTemp, DestTemp);
			destination.addToList(DestTemp);
		}
		result = true;
	}
	else if (!source.isListSet())
	{
		cout << "Error: source list is not set" << endl;
	}
	else if (destination.isListSet())
	{
		cout << "Error: destination list is already set" << endl;
	}
	return result;
}

// FV_edit: 30 June 2022; Template Specialization for Temperature reading.
// Specialization of templated "Sensor data list" for BKLab Temperatuere sensor data: 
// the BKLabTemperatureSensorDataEntry : combination of the BKL-Date_Time, Temp-Readings and info (and uchar for notde ID: max 255 nodes)
typedef CS_SensorDataList <BKLabTemperatureSensorDataEntry> BKLabTemperatureSensorDataList;


// Specialization of templated "Sensor data Network" for BKLab sensor data: 
typedef CS_SensorNetworkData <BKLabSensorDataEntry> BKLabSensorNetwork;

template <class sns_Network_type1, class sns_Network_type2, class entry_type1, class entry_type2>
bool crossCopyNetworkSourceToDest(sns_Network_type1& source, sns_Network_type2& destination)
{
	int size = source.getNetworkSize();
	bool result = false;
	if (size >= 0)
	{
		destination.fullReset();
		entry_type1 SourceTemp;
		entry_type2 DestTemp;
		for (int i = 0; i < size; i++)
		{
			int sizeOfList = source.returnDataListSize(i);
			if (sizeOfList > 0)
			{
				for (int j = 0; j < sizeOfList; j++)
				{
					source.getDataFromNetwork(i, j, SourceTemp, false);
					crossCopyDataEntrySourceToDest<entry_type1, entry_type2>(SourceTemp, DestTemp);
					destination.addDataToNetwork(DestTemp);
				}
			}
		}
		result = true;
	}
	return result;
}


// FV_edit: 30 June 2022; Template Specialization for Temperature reading.
// Specialization of templated "Sensor data Network" for BKLab sensor data: 
typedef CS_SensorNetworkData <BKLabTemperatureSensorDataEntry> BKLabTemperatureSensorNetwork;