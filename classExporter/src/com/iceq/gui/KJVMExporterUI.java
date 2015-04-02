package com.iceq.gui;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.TransferHandler;
import javax.swing.table.AbstractTableModel;

import com.iceq.KClass;
import com.iceq.KJVMExporter;


public class KJVMExporterUI extends JPanel {
	File m_save = new File("save.txt");
	KJVMExporterUI m_instance = null;
	KJVMExporter m_exporter;

	public KJVMExporterUI() {
		super(new GridLayout(1, 0));

		m_instance = this;

		m_exporter = new KJVMExporter();

		JSplitPane mainSplitPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
		JSplitPane microKernelSplitPanel = new JSplitPane(JSplitPane.VERTICAL_SPLIT);

		createMicroKernelInterface(microKernelSplitPanel);
		
		mainSplitPanel.add(microKernelSplitPanel);
		JSplitPane appSplitPanel = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		
		createAppInterface(appSplitPanel);
		
		mainSplitPanel.add(appSplitPanel);
		add(mainSplitPanel);
	}

	void createMicroKernelInterface(JSplitPane splitPanel) {
		JFileChooser fd = new JFileChooser();

		JPanel panel = new JPanel();

		JButton loadMicro = new JButton("Load");
		loadMicro.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				fd.setDialogType(JFileChooser.OPEN_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fd.setDialogTitle("Add microkernel class or file set.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int i = fd.showDialog(m_instance, "Open");
				if (i == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					setCurrentPath(selectedFile.getParentFile()
							.getAbsolutePath());
					m_exporter.loadPackage(selectedFile,true);
				}
			}
		});
		panel.add(loadMicro);

