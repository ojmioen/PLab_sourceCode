package application;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;

public class MyController {
	
	private PLabSerial serialConnection = new PLabSerial();;
	
	@FXML
    private TextField serialPortNameField;
	
	@FXML
	void initialize() {
		String portName;
		if(serialConnection.openPLabPort()) {	
		    portName = serialConnection.getOpenPortName();}
		else {
			portName = "No PLab BT device found.";
		};
		serialPortNameField.setText(portName);;		
	}
	
	@FXML
	void beastModeButtonPressed() {
		System.out.println("BEAST MODE ENGAGED");
	    serialConnection.sendMessage("b",0);
	}
	
	@FXML
	void noBeastModeButtonPressed() {
		System.out.println("BEAST MODE DEACTIVATED");
	    serialConnection.sendMessage("n",0);
	}
	
	@FXML
	void startZumoPressed() {
		System.out.println("Zumo robot started.");
		serialConnection.sendMessage("s", 0);
	}
	
}
