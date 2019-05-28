# Black-Box
Coding Challenge

## Challenge:

We recently recovered the black box of an airplane. We want to determine its flight history that we can start to understand what went wrong. The black box records the GPS coordinates of the airplane every one second that it’s powered on. Your goal is to reconstruct the sequence of airports the airplane visited. 


### Input:
You should read from standard input. You will read in an airport database and a series of one or more flights.

##### Input Format
```
  <airports>                      # integeger: number of airports in the database
  <id> <latitude> <longitude>	  # id: A-Z0-9 (string), latitude: double, longitude: double
  <id> <latitude> <longitude>	  # id: A-Z0-9 (string), latitude: double, longitude: double
  ….. (repeated airports times)
  <flights>                       # integer: number of flights. Data below is repeated <flight> times
  <num_waypoints>                 # integer: number of waypoints in the flight
  <latitude> <longitude>          # latitude: double, longitude: double. Repeated num_waypoints times
  …… (repeated num_waypoints times)

```
##### Sample Input
```
3					# 3 airports
RHV 37.3329 -121.82			# First Airport
SJC 37.363 -121.929			# Second Airport
SBP 35.2373 -120.643		        # Third Airport
2					# 2 flights follow
4					# 4 waypoints in first flight
37.333 -121.80			        # Coordinates of first way point
37.350 -121.85			        # Coordinates of second way point
37.355 -121.92			        # Coordinates of third way point
37.362 -121.93			        # Coordinates of fourth way point
3					# 3 waypoints in second flight
35.2373 -120.643			# Coordinates of first point in second flight
36.0 -121.0				# Coordinates of second point in second flight
37.333 -121.81			        # Coordinates of third point in second flight
```

### Airport Database
The first lines will be of the format

	<string> <double> <double>
Where the string is an airport identifier made up of letters A-Z and the numbers 0-9, and the following doubles represent the latitude and longitude of the airport (in degrees, from -180 to 180). This will repeat for every airport in the database. That’s around 20,000 in the test cases.

The number of airports is given as an integer which is the first line of input.

### Flights
Next will come a series of one or more flights. Immediately after the last airport, the next line will have an integer indicating how many “flights” are following. Each “flight is a list of GPS coordinates. There is no trailing newline after the last flight. The format is 

	<double> <double>
	<double> <double>
The black box records the location every one second, so each line represents the plane’s location -in latitude and longitude, at a given point in time. Within a flight, all of the recorded locations are in-order. The first line is the location when the plane was powered on, and the last line is the location where the plane was powered down. A flight starts at an airport and ends at an airport, and may contain landings at intermediate airports to refuel (the plane will not be powered down at a refueling stop)

### Goal
Your goal is as follows.
1. For each flight, determine the starting point (where the flight took off), any intermediate airports (where the plane landed and subsequently took off from) and the destination airport ( where the plane landed for the last time on the flight).
2. Order flights to form a one-way trip formed of contiguous segments. No airport is visited twice. Unfortunately, the black box data got somewhat scrambled, so you’ll need to figure out the order of the flights. The flight that appears first in the data is not necessarily the first flight that was actually flown. For instance, if there’s a flight from SJC-TVL and a flight from JFK-DEN-SJC (DEN being an intermediate stop on the flight), the first trip would be JFK-DEN-SJC  and the second trip SJC-TVL, as JFK-DEN-SJC-TVL forms a contiguous trip.

### Output
You should output the airports, one per line, in the order they were visited. So, in our example above, the output would be

	JFK
	DEN
	SJC
	TVL

##### Sample Input
	5
	SJC 37.363 -121.929
	SBP,35.2373 -120.643
	JFK,40.6399 -73.7787
	LGA,40.7773 -73.8726
	TVL,38.8939 -119.995
	1
	2000
	37.353672 -121.9239578
	37.3536797 -121.9239578
	37.3536797 -121.9239578
	... (1992 more lines)
	38.8937836 -119.9976273
	38.8937721 -119.9976425
	38.8937645 -119.9976578
	38.8937531 -119.9976578
	38.8937416 -119.9976578

##### Sample Output
```
SJC
TVL
```

### Important considerations:
- We will consider an airplane to have taken off when it accelerates past 100km/h
- We will consider an airplane to have landed when it decelerates below 50km/h
- The coordinates given are for the center of an airport. An airplane usually takes off and lands on a runway, which is not at the center of an airport. An extreme example of this is Amsterdam, where the north end of the furthest runway (De Polderbaan) is almost 8km away from the center of the airport.
- When printing out the list of airports, don't forget to include any intermediate airports that the plane may have landed at during the flight.
- To calculate the distance between two GPS coordinates, most people use the Haversine formula.

