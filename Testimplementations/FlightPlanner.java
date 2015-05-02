import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Ellipse2D;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.io.File;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.TreeMap;
import java.util.Vector;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
/**
 * DO NOT CHANGE THIS FILE
 * 
 * This program displays a graph that connects
 * various airports in the US. It uses the AirportGraph
 * to represent the data, and lets the user find
 * paths from one city to another.
 * 
 * @author Richard McKenna
 */
public class FlightPlanner extends JFrame implements ActionListener
{
	/*
	 * This method responds button clicks on the 
	 * radio buttons that choose the path finding
	 * algorithms.
	 */
	public void actionPerformed(ActionEvent ae)
	{
		this.updateLabel();
		mapPanel.repaint();
	}
	
	/*
	 * This method responds to the window being 
	 * resized by redrawing the map and graph using
	 * the new window size.
	 */
	class WindowResizeHandler extends ComponentAdapter
	{
		public void componentResized(ComponentEvent ce)
		{
			mapPanel.shapesInitialized = false;
		}
	}

	// HERE'S THE GRAPH
	private AirportGraph graph = new AirportGraph();
	
	// HERE'S THE PATH
	private LinkedList<String> path = new LinkedList<String>();
	
	// HERE ARE THE GUI COMPONENTS
	private JPanel northPanel = new JPanel();
	private JPanel eastNorthPanel = new JPanel();
	private ButtonGroup buttonGroup = new ButtonGroup();
	private JRadioButton dfsRadioButton = new JRadioButton("Find Greedy Depth First Path");
	private JRadioButton bfsRadioButton = new JRadioButton("Find Shortest Path:");
	private JLabel northLabel = new JLabel();
	
	// HERE'S WHERE WE WILL RENDER EVERYTHING
	private MapPanel mapPanel = new MapPanel();

	/*
	 * This helper method claculates the total
	 * trip mileage for display purposes.
	 */
	public float calculateTripMileage()
	{
		float mileage = 0.0f;
		if (path.size() > 1)
		{
			for (int i = 0; i < path.size()-1; i++)
			{
				Airport a1 = graph.getAirport(path.get(i));
				Airport a2 = graph.getAirport(path.get(i+1));
				mileage += Airport.calculateDistance(a1, a2);
			}
		}
		return mileage;
	}

	/*
	 * This method updates the display panel that
	 * provides the full trip summary. It also will
	 * update the lines to be used for rendering
	 * the path. This method should be called after
	 * each time the path is changed, either because
	 * the source or destination city has changed,
	 * or if the pathing algorithm has changed.
	 */
	public void updateLabel()
	{
		String src = "None Selected";
		if (mapPanel.sourceAirport != null)
			src = mapPanel.sourceAirport.id;
		String dst = "None Selected";

		if (mapPanel.destinationAirport != null)
			dst = mapPanel.destinationAirport.id;
		if (dfsRadioButton.isSelected())
			path = graph.generateGreedyDepthFirstPath(src,dst);
		else
			path = graph.generateShortestPath(src,dst);
		mapPanel.updatePathLines();
		int legsOfTrip = path.size()-1;
		if (legsOfTrip < 0)
			legsOfTrip = 0;
		String html = 	"<html>\n"
					+	" <body style=\"font-family:monospace; font-size:20pt\">\n"
					+	"  <p>\n"
					+   "   Source Airport: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\">" + src + "</font><br />\n"
					+	"   Destination Airport: <font color=\"blue\">" + dst + "</font><br />\n"
					+	"   Legs of Trip: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + legsOfTrip + "<br />\n"
					+	"	Total Mileage: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + calculateTripMileage() + "\n"
					+   "  </p>"
					+	" </body>\n"
					+   "</html>";
		northLabel.setText(html);
	}

	/*
	 * All rendering will be done inside this panel.
	 */
	class MapPanel extends JPanel implements MouseListener
	{
		// HERE'S THE MAP
		Image mapImage;
		
		// THIS CONTROLS DATA INITIALIZATION, WHICH
		// MUST HAPPEN BEFORE RENDERING
		boolean shapesInitialized = false;
		
