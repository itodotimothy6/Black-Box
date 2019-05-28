#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct coordinate {
    double latitude;
    double longitude;
    coordinate(double latitude, double longitude) {
        this->latitude = latitude;
        this->longitude = longitude;
    }
    bool operator==(const coordinate &other) const {
        return (latitude == other.latitude && longitude == other.longitude);
    }
};

struct airport {
    string id;
    double latitude;
    double longitude;
    airport(string id, double latitude , double longitude) {
        this->id = id;
        this->latitude = latitude;
        this->longitude = longitude;
    }
};

// Custom hash function to enable coordinate struct to be key in a hash map
namespace std {
    template <>
    struct hash<coordinate> {
        std::size_t operator()(const coordinate& c) const {
            using std::size_t;
            using std::hash;
            return ((hash<double>()(c.latitude) ^ (hash<double>()(c.longitude) << 1)));
        }
    };
}

const int RADIUS_OF_EARTH = 6371;
inline double ToRadians (double in) { return in * (3.1415926536 / 180); }

// Returns: Distance (in km) between two coordinates
double Haversine(double latitude1, double longitude1, double latitude2, double longitude2) {
    latitude1 = ToRadians(latitude1);
    latitude2 = ToRadians(latitude2);
    longitude1 = ToRadians(longitude1);
    longitude2 = ToRadians(longitude2);
    return (2 * RADIUS_OF_EARTH * asin(sqrt(
                                            sin((latitude2 - latitude1)/2) * sin((latitude2 - latitude1)/2) +
                                            cos(latitude1) * cos(latitude2) *
                                            sin((longitude2 - longitude1)/2) * sin((longitude2 - longitude1) / 2)
                                            )));
}

// Returns: Current Speed (in km/hr) given initial coordinate(previous waypoint) and current coordinate(current waypoint)
double speed(double latitude1, double longitude1, double latitude2, double longitude2) {
    return Haversine(latitude1, longitude1, latitude2, longitude2) * 3600;
}

// Rounds double to 2 dp
double round(double var) {
    double value = (int)(var * 100 + .5);
    return (double)value / 100;
}

// Returns: Airport ID if coordinate within 8km of any airport in airportList
string getAirportID(double latitude, double longitude, vector <airport> airportList) {
    string airportID;
    for (int i = 0; i < airportList.size(); i++){
        if (Haversine(latitude, longitude, airportList[i].latitude, airportList[i].longitude) <= 8.0) {
            return airportList[i].id;
        }
    }
    return "";
}

int main() {
    int numAirports;
    string airportID;
    double airportLat;
    double airportLong;
    
    
    // Airport Entry
    cin >> numAirports;
    
    // Hashmap with coordinates(rounded to 2 dp) as key and airport ID as value
    unordered_map<coordinate, string> airportListMap;
    
    // Airport list with its ID and precise coordinate
    vector<airport> airportList;
    
    for (int i = 0; i < numAirports; i++){
        cin >> airportID;
        cin >> airportLat;
        cin >> airportLong;
        
        coordinate airportLocation(round(airportLat), round(airportLong));
        airportListMap[airportLocation] = airportID;
        
        airport a(airportID, airportLat, airportLong);
        airportList.push_back(a);
    }
    
    // Flights
    int numFlights;
    unordered_map<string, vector<string>> flightSequenceMap; // Hash map:  Key-> Starting airport; Value->FlightSequence
    unordered_set<string> destinationAirportSet; // Hash set to store destination airport
    cin >> numFlights;
    
    for (int i = 0; i < numFlights; i++){
        int numWaypoints;
        cin >> numWaypoints;
        vector<string> flightSequence;
        
        // Initialize flight sequence
        double gpsLong, gpsLat;
        cin >> gpsLat;
        cin >> gpsLong;
        coordinate location1(round(gpsLat), round(gpsLong)); // coordinate of first waypoint
        
        if(airportListMap.find(location1) == airportListMap.end()) {
            flightSequence.push_back(getAirportID(gpsLat, gpsLong, airportList));
        }
        else {
            flightSequence.push_back(airportListMap[location1]);
        }
        
        double prevLat = gpsLat;
        double prevLong = gpsLong;
        
        for(int j = 1; j < numWaypoints; j++){
            cin >> gpsLat;
            cin >> gpsLong;
            
            coordinate location(round(gpsLat), round(gpsLong));
            
            if(speed(prevLat, prevLong, gpsLat, gpsLong) < 50){
                if(airportListMap.find(location) != airportListMap.end()){ // location was found in hash map
                    // Prevent repitition of airport ID in flightSequence
                    if(flightSequence[flightSequence.size()-1] != airportListMap[location]){
                        flightSequence.push_back(airportListMap[location]);
                    }
                }
                else{ // location was not found in hash map. (Do the brute-force solution)
                    string id = getAirportID(gpsLat, gpsLong, airportList);
                    // Prevent repition of airport ID in flightSequence
                    if(flightSequence[flightSequence.size()-1] != id){
                        flightSequence.push_back(id);
                    }
                }
            }
            prevLat = gpsLat;
            prevLong = gpsLong;
        }
    
        // Put flightSequence in map with the starting airport(first elem of flightSequence) as key
        flightSequenceMap[flightSequence[0]] = flightSequence;
        
        // Store destination airport in destinationAirportSet
        destinationAirportSet.insert(flightSequence[flightSequence.size()-1]);
    }
    
    
    /*********** Goal 2: Order the Flights *********/
    
    string start;
    
    // If an airport is a flightSequenceMap key but not in the destinationAirportSet,
    // the airport is the starting airport of the first flight
    for (auto x : flightSequenceMap){
        if (destinationAirportSet.find(x.first) == destinationAirportSet.end()){
            start = x.first;
            break;
        }
    }
    
    string end;
    end = flightSequenceMap[start][flightSequenceMap[start].size()-1];
    
    // Print Output
    cout << start << endl;
    while (flightSequenceMap.find(end) != flightSequenceMap.end()){
        for (int i=1; i<flightSequenceMap[start].size(); i++){
            cout << flightSequenceMap[start][i] << endl;
        }
        start = end;
        end =  end = flightSequenceMap[start][flightSequenceMap[start].size()-1];
    }
    for(int i=1; i<flightSequenceMap[start].size(); i++){
        cout << flightSequenceMap[start][i] << endl;
    }
    
    return 0;
}
