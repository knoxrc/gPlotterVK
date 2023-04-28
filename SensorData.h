#pragma once
#include "SensorDataBasicClasses.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>      // ifstream
using namespace std;

//*
//
// 
// 
// one data point; to be used as the element in sequence
template <class sns_DATE_N_TIME, class sns_READING, class sns_INFO, class sns_ID>
class SensorDataEntry {
protected:
	;
public:
	sns_ID nodeID;
	sns_DATE_N_TIME dateNtime;
	sns_READING reading;
	sns_INFO info;// anything not captured by ID, date&time, reading
public:
	SensorDataEntry() { ; }
	SensorDataEntry(const SensorDataEntry& other) { copyFrom(other, true); }
	void copyFrom(const SensorDataEntry& other, bool allowOverwrite = false)
	{
		// FV_edit: 05 July 2022; avoid self-copy (each internal component already does that anyway)
		if (this != &other)
		{
			if (other.getInit())
			{
				nodeID.copyFrom(other.nodeID, allowOverwrite);
				//info.copyFrom(other.info);
				dateNtime.copyFrom(other.dateNtime, allowOverwrite);
				reading.copyFrom(other.reading, allowOverwrite);
			}
		}

	}
	// FV Edit: 20 July 2022; include INFO (always init by default)
	bool getInit()const { return ((dateNtime.getInit() || reading.getInit() || nodeID.getInit()) && info.getInit()); }
	// FV_edit: 04 July 2022 - rest all fields;
	void resetAll()
	{
		nodeID.reset();
		dateNtime.resetDateAndTime();
		reading.resetAll();
		info.reset();
	}
	// this is generic: may need to be specialized
	string convert2string(bool longform, bool includeNodeID = true) const
	{
		string str_date_and_time, str_reading, str_nodeID, str_all;
		if (dateNtime.getInit())
		{
			str_date_and_time = dateNtime.convert2string(longform);
			str_all += str_date_and_time;
			str_all += ' ';
		}
		if (includeNodeID && nodeID.getInit())
		{
			str_nodeID = nodeID.convert2string(longform);
			str_all += str_nodeID;
			str_all += ' ';
		}
		if (reading.getInit())
		{
			str_reading = reading.convert2string(longform);
			str_all += str_reading;
		}
		return str_all;
	}
	// left to be speicialized
	bool setFromString(const string& fullLineInputString, bool allowOverwrite = true)
	{
		cout << "setFromString() needs template specialization " << endl;
		return false;
	}
	SensorDataEntry& operator = (const SensorDataEntry& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}

	//knox edits below:
	
};


// Templated function to convert from one type of SensorDataEntry to another (must be specialized)
template <class sns_DATA_ENTRY_type1, class sns_DATA_ENTRY_type2>
bool crossCopySourceToDest(sns_DATA_ENTRY_type1& source, sns_DATA_ENTRY_type2& destination)
{
	cout << "This MUST be specialized" << endl;
	return false;
}
//

typedef SensorDataEntry<basicSensorDateAndTime, basicSensorMultipleScalarReading, SensorOrReadingInfo, basicSensorID> BasicSensorDataEntry;