		// HERE'S THE DATA FOR RENDERING
		TreeMap<String,Ellipse2D.Float> vertices = new TreeMap<String,Ellipse2D.Float>();
		Vector<Line2D.Float> edges = new Vector<Line2D.Float>();
		Ellipse2D.Float sourceVertex = null;
		Airport sourceAirport = null;
		Ellipse2D.Float destinationVertex = null;
		Airport destinationAirport = null;
		float MAX_LAT = 50.0f;
		float MIN_LAT = 25.0f;
		float MAX_LONG = 125.0f;
		float MIN_LONG = 65.0f;
		float POINT_DIAMETER = 10.0f;
		Vector<Line2D.Float> pathLines = new Vector<Line2D.Float>();

		/*
		 * Default Constructor, it will load the map
		 * image.
		 */
		public MapPanel() 
		{
			Airport airport;
			Toolkit tk = Toolkit.getDefaultToolkit();
			File file = new File("usa_map.jpg");
			mapImage = tk.createImage(file.toString());
			MediaTracker tracker = new MediaTracker(this);
			tracker.addImage(mapImage, 0);
			try
			{
				tracker.waitForID(0);
			}
			catch(InterruptedException ie)
			{
				String message = "Error Reading " + file;
				JOptionPane.showMessageDialog(this, message, "File Reading Error", JOptionPane.ERROR_MESSAGE);
			}
			addMouseListener(this);
		}

		/*
		 * This method should be called each time
		 * the path changes, it will rebuild the vector
		 * containing the lines to render.
		 */
		public void updatePathLines()
		{
			pathLines = new Vector<Line2D.Float>();
			if (path.size() > 1)
			{
				for (int j = 0; j < path.size()-1; j++)
				{
					Airport a1 = graph.getAirport(path.get(j));
					Airport a2 = graph.getAirport(path.get(j+1));
					Line2D.Float line = constructLine(a1.id,a2.id);
					pathLines.add(line);
				}
			}
			
		}

		/*
		 * This method constructs and returns a point
		 * for a given airport.
		 */
		public Point2D.Float getPoint(Airport airport)
		{
			float lon = ((float)airport.longitudeDegrees) + ((float)airport.longitudeMinutes/60.0f);
			float lat = ((float)airport.latitudeDegrees) + ((float)airport.latitudeMinutes/60.0f);
			float x = (getWidth() - ((lon - MIN_LONG)/(MAX_LONG - MIN_LONG)) * getWidth());
			float y = (getHeight() - ((lat - MIN_LAT)/(MAX_LAT - MIN_LAT)) * getHeight());
			Point2D.Float dot = new Point2D.Float(x,y);
			return dot;
		}

		/*
		 * This method constructs and returns a line
		 * that connects the two airport arguments.
		 */
		public Line2D.Float constructLine(String a1, String a2)
		{
			Airport ap1 = graph.getAirport(a1);
			Point2D.Float p1 = getPoint(ap1);

			Airport ap2 = graph.getAirport(a2);
			Point2D.Float p2 = getPoint(ap2);
			
			Line2D.Float line = new Line2D.Float(	p1.x + (POINT_DIAMETER/2.0f),
													p1.y + (POINT_DIAMETER/2.0f),
													p2.x + (POINT_DIAMETER/2.0f),
													p2.y + (POINT_DIAMETER/2.0f));
			return line;
		}

		/*
		 * This method initializes the data structures
		 * that store our renderable objects.
		 */
		public void initVectors()
		{
			vertices = new TreeMap<String,Ellipse2D.Float>();
			edges = new Vector<Line2D.Float>();
			if (sourceAirport != null)
			{
				Point2D.Float ps = getPoint(sourceAirport);
				sourceVertex = new Ellipse2D.Float(ps.x,ps.y,POINT_DIAMETER,POINT_DIAMETER);
			}
			if (destinationAirport != null)
			{
				Point2D.Float pd = getPoint(destinationAirport);
				destinationVertex = new Ellipse2D.Float(pd.x,pd.y,POINT_DIAMETER,POINT_DIAMETER);
			}
			updatePathLines();
		}

