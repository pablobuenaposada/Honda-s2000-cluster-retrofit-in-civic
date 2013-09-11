package SerialPort;

import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;

public class Serial {
	
    private final static String usbPort = "/dev/tty.usbserial";
    private final static String appName = "S2000 Cluster setup";
    private final static int TIMEOUT=3000;
    private static CommPortIdentifier portId;    
    private static SerialPort serialPort;
    private static OutputStream outputStream;
    private static InputStream inputStream;    
    
    
    /*public static void main (final String[] args) throws NoSuchPortException, PortInUseException, UnsupportedCommOperationException, IOException, InterruptedException {
    	configPort(usbPort,appName);
    	write("a/n");
        Thread.sleep(500);
        System.out.println(read());
        closePort();
    }*/
    
    public void closePort() throws IOException{
    	outputStream.close();
        serialPort.removeEventListener();
        serialPort.close();        	
    }
    
    public void configPort(String usbPort,String appName) throws NoSuchPortException, PortInUseException, UnsupportedCommOperationException, IOException{
    	portId = CommPortIdentifier.getPortIdentifier(usbPort);

        serialPort = (SerialPort)portId.open(appName, 2000);                  
        serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);

        outputStream = serialPort.getOutputStream();
        inputStream = serialPort.getInputStream();
    }
    
    public void write(String data) throws IOException{
    	outputStream.write(data.getBytes());
    }
    
    public String read() throws IOException{    	
    	String received="";
    	int bytesRead=0;
    	byte[] buffer = new byte[100];
	long timeoutTime = System.currentTimeMillis()+TIMEOUT;
    			
    	while (!received.contains(Character.toString('\n'))){				
            if (inputStream.available() > 0){
                bytesRead = inputStream.read(buffer);
		received = received + new String(buffer).substring(0, bytesRead);
            }
            else{
		if (System.currentTimeMillis() > timeoutTime){						
                    return null;
		}				
            }
        }				
        return received;
    }
    
    


}