// A sequence of chronologically sorted (CS) data points related to one node.
// The node is (internally) identified by an integer number
template <class sns_DATA_ENTRY>
class CS_SensorDataList {
protected:
	bool listSet;
	unsigned int listNodeID;
	list<sns_DATA_ENTRY> dataList;
	// this is for a sorted list and for a valid element
	bool sortedInsertionFromFront(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		if (dataList.empty())
		{
			dataList.push_back(in_sns_dataEntry);
			return true;
		}
		// search from Start
		int item_pos = 0;
		for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
		{
			string curr_str, in_str;
			cout << endl << "Checking Element " << item_pos << endl;
			// compare current item to in_entry			
			in_str = in_sns_dataEntry.dateNtime.convert2string(false);
			curr_str = itm_ptr->dateNtime.convert2string(false);
			cout << "Input item " << in_str << endl;
			cout << "Curr item " << curr_str << endl;
			if (in_sns_dataEntry.dateNtime < (itm_ptr->dateNtime))
			{
				//entry point found: insert before current iterator
				cout << "Inserting entry at position " << item_pos << endl;
				dataList.insert(itm_ptr, in_sns_dataEntry);
				return true;
			}
			item_pos++;
		}
		// insertion point not found: element goes to the end of the list
		cout << "Inserting entry at position " << item_pos << endl;
		dataList.push_back(in_sns_dataEntry);
		return true;
	}
	bool sortedInsertionFromBack(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		if (dataList.empty())
		{
			//cout << "Inserting entry in empty list." << endl;
			dataList.push_back(in_sns_dataEntry);
			return true;
		}
		// search from end
		//list<sns_DATA_ENTRY>::iterator test_ptr;
		auto itm_ptr = dataList.end();
		itm_ptr--; // point to last "occupied" element 
		int item_pos = getListSize() - 1;
		for (; item_pos >= 0; item_pos--)
		{
			// test/debug
			//string curr_str, in_str;
			//in_str = in_sns_dataEntry.dateNtime.convert2string(false);
			//curr_str = itm_ptr->dateNtime.convert2string(false);
			//cout << "Pos " << item_pos << " item: " << curr_str << "; (Input item " << in_str << ")";
			//
			// compare current item to in_entry			
			if (in_sns_dataEntry.dateNtime >= (itm_ptr->dateNtime))
			{
				//entry point found: insert after current iterator
				//cout << " => Insert at position " << item_pos+1 << endl;
				// [insert before position (itm_ptr+1)]
				dataList.insert(++itm_ptr, in_sns_dataEntry);
				return true;
			}
			//cout << " ... Keep moving" << endl;
			// last lop iteration, but must avoid attempt to decrement the pointer any further.
			if (item_pos == 0)
				break;
			itm_ptr--;
		}
		// insertion point not found: element goes to the beginning of the list
		//cout << "Inserting entry as first element." << endl;
		dataList.push_front(in_sns_dataEntry);
		return true;
	}
	bool checkEntry(const sns_DATA_ENTRY& in_sns_dataEntry) const
	{
		unsigned int in_NodeID;
		if (in_sns_dataEntry.nodeID.getNodeID_num(in_NodeID) && in_sns_dataEntry.dateNtime.getInit() && in_sns_dataEntry.reading.getInit())
			if (in_NodeID == listNodeID)
				return true;
		return false;
	}
	void blankInitialization() { listNodeID = 0; listSet = false; }
public:
	CS_SensorDataList() { blankInitialization(); }
	~CS_SensorDataList() { blankInitialization(); } //The destructor of list< > will do the rest
	unsigned int getlistNodeID()const
	{
		if (!isListSet())
			cout << "Error retriving listNodeID: list is NOT set" << endl;
		return listNodeID;
	}
	int getListSize() const { return dataList.size(); }
	bool isListSet() const { return listSet; }
	bool setUpList(unsigned int in_nodeID)
	{
		bool result = false;
		if (!isListSet())
		{
			listNodeID = in_nodeID;
			listSet = true;
			result = true;
		}
		else
			cout << "Error setting Data List: list is already set" << endl;
		return result;
	}
	//
	bool addToList(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		bool result = false;
		if (isListSet())
		{
			if (checkEntry(in_sns_dataEntry))
				result = sortedInsertionFromBack(in_sns_dataEntry);//= sortedInsertion(in_sns_dataEntry);
			else
				cout << "Error adding Data to List: Entry is not valid" << endl;
		}
		else
			cout << "Error adding Data to List: list ID not set" << endl;
		return result;
	}
	bool getFromList(unsigned int in_pos, sns_DATA_ENTRY& in_sns_dataEntry, bool remove = false)
	{
		bool result = false;
		if (in_pos < getListSize() && in_pos >= 0)
		{
			int item_pos = 0;
			for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
			{
				// find element and copy into inp/out param;
				if (item_pos == in_pos)
				{
					in_sns_dataEntry = (*itm_ptr);
					if (remove)
					{
						//create new empty list and "splice". Then erase from new list.
						list<sns_DATA_ENTRY> temp;
						int tempcount = getListSize();
						temp.splice(temp.begin(), dataList, itm_ptr);
						tempcount = getListSize();
						temp.clear();
					}
					result = true;
					break;
				}
				item_pos++;
			}
		}
		return result;
	}
	

