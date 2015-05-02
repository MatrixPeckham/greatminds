import java.awt.*;
import java.awt.event.*;
import java.util.Enumeration;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.filechooser.*;
import javax.swing.tree.*;

/*
 * William Peckham
 * HTMLFrame - used for PART 1 of CSE 214 HW 4
 * 
 * YOU MUST FILL IN THE CODE FOR THE FOLLOWING TWO METHODS:
 * - initTreeData 
 * - generateHTMLText
 */

public class HTMLFrame extends JFrame implements ActionListener, TreeSelectionListener
{
	// tree DISPLAYS OUR TREE, HANDLING ALL RENDERING
	private JTree tree = new JTree();
	private JScrollPane treeScrollPane = new JScrollPane(tree);
	
	// treeManager IS THE TREE-MANAGING CLASS, YOU WILL DEAL MAINLY
	// WITH THIS OBJECT AND IT'S NODE OBJECTS (type DefaultMutableTreeNode)
	private DefaultTreeModel treeManager = (DefaultTreeModel)tree.getModel();

	// GUI CONTROLS IN THE NORTH
	private JPanel northPanel = new JPanel();
	private JLabel tagLabel = new JLabel("HTML Tags:");
	private String[] htmlTags = 	{			"<p>",
												"<h1>",
												"<h2>",
												"<h3>",
												"<h4>",
												"<h5>",
												"<br />",
												"<hr />"
											};
	private JComboBox tagChoices = new JComboBox(htmlTags);
	private String addTagCommand = "Add Tag";
	private JButton addTagButton = new JButton(addTagCommand);
	private String removeCommand = "Remove";
	private JButton removeButton = new JButton(removeCommand);
	private String previewCommand = "Preview";
	private JButton previewButton = new JButton(previewCommand);