		/*
		 * Here is where the rendering is actually
		 * done.
		 */
		public void paintComponent(Graphics g)
		{
			if (getWidth() > 0)
			{
				// IF THE DATA HAS NOT YET BEEN 
				// INITIALIZED, THEN MAKE SURE TO
				// DO IT
 				if (!shapesInitialized)
				{
 					initVectors();
					Airport airport;
					float longWidth = MAX_LONG - MIN_LONG;
					float latHeight = MAX_LAT - MIN_LAT;
					float realLong, realLat;
					float r = 10;
					Iterator<Airport> airportIt = graph.getAirportIterator();
					while (airportIt.hasNext())
					{
						airport = airportIt.next();
						Point2D.Float p = getPoint(airport);
						Ellipse2D.Float dot = new Ellipse2D.Float(p.x,p.y,r,r);
						vertices.put(airport.id,dot);
					}
					
					Iterator<String> airportCodeIt = graph.getAirportCodeIterator();
					while (airportCodeIt.hasNext())
					{
						String src = airportCodeIt.next();
						Iterator<String> destIt = graph.getEdgeIteratorForAirport(src);
						while (destIt.hasNext())
						{
							String dest = destIt.next();
							Line2D.Float line = constructLine(src,dest);
							edges.add(line);
						}
					}
					shapesInitialized = true;
				}

 				// NOW WE'RE READY TO RENDER
 				if (shapesInitialized)
 				{
 					Graphics2D g2 = (Graphics2D)g;
 					
 					// RENDER THE MAP FIRST
 					g2.drawImage(mapImage, 0, 0, getWidth(), getHeight(), null);
 					
 					// NOW RENDER THE GRAPH LINES
 					BasicStroke bs = new BasicStroke(POINT_DIAMETER/4.0f);
 					((Graphics2D)g).setStroke(bs);
 					g2.setColor(Color.lightGray);
 					for (int i = 0; i < edges.size(); i++)
 					{
 						Line2D.Float line = edges.get(i);
 						if (!pathLines.contains(line))
 						{
 							g2.draw(line);
 						
 						}
 					}
 					
 					// NOW RENDER EACH NODE, BOTH
 					// THE DOT AND THE CODE
 					g2.setColor(Color.white);
 					Iterator<String> airportCodeIt = graph.getAirportCodeIterator();
 					while(airportCodeIt.hasNext())
 					{
 						String airportCode = airportCodeIt.next();
 						Ellipse2D.Float dot = vertices.get(airportCode);
 						Airport airport = graph.getAirport(airportCode);
 						if ((dot != sourceVertex)
							&&
							(dot != destinationVertex))
 						{
 							g2.fill(dot);
 							g2.drawString(airport.id, dot.x+POINT_DIAMETER, dot.y+POINT_DIAMETER);
 						}							
 					}
 					// RENDER THE SOURCE AIRPORT
 					// NODE IN RED
 					if (sourceVertex != null)
 					{
 						g2.setColor(Color.red);
 						g2.fill(sourceVertex);
 						g2.drawString(sourceAirport.id, sourceVertex.x+10, sourceVertex.y);
 					}
 					// RENDER THE DESTINATION AIRPORT
 					// NODE IN BLUE
 					if (destinationVertex != null)
 					{
 						g2.setColor(Color.blue);
 						g2.fill(destinationVertex);
 						g2.drawString(destinationAirport.id,destinationVertex.x+10,destinationVertex.y);
 					}
 					// DRAW THE PATH IN PURPLE
 					g2.setColor(Color.magenta);
 					for (int j = 0; j < pathLines.size(); j++)
 					{
 						g2.draw(pathLines.get(j));
 					}
 				}
			}
		}
		
		/*
		 * This method tests to see if the x,y
		 * coordinate arguments are inside the dot
		 * argument. True is returned if it is,
		 * false otherwise.
		 */
		public boolean contains(Ellipse2D.Float dot, float x, float y)
		{
			if (x < dot.x)
				return false;
			if (y < dot.y)
				return false;
			if (x > (dot.x + dot.width))
				return false;
			if (y > (dot.y + dot.height))
				return false;
			return true;
		}