		JButton saveMicro = new JButton("Save");
		saveMicro.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.SAVE_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fd.setDialogTitle("Save microkernel.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int res = fd.showDialog(m_instance, "Save");
				if (res == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					setCurrentPath(selectedFile.getParentFile()
							.getAbsolutePath());
					m_exporter.savePackage(selectedFile,true);
				}
			}
		});
		panel.add(saveMicro);

		JButton addKernelFiles = new JButton("Add Class");
		addKernelFiles.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.OPEN_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
				fd.setDialogTitle("Add microkernel class or file set.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int i = fd.showDialog(m_instance, "Open");
				if (i == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					if (selectedFile.isDirectory())
						setCurrentPath(selectedFile.getAbsolutePath());
					else
						setCurrentPath(selectedFile.getParentFile()
								.getAbsolutePath());

					m_exporter.load(selectedFile,null,true);
				}
			}
		});
		panel.add(addKernelFiles);

		JButton exportKernelFiles = new JButton("Export");
		exportKernelFiles.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.SAVE_DIALOG);
				fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				fd.setDialogTitle("Export microkernel classes.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int res = fd.showDialog(m_instance, "Save");
				if (res == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					/*
					 * if (selectedFile.isDirectory())
					 * setCurrentPath(selectedFile.getAbsolutePath()); else
					 * setCurrentPath(selectedFile.getParentFile()
					 * .getAbsolutePath());
					 */

					m_exporter.exportMicroKernel(selectedFile);
				}
			}
		});
		panel.add(exportKernelFiles);

		splitPanel.add(panel);

		JTable table = new JTable(new MyTableModel(true));
		table.setDragEnabled(true);
		table.setTransferHandler(new TableTransferHandler(true));
		table.setPreferredScrollableViewportSize(new Dimension(500, 70));
		table.setFillsViewportHeight(true);

		// Create the scroll pane and add the table to it.
		JScrollPane scrollPane = new JScrollPane(table);

		// Add the scroll pane to this panel.
		splitPanel.add(scrollPane);
	}
	
	void createAppInterface(JSplitPane splitPanel) {
		JFileChooser fd = new JFileChooser();
		JPanel panel = new JPanel();
		JButton loadPackage = new JButton("Load");
		loadPackage.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				fd.setDialogType(JFileChooser.OPEN_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fd.setDialogTitle("Add microkernel class or file set.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int i = fd.showDialog(m_instance, "Open");
				if (i == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					setCurrentPath(selectedFile.getParentFile()
							.getAbsolutePath());
					m_exporter.loadPackage(selectedFile,false);
				}
			}
		});
		panel.add(loadPackage);

		JButton savePacakge = new JButton("Save");
		savePacakge.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.SAVE_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_ONLY);
				fd.setDialogTitle("Save microkernel.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int res = fd.showDialog(m_instance, "Save");
				if (res == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					setCurrentPath(selectedFile.getParentFile()
							.getAbsolutePath());
					m_exporter.savePackage(selectedFile,false);
				}
			}
		});
		panel.add(savePacakge);

		JButton addPackageFiles = new JButton("Add Class");
		addPackageFiles.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.OPEN_DIALOG);
				fd.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
				fd.setDialogTitle("Add microkernel class or file set.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int i = fd.showDialog(m_instance, "Open");
				if (i == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();

					if (selectedFile.isDirectory())
						setCurrentPath(selectedFile.getAbsolutePath());
					else
						setCurrentPath(selectedFile.getParentFile()
								.getAbsolutePath());

					m_exporter.load(selectedFile,null,false);
				}
			}
		});
		panel.add(addPackageFiles);

		JButton exportPackageFiles = new JButton("Export");
		exportPackageFiles.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				fd.setDialogType(JFileChooser.SAVE_DIALOG);
				fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				fd.setDialogTitle("Export Project classes.");

				String crtPath = getCurrentPath();
				if (crtPath != null)
					fd.setCurrentDirectory(new File(crtPath));

				int res = fd.showDialog(m_instance, "Save");
				if (res == JFileChooser.APPROVE_OPTION) {
					final File selectedFile = fd.getSelectedFile();
					
					m_exporter.exportClasses(selectedFile);
				}
			}
		});
		panel.add(exportPackageFiles);

		splitPanel.add(panel);

		JTable table = new JTable(new MyTableModel(false));
		table.setDragEnabled(true);
		table.setTransferHandler(new TableTransferHandler(false));
		table.setPreferredScrollableViewportSize(new Dimension(500, 70));
		table.setFillsViewportHeight(true);

		// Create the scroll pane and add the table to it.
		JScrollPane scrollPane = new JScrollPane(table);

		// Add the scroll pane to this panel.
		splitPanel.add(scrollPane);
	}

	class MyTableModel extends AbstractTableModel 
	{
		private String[] columnNames = { "ID", "Export", "Class" };
		private boolean m_kernel;
		MyTableModel(boolean isKernel){
			m_kernel = isKernel;
		}
		
		private Vector<KClass> getClasses(){
			if(m_kernel)
				return m_exporter.getMicroKernelClassFiles();
			else
				return m_exporter.getProjectClassFiles();
		}
		
		@Override
		public int getColumnCount() {
			return columnNames.length;
		}

		@Override
		public int getRowCount() {
			return getClasses().size();
		}

		@Override
		public String getColumnName(int col) {
			return columnNames[col];
		}

		@Override
		public Object getValueAt(int row, int col) {
			if (col == 0)
				return row;
			if (col == 1)
				return getClasses().get(row).m_export;

			if (col == 2)
				return getClasses().get(row).m_name;
			return "UNKNOWN COLUMN";
		}

		/*
		 * JTable uses this method to determine the default renderer/ editor for
		 * each cell. If we didn't implement this method, then the last column
		 * would contain text ("true"/"false"), rather than a check box.
		 */
		@Override
		public Class getColumnClass(int c) {
			return getValueAt(0, c).getClass();
		}

		/*
		 * Don't need to implement this method unless your table's editable.
		 */
		@Override
		public boolean isCellEditable(int row, int col) {
			// Note that the data/cell address is constant,
			// no matter where the cell appears onscreen.
			if (col != 1) {
				return false;
			} else {
				return true;
			}
		}

		/*
		 * Don't need to implement this method unless your table's data can
		 * change.
		 */
		@Override
		public void setValueAt(Object value, int row, int col) {
			if (col == 1)
				getClasses().get(row).m_export = (Boolean) value;

			fireTableCellUpdated(row, col);
		}
	}

	/**
	 * Create the GUI and show it. For thread safety, this method should be
	 * invoked from the event-dispatching thread.
	 */
	private static void createAndShowGUI(String args[]) {
		// Create and set up the window.
		JFrame frame = new JFrame("TableDemo");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		KJVMExporterUI processor = new KJVMExporterUI();

		processor.setOpaque(true); // content panes must be opaque
		frame.setContentPane(processor);

		// Display the window.
		frame.pack();
		frame.setVisible(true);
	}

	public static void main(String[] args) {
		// Schedule a job for the event-dispatching thread:
		// creating and showing this application's GUI.
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				createAndShowGUI(args);
			}
		});
	}

	private String getCurrentPath() {
		try {
			DataInputStream dis = new DataInputStream(new FileInputStream(
					m_save));
			return dis.readUTF();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return null;
	}

	private void setCurrentPath(String str) {
		try {
			DataOutputStream dis = new DataOutputStream(new FileOutputStream(
					m_save));
			dis.writeUTF(str);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * TableTransferHandler.java is used by the 1.4 ExtendedDnDDemo.java
	 * example.
	 */

	class TableTransferHandler extends TransferHandler 
	{
		boolean m_kernel;
		TableTransferHandler(boolean iskernel){
			m_kernel = iskernel;
		}
		private Vector<KClass> getClasses(){
			if(m_kernel)
				return m_exporter.getMicroKernelClassFiles();
			else
				return m_exporter.getProjectClassFiles();
		}
		
		public int getSourceActions(JComponent c) {
			return DnDConstants.ACTION_MOVE;
		}

		public Transferable createTransferable(JComponent comp) {
			JTable table = (JTable) comp;
			int row = table.getSelectedRow();

			String value = "" + row;
			StringSelection transferable = new StringSelection(value);

			return transferable;
		}

		public boolean canImport(TransferHandler.TransferSupport info) {
			return true;
		}

		public boolean importData(TransferSupport support) {

			if (!support.isDrop()) {
				return false;
			}

			if (!canImport(support)) {
				return false;
			}

			JTable table = (JTable) support.getComponent();

			JTable.DropLocation dl = (JTable.DropLocation) support
					.getDropLocation();

			int row = dl.getRow();

			String data;
			try {
				data = (String) support.getTransferable().getTransferData(
						DataFlavor.stringFlavor);
			} catch (UnsupportedFlavorException e) {
				return false;
			} catch (IOException e) {
				return false;
			}
			int sourceRow = Integer.parseInt(data);
			KClass elem = getClasses().get(sourceRow);

			getClasses().remove(sourceRow);
			getClasses().insertElementAt(elem, row);
			((AbstractTableModel) table.getModel()).fireTableDataChanged();
			return true;
		}

	}
}