	// RIGHT SIDE OF THE SCREEN WHERE WE CAN CHANGE HTML CONTENT
	private JPanel centerPanel = new JPanel();
	private JPanel northCenterPanel = new JPanel();
	private String setContentCommand = "Set Content";
	private JButton setContentButton = new JButton(setContentCommand);
	private JTextArea textArea = new JTextArea();
	private JScrollPane textAreaScrollPane = new JScrollPane(textArea);

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * Default Constructor, it starts all the initialization of the tree,
	 * including calling your initTreeData method.
	 */
	public HTMLFrame()
	{
		super("HTMLFrame");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setExtendedState(MAXIMIZED_BOTH);
		tree.setModel(treeManager);
		layoutGUI();
		initTreeData();
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This method places all the GUI components in their proper places.
	 */
	public void layoutGUI()
	{
		northPanel.add(tagLabel);
		northPanel.add(tagChoices);
		northPanel.add(addTagButton);
		northPanel.add(removeButton);
		northPanel.add(previewButton);
		
		centerPanel.setLayout(new BorderLayout());
		centerPanel.add(northCenterPanel, BorderLayout.NORTH);
		centerPanel.add(textAreaScrollPane, BorderLayout.CENTER);
		northCenterPanel.add(setContentButton);

		add(northPanel, BorderLayout.NORTH);
		add(treeScrollPane, BorderLayout.CENTER);
		add(centerPanel, BorderLayout.EAST);

		addTagButton.addActionListener(this);
		removeButton.addActionListener(this);
		previewButton.addActionListener(this);
		setContentButton.addActionListener(this);
		setContentButton.setEnabled(false);
		tree.addTreeSelectionListener(this);
		tree.setRootVisible(true);
		textArea.setColumns(30);
		textArea.setLineWrap(true);
		textArea.setWrapStyleWord(true);
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This method makes sure user selections of tree nodes
	 * runs smoothly.
	 */
	public void valueChanged(TreeSelectionEvent e)
	{
		TreePath path = e.getNewLeadSelectionPath();
		if (path != null)
		{
			Object node = path.getLastPathComponent();
			String contents = node.toString();
			if (contents.startsWith("<"))
				setContentButton.setEnabled(false);
			else
			{
				setContentButton.setEnabled(true);
				textArea.setText(contents);
			}
		}
	}

	/*
	 * DO NOT CHANGE THIS METHOD
	 * 
	 * This method responds to user input.
	 */
	public void actionPerformed(ActionEvent ae)
	{
		String command = ae.getActionCommand();
		
		// ADD A TAG TO THE TREE
		if (command.equals(addTagCommand))
		{
			TreePath treePath = tree.getSelectionPath();
			
			// NO NODE IS SELECTED IN THE TREE, SO WE CAN't ADD IT
			if (treePath == null)
			{
				JOptionPane.showMessageDialog(	this,
						"You must select a parent node!",
						"No parent node selected",
						JOptionPane.WARNING_MESSAGE);
				return;				
			}
			// A NODE IS SELECTED, LET'S TRY TO ADD A CHILD TO IT
			else
			{
				// GET THE SELECTED NODE
				DefaultMutableTreeNode selectedNode = (DefaultMutableTreeNode)(treePath.getLastPathComponent());
				String nodeText = (String)selectedNode.getUserObject(); 
				
				// WE ARE ONLY ALLOWED TO ADD CHILDREN TO THE body & p NODES
				if (nodeText.equals("<body>") || nodeText.equals("<p>"))
				{
					// WHAT tag ARE WE TRYING TO ADD?
					String tag = (String)tagChoices.getSelectedItem();
					
					// WE DON'T WANT ANY NESTED TAGS
					if (nodeText.equals(tag))
					{
						JOptionPane.showMessageDialog(	this,
							"You may not add that node there!",
							"Illegal Operation",
							JOptionPane.WARNING_MESSAGE);
						return;
					}
					else
					{
						// CONSTRUCT OUR NEW NODE
						DefaultMutableTreeNode nodeToAdd = new DefaultMutableTreeNode();
						nodeToAdd.setUserObject(tag);
						
						// ADD IT TO THE TREE MODEL
						treeManager.insertNodeInto(nodeToAdd, selectedNode, selectedNode.getChildCount());

						// IF IT'S A PARAGRAPH WE NEED A SECOND NODE
						// FOR THE TEXT INSIDE
						if (tag.equals("<p>"))
						{
							DefaultMutableTreeNode textNode = new DefaultMutableTreeNode();
							textNode.setUserObject("Paragraph Text");
							treeManager.insertNodeInto(textNode, nodeToAdd, 0);
							tree.scrollPathToVisible(new TreePath(nodeToAdd.getPath()));
						}
						// IF IT'S A HEADER TAG WE NEED A SECOND NODE
						// FOR THE TEXT INSIDE
						else if (tag.startsWith("<h") && (!tag.equals("<hr />")))
						{
							DefaultMutableTreeNode textNode = new DefaultMutableTreeNode();
							textNode.setUserObject("Header Text");
							treeManager.insertNodeInto(textNode, nodeToAdd, 0);
							tree.scrollPathToVisible(new TreePath(nodeToAdd.getPath()));
						}
						else
						{
							// MAKE SURE OUR NEW NODE IS ON DISPLAY
							tree.scrollPathToVisible(new TreePath(nodeToAdd.getPath()));
						}
					}
				}
			}
		}
		// THE USER WANTS TO REMOVE A NODE
		else if (command.equals(removeCommand))
		{
			// WHICH NODE?
			DefaultMutableTreeNode selectedNode = (DefaultMutableTreeNode)(tree.getSelectionPath().getLastPathComponent());
			String nodeText = (String)selectedNode.getUserObject();

			// THESE NODES CANNOT BE REMOVED
			if (	selectedNode.isRoot() 		||
					nodeText.equals("<body>") 	||
					nodeText.equals("<head>") 	||
					nodeText.equals("<title>")	||
					(nodeText.charAt(0) != '<')	)
			{
				JOptionPane.showMessageDialog(	this,
												"You may not remove that node!",
												"Illegal Operation",
												JOptionPane.WARNING_MESSAGE);
				return;
			}
			else
			{
				// TAKE IT OUT OF THE TREE MODEL
				DefaultMutableTreeNode parentNode = (DefaultMutableTreeNode)selectedNode.getParent();
				treeManager.removeNodeFromParent(selectedNode);
				tree.scrollPathToVisible(new TreePath(parentNode.getPath()));
			}
		}
		// THE USER WANT'S TO VIEW THE WEB PAGE THE WAY IT 
		// WOULD BE SEEN INSIDE A WEB BROWSER
		else if (command == previewCommand)
		{
			JDialog dialog = new JDialog();
			dialog.setSize(new Dimension(800,600));
			dialog.setLocation(100, 100);
			dialog.setTitle("HTML Preview");
			
			// WE ARE CALLING YOUR METHOD HERE. YOUR METHOD SHOULD
			// GENERATE PERFECT HTML
			String webPageText = generateHTMLText((DefaultMutableTreeNode)treeManager.getRoot());
			System.out.println(webPageText);
			JEditorPane webViewer = new JEditorPane("text/html", webPageText);
			JScrollPane webScroller = new JScrollPane(webViewer);
			dialog.add(webScroller);
			dialog.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			dialog.setVisible(true);
		}
		// THE USER WANTS TO CHANGE SOME CONTENT
		else if (command == setContentCommand)
		{
			String text = textArea.getText();
			DefaultMutableTreeNode selectedNode = (DefaultMutableTreeNode)(tree.getSelectionPath().getLastPathComponent());
			selectedNode.setUserObject(text);
			treeManager.nodeChanged(selectedNode);
		}
	}

	/*
	 * YOU MUST DEFINE THIS METHOD
	 * 
	 * This method should build the default starting tree. It's
	 * structure should be a basic Web document:
	 * <html>
	 *  <head>
	 *   <title>
	 *    Title Text
	 *  <body>
	 *   <p>
	 *    ParagraphText
	 *    
	 *  Note that I did not include end tags (like </html>), it is
	 *  not necessary to store that data in our nodes, but when the Web
	 *  page is generated using your generateHTMLText, make sure you don't
	 *  forget them.
	 */
	public void initTreeData()
	{
		//I use braces just to keep myself organized
		//set up tree
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("<html>&lt;html&gt;</html>");
		treeManager.setRoot(root);
		//set up head tag
		{
			DefaultMutableTreeNode head = new DefaultMutableTreeNode("<head>");
			treeManager.insertNodeInto(head, root, 0);
			//set up title
			{
				DefaultMutableTreeNode title = new DefaultMutableTreeNode("<title>");
				treeManager.insertNodeInto(title, head, 0);
				//text for the title
				{
					DefaultMutableTreeNode titleText = new DefaultMutableTreeNode("WebTitle");
					treeManager.insertNodeInto(titleText, title, 0);
				}
				//done with text for the title
			}
			//done witht the title
		}
		//done with head tag
		//set up the body
		{
			DefaultMutableTreeNode body = new DefaultMutableTreeNode("<body>");
			treeManager.insertNodeInto(body, root, 1);
			//add a p tag
			{
				DefaultMutableTreeNode pTag = new DefaultMutableTreeNode("<p>");
				treeManager.insertNodeInto(pTag, body, 0);
				//add text to the p
				{
					treeManager.insertNodeInto(new DefaultMutableTreeNode("hello"), pTag, 0);
				}
				//done with text
			}
			//done with the p tag
		}
		//done with the body
		//done with tree
	}
	
	/*
	 * YOU MUST DEFINE THIS METHOD
	 * 
	 * This method will walk the tree and build a String that will ultimately
	 * be a perfectly constructed Web page. Don't forget to add closing tags.
	 * 
	 * Note, <br /> and <hr /> don't have closing tags.
	 */
	public String generateHTMLText(DefaultMutableTreeNode node)
	{
		return genText((DefaultMutableTreeNode) treeManager.getRoot());
	}
	
	//recursive method to make string, I think it's easier to do this way
	private String  genText(DefaultMutableTreeNode node){
		String str="";
		String tag = (String)node.getUserObject();
		boolean close = true;
		if(tag.equals("<html>&lt;html&gt;</html>")){
			tag = "html";
			str += "<"+tag+">\n";
		} else if(tag.charAt(0)=='<'){
			tag = tag.substring(1,tag.length()-1);
			str += "<"+tag+">\n";
		} else {
			str+=tag+"\n";
			close=false;
		}
		Enumeration<DefaultMutableTreeNode> children = (Enumeration<DefaultMutableTreeNode>)node.children();
		
		while(children.hasMoreElements()){
			str += genText(children.nextElement());
		}
		
		if(!(tag.equals("br /")||tag.equals("hr /"))&&close){
			str += "</"+tag+">\n";
		}
		return str;
	}
	
	public static void main(String[] args) 
	{
		HTMLFrame frame = new HTMLFrame();
		frame.setVisible(true);
	}
}