		/*
		 * This method responds to mouse clicks on
		 * the map, allowing us to pick source and
		 * destination nodes.
		 */
		public void mouseClicked(MouseEvent me) 
		{
			Iterator<String> keysIt = vertices.keySet().iterator();
			while (keysIt.hasNext())
			{
				String key = keysIt.next();
				Ellipse2D.Float vertex = vertices.get(key);
				float meX = (float)me.getX();
				float meY = (float)me.getY();
				Point2D p = new Point2D.Float(meX, meY);
				
				// WE FOUND THE NODE THAT WE CLICKED ON
				if (contains(vertex, meX, meY))
				{
					// LEFT BUTTON FOR THE SOURCE
					if (me.getButton() == MouseEvent.BUTTON1)
					{
						sourceVertex = vertex;
						sourceAirport = graph.getAirport(key);
					}
					// RIGHT BUTTON FOR THE DESTINATION
					else
					{
						destinationVertex = vertex;
						destinationAirport = graph.getAirport(key);
					}
					updateLabel();
					repaint();
					return;
				}
			}
		}
		
		// WE ARE INCLUDING THESE EMPTY METHODS SIMPLY
		// TO SATISFY THE COMPILER, SINCE THIS CLASS
		// implements MoustListener, WE MUST DEFINE
		// ALL METHODS IN THAT INTERFACE
		public void mouseEntered(MouseEvent me)	{}
		public void mouseExited(MouseEvent me) 	{}
		public void mousePressed(MouseEvent me) {}
		public void mouseReleased(MouseEvent me){}
	}

	/*
	 * Default Constructor, it initializes the GUI
	 * components as well as the graph data.
	 */
	public FlightPlanner()
	{
		super("Flight Planner");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setExtendedState(MAXIMIZED_BOTH);
		initAirportGraph();		
		layoutGUI();
		mapPanel.addComponentListener(new WindowResizeHandler());
	}
	