	bool getFirstOrLastVal(sns_DATA_ENTRY& in_sns_dataEntry, bool first) const
	{
		bool result = false;
		int size = getListSize();
		if (size > 0)
		{
			auto itm_ptr = dataList.begin();
			// change if you want the last;
			if (size > 1 && !first)
			{
				itm_ptr = dataList.end();
				itm_ptr--; // point to last "occupied" element 
			}
			in_sns_dataEntry = (*itm_ptr);
			result = true;
		}
		return result;
	}
	//
	void printToScreen(bool longform)const
	{
		if (!isListSet())
		{
			cout << "List is NOT set." << endl;
			return;
		}
		cout << " Node ID: " << listNodeID << endl;
		if (!dataList.empty())
		{
			int item_pos = 0;
			string item_string;
			for (auto itm_ptr = dataList.begin(); itm_ptr != dataList.end(); itm_ptr++)
			{
				item_string = itm_ptr->convert2string(longform, false);
				cout << "Element " << item_pos << ": " << item_string << endl;
				item_pos++;
			}
		}
		else
			cout << "List is empty." << endl;
	}

	void PrintInfoToScreen()const
	{
		sns_DATA_ENTRY dataEntry;
		if (isListSet())
		{
			cout << " Node ID: " << (unsigned long int)listNodeID << ". Tot entries: " << getListSize() << "; ";
			if (getListSize())
			{
				// get first;
				if (getFirstOrLastVal(dataEntry, true))
				{
					cout << "First Entry on: " << dataEntry.dateNtime.convert2string(false) << "; ";
				}
			}
			if (getListSize() > 1)
			{
				// get last entry;
				// get first;
				if (getFirstOrLastVal(dataEntry, false))
				{
					cout << "Last Entry on: " << dataEntry.dateNtime.convert2string(false) << "; ";
				}
			}
			cout << endl;
		}
		else
			cout << " Sensor data not set" << endl;
	}
	// function to write content to file 
	//bool storeDataToFile(ofstream& ofs, const sns_DATA_ENTRY& start, const sns_DATA_ENTRY& stop, bool remove = false)
	bool storeDataToFile(ofstream& ofs, bool remove = false)
	{
		list<sns_DATA_ENTRY> temp_list;
		sns_DATA_ENTRY temp_dataEntry;
		bool result = false;
		int tot_entries_temp, tot_entries_after, tot_entries = getListSize();
		bool item_removed;
		string entry_string;
		bool stopToCheck;
		if (ofs.is_open() && tot_entries > 0)
		{
			for (unsigned int pos = 0; pos < tot_entries; pos++)
			{
				// get element and remove (then add it back: this is faster)
				item_removed = getFromList(0, temp_dataEntry, true);
				if (item_removed)
				{	// convert to string and write to file;  
					entry_string.clear();
					entry_string = temp_dataEntry.convert2string(false);
					ofs << entry_string << endl;
					// copy to temp list if needed
					if (!remove)
						temp_list.push_back(temp_dataEntry);
				}
				else
					break;
			}
			if (!remove)
			{	// put back, if needed
				tot_entries_temp = temp_list.size();
				if (tot_entries_temp != tot_entries)
					cout << "Warning: Copy to file incomplete" << endl;
				// splice the list;
				tot_entries_after = dataList.size();
				dataList.splice(dataList.begin(), temp_list);
				tot_entries_after = dataList.size();
				tot_entries_temp = temp_list.size();
				if (tot_entries_after == tot_entries)
					result = true;
			}
			else
			{
				tot_entries_after = dataList.size();
				if (tot_entries_after == 0)
					result = true;
			}
		}
		return result;
	}
};

