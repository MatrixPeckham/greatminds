/*
 * DO NOT CHANGE THIS CLASS
 * 
 * This class is used to represent an airport
 * for our graph of airports. In addition to
 * representing an Airport object, this class also
 * provides a static method for calculating the
 * distance in miles between two airports.
 */
public class Airport implements Comparable
{
	public String id;
	public int latitudeDegrees;
	public int latitudeMinutes;
	public int longitudeDegrees;
	public int longitudeMinutes;

	public Airport(	String initID,
					int initLatitudeDegrees,
					int initLatitudeMinutes,
					int initLongitudeDegrees,
					int initLongitudeMinutes)
	{
		id = initID;
		latitudeDegrees = initLatitudeDegrees;
		latitudeMinutes = initLatitudeMinutes;
		longitudeDegrees = initLongitudeDegrees;
		longitudeMinutes = initLongitudeMinutes;
	}
	
	public int compareTo(Object obj) 
	{ return id.compareTo(((Airport)obj).id); }

	// CONSTANTS USED FOR DISTANCE CALCULATION
	public static final float RADIAN_FACTOR = 180.0f/(float)Math.PI;
	public static final float EARTH_RADIUS = 3963.0f;

	/*
	 * calculateDistance - This method returns the
	 * distance in miles of a1 to a2.
	 */
	public static float calculateDistance(	Airport a1,
											Airport a2)
	{
		float lat1 = (float)a1.latitudeDegrees + (float)a1.latitudeMinutes/60.0f;
		lat1 = lat1/RADIAN_FACTOR;
		float long1 = -(float)a1.longitudeDegrees + (float)a1.longitudeMinutes/60.0f;
		long1 = long1/RADIAN_FACTOR;
		float lat2 = (float)a2.latitudeDegrees + (float)a2.latitudeMinutes/60.0f;
		lat2 = lat2/RADIAN_FACTOR;
		float long2 = -(float)a2.longitudeDegrees + (float)a2.longitudeMinutes/60.0f;
		long2 = long2/RADIAN_FACTOR;
		
		float x = (float)( 
					(Math.sin(lat1) * Math.sin(lat2))
				+	(Math.cos(lat1)
				*	Math.cos(lat2)
				*	Math.cos(long2 - long1))
				);
		float x2 = (float)(Math.sqrt(1.0f - (x*x))/x);
		float distance = (float)(EARTH_RADIUS *	Math.atan(x2));
		return distance;
	}
}