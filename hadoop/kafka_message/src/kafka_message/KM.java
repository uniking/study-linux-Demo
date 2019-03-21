package kafka_message;

import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.EmptyBorder;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import java.beans.PropertyChangeListener;
import java.io.File;
import java.beans.PropertyChangeEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.Color;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.UIManager;
import javax.swing.JTextArea;

public class KM extends JDialog implements Observer{
	static String uName = null;
	static String kafkaIP = null;
	private JTextField userName;
	
	static JTextArea messageShow;
	static StringBuffer msBuffer = new StringBuffer();
	
	static MProducer mpro = null;
	static Thread tGmt = null;
	static GetMsgThread gmt = null;
	static boolean run = true;
	private JTextArea msgSend;
	private JTextField textField;
	
	void initConfig()
	{
		try {
			File hF = new File("KM.conf");
			if(hF.exists())
			{
				Map conf = GJson.File2Map("KM.conf");
				uName = (String)conf.get("name");
				if(uName == null)
					uName = "anyone";
				kafkaIP = (String)conf.get("kafka_ip");
				if(kafkaIP == null)
					kafkaIP = "192.168.220.205:9092";
			}
			else
			{
				Map conf = new HashMap<String, Object>();
				conf.put("name", "anyone");
				conf.put("kafka_ip", "192.168.220.205:9092");
				GJson.Map2File(conf, "KM.conf");
				
				uName = "anyone";
				kafkaIP = "192.168.220.205:9092";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void init()
	{
		initConfig();
		
		mpro = new MProducer(kafkaIP, "ndlp_msg");
		
		gmt = new GetMsgThread(kafkaIP, uName);
		gmt.addObserver(this);
		tGmt = new Thread(gmt);
		tGmt.start();
		
		
		KM dialog = new KM();
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		dialog.setVisible(true);
	}

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		try {
			run = true;
			new KM().init();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the dialog.
	 */
	public KM() {
		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosed(WindowEvent e) {
				gmt.stopServer();
				//tGmt.interrupt();
				run = false;
			}
		});
		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentShown(ComponentEvent e) {
			}
		});
		getContentPane().setEnabled(false);
		
		JButton SendButton = new JButton("发送");
		SendButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				if(userName.getText().length() == 0 || msgSend.getText().length() == 0)
					return;
				
				gmt.setUserName(userName.getText());
				MessageProtocol mp = new MessageProtocol(userName.getText(), "txt", msgSend.getText());
				mpro.SendMeg(mp.getMessageProtocol());
				msgSend.setText("");
			}
		});
		
		userName = new JTextField();
		userName.addPropertyChangeListener(new PropertyChangeListener() {
			public void propertyChange(PropertyChangeEvent evt) {
			}
		});
		userName.setText(uName);
		userName.setColumns(10);
		
		JLabel label_name = new JLabel("用户名");
		
		JScrollPane scrollPane = new JScrollPane();
		
		JScrollPane scrollPane_1 = new JScrollPane();
		
		JButton button = new JButton("更改配置");
		button.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				uName = userName.getText();
				
				Map conf = new HashMap<String, Object>();
				conf.put("name", uName);
				conf.put("kafka_ip", kafkaIP);
				try {
					GJson.Map2File(conf, "KM.conf");
					JOptionPane.showMessageDialog(null,"信息保存到配置文件");
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		
		JLabel lblKafkaip = new JLabel("kafka_ip");
		
		textField = new JTextField();
		textField.setText("192.168.220.205:9029");
		textField.setColumns(10);
		GroupLayout groupLayout = new GroupLayout(getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(155)
							.addComponent(SendButton, GroupLayout.PREFERRED_SIZE, 117, GroupLayout.PREFERRED_SIZE))
						.addGroup(groupLayout.createSequentialGroup()
							.addContainerGap()
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
								.addComponent(scrollPane, GroupLayout.PREFERRED_SIZE, 609, GroupLayout.PREFERRED_SIZE)
								.addGroup(groupLayout.createSequentialGroup()
									.addComponent(scrollPane_1, GroupLayout.PREFERRED_SIZE, 381, GroupLayout.PREFERRED_SIZE)
									.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
										.addGroup(groupLayout.createSequentialGroup()
											.addGap(18)
											.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING)
												.addGroup(Alignment.LEADING, groupLayout.createSequentialGroup()
													.addComponent(label_name)
													.addGap(18)
													.addComponent(userName, GroupLayout.DEFAULT_SIZE, 569, Short.MAX_VALUE))
												.addComponent(button)))
										.addGroup(groupLayout.createSequentialGroup()
											.addGap(14)
											.addComponent(lblKafkaip)
											.addGap(10)
											.addComponent(textField, GroupLayout.DEFAULT_SIZE, 569, Short.MAX_VALUE)))))))
					.addGap(13))
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addComponent(scrollPane, GroupLayout.PREFERRED_SIZE, 367, GroupLayout.PREFERRED_SIZE)
					.addGap(15)
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
						.addGroup(groupLayout.createSequentialGroup()
							.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
								.addComponent(label_name)
								.addComponent(userName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
							.addPreferredGap(ComponentPlacement.UNRELATED)
							.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
								.addComponent(textField, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addComponent(lblKafkaip))
							.addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
							.addComponent(button))
						.addComponent(scrollPane_1, GroupLayout.PREFERRED_SIZE, 113, GroupLayout.PREFERRED_SIZE))
					.addGap(18)
					.addComponent(SendButton, GroupLayout.PREFERRED_SIZE, 37, GroupLayout.PREFERRED_SIZE)
					.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);
		
		messageShow = new JTextArea();
		scrollPane.setViewportView(messageShow);
		
		msgSend = new JTextArea();
		scrollPane_1.setViewportView(msgSend);
		getContentPane().setLayout(groupLayout);
		setBounds(100, 100, 647, 615);
	}

	@Override
	public void update(Observable o, Object arg) {
		// TODO Auto-generated method stub
		
		System.out.println("update");
		GetMsgThread gmt = (GetMsgThread)o;
		List<String> msg = new ArrayList<String>();
		gmt.getMsg(msg);
		System.out.println("msg size:" + msg.size());
		for(String one:msg)
		{
			
			try {
				MessageProtocol mp = new MessageProtocol(one);
				System.out.println("this.MessageBoard.setText:" + mp.getMessage());
				//km_MessageBoard.setText(mp.getName() + "\n" + mp.getMessage());
				
				msBuffer.append(mp.getName() + ":\n" + mp.getMessage()).append("\n----------\n");
				messageShow.setText(msBuffer.toString());
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
}