typedef CS_SensorDataList <BasicSensorDataEntry> BasicSensorDataList;

// Data points of an entire Network: a sequence of CS_SensorDataList (one for each sensor_node in the network)
// based on a dynamically allocated array: full random access
template <class sns_DATA_ENTRY>
class CS_SensorNetworkData {
protected:
	CS_SensorDataList<sns_DATA_ENTRY>** SensDataListArray;
	unsigned int SensDataListArray_SIZE;
	//
	void blank_init()
	{
		SensDataListArray = NULL;
		SensDataListArray_SIZE = 0;
	}
	bool extendArray(unsigned int new_size)
	{
		if (new_size > SensDataListArray_SIZE)
		{
			// allocate the new array and check the mem;
			CS_SensorDataList<sns_DATA_ENTRY>** NEW_SensDataListArray = (CS_SensorDataList<sns_DATA_ENTRY>**) calloc(sizeof(CS_SensorDataList<sns_DATA_ENTRY>*), new_size);
			if (NEW_SensDataListArray == NULL)
			{
				cout << "Out of mem allocation the extended array" << endl;
				return false;
			}
			// copy across all the (pointers to) existing node lists;
			for (unsigned int i = 0; i < SensDataListArray_SIZE; i++)
			{
				NEW_SensDataListArray[i] = SensDataListArray[i];
				SensDataListArray[i] = NULL;
			}
			// deallocate the old array
			if (SensDataListArray != NULL)
			{
				free(SensDataListArray);
				SensDataListArray = NULL;
			}

			// set the new values
			SensDataListArray = NEW_SensDataListArray;
			SensDataListArray_SIZE = new_size;
			// done
			return true;
		}
		return false;
	}
	bool addNodeToNetwork(unsigned int in_nodeID)
	{
		if (in_nodeID < SensDataListArray_SIZE)
		{
			// array entry exists: check and allocate
			if (SensDataListArray[in_nodeID] == NULL)
			{
				SensDataListArray[in_nodeID] = new CS_SensorDataList<sns_DATA_ENTRY>;
				if (SensDataListArray[in_nodeID] != NULL)
				{
					if (SensDataListArray[in_nodeID]->setUpList(in_nodeID))
						return true;
					return false;
				}
				cout << "Error: out of memory allocating list for node " << in_nodeID << endl;
				return false;
			}
			cout << "Error: Node " << in_nodeID << " already exists in the Network" << endl;
			return false;
		}
		else
		{
			// array is to small, needs to be extended
			bool mem_ok = extendArray(in_nodeID + 1);
			// if it succeeds: then recursive call;
			if (mem_ok)
				return addNodeToNetwork(in_nodeID);
			else
			{
				cout << "Error: Out of memory while extending the array to add node" << in_nodeID << endl;
				return false;
			}
		}
		cout << "Error: addNodeToNetwork should not get here" << endl;
		return false;
	}
	void releaseAllMem()
	{
		if ((SensDataListArray_SIZE > 0) && (SensDataListArray != NULL))
		{
			for (unsigned int i = 0; i < SensDataListArray_SIZE; i++)
			{
				if (SensDataListArray[i] != NULL)
				{
					delete SensDataListArray[i];
					SensDataListArray[i] = NULL;
				}
			}
			free(SensDataListArray);
			SensDataListArray = NULL;
			SensDataListArray_SIZE = 0;
		}
	}
	bool findNodeAndAddToDataList(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		unsigned int in_nodeID;
		if (!in_sns_dataEntry.nodeID.getNodeID_num(in_nodeID))
			return false;

		if (in_nodeID < SensDataListArray_SIZE)
			if (SensDataListArray[in_nodeID] != NULL)
				return SensDataListArray[in_nodeID]->addToList(in_sns_dataEntry);
		return false;
	}
public:
	CS_SensorNetworkData() { blank_init(); }
	~CS_SensorNetworkData() { releaseAllMem(); }
	// Hard reset: destroys all data (and releases all the memory)
	void fullReset()
	{
		releaseAllMem();
		blank_init();
	}
	//RK Edit
	int returnDataListSize(int node_ID) const
	{
		int size;
		size = getSensDataListSize(node_ID);

		return size;
	}