	/*
	 * This method builds the graph, constructing
	 * and adding all the Airport objects as well
	 * as specifying all connections.
	 */
	public void initAirportGraph()
	{
		// FIRST ADD ALL THE AIRPORTS
		graph.addAirport(new Airport("AUS",30,18,97,42));
		graph.addAirport(new Airport("AVL",36,00,82,32));
		graph.addAirport(new Airport("BGR",44,48,68,40));
		graph.addAirport(new Airport("BOS",42,22,71,2));
		graph.addAirport(new Airport("BUF",42,56,78,44));
		graph.addAirport(new Airport("BVT",44,28,73,9));
		graph.addAirport(new Airport("CLE",41,24,81,81));
		graph.addAirport(new Airport("COD",44,33,109,4));
		graph.addAirport(new Airport("DCA",38,51,77,2));
		graph.addAirport(new Airport("DEN",39,45,104,52));
		graph.addAirport(new Airport("DFW",32,51,96,51));
		graph.addAirport(new Airport("ELP",31,48,106,24));
		graph.addAirport(new Airport("HOU",29,59,95,22));
		graph.addAirport(new Airport("IND",39,44,86,17));
		graph.addAirport(new Airport("JAX",30,30,81,42));
		graph.addAirport(new Airport("LAX",33,56,118,24));
		graph.addAirport(new Airport("LGA",40,47,73,58));
		graph.addAirport(new Airport("LIT",34,44,92,14));		
		graph.addAirport(new Airport("MCI",39,7,94,35));
		graph.addAirport(new Airport("MIA",25,48,80,16));
		graph.addAirport(new Airport("MKE",42,87,87,74));
		graph.addAirport(new Airport("MOT",48,25,101,21));
		graph.addAirport(new Airport("MSY",29,59,90,15));
		graph.addAirport(new Airport("OMA",41,30,95,80));
		graph.addAirport(new Airport("PHX",33,26,112,1));
		graph.addAirport(new Airport("PWM",43,39,70,19));
		graph.addAirport(new Airport("SFO",37,46,122,26));
		graph.addAirport(new Airport("SEA",47,39,122,18));
		graph.addAirport(new Airport("SLC",40,46,111,58));
		graph.addAirport(new Airport("VGT",36,5,115,10));

		// NOW CONNECT THE AIRPORTS
		graph.addConnection("AUS", "DFW");
		graph.addConnection("AVL", "IND");
		graph.addConnection("BOS", "BUF");
		graph.addConnection("BOS", "CLE");
		graph.addConnection("BOS", "PWM");
		graph.addConnection("BUF", "CLE");
		graph.addConnection("BVT", "BGR");
		graph.addConnection("CLE", "IND");
		graph.addConnection("CLE", "MKE");
		graph.addConnection("COD", "MOT");
		graph.addConnection("COD", "SEA");
		graph.addConnection("COD", "DEN");
		graph.addConnection("DCA", "AVL");
		graph.addConnection("DCA", "JAX");
		graph.addConnection("DEN", "DFW");
		graph.addConnection("DEN", "ELP");
		graph.addConnection("DFW", "HOU");
		graph.addConnection("ELP", "PHX");
		graph.addConnection("HOU", "LIT");
		graph.addConnection("IND", "MKE");
		graph.addConnection("JAX", "AVL");
		graph.addConnection("LAX", "MIA");
		graph.addConnection("LAX", "SFO");
		graph.addConnection("LAX", "VGT");
		graph.addConnection("LGA", "IND");
		graph.addConnection("LGA", "DCA");
		graph.addConnection("LGA", "BOS");
		graph.addConnection("LGA", "MIA");
		graph.addConnection("MCI", "DEN");
		graph.addConnection("MCI", "DFW");
		graph.addConnection("MCI", "IND");
		graph.addConnection("MCI", "OMA");
		graph.addConnection("MIA", "BOS");
		graph.addConnection("MIA", "JAX");
		graph.addConnection("MIA", "MSY");
		graph.addConnection("MKE", "MOT");
		graph.addConnection("MOT", "SEA");
		graph.addConnection("MOT", "DEN");
		graph.addConnection("MOT", "IND"); 
		graph.addConnection("MSY", "JAX");
		graph.addConnection("MSY", "IND");
		graph.addConnection("OMA", "DEN");
		graph.addConnection("OMA", "MOT");
		graph.addConnection("OMA", "IND");
		graph.addConnection("PHX", "LAX");
		graph.addConnection("PWM", "BVT");
		graph.addConnection("PWM", "BGR");
		graph.addConnection("SFO", "VGT");
		graph.addConnection("SFO", "DEN");
		graph.addConnection("SFO", "SEA");
		graph.addConnection("SLC", "SFO");
		graph.addConnection("SLC", "DEN");
		graph.addConnection("SLC", "SEA");
		graph.addConnection("VGT", "PHX");
	}

	/*
	 * This method lays out all the GUI components.
	 */
	public void layoutGUI()
	{
		updateLabel();
		northPanel.setLayout(new BorderLayout());
		northPanel.add(northLabel, BorderLayout.CENTER);
		
		Border border = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
		Border tBorder = BorderFactory.createTitledBorder(border, "Pathing Algorithm");
		eastNorthPanel.setBorder(tBorder);

		eastNorthPanel.add(dfsRadioButton);
		eastNorthPanel.add(bfsRadioButton);
		buttonGroup.add(dfsRadioButton);
		buttonGroup.add(bfsRadioButton);
		northPanel.add(eastNorthPanel, BorderLayout.EAST);
		add(northPanel, BorderLayout.NORTH);
		add(mapPanel, BorderLayout.CENTER);
		dfsRadioButton.setSelected(true);
		
		dfsRadioButton.addActionListener(this);
		bfsRadioButton.addActionListener(this);
	}
	
	public static void main(String[] args) 
	{
		JFrame frame = new FlightPlanner();
		frame.setVisible(true);
	}
}