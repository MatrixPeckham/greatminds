import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.TreeMap;
import java.util.Vector;

//I IMPORTED THESE TO USE FOR GREEDY and shortest AS A FAST SEARCHABLE VISITED LIST
import java.util.HashSet;
import java.util.Stack;

/*
 * William Peckham
 * YOU MUST FILL IN THE CODE FOR THE FOLLOWING METHOD:
 * - generateGreedyDepthFirstPath
 * 
 * IN ADDITION, FOR THE BONUS PART OF THE ASSIGNMENT,
 * DEFINE THE FOLLOWING:
 * - generateShortestPath
 * 
 * This class is a graph-manager, connecting Airports
 * to one another.
 */
public class AirportGraph {
	// THESE ARE THE NODES IN THE GRAPH
	public TreeMap<String, Airport> vertices = new TreeMap<String, Airport>();

	// THESE ARE THE CONNECTIONS BETWEEN THE NODES
	// FOR EACH AIRPORT CODE, WE HAVE A SORTED Vector
	// OF THE ADJACENT AIRPORT CODES
	public TreeMap<String, Vector<String>> edges = new TreeMap<String, Vector<String>>();

	// DEFAULT CONSTRUCTOR
	public AirportGraph() {
	}

	// THESE ITERATORS ARE FOR RENDERING

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This Iterator lets us go through all of the Airports in the graph.
	 */
	public Iterator<Airport> getAirportIterator() {
		return vertices.values().iterator();
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This Iterator lets us go through all codes of Airports in the graph.
	 */
	public Iterator<String> getAirportCodeIterator() {
		return vertices.keySet().iterator();
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This Iterator lets us get all of the airport codes adjacent to the code
	 * argument.
	 */
	public Iterator<String> getEdgeIteratorForAirport(String code) {
		Vector<String> v = edges.get(code);
		return v.iterator();
	}

	// ACCESSOR METHODS FOR GETTING INFORMATION
	// ABOUT THE GRAPH

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * Accessor method returning the number of Airports in the graph.
	 */
	public int getNumAirports() {
		return vertices.size();
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * Accessor method returning the number of edges in the graph.
	 */
	public int getNumConnections() {
		Iterator<Vector<String>> it = edges.values().iterator();
		int numConnections = 0;
		while (it.hasNext()) {
			numConnections += it.next().size();
		}
		return numConnections;
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * addAirport - This method is used for adding a node to the graph. We use a
	 * BST (TreeMap) for our vertices so that we may find an airport in O(logN)
	 * time.
	 */
	public void addAirport(Airport airportToAdd) {
		if (!(vertices.containsKey(airportToAdd.id))) {
			vertices.put(airportToAdd.id, airportToAdd);
			edges.put(airportToAdd.id, new Vector<String>());
		}
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * addConnection - This method adds a connection (edge) between the two
	 * provided airports. If either airport is not already in the graph, or if
	 * the connection already exists, nothing is changed.
	 */
	public void addConnection(String airportID1, String airportID2) {
		Vector<String> airport1Connections = edges.get(airportID1);
		Vector<String> airport2Connections = edges.get(airportID2);

		// IF ONE OF THE AIRPORTS DOESN'T EXIST
		if ((airport1Connections == null) || (airport2Connections == null))
			return;

		// ONLY ADD THE EDGES IF THEY ARE NOT
		// ALREADY THERE
		if (!airport1Connections.contains(airportID2)) {
			airport1Connections.add(airportID2);
			Collections.sort(airport1Connections);
		}
		if (!airport2Connections.contains(airportID1)) {
			airport2Connections.add(airportID1);
			Collections.sort(airport2Connections);
		}
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This is an accessor method for geting the Airport object associated with
	 * the code argument.
	 */
	public Airport getAirport(String code) {
		return vertices.get(code);
	}

	/*
	 * DO NOT CHANGE THIS CLASS
	 * 
	 * AirportComparator - This class is used for comparisons between Airport
	 * objects. This is used by the Collections.binarySearch method.
	 */
	class AirportComparator implements Comparator {
		public int compare(Object o1, Object o2) {
			Airport a1 = (Airport) o1;
			Airport a2 = (Airport) o2;
			return a1.compareTo(a2);
		}

		public boolean equals(Object o1, Object o2) {
			Airport a1 = (Airport) o1;
			Airport a2 = (Airport) o2;
			return (a1.compareTo(a2) == 0);
		}
	}

	/*
	 * YOU MUST DEFINE THIS METHOD
	 * 
	 * generateGreedyDepthFirstPath - This method should use a greedy depth
	 * first search algorithm to find a path from sourceID airport to
	 * destinationID airport. This method will then return the path as a
	 * LinkedList of Strings, where each String is the airport code of the
	 * airport in the path. The first airport in the path should be the source,
	 * and the last airport should be the destination.
	 */
	public LinkedList<String> generateGreedyDepthFirstPath(String sourceID,
			String destinationID) {
		HashSet<String> visited = new HashSet<String>();
		LinkedList<String> path = new LinkedList<String>();
		String cur = sourceID;
		LinkedList<String> neighbors = new LinkedList<String>();
		boolean start = true;
		if(!(vertices.containsKey(sourceID)&&vertices.containsKey(destinationID))){
			return new LinkedList<String>();
		}
		while (true) {
			if (start) {
				path.addLast(cur);
				visited.add(cur);
			}
			neighbors.clear();
			for (String s : edges.get(cur)) {
				if (!visited.contains(s)) {
					neighbors.add(s);
				}
			}
			if (neighbors.isEmpty()) {
				path.removeLast();
				if (path.isEmpty()) {
					return new LinkedList<String>();
				} else {
					cur = path.getLast();
					start = false;
					continue;
				}
			} else {
				if (neighbors.contains(destinationID)) {
					path.addLast(destinationID);
					return path;
				} else {
					double dist = Double.MAX_VALUE;
					String closest = "";
					for (String s : neighbors) {
						double d = Airport.calculateDistance(vertices.get(cur),
								vertices.get(s));
						if (d < dist) {
							dist = d;
							closest = s;
						}
					}
					cur = closest;
					start = true;
					continue;
				}
			}
		}
	}

	/*
	 * THIS METHOD IS WORTH 200 BONUS POINTS
	 * 
	 * generateShortestPath - This method should use a breadth first search
	 * algorithm to find the shortest path from sourceID to destinationID. In
	 * order to do this, you'll have to keep track of the shortest path from the
	 * source to each node you encounter, and update this path when you find one
	 * shorter.
	 */
	// FIXME needs fixing
	public LinkedList<String> generateShortestPath(String sourceID,
			String destinationID) {
		if(!(edges.containsKey(sourceID)&&edges.containsKey(destinationID))){
			return new LinkedList<String>();
		}
		// A* algorithm using a treemap as a priority queue
		TreeMap<Double,String> queue = new TreeMap<Double,String>(); //queue for visiting
		HashSet<String> visited = new HashSet<String>(); //visited
		TreeMap<String,Double> g_score = new TreeMap<String,Double>(); //distance from start to node
		TreeMap<String,Double> h_score = new TreeMap<String,Double>(); //straight distance from node to dest
		TreeMap<String,Double> f_score = new TreeMap<String,Double>(); //total plausable distance
		TreeMap<String,String> toKfromV = new TreeMap<String,String>(); //for current path
		String cur;
				
		g_score.put(sourceID,0D);
		h_score.put(sourceID, str8Dist(sourceID,destinationID));
		f_score.put(sourceID, h_score.get(sourceID));

		queue.put(f_score.get(sourceID),sourceID);
		
		while(!queue.isEmpty()){
			cur=queue.get(queue.firstKey());
			if(cur.equals(destinationID)){
				return getList(toKfromV,destinationID);
			}
			queue.remove(queue.firstKey());
			visited.add(cur);
			for(String s : edges.get(cur)){
				if(visited.contains(s)){
					continue;
				}
				double d = g_score.get(cur)+str8Dist(cur,s);
				if(!queue.containsValue(s)){
					toKfromV.put(s, cur);
					g_score.put(s, d);
					h_score.put(s, str8Dist(s,destinationID));
					f_score.put(s, g_score.get(s)+h_score.get(s));
					queue.put(f_score.get(s),s);
				} else if(d<g_score.get(cur)) {
					toKfromV.put(s, cur);
					g_score.put(s, d);
					h_score.put(s, str8Dist(s,destinationID));
					f_score.put(s, g_score.get(s)+h_score.get(s));
				}
			}
		}
		
		return new LinkedList<String>();
	}
	
	public double str8Dist(String s1, String s2){
		return Airport.calculateDistance(vertices.get(s1), vertices.get(s2));
	}
	
	public LinkedList<String> getList(TreeMap<String,String> tree, String dest){
		Stack<String> temp = new Stack<String>();
		
		if(!tree.containsKey(dest)){
			return new LinkedList<String>();
		}
		
		String cur = dest;
		temp.push(cur);
		while(tree.containsKey(tree.get(cur))){
			cur=tree.get(cur);
			temp.push(cur);
		}
		temp.push(tree.get(cur));
		
		LinkedList<String> lst = new LinkedList<String>();
		
		Iterator<String> it = temp.iterator();
		while(it.hasNext()){
			lst.addLast(it.next());
		}
		
		return lst;
	}
}