	// FV Edit: 20 July 2022; make the following two funcitons public
	bool isNodeInNetwork(unsigned int in_nodeID) const
	{
		if (in_nodeID < SensDataListArray_SIZE)
			if (SensDataListArray[in_nodeID] != NULL)
				return true;
		return false;
	}
	int getSensDataListSize(unsigned int in_nodeID) const
	{
		if (isNodeInNetwork(in_nodeID))
			return SensDataListArray[in_nodeID]->getListSize();
		return -1;
	}
	//
	unsigned int getNetworkSize() const { return SensDataListArray_SIZE; }
	bool getTotals(unsigned int& totDataEntries, unsigned int& totNodesWithData) const
	{
		bool result = true;
		int totNodeDataEntries;
		totDataEntries = 0;
		totNodesWithData = 0;
		for (unsigned int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				totNodeDataEntries = SensDataListArray[i]->getListSize();
				if (totNodeDataEntries > 0)
				{
					totDataEntries += totNodeDataEntries;
					totNodesWithData++;
				}
			}
		}
		return result;
	}
	//
	bool addDataToNetwork(const sns_DATA_ENTRY& in_sns_dataEntry)
	{
		unsigned int in_nodeID;
		if (!in_sns_dataEntry.nodeID.getNodeID_num(in_nodeID))
			return false;

		bool mem_OK = true;

		if (!isNodeInNetwork(in_nodeID))
			mem_OK = addNodeToNetwork(in_nodeID);

		if (mem_OK)
			return findNodeAndAddToDataList(in_sns_dataEntry);

		cout << "Insertion failed: Error Extending network size to accomodate new node " << in_nodeID << endl;
		return false;
	}
	bool getDataFromNetwork(unsigned int in_nodeID, unsigned int in_pos, sns_DATA_ENTRY& in_sns_dataEntry, bool removeDataFromNetwork = false)
	{
		bool result = false;
		if (isNodeInNetwork(in_nodeID))
			if (in_pos < getSensDataListSize(in_nodeID))
				return SensDataListArray[in_nodeID]->getFromList(in_pos, in_sns_dataEntry, removeDataFromNetwork);

		return result;
	}


	bool getEarliestDataFromNetwork(sns_DATA_ENTRY& in_sns_dataEntry, bool removeDataFromNetwork = false)
	{
		sns_DATA_ENTRY temp_dataEntry, earliest_dataEntry;
		bool first_comparison = true;
		bool result = false;
		unsigned int i_earliest;
		// test the earliest entry for each node
		for (unsigned int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				if (SensDataListArray[i]->getFromList(0, temp_dataEntry, false))
				{
					if (first_comparison)
					{
						earliest_dataEntry = temp_dataEntry;
						first_comparison = false;
						i_earliest = i; // to remove the element later, if needed !!!
					}
					else
						if (temp_dataEntry.dateNtime < earliest_dataEntry.dateNtime)
						{
							earliest_dataEntry = temp_dataEntry;
							i_earliest = i;
						}
				}
			}
		}

		if (!first_comparison)
		{// found at least one data point, and found the earliest
			result = true;
			in_sns_dataEntry = earliest_dataEntry;
			// remove the element if needed
			if (removeDataFromNetwork)
				SensDataListArray[i_earliest]->getFromList(0, temp_dataEntry, true);
		}

		return result;
	}
	//
	void printNetworkInfoToScreen()const
	{
		sns_DATA_ENTRY temp_dataEntry, earliest_dataEntry, latest_dataEntry;
		bool first_comparison = true;
		cout << "Network (max) Size: " << getNetworkSize() << endl;
		for (unsigned int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				SensDataListArray[i]->PrintInfoToScreen();
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, true))
				{
					if (first_comparison)
					{
						earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						first_comparison = false;
					}
					else
						if (temp_dataEntry.dateNtime < earliest_dataEntry.dateNtime)
							earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
				}
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, false))
					if (temp_dataEntry.dateNtime > latest_dataEntry.dateNtime)
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
			}
			else
				cout << " Node ID: " << i << ". Not set (no entry)." << endl;
		}
		if (!first_comparison)
		{
			cout << endl << "Network first entry on: " << earliest_dataEntry.dateNtime.convert2string(false);
			cout << "; Network last entry on: " << latest_dataEntry.dateNtime.convert2string(false) << endl;
		}
		cout << endl;
	}
	void printNetworkSummaryToScreen()const
	{
		sns_DATA_ENTRY temp_dataEntry, earliest_dataEntry, latest_dataEntry;
		bool first_comparison = true;
		int totNodesWithData = 0;
		int nodeDataEntries, totDataEntries = 0;
		cout << "Network (max) Size: " << getNetworkSize() << endl;
		for (unsigned int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				nodeDataEntries = getSensDataListSize(i);
				if (nodeDataEntries > 0)
				{
					totNodesWithData++;
					totDataEntries += nodeDataEntries;
				}
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, true))
				{
					if (first_comparison)
					{
						earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
						first_comparison = false;
					}
					else
						if (temp_dataEntry.dateNtime < earliest_dataEntry.dateNtime)
							earliest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
				}
				if (SensDataListArray[i]->getFirstOrLastVal(temp_dataEntry, false))
					if (temp_dataEntry.dateNtime > latest_dataEntry.dateNtime)
						latest_dataEntry.dateNtime = temp_dataEntry.dateNtime;
			}
		}
		if (!first_comparison)
		{
			cout << " Network first entry on: " << earliest_dataEntry.dateNtime.convert2string(false);
			cout << "; Network last entry on: " << latest_dataEntry.dateNtime.convert2string(false) << endl;
			cout << " Network contains " << totDataEntries << " data entries, across " << totNodesWithData << " active nodes" << endl;
		}
		else
			cout << " Empty Network" << endl;

		cout << endl;
	}
	//
	virtual bool loadNetworkDataFromOpenFile(ifstream& ifs, bool print_err_msg = false)
	{
		string curr_line;
		sns_DATA_ENTRY curr_input;
		if (ifs.is_open())
		{
			unsigned long int succ_line_count = 0;
			unsigned long int fail_line_count = 0;
			unsigned long int line_count;
			for (line_count = 0; ifs.good(); line_count++)
			{
				// FV_edit: 04 July 2022 - clear any data from prev iteration;
				curr_input.resetAll();

				curr_line.clear();
				// read line by line
				getline(ifs, curr_line);

				// FV_edit: 30 June 2022
				// stop the attempt to process "empty" data when going past the last line of text.
				if (!ifs.good())
					break;

				if (curr_input.setFromString(curr_line, true))
				{
					if (addDataToNetwork(curr_input))
						succ_line_count++;
					else
					{
						cout << "Error adding data to Network" << endl;
					}
				}
				else
				{
					if (print_err_msg)
						cout << "Error setting entry from line (" << (line_count + 1) << "): " << curr_line << endl;
					fail_line_count++;
				}
				if (print_err_msg && (line_count % 10000 == 0))
					cout << "Processing input file, please wait." << endl;
			}
			cout << endl << "Lines parsed: " << line_count;
			cout << ". Inserted: " << succ_line_count;
			cout << ". Discarded: " << fail_line_count << " (";
			cout << (fail_line_count * 100) / line_count << "%)." << endl;
			return true;
		}
		else
		{
			cout << "Error: File is not open" << endl;
			return false;
		}
	};
	virtual bool loadNetworkDataFromFile(string basedir, string filename, bool print_err_msg = false)
	{
		bool result = false;
		ifstream ifs;
		string fullFilename = basedir;
		fullFilename += filename;
		ifs.open(fullFilename);
		if (ifs.is_open())
		{
			// skip first line if any (not needed here)
			if (loadNetworkDataFromOpenFile(ifs, print_err_msg))
			{
				//cout << endl << "Data Loaded. Summary:" << endl;
				//printNetworkInfoToScreen();
				result = true;
			}
			else
				cout << "Loading from file failed" << endl;
		}
		ifs.close();
		//cout << endl << "Done." << endl;
		return result;
	}
	//
	//
	virtual bool storeNetworkDataToOpenFile(ofstream& ofs, bool remove = false)
	{
		bool result = false;
		for (int i = 0; i < getNetworkSize(); i++)
		{
			if (SensDataListArray[i] != NULL)
			{
				if (SensDataListArray[i]->storeDataToFile(ofs, remove))
					result = true;
				else
					return false;
			}
		}
		return result;
	}
	// stores to file in strict chronological order interleaving data from different sensors if needed)  
	virtual bool storeNetworkDataToOpenFileStrictChronoOrder(ofstream& ofs, bool remove = false)
	{
		CS_SensorNetworkData <sns_DATA_ENTRY> tempNetwork;
		sns_DATA_ENTRY temp_dataEntry;
		string entry_string;
		unsigned long int tot_written = 0; unsigned long int tot_written2 = 0;
		unsigned long int prev_tot_written = 0;

		bool temporarilyRemoveDataFromNetwork = true;
		while (getEarliestDataFromNetwork(temp_dataEntry, temporarilyRemoveDataFromNetwork))
		{
			// convert to string and write to file;  
			entry_string.clear();
			entry_string = temp_dataEntry.convert2string(false);
			ofs << entry_string << endl;
			tot_written++;

			// if network data needs to be preserved, push to local copy
			if (!remove)
				tempNetwork.addDataToNetwork(temp_dataEntry);
		}
		cout << " Finished writing to file " << tot_written << " items." << endl;

		// if network data needs to be preserved, copy back from local copy
		if (!remove)
		{
			cout << " Reinstating data to network" << endl;
			while (tempNetwork.getEarliestDataFromNetwork(temp_dataEntry, true))
			{
				if (addDataToNetwork(temp_dataEntry))
					tot_written2++;
				else
					cout << " Error: reinstating entry failed" << endl;
			}
		}
		cout << "Done. " << endl;
		if (remove)
			return true;
		else
			if (tot_written == tot_written2)
				return true;
		return false;
	}
	virtual bool storeNetworkDataToFile(string basedir, string filename, bool remove, bool useStrictChronoOrder)
	{
		bool result = false;
		ofstream ofs;
		string fullFilename = basedir;
		fullFilename += filename;

		cout << endl << "Ready to store data." << endl;
		cout << endl << " Current Network Summary:" << endl;
		printNetworkInfoToScreen();

		ofs.open(fullFilename);
		if (ofs.is_open())
		{
			// addfirst line if any (not needed here)
			if (useStrictChronoOrder)
				result = storeNetworkDataToOpenFileStrictChronoOrder(ofs, remove);
			else
				result = storeNetworkDataToOpenFile(ofs, remove);

			if (result)
			{
				cout << endl << "Data Stored." << endl;
				cout << endl << "Current Network Summary:" << endl;
				printNetworkInfoToScreen();
			}
			else
				cout << "Storing to file failed" << endl;
		}
		ofs.close();
		cout << endl << "Done." << endl;
		return result;
	}
	static bool loadNetworkDataFromFileAndStoreStrictChronoOrder(string basedir, string in_filename, string out_filename, bool verbose = false)
	{
		bool result = false;
		CS_SensorNetworkData tempNetwork;
		cout << "Loading data from file (this may take a while)" << endl;
		result = tempNetwork.loadNetworkDataFromFile(basedir, in_filename, verbose);
		if (result)
			cout << " Done; all fine." << endl;
		else
			cout << " Done; something went wrong." << endl;

		if (result)
		{
			cout << "Storing data to file (this too may take a while)" << endl;
			result = tempNetwork.storeNetworkDataToFile(basedir, out_filename, true, true);
			if (result)
				cout << " Done; all fine." << endl;
			else
				cout << " Done; something went wrong." << endl;
		}
		return result;
	}
};

typedef CS_SensorNetworkData <BasicSensorDataEntry> BasicSensorNetwork